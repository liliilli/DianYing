#include <precompiled.h>
#if defined(MDY_FLAG_IN_EDITOR)
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
#include <Dy/Management/Editor/GuiManager.h>
#include <Dy/Management/LoggingManager.h>

namespace dy::editor
{

EDySuccess MDyEditorGui::pfInitialize()
{
  MDY_LOG_INFO_D("MDyEditorGui manager initialized.");

  auto ptr = FDyEditorGuiWindowFactory::CreateGuiWindow<FDyMainMenu>(PDyGuiComponentEmptyDescriptor{});
  PHITOS_ASSERT(static_cast<bool>(ptr), "Failed to create MainMenu!");

  this->mMainMenu = std::unique_ptr<FDyMainMenu>(static_cast<FDyMainMenu*>(ptr.release()));

  return DY_SUCCESS;
}

EDySuccess MDyEditorGui::pfRelease()
{
  MDY_LOG_INFO_D("MDyEditorGui manager released.");
  return DY_SUCCESS;
}

void MDyEditorGui::DrawWindow(float dt) noexcept
{
  if (mMainMenu)
  {
    this->mMainMenu->DrawWindow(dt);
  }
}

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR