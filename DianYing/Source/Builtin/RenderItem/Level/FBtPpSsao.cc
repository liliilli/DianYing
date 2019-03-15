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

#include <Dy/Builtin/RenderItem/Level/FBtPpSsao.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Builtin/Constant/SSAO.h>
#include <Dy/Helper/Math/Random.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>

namespace dy
{

void FBtRenderItemSsao::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("1f986c21-bf65-42e2-8dac-a59a4f2984f3", true);
}

FBtRenderItemSsao::FBtRenderItemSsao()
{
  for (TU32 i = 0; i < kSSAORayCount; ++i)
  {
    DVector3 sample = DVector3{
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

EDySuccess FBtRenderItemSsao::OnPreRenderCheckCondition()
{
  const auto& information = MSetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultSsao == false)
  {
    return DY_FAILURE;
  }

  return this->AreResourcesValid() ? DY_SUCCESS : DY_FAILURE;
}

bool FBtRenderItemSsao::AreResourcesValid()
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

void FBtRenderItemSsao::OnFailedCheckCondition()
{
  if (this->AreResourcesValid() == false) { return; }

  this->mBinderFbSSAOBlur->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_COLOR, 0, &one);
  this->mBinderFbSSAOBlur->UnbindFrameBuffer();
}

void FBtRenderItemSsao::OnSetupRenderingSetting()
{
  // SSAO (Opaque -> SSAO output)
  // SSAO Blur (SSAO Output -> SSAO Blurred) does not have to setup.
  if (this->mIsRayInserted == false)
  {
    using EUniform = EUniformVariableType;
    this->mBinderShSSAO->TryUpdateUniform<EUniform::Vector3Array>("uRaySamples[0]", this->mRayContainer);
    this->mIsRayInserted = true;
  }
}

void FBtRenderItemSsao::OnRender()
{
  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT_MSG(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();

  { // Check SSAO target.
    this->mBinderFbSSAO->BindFrameBuffer();

    this->mBinderShSSAO->TryInsertTextureRequisition(0, this->mBinderAttWorldNorm->GetSourceAttachmentId());
    this->mBinderShSSAO->TryInsertTextureRequisition(1, this->mBinderAttWorldPos->GetSourceAttachmentId());
    this->mBinderShSSAO->TryInsertTextureRequisition(2, this->mBinderTexNoise->GetTextureId());
    this->mBinderShSSAO->UseShader();
    this->mBinderShSSAO->TryUpdateUniformList();

    XGLWrapper::Draw(EDrawType::Triangle, true, 3);
  }

  { // Box Blurring (Fast and easy!)
    this->mBinderFbSSAOBlur->BindFrameBuffer();
    this->mBinderShSSAOBlur->TryInsertTextureRequisition(0, this->mBinderAttSSAOOpt->GetSourceAttachmentId());
    this->mBinderShSSAOBlur->UseShader();
    this->mBinderShSSAOBlur->TryUpdateUniformList();

    XGLWrapper::Draw(EDrawType::Triangle, true, 3);

    this->mBinderShSSAOBlur->DisuseShader();
    this->mBinderFbSSAOBlur->UnbindFrameBuffer();
  }

  XGLWrapper::UnbindVertexArrayObject();
}

void FBtRenderItemSsao::OnReleaseRenderingSetting()
{
  /* Do nothing */
}

void FBtRenderItemSsao::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace
