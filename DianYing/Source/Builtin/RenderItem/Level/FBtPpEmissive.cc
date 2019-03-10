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
  const GLfloat one = 1.0f;

  this->mFbBlur->BindFrameBuffer();
  glClearBufferfv(GL_COLOR, 0, &one);
  this->mFbBlur->Swap();
  this->mFbBlur->BindFrameBuffer();
  glClearBufferfv(GL_COLOR, 0, &one);
  this->mFbBlur->Swap();

  this->mFbTone->BindFrameBuffer();
  glClearBufferfv(GL_COLOR, 0, &one);
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
    this->mShBlur->TryUpdateUniform<EDyUniformVariableType::Bool>("uIsHorizon", i % 2 == 0 ? true : false);
    this->mShBlur->UseShader();
    this->mShBlur->TryUpdateUniformList();

    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);
    this->mFbBlur->Swap();
  }

  { // Tone mapping.
    this->mFbTone->BindFrameBuffer();
    this->mShTone->TryInsertTextureRequisition(0, this->mAtBlur->GetSourceAttachmentId());
    this->mShTone->TryInsertTextureRequisition(1, this->mAtEmissive->GetSourceAttachmentId());
    this->mShTone->UseShader();
    this->mShTone->TryUpdateUniformList();

    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

    this->mShTone->DisuseShader();
    this->mFbTone->UnbindFrameBuffer();
  }

  FDyGLWrapper::UnbindVertexArrayObject();
}

} /// ::dy namespace
