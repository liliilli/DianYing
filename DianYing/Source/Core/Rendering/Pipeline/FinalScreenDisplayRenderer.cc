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
#include <Dy/Core/Rendering/Pipeline/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>

#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

namespace dy
{

void FDyFinalScreenDisplayRenderer::RenderScreen()
{
  if (this->IsReady() == false) { return; }

  // Bind vertex array object.
  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "");
  (*submeshList[0])->BindVertexArray();

  // Set shader 
  this->mBinderShader->TryInsertTextureRequisition(0, this->mBinderAttSceneFinal->GetAttachmentId());
  this->mBinderShader->TryInsertTextureRequisition(1, this->mBinderAttUIFinal->GetAttachmentId());
  this->mBinderShader->TryInsertTextureRequisition(2, this->mBinderAttDbgFinal->GetAttachmentId());

  this->mBinderShader->UseShader();
  this->mBinderShader->TryUpdateUniformList();

  // Bind g-buffers as textures and draw.
  FDyGLWrapper::Draw(EDyDrawType::Triangle, false, 3);

  // Rewind
  FDyGLWrapper::UnbindVertexArrayObject();
  this->mBinderShader->DisuseShader();
}

void FDyFinalScreenDisplayRenderer::Clear()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FDyFinalScreenDisplayRenderer::IsReady() const noexcept
{
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderTriangle.IsResourceExist() == false
  ||  this->mBinderAttSceneFinal.IsResourceExist() == false) { return false; }

  return true;
}

EDySuccess FDyFinalScreenDisplayRenderer::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return DY_SUCCESS;
}

EDySuccess FDyFinalScreenDisplayRenderer::TryPopRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return DY_SUCCESS;
}

} /// ::dy namespace