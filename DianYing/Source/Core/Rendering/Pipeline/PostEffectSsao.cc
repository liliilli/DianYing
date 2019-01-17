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
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttWorldNorm->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttWorldPos->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderTexNoise->GetTextureId());

  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "Unexpected error occurred.");

  submeshList[0]->Get()->BindVertexArray();
  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

  this->mBinderFbSSAOBlur->BindFrameBuffer();
  this->mBinderShSSAOBlur->UseShader();
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttSSAOOpt->GetAttachmentId());

  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

  FDyGLWrapper::UnbindVertexArrayObject();
  this->mBinderShSSAOBlur->DisuseShader();
  this->mBinderFbSSAOBlur->UnbindFrameBuffer();
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
  &&  this->mBinderShSSAOBlur.IsResourceExist() == true
  &&  this->mBinderTriangle.IsResourceExist() == true;
}

EDySuccess FDyPostEffectSsao::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  // SSAO (Opaque -> SSAO output)
  // SSAO Blur (SSAO Output -> SSAO Blurred) does not have to setup.
  this->mBinderShSSAO->UseShader();
  if (this->mIsRayInserted == false)
  {
    this->mBinderShSSAO.TryUpdateUniform<EDyUniformVariableType::Vector3Array>("uRaySamples[0]", this->mRayContainer);
    this->mBinderShSSAO.TryUpdateUniformList();
    this->mIsRayInserted = true;
  }
  this->mBinderShSSAO->DisuseShader();

  return DY_SUCCESS;
}

void FDyPostEffectSsao::Clear()
{
  if (this->IsReady() == false) { return; }
}

} /// ::dy namespace