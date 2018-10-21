#ifndef GUARD_DY_MANAGEMENT_EDITOR_GUISETTING_H
#define GUARD_DY_MANAGEMENT_EDITOR_GUISETTING_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

namespace dy::editor
{

class MDyEditorSetting final : public ISingleton<MDyEditorSetting>
{
  MDY_SINGLETON_PROPERTIES(MDyEditorSetting);
  MDY_SINGLETON_DERIVED(MDyEditorSetting);
private:
  // Check if project is loaded or not. IF not, all buttons related to project will be deactivated.
  bool mIsLoadedProject             = false;
  //
  bool mIsProcessingBackground      = false;
  // Enable rendering grid or not.
  bool mIsEnabledViewportRenderGrid = true;

  //
  TU32 mInitScreenSizeWidth         = MDY_INITIALIZE_DEFUINT;
  //
  TU32 mInitScreensizeHeight        = MDY_INITIALIZE_DEFUINT;

public:
  MDY_GETSET(mIsLoadedProject);
  MDY_GETSET(mIsProcessingBackground);
  MDY_GETSET(mIsEnabledViewportRenderGrid);
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_MANAGEMENT_EDITOR_GUISETTING_H