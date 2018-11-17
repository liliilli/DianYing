#ifndef GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPT_H
#define GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPT_H
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

#include <Dy/Element/Interface/IDyScriptable.h>

//!
//! Forward declaration
//!

namespace dy
{

class FDyUiWidget;

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyWidgetScript
/// @brief
///
class CDyWidgetScript : public IDyScriptable
{
public:
  CDyWidgetScript(_MINOUT_ FDyUiWidget& widget) : mBindedWidget(&widget) { }

  ///
  /// @brief Release derived resources of `CDyScript`.
  /// This function shadowing derived Release function intentionally.
  ///
  void Release()
  {
    return this->pScriptRelease();
  }

protected:
  /// @brief
  virtual void pScriptRelease() = 0;

  /// Script name for specification and searching.
  MDY_TRANSIENT std::string mScriptName   = MDY_INITIALIZE_EMPTYSTR;
  /// Flag for checking binded script instance.
  bool                      mIsScriptInstanceBinded = false;

private:
  /// Binded actor raw pointer.
  FDyUiWidget*              mBindedWidget = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPT_H