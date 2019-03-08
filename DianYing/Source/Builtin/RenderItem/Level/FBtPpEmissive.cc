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
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDyDrawType.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>

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
#ifdef false
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
#endif
  return false;
}

void FBtRenderItemEmissive::OnFailedCheckCondition()
{
#ifdef false
  if (this->AreResourcesValid() == false) { return; }

  this->mBinderFbSSAOBlur->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_COLOR, 0, &one);
  this->mBinderFbSSAOBlur->UnbindFrameBuffer();
#endif
}

void FBtRenderItemEmissive::OnRender()
{
#ifdef false
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
#endif
}

} /// ::dy namespace
