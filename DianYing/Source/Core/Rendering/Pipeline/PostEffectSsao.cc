#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Core/Rendering/Pipeline/PostEffectSsao.h>
#include <Dy/Builtin/Constant/SSAO.h>

#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>

#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Helper/Math/Random.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/WorldManager.h>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

namespace dy
{

FDyPostEffectSsao::FDyPostEffectSsao()
{
  for (TU32 i = 0; i < kSSAORayCount; ++i)
  {
    DDyVector3 sample = DDyVector3{
        random::RandomFloatRange(-1, 1), 
        random::RandomFloatRange(-1, 1), 
        random::RandomFloatRange(0, 1)
    };
    sample =  sample.Normalize();
    sample *= random::RandomFloatRange(0, 1);

    TF32 scale = static_cast<TF32>(i) / kSSAORayCount;
    scale = math::Lerp(0.1f, 1.0f, scale * scale);
    sample *= scale;
    this->mRayContainer.push_back(sample);
  }
}

void FDyPostEffectSsao::RenderScreen()
{
  if (this->IsReady() == false) { return; }
  
  this->mBinderFbSSAO->BindFrameBuffer();
  this->mBinderShSSAO->UseShader();
  // Check Textures.
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttWorldPos->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttWorldNorm->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderTexNoise->GetTextureId());

  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();
  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);
  FDyGLWrapper::UnbindVertexArrayObject();
  this->mBinderShSSAO->DisuseShader();
  this->mBinderFbSSAO->UnbindFrameBuffer();

#ifdef false
  MDY_ASSERT(this->mSsaoShaderPtr,     "FDyPostEffectSsao::mSsaoShaderPtr must not be nullptr.");

  //!
  //! SSAO rendering
  //!

  glBindFramebuffer (GL_FRAMEBUFFER, this->mSsaoFrameBufferId);
  glClear           (GL_COLOR_BUFFER_BIT);
  this->mSsaoShaderPtr->UseShader();
  glBindVertexArray (this->mTriangleVao);

  // Bind g-buffers as textures.
  const auto& renderingManager = MDyRendering::GetInstance();
  glActiveTexture   (GL_TEXTURE0 + 0); glBindTexture(GL_TEXTURE_2D, renderingManager.mAttachmentBuffers[3]); // ugPosition
  glActiveTexture   (GL_TEXTURE0 + 1); glBindTexture(GL_TEXTURE_2D, renderingManager.mAttachmentBuffers[1]); // ugNormal
  glActiveTexture   (GL_TEXTURE0 + 2); glBindTexture(GL_TEXTURE_2D, this->mSsaoNoiseTextureId);              // uTextureNoise
  glUniformMatrix4fv(this->mUniformProjection, 1, GL_FALSE, &MDyWorld::GetInstance().GetMainCameraPtr()->GetProjectionMatrix()[0].X);
  glDrawArrays      (GL_TRIANGLES, 0, 3);

  glBindVertexArray (0);
  this->mSsaoShaderPtr->UnuseShader();

  //!
  //! SSAO blurring rendering
  //!

  MDY_ASSERT(this->mSsaoBlurShaderPtr, "FDyPostEffectSsao::mSsaoBlurShaderPtr must not be nullptr.");

  glBindFramebuffer (GL_FRAMEBUFFER, this->mSsaoBlurFrameBufferId);
  glClear           (GL_COLOR_BUFFER_BIT);
  this->mSsaoBlurShaderPtr->UseShader();
  glBindVertexArray (this->mTriangleVao);

  glActiveTexture   (GL_TEXTURE0 + 0); glBindTexture(GL_TEXTURE_2D, this->mSsaoColorBuffer);
  glDrawArrays      (GL_TRIANGLES, 0, 3);

  glBindVertexArray (0);
  this->mSsaoBlurShaderPtr->UnuseShader();

  glBindFramebuffer (GL_FRAMEBUFFER, 0);
#endif
}

bool FDyPostEffectSsao::IsReady() const noexcept
{
  return 
      this->mBinderFbSSAO.IsResourceExist() == true
  &&  this->mBinderFbSSAOBlur.IsResourceExist() == true
  &&  this->mBinderAttWorldNorm.IsResourceExist() == true
  &&  this->mBinderAttWorldPos.IsResourceExist() == true
  &&  this->mBinderAttSSAOOpt.IsResourceExist() == true
  &&  this->mBinderShSSAO.IsResourceExist() == true
  &&  this->mBinderTexNoise.IsResourceExist() == true
  &&  this->mBinderTransShader.IsResourceExist() == true
  &&  this->mBinderTriangle.IsResourceExist() == true;
}

EDySuccess FDyPostEffectSsao::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  // SSAO (Opaque -> SSAO output)
  this->mBinderFbSSAO->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFbSSAO->UnbindFrameBuffer();

  this->mBinderShSSAO->UseShader();
  if (this->mIsRayInserted == false)
  {
    this->mBinderShSSAO.TryUpdateUniform<EDyUniformVariableType::Vector3Array>("uRaySamples[0]", this->mRayContainer);
    this->mIsRayInserted = true;
  }
  this->mBinderShSSAO.TryUpdateUniformList();
  this->mBinderShSSAO->DisuseShader();

  // SSAO Blur (SSAO Output -> SSAO Blurred)

  return DY_SUCCESS;
}

