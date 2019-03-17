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

#include <Dy/Component/Interface/IScriptable.h>

//!
//! Forward declaration
//!

namespace dy
{

class FWidget;

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CBaseWidgetScript
/// @brief Widget script base type. this type should not be used independently. \n
/// Instead of this, just use `CWidgetScript`.
class CBaseWidgetScript : public IScriptable
{
public:
  explicit CBaseWidgetScript(FWidget& ioWidget); 

  /// @brief Get mutable reference of widget.
  MDY_NODISCARD FWidget& GetWidgetReference() noexcept;

protected:
  /// Script name for specification and searching.
  std::string  mScriptName;
  /// Flag for checking binded script instance.
  bool         mIsScriptInstanceBinded = false;

private:
  FWidget* mBindedWidget = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_UI_CDYWIDGETSCRIPTBASE_H