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

  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT_MSG(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();

  { // Check SSAO target.
    this->mBinderFbSSAO->BindFrameBuffer();

    this->mBinderShSSAO->TryInsertTextureRequisition(0, this->mBinderAttWorldNorm->GetAttachmentId());
    this->mBinderShSSAO->TryInsertTextureRequisition(1, this->mBinderAttWorldPos->GetAttachmentId());
    this->mBinderShSSAO->TryInsertTextureRequisition(2, this->mBinderTexNoise->GetTextureId());
    this->mBinderShSSAO->UseShader();
    this->mBinderShSSAO->TryUpdateUniformList();

    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);
  }

  { // Box Blurring (Fast and easy!)
    this->mBinderFbSSAOBlur->BindFrameBuffer();
    this->mBinderShSSAOBlur->TryInsertTextureRequisition(0, this->mBinderAttSSAOOpt->GetAttachmentId());
    this->mBinderShSSAOBlur->UseShader();
    this->mBinderShSSAOBlur->TryUpdateUniformList();

    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

    this->mBinderShSSAOBlur->DisuseShader();
    this->mBinderFbSSAOBlur->UnbindFrameBuffer();
  }

  FDyGLWrapper::UnbindVertexArrayObject();
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

EDySuccess FDyPostEffectSsao::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  // SSAO (Opaque -> SSAO output)
  // SSAO Blur (SSAO Output -> SSAO Blurred) does not have to setup.
  if (this->mIsRayInserted == false)
  {
    using EUniform = EDyUniformVariableType;
    this->mBinderShSSAO->TryUpdateUniform<EUniform::Vector3Array>("uRaySamples[0]", this->mRayContainer);
    this->mIsRayInserted = true;
  }

  return DY_SUCCESS;
}

void FDyPostEffectSsao::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFbSSAOBlur->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_COLOR, 0, &one);
  this->mBinderFbSSAOBlur->UnbindFrameBuffer();
}

} /// ::dy namespace