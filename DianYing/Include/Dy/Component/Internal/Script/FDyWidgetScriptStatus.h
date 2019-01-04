#ifndef GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDYWIDGETSCRIPTSTATUS_H
#define GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDYWIDGETSCRIPTSTATUS_H
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

#include <Dy/Component/Internal/ScriptState.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Component/Internal/Widget/CDyWidgetScriptBase.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyUiWidget;
struct PDyScriptInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyWidgetScriptState
/// @brief Status binding type for widget script easing calling functions.
///
class FDyWidgetScriptState final
{
public:
  FDyWidgetScriptState(_MIN_ FDyUiWidget& widgetReference, _MIN_ const PDyScriptInstanceMetaInfo& descriptor); 

  /// @brief Call widget script function properly.
  void CallScriptFunction(_MIN_ float dt) noexcept;

  /// @brief Call destroy function.
  void MDY_PRIVATE_SPECIFIER(CallDestroyFunctionAnyway)() noexcept;

  /// @brief Get script type (Cpp, Lua).
  MDY_NODISCARD EDyScriptType GetScriptType() const noexcept;

  /// @brief Get script present status.
  MDY_NODISCARD EDyScriptState GetScriptStatus() const noexcept;

  /// @brief Get internal widget script instance (level 2)
  MDY_NODISCARD CDyWidgetScriptBase* MDY_PRIVATE_SPECIFIER(GetPtrInternalWidgetScript)() const noexcept;

private:
  /// @brief Widget script pointer instance.
  std::unique_ptr<CDyWidgetScriptBase> mScriptInstance = MDY_INITIALIZE_NULL;
  /// @brief Status of this widget script.
  EDyScriptState mStatus  = EDyScriptState::NoneError;
  /// @brief Internal script type of this status instance.
  EDyScriptType  mType    = EDyScriptType::NoneError;
};

} /// ::dy namespace

#endif /// GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDYWIDGETSCRIPTSTATUS_H
