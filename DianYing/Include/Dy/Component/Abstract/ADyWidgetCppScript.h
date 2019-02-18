#ifndef GUARD_DY_COMPONENT_ABSTRACT_ADYWIDGETCPPSCRIPT_H
#define GUARD_DY_COMPONENT_ABSTRACT_ADYWIDGETCPPSCRIPT_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Component/Interface/IDyScriptable.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyWidgetScriptCpp;
class FDyUiWidget;
class FDyTimerHandle;
class MDyGameTimer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

MDY_ABSTRACT ADyWidgetCppScript : public IDyScriptable
{
public:
  /// @brief Get widget reference.
  MDY_NODISCARD FDyUiWidget& GetWidgetReference();
  
  /// @brief Get Game-Timer manager.
  MDY_NODISCARD MDyGameTimer& GetGameTimerManager() noexcept;

  void MDY_PRIVATE(BindPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE(DetachPtrTimerHandle)(_MIN_ FDyTimerHandle& iRefTimerHandler);
  void MDY_PRIVATE(AbortAllValidTimerHandler)();
private:
  /// @brief Set reference to outside world.
  /// @param outsideReference Reference of WidgetScriptCpp instance.
  void pfSetOutsideReference(_MIN_ CDyWidgetScriptCpp& outsideReference) noexcept;

  CDyWidgetScriptCpp* mOutside = nullptr;

  std::vector<NotNull<FDyTimerHandle*>> mPtrTimerHandleList = {};
  friend class CDyWidgetScriptCpp;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_ABSTRACT_ADYWIDGETCPPSCRIPT_H