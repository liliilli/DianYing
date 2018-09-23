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

///
/// @macro MDY_GETSET
/// @brief Helping construction of get and set boilerplate function of member variable.
///
#define MDY_GETSET(__MAVariable__)                                                    \
  [[nodiscard]] const decltype(__MAVariable__)& Get##__MAVariable__() const noexcept  \
  {                                                                                   \
    return this->__MAVariable__;                                                      \
  }                                                                                   \
                                                                                      \
  void Set##__MAVariable__(const decltype(__MAVariable__)& input##__MAVariable__)     \
  {                                                                                   \
    this->__MAVariable__ = input##__MAVariable__;                                     \
  }

namespace dy::editor
{

class MDyEditorSetting final : public ISingleton<MDyEditorSetting>
{
  MDY_SINGLETON_PROPERTIES(MDyEditorSetting);
  MDY_SINGLETON_DERIVED(MDyEditorSetting);
private:
  // Enable rendering grid or not.
  bool mIsEnabledViewportRenderGrid = true;
  TU32 mInitScreenSizeWidth         = MDY_NOT_INITIALIZED_0;
  TU32 mInitScreensizeHeight        = MDY_NOT_INITIALIZED_0;

public:
  MDY_GETSET(mIsEnabledViewportRenderGrid);
};

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR
#endif /// GUARD_DY_MANAGEMENT_EDITOR_GUISETTING_H