void FDyPostEffectSsao::Clear()
{
  if (this->IsReady() == false) { return; }
}

#ifdef false
void FDyPostEffectSsao::pCreateSsaoFrameBufferComponents()
{
  auto& settingManager = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();

  glGenFramebuffers(1, &this->mSsaoFrameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->mSsaoFrameBufferId);

  // Unlit g-buffer
  glGenTextures   (1, &this->mSsaoColorBuffer);
  glBindTexture   (GL_TEXTURE_2D, this->mSsaoColorBuffer);
  glTexImage2D    (GL_TEXTURE_2D, 0, GL_R16F, overallScreenWidth, overallScreenHeight, 0, GL_RED, GL_FLOAT, nullptr);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mSsaoColorBuffer, 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    MDY_UNEXPECTED_BRANCH();
  }

  const GLenum attachment[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachment);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Generate sample kernel (64)
  for (TU32 i = 0; i < 64; ++i)
  {
    DDyVector3 randomizedVector = random::RandomVector3Length(1) * (i / 64.0f);
    this->mSsaoKernel.emplace_back(randomizedVector);
  }

  // Generate noise texture (4x4)
  for (TU32 i = 0; i < 16; ++i)
  {
    DDyVector3 randomizedNoise = DDyVector3(random::RandomVector2Length(1));
    this->mSsaoNoise.emplace_back(randomizedNoise);
  }

  glGenTextures   (1, &this->mSsaoNoiseTextureId);
  glBindTexture   (GL_TEXTURE_2D, this->mSsaoNoiseTextureId);
  glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &this->mSsaoNoise[0]);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture   (GL_TEXTURE_2D, 0);
}

void FDyPostEffectSsao::pCreateBlurFrameBufferComponent()
{
  auto& settingManager = MDySetting::GetInstance();
  const auto overallScreenWidth   = settingManager.GetWindowSizeWidth();
  const auto overallScreenHeight  = settingManager.GetWindowSizeHeight();

  glGenFramebuffers(1, &this->mSsaoBlurFrameBufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->mSsaoBlurFrameBufferId);

  glGenTextures   (1, &this->mSsaoBlurColorBuffer);
  glBindTexture   (GL_TEXTURE_2D, this->mSsaoBlurColorBuffer);
  glTexImage2D    (GL_TEXTURE_2D, 0, GL_RED, overallScreenWidth, overallScreenHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mSsaoBlurColorBuffer, 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    MDY_UNEXPECTED_BRANCH();
  }

  const GLenum attachment[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachment);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyPostEffectSsao::pCreateSsaoShaderResource()
{
#ifdef false
  //this->mSsaoShaderPtr = manResc.GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderDefaultSSAO::sName));
  MDY_ASSERT(this->mSsaoShaderPtr, "FDyDeferredRenderingMesh::mSsaoShaderPtr must not be nullptr.");

  this->mSsaoShaderPtr->UseShader();
  glUniform1i(glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "ugPosition"), 0);
  glUniform1i(glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "ugNormal"), 1);
  glUniform1i(glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uTextureNoise"), 2);

  this->mUniformSamples     = glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uSamples");
  this->mUniformKernelSize  = glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uKernelSize");
  this->mUniformRadius      = glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uRadius");
  this->mUniformBias        = glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uBias");
  this->mUniformScreenSize  = glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uScreenSize");
  this->mUniformProjection  = glGetUniformLocation(this->mSsaoShaderPtr->GetShaderProgramId(), "uProjection");

  glUniform3fv(this->mUniformSamples, static_cast<GLsizei>(this->mSsaoKernel.size()), &this->mSsaoKernel[0].X);
  this->mSsaoShaderPtr->UnuseShader();
#endif
}

void FDyPostEffectSsao::pCreateSsaoBlurShaderResource()
{
#ifdef false
  // Make deferred shader
  //this->mSsaoBlurShaderPtr = manResc.GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderDefaultSSAOBlurring::sName));
  MDY_ASSERT(this->mSsaoBlurShaderPtr, "FDyDeferredRenderingMesh::mSsaoBlurShaderPtr must not be nullptr.");

  this->mSsaoBlurShaderPtr->UseShader();
  glUniform1i(glGetUniformLocation(this->mSsaoBlurShaderPtr->GetShaderProgramId(), "uSsaoInput"), 0);
  this->mSsaoBlurShaderPtr->UnuseShader();
#endif
}

void FDyPostEffectSsao::pDeleteFrameBufferComponents()
{
  this->mSsaoKernel .clear();
  this->mSsaoNoise  .clear();

  glDeleteTextures(1, &this->mSsaoNoiseTextureId);
  glDeleteTextures(1, &this->mSsaoColorBuffer);
  if (this->mSsaoFrameBufferId)
  {
    glDeleteFramebuffers(1, &this->mSsaoFrameBufferId);
  }

  MDY_LOG_INFO("{}::{} | Geometry buffer released.", "MDyRendering", "pCreateGeometryBuffers");
}

void FDyPostEffectSsao::pDeleteSsaoShaderResource()
{

}

void FDyPostEffectSsao::pDeleteSsaoBlurShaderResource()
{

}
#endif

} /// ::dy namespace