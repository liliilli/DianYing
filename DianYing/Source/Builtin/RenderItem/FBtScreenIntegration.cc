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

#include <Dy/Builtin/RenderItem/FBtScreenIntegration.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Type/EDyDrawType.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Component/CDyLightDirectional.h>
#include <Dy/Element/Level.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>

namespace dy
{

void FBtRenderItemScreenIntegeration::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("f1af3f0b-a654-459a-bd7a-aefa234b389e", true);
}

EDySuccess FBtRenderItemScreenIntegeration::OnPreRenderCheckCondition()
{
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderTriangle.IsResourceExist() == false
  ||  this->mBinderAttSceneFinal.IsResourceExist() == false) 
  { 
    return DY_FAILURE; 
  }

  return DY_SUCCESS;
}

void FBtRenderItemScreenIntegeration::OnSetupRenderingSetting()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBtRenderItemScreenIntegeration::OnRender()
{
  // Bind vertex array object.
  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT_MSG(submeshList.size() == 1, "");
  (*submeshList[0])->BindVertexArray();

  // Set shader 
  this->mBinderShader->TryInsertTextureRequisition(0, this->mBinderAttSceneFinal->GetSourceAttachmentId());
  this->mBinderShader->TryInsertTextureRequisition(1, this->mBinderAttUIFinal->GetSourceAttachmentId());
  this->mBinderShader->TryInsertTextureRequisition(2, this->mBinderAttDbgFinal->GetSourceAttachmentId());

  this->mBinderShader->UseShader();
  this->mBinderShader->TryUpdateUniformList();

  // Bind g-buffers as textures and draw.
  FDyGLWrapper::Draw(EDyDrawType::Triangle, false, 3);

  // Rewind
  FDyGLWrapper::UnbindVertexArrayObject();
  this->mBinderShader->DisuseShader();
}

void FBtRenderItemScreenIntegeration::OnReleaseRenderingSetting()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} /// ::dy namespace
