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

#include <Dy/Builtin/RenderItem/Level/FBtPpSkybox.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>
#include <Dy/Component/CSkybox.h>

namespace dy
{

void FBtRenderItemSkybox::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("cef415e9-779d-43d0-a67f-5457cc17cadd", true);
}

EDySuccess FBtRenderItemSkybox::OnPreRenderCheckCondition()
{
  // Check skybox is attached to rendering manager.
  auto optSkybox = MWorld::GetInstance().GetPtrMainLevelSkybox();
  if (optSkybox.has_value() == false)
  {
    return EDySuccess::DY_FAILURE;
  }

  // Second.
  return this->AreResourcesValid() ? EDySuccess::DY_SUCCESS : EDySuccess::DY_FAILURE;
}

bool FBtRenderItemSkybox::AreResourcesValid()
{
  return this->mBinderAttUnlit.IsResourceExist() == true
      && this->mBinderFbSkyRend.IsResourceExist() == true
      && this->mBinderShdSkybox.IsResourceExist() == true
      && this->mBinderMeshSkybox.IsResourceExist() == true;
}

void FBtRenderItemSkybox::OnFailedCheckCondition()
{
  if (this->AreResourcesValid() == false) { return; }

  XGLWrapper::ClearColorFrameBuffer(
    this->mBinderFbSkyRend->GetTargetFrameBufferId(),
    DColorRGBA{0, 0, 0, 0});
}

void FBtRenderItemSkybox::OnSetupRenderingSetting()
{
  XGLWrapper::ClearColorFrameBuffer(
    this->mBinderFbSkyRend->GetTargetFrameBufferId(),
    DColorRGBA{0, 0, 0, 0});
  this->mBinderFbSkyRend->BindFrameBuffer();
}

void FBtRenderItemSkybox::OnRender()
{
  // https://www.khronos.org/opengl/wiki/Cubemap_Texture
  // Render
  auto optSkybox = MWorld::GetInstance().GetPtrMainLevelSkybox();
  MDY_ASSERT_MSG(optSkybox.has_value() == true, "Unexpected error occurred.");

  auto ptrSkyboxTexture  = optSkybox.value();
  auto& refBinderTexture = ptrSkyboxTexture->MDY_PRIVATE(GetTextureBinderReference)();
  if (refBinderTexture.IsResourceExist() == false) { return; } // Failure check

  this->mBinderMeshSkybox->BindVertexArray();

  using EUniform = EUniformVariableType;
  this->mBinderShdSkybox->TryUpdateUniform<EUniform::Float>(
    "uExposure",
    ptrSkyboxTexture->GetExposure());
  this->mBinderShdSkybox->TryUpdateUniform<EUniform::Float>(
    "uRotationDegree", 
    ptrSkyboxTexture->GetRotationDegree());
  this->mBinderShdSkybox->TryUpdateUniform<EUniform::Vector3>(
    "uTintColor",    
    static_cast<DVec3>(ptrSkyboxTexture->GetTintColor()));
  this->mBinderShdSkybox->TryInsertTextureRequisition(0, refBinderTexture->GetTextureId());
  //this->mBinderShdSkybox->TryInsertTextureRequisition(1, this->mBinderAttUnlit->GetSourceAttachmentId());

  this->mBinderShdSkybox->UseShader();
  this->mBinderShdSkybox->TryUpdateUniformList();

  if (this->mBinderMeshSkybox->IsEnabledIndices() == true)
  {
    XGLWrapper::Draw(EDrawType::Triangle, true, this->mBinderMeshSkybox->GetIndicesCounts()); 
  }
  else
  {
    XGLWrapper::Draw(EDrawType::Triangle, false, this->mBinderMeshSkybox->GetVertexCounts()); 
  }

  this->mBinderShdSkybox->DisuseShader();
}

void FBtRenderItemSkybox::OnReleaseRenderingSetting()
{
  this->mBinderFbSkyRend->UnbindFrameBuffer();
}

void FBtRenderItemSkybox::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace
