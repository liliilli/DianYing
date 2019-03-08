#ifndef GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTBASE_H
#define GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTBASE_H
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

#include <Dy/Component/Interface/IDyScriptable.h>

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
/// @class CDyWidgetScriptBase
/// @brief Widget script base type. this type should not be used independently. \n
/// Instead of this, just use `CDyWidgetScript`.
///
class CDyWidgetScriptBase : public IDyScriptable
{
public:
  CDyWidgetScriptBase(_MINOUT_ FDyUiWidget& widget) : mBindedWidget(&widget) { }

  /// @brief Get mutable reference of widget.
  MDY_NODISCARD FDyUiWidget& GetWidgetReference() noexcept
  {
    MDY_ASSERT_MSG(this->mBindedWidget != nullptr, "Unexpected error occurred.");
    return *this->mBindedWidget;
  }

protected:
  /// Script name for specification and searching.
  std::string   mScriptName   = MDY_INITIALIZE_EMPTYSTR;
  /// Flag for checking binded script instance.
  bool          mIsScriptInstanceBinded = false;

private:
  FDyUiWidget*  mBindedWidget = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTBASE_H