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
#include <Dy/Core/Rendering/Pipeline/UIBasicRenderer.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Management/FontManager.h>
#include <Dy/Management/WorldManager.h>

namespace dy
{

FDyUIBasicRenderer::FDyUIBasicRenderer()
{
  MDY_ASSERT(this->mBinderFrameBuffer.IsResourceExist() == true, "UI Framebuffer must be valid.");

  // @TODO TEMPORAL
  MDY_CALL_ASSERT_SUCCESS(MDyFont::GetInstance().CreateFontResourceContainer("Arial"));
}

FDyUIBasicRenderer::~FDyUIBasicRenderer()
{ }

void FDyUIBasicRenderer::RenderScreen(_MIN_ std::vector<NotNull<FDyUiObject*>>& uiRenderList)
{
  this->mBinderFrameBuffer->BindFrameBuffer();
  
  for (auto& ptrRenderUi : uiRenderList)
  {
    ptrRenderUi->Render();
  }
  
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FDyUIBasicRenderer::Clear()
{ 
  glBindFramebuffer(GL_FRAMEBUFFER, this->mBinderFrameBuffer->GetFrameBufferId());
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} /// ::dy namespace