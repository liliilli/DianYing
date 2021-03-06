#ifndef GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDYWIDGETSCRIPTSTATUS_H
#define GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDYWIDGETSCRIPTSTATUS_H
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

#include <Dy/Component/Internal/EScriptState.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Component/Internal/Widget/CBaseWidgetScript.h>

//!
//! Forward declaration
//!

namespace dy
{
class FWidget;
struct PDyScriptInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FWidgetScriptState
/// @brief Status binding type for widget script easing calling functions.
class FWidgetScriptState final
{
public:
  FWidgetScriptState(FWidget& iWidgetReference, const PDyScriptInstanceMetaInfo& iDescriptor); 

  /// @brief Call widget script function properly.
  void CallScriptFunction(TF32 dt) noexcept;

  /// @brief Call destroy function.
  void MDY_PRIVATE(CallDestroyFunctionAnyway)() noexcept;

  /// @brief Get script type (Cpp, Lua).
  MDY_NODISCARD EDyScriptType GetScriptType() const noexcept;

  /// @brief Get script present status.
  MDY_NODISCARD EScriptState GetScriptStatus() const noexcept;

  /// @brief Get internal widget script instance (level 2)
  MDY_NODISCARD CBaseWidgetScript* MDY_PRIVATE(GetPtrInternalWidgetScript)() const noexcept;

private:
  /// @brief Widget script pointer instance.
  std::unique_ptr<CBaseWidgetScript> mScriptInstance = MDY_INITIALIZE_NULL;
  /// @brief Status of this widget script.
  EScriptState mStatus  = EScriptState::NoneError;
  /// @brief Internal script type of this status instance.
  EDyScriptType  mType    = EDyScriptType::NoneError;
};

} /// ::dy namespace

#endif /// GAURD_DY_COMPONENT_INTERNAL_SCRIPT_FDYWIDGETSCRIPTSTATUS_H
