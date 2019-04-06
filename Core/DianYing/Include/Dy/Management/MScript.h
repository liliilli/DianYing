#ifndef GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H
#define GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Component/Internal/EScriptState.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyScriptType;
class FActor;
class FWidget;
class FActorScriptState;
class FWidgetScriptState;
} /// ::dy namespace.

namespace sol
{
class state;
} /// ::sol namespace.

//!
//! Implementation
//!

namespace dy
{

/// @class MScript
/// @brief Manages script (resource loading, animation, actor script, ui script, etc).
class MScript final : public ISingleton<MScript>
{
public:
  DY_PIMPL_SINGELTON_DERIVED(MScript);
  MDY_SINGLETON_PROPERTIES(MScript);

  /// @brief Get reference of lua instance.
  /// @return lua instance l-value reference.
  MDY_NODISCARD sol::state& GetLuaInstance() noexcept;

  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefWidget
  /// @param iIsAwakened
  MDY_NODISCARD FWidgetScriptState* 
  CreateWidgetScript(const std::string& iScriptSpecifier, FWidget& iRefWidget, bool iIsAwakened);
  /// @brief Try remove widget script from dy system.
  /// But, removed widget script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardWidgetScriptToGCList(const FWidgetScriptState* iPtrWidgetScriptState);
  /// @brief Try move inserted widget script to main container.
  void TryMoveInsertWidgetScriptToMainContainer();
  
  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefActor
  /// @param iIsAwakened
  MDY_NODISCARD FActorScriptState* 
  CreateActorScript(const std::string& iScriptSpecifier, FActor& iRefActor, bool iIsAwakened);
  /// @brief Try remove actor script from dy system.
  /// But, removed actor script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardActorScriptToGCList(const FActorScriptState* iPtrWidgetScriptState);
  /// @brief Try move inserted actor script to main container.
  void TryMoveInsertActorScriptToMainContainer();

  /// @brief Update widget script.
  void UpdateWidgetScript(TF32 dt);
  /// @brief Update widget script if only script present type is type.
  void UpdateWidgetScript(TF32 dt, EScriptState type);
  /// @brief Check widget script that must be gced is exist on list.
  MDY_NODISCARD bool IsGcedWidgetScriptExist() const noexcept;
  /// @brief Call `destroy` GCed widget script 
  void CallDestroyFuncWidgetScriptGCList();
  /// @brief Clear widget script gc list `mGCedWidgetScriptList` anyway.
  void ClearWidgetScriptGCList();
  /// @brief remove emptied script list.
  void RemoveEmptyOnWidgetScriptList();

  /// @brief Update actor script.
  void UpdateActorScript(TF32 dt);
  /// @brief Update actor script if only script present type is type.
  void UpdateActorScript(TF32 dt, EScriptState type);
  /// @brief Check there are gced -candidate actor script instances.
  MDY_NODISCARD bool IsGcedActorScriptExist() const noexcept;
  /// @brief Call `destroy` actor script 
  void CallDestroyFuncActorScriptGCList();
  /// @brief Clear actor script gc list `mGCedActorScriptList` anyway.
  void ClearActorScriptGCList();

  /// @brief Create global script instance list.
  /// This function must be called once per application runtime.
  void CreateGlobalScriptInstances();
  /// @brief Remove global script instance list.
  /// This function must be called once per application runtime.
  void RemoveGlobalScriptInstances();
  /// @brief Call `onStart` function to all global script.
  void CallonStartGlobalScriptList();
  /// @brief Call `onEnd` function to all global script.
  void CallonEndGlobalScriptList();
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H