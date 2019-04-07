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

#include <Dy/Builtin/RenderItem/Level/FBtPpEmissive.h>
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Core/Resource/Resource/FResourceModel.h>
#include <Dy/Core/Resource/Resource/AResourceAttachmentBase.h>

namespace dy
{

void FBtRenderItemEmissive::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("a9c7a9aa-5f16-4857-8b31-58f6a414465f", true);
}

EDySuccess FBtRenderItemEmissive::OnPreRenderCheckCondition()
{
  return this->AreResourcesValid() ? DY_SUCCESS : DY_FAILURE;
}

bool FBtRenderItemEmissive::AreResourcesValid()
{
  return 
      this->mFbBlur.IsResourceExist() == true
  &&  this->mFbTone.IsResourceExist() == true
  &&  this->mAtEmissive.IsResourceExist() == true
  &&  this->mAtBlur.IsResourceExist() == true
  &&  this->mShBlur.IsResourceExist() == true
  &&  this->mShTone.IsResourceExist() == true
  &&  this->mBinderTriangle.IsResourceExist() == true;
}

void FBtRenderItemEmissive::OnFailedCheckCondition()
{
  if (this->AreResourcesValid() == false) { return; }

  this->ClearFramebuffers();
  this->mFbTone->UnbindFrameBuffer();
}

void FBtRenderItemEmissive::ClearFramebuffers()
{
  const auto one = DColorRGBA{1, 1, 1, 1};

  XGLWrapper::ClearColorFrameBuffer(this->mFbBlur->GetTargetFrameBufferId(), one);
  this->mFbBlur->Swap();
  XGLWrapper::ClearColorFrameBuffer(this->mFbBlur->GetTargetFrameBufferId(), one);
  this->mFbBlur->Swap();

  XGLWrapper::ClearColorFrameBuffer(this->mFbTone->GetTargetFrameBufferId(), one);
  this->mFbTone->BindFrameBuffer();
}

void FBtRenderItemEmissive::OnSetupRenderingSetting()
{
  this->ClearFramebuffers();
}

void FBtRenderItemEmissive::OnRender()
{
  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT_MSG(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();

  for (size_t i = 0; i < 4; ++i)
  { 
    // Blurring with gaussian filtering.
    this->mFbBlur->BindFrameBuffer();

    this->mShBlur->TryInsertTextureRequisition(0, 
      i == 0 
      ? this->mAtEmissive->GetSourceAttachmentId()
      : this->mAtBlur->GetSourceAttachmentId());
    this->mShBlur->TryUpdateUniform<EUniformVariableType::Bool>("uIsHorizon", i % 2 == 0 ? true : false);
    this->mShBlur->UseShader();
    this->mShBlur->TryUpdateUniformList();

    XGLWrapper::Draw(EDrawType::Triangle, true, 3);
    this->mFbBlur->Swap();
  }

  { // Tone mapping.
    this->mFbTone->BindFrameBuffer();
    this->mShTone->TryInsertTextureRequisition(0, this->mAtBlur->GetSourceAttachmentId());
    this->mShTone->TryInsertTextureRequisition(1, this->mAtEmissive->GetSourceAttachmentId());
    this->mShTone->UseShader();
    this->mShTone->TryUpdateUniformList();

    XGLWrapper::Draw(EDrawType::Triangle, true, 3);

    this->mShTone->DisuseShader();
    this->mFbTone->UnbindFrameBuffer();
  }

  XGLWrapper::UnbindVertexArrayObject();
}

} /// ::dy namespace
