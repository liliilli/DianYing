#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Core/Component/Object/PostEffectSsao.h>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/RenderingManager.h>
#include <Dy/Helper/Math/Random.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Core/Component/Object/Camera.h>

namespace
{

struct DDyMeshInfo final
{
  dy::DDyVector3 mPosition = {};
  dy::DDyVector2 mTexCoord = {};

  DDyMeshInfo(const dy::DDyVector3& position, const dy::DDyVector2& texCoord) :
      mPosition{position}, mTexCoord{texCoord}
  {};
};

} /// ::unnamed namespace

namespace dy
{

FDyPostEffectSsao::FDyPostEffectSsao()
{
  this->pCreateSsaoFrameBufferComponents();
  this->pCreateSsaoShaderResource();

  this->pCreateBlurFrameBufferComponent();
  this->pCreateSsaoBlurShaderResource();

  this->pCreateMesh();
}

FDyPostEffectSsao::~FDyPostEffectSsao()
{
  if (this->mTriangleVbo) glDeleteBuffers(1, &this->mTriangleVbo);
  if (this->mTriangleVao) glDeleteVertexArrays(1, &this->mTriangleVao);

  this->pDeleteFrameBufferComponents();

#ifdef false
  auto& manResc = MDyHeapResource::GetInstance();
#endif
}

void FDyPostEffectSsao::RenderScreen()
{
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
}

void FDyPostEffectSsao::pCreateMesh()
{
  // Make triangle that can represent context.
  glGenVertexArrays (1, &this->mTriangleVao);
  glBindVertexArray (this->mTriangleVao);

  glGenBuffers      (1, &this->mTriangleVbo);
  glBindBuffer      (GL_ARRAY_BUFFER, this->mTriangleVbo);

  std::vector<DDyMeshInfo> mVertexInformations;
  mVertexInformations.emplace_back(DDyVector3{-1, -1, 0}, DDyVector2{0, 0});
  mVertexInformations.emplace_back(DDyVector3{ 3, -1, 0}, DDyVector2{2, 0});
  mVertexInformations.emplace_back(DDyVector3{-1,  3, 0}, DDyVector2{0, 2});
  glBufferData      (GL_ARRAY_BUFFER, sizeof(DDyMeshInfo) * 3, &mVertexInformations[0], GL_STATIC_DRAW);
  glBindVertexBuffer(0, this->mTriangleVbo, 0, sizeof(DDyMeshInfo));

  // DDyMeshInfo.mPosition (DDyVector3)
  glEnableVertexAttribArray (0);
  glVertexAttribFormat      (0, 3, GL_FLOAT, GL_FALSE, offsetof(DDyMeshInfo, mPosition));
  glVertexAttribBinding     (0, 0);
  // DDyMeshInfo.mTexCoord (DDyVector2)
  glEnableVertexAttribArray (1);
  glVertexAttribFormat      (1, 2, GL_FLOAT, GL_FALSE, offsetof(DDyMeshInfo, mTexCoord));
  glVertexAttribBinding     (1, 0);

  glBindVertexArray(0);
}

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
  auto& manInfo = MDyDataInformation::GetInstance();
  auto& manResc = MDyHeapResource::GetInstance();

  // Make deferred shader
  PDyShaderConstructionDescriptor shaderDesc;
  {
    shaderDesc.mShaderName = "dyPostEffectSsaoShader";
    {
      PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./ShaderResource/Gl/glSsao.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./ShaderResource/Gl/glSsao.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(manResc.CreateShaderResource(shaderDesc.mShaderName));

  this->mSsaoShaderPtr = manResc.GetShaderResource(shaderDesc.mShaderName);
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
}

void FDyPostEffectSsao::pCreateSsaoBlurShaderResource()
{
  auto& manInfo = MDyDataInformation::GetInstance();
  auto& manResc = MDyHeapResource::GetInstance();

  // Make deferred shader
  PDyShaderConstructionDescriptor shaderDesc;
  {
    shaderDesc.mShaderName = "dyPostEffectSsaoBlurShader";
    {
      PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./ShaderResource/Gl/glSsaoBoxBlur.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./ShaderResource/Gl/glSsaoBoxBlur.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));
  MDY_CALL_ASSERT_SUCCESS(manResc.CreateShaderResource(shaderDesc.mShaderName));

  this->mSsaoBlurShaderPtr = manResc.GetShaderResource(shaderDesc.mShaderName);
  MDY_ASSERT(this->mSsaoBlurShaderPtr, "FDyDeferredRenderingMesh::mSsaoBlurShaderPtr must not be nullptr.");

  this->mSsaoBlurShaderPtr->UseShader();
  glUniform1i(glGetUniformLocation(this->mSsaoBlurShaderPtr->GetShaderProgramId(), "uSsaoInput"), 0);
  this->mSsaoBlurShaderPtr->UnuseShader();
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

} /// ::dy namespace