#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYWIDGETCPPSCRIPT_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYWIDGETCPPSCRIPT_H
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

#include <Dy/Component/Interface/IDyScriptable.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyWidgetScriptCpp;
class FDyUiWidget;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

MDY_ABSTRACT ADyWidgetCppScript : public IDyScriptable
{
public:
  ///
  /// @brief Get widget reference.
  ///
  MDY_NODISCARD FDyUiWidget& GetWidgetReference();

private:
  ///
  /// @brief Set reference to outside world.
  /// @param outsideReference Reference of WidgetScriptCpp instance.
  ///
  void pfSetOutsideReference(CDyWidgetScriptCpp& outsideReference) noexcept
  {
    this->mOutside = &outsideReference;
  }

  CDyWidgetScriptCpp* mOutside = nullptr;

  friend class CDyWidgetScriptCpp;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYWIDGETCPPSCRIPT_H