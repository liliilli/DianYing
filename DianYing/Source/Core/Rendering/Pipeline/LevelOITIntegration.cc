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
#include <Dy/Core/Rendering/Pipeline/LevelOITIntegration.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Element/Actor.h>

namespace dy
{

bool FDyLevelOITIntegration::IsReady() const noexcept
{
  return 
      this->mBinderFrameBuffer.IsResourceExist() == true
  &&  this->mBinderShader.IsResourceExist() == true
  &&  this->mBinderTriangle.IsResourceExist() == true;
}

EDySuccess FDyLevelOITIntegration::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->Clear();

  this->mBinderFrameBuffer->BindFrameBuffer();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  /// TEMPORARY
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  this->mBinderShader->UseShader();
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttOITColor->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttOITWeigh->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderAttOpaqScre->GetAttachmentId());
  this->mBinderShader.TryUpdateUniformList();

  return DY_SUCCESS;
}

void FDyLevelOITIntegration::RenderScreen()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  this->mBinderTriangle->BindVertexArray();
  this->mBinderShader->UseShader();

  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

  FDyGLWrapper::UnbindVertexArrayObject();
  this->mBinderShader->DisuseShader();
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FDyLevelOITIntegration::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace