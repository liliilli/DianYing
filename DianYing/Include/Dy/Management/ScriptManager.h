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
#include <Dy/Component/Internal/ScriptState.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyScriptType;
class FDyActor;
class FDyUiWidget;
class FDyActorScriptState;
class FDyWidgetScriptState;
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

/// @class MDyScript
/// @brief Manages script (resource loading, animation, actor script, ui script, etc).
class MDyScript final : public IDySingleton<MDyScript>
{
public:
  MDY_SINGLETON_DERIVED(MDyScript);
  MDY_SINGLETON_PROPERTIES(MDyScript);

  /// @brief Get reference of lua instance.
  /// @return lua instance l-value reference.
  MDY_NODISCARD sol::state& GetLuaInstance() noexcept;

  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefWidget
  /// @param iIsAwakened
  MDY_NODISCARD FDyWidgetScriptState* 
  CreateWidgetScript(const std::string& iScriptSpecifier, FDyUiWidget& iRefWidget, bool iIsAwakened);
  /// @brief Try remove widget script from dy system.
  /// But, removed widget script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardWidgetScriptToGCList(_MIN_ const FDyWidgetScriptState* iPtrWidgetScriptState);
  /// @brief Try move inserted widget script to main container.
  void TryMoveInsertWidgetScriptToMainContainer();
  
  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefActor
  /// @param iIsAwakened
  MDY_NODISCARD FDyActorScriptState* 
  CreateActorScript(const std::string& iScriptSpecifier, FDyActor& iRefActor, bool iIsAwakened);
  /// @brief Try remove actor script from dy system.
  /// But, removed actor script does not actually removed instantly, \n
  /// moved gc list and removed actually on next frame prior to update.
  EDySuccess TryForwardActorScriptToGCList(_MIN_ const FDyActorScriptState* iPtrWidgetScriptState);
  /// @brief Try move inserted actor script to main container.
  void TryMoveInsertActorScriptToMainContainer();

  /// @brief Update widget script.
  void UpdateWidgetScript(_MIN_ TF32 dt);
  /// @brief Update widget script if only script present type is type.
  void UpdateWidgetScript(_MIN_ TF32 dt, _MIN_ EDyScriptState type);
  /// @brief Check widget script that must be gced is exist on list.
  MDY_NODISCARD bool IsGcedWidgetScriptExist() const noexcept;
  /// @brief Call `destroy` GCed widget script 
  void CallDestroyFuncWidgetScriptGCList();
  /// @brief Clear widget script gc list `mGCedWidgetScriptList` anyway.
  void ClearWidgetScriptGCList();
  /// @brief remove emptied script list.
  void RemoveEmptyOnWidgetScriptList();

  /// @brief Update actor script.
  void UpdateActorScript(_MIN_ TF32 dt);
  /// @brief Update actor script if only script present type is type.
  void UpdateActorScript(_MIN_ TF32 dt, _MIN_ EDyScriptState type);
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

private:
  class Impl; Impl* mPimpl = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H