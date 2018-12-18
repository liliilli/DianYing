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
#include <Dy/Core/Rendering/Pipeline/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>

#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>

namespace dy
{

FDyFinalScreenDisplayRenderer::FDyFinalScreenDisplayRenderer()
{
  MDY_ASSERT(this->mBinderAttSceneFinal.IsResourceExist() == true,  "Scene final output must be valid.");
  MDY_ASSERT(this->mBinderAttUIFinal.IsResourceExist() == true,     "UI final output must be valid.");

  this->mBinderShader->UseShader();
  const auto id = this->mBinderShader->GetShaderProgramId();
  glUniform1i(glGetUniformLocation(id, "uSceneTexture"), 0);
  glUniform1i(glGetUniformLocation(id, "uUiTexture")   , 1);
  this->mBinderShader->DisuseShader();
}

FDyFinalScreenDisplayRenderer::~FDyFinalScreenDisplayRenderer() { }

void FDyFinalScreenDisplayRenderer::RenderScreen()
{
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderTriangle.IsResourceExist() == false
  ||  this->mBinderAttSceneFinal.IsResourceExist() == false) { return; }

  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "");
  // Bind vertex array

  // Set
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  this->mBinderShader->UseShader();
  const auto& mesh = submeshList[0];
  glBindVertexArray(mesh.GetVertexArrayId());

  // Bind g-buffers as textures and draw.
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttSceneFinal->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttUIFinal->GetAttachmentId());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  // Rewind
  glBindVertexArray(0);
  this->mBinderShader->DisuseShader();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyFinalScreenDisplayRenderer::Clear()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} /// ::dy namespace