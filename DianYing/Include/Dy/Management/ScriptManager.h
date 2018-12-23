#ifndef GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H
#define GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H
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

#define SOL_CHECK_ARGUMENT 1
#include <sol2/sol.hpp>
#include <Dy/Component/Internal/Actor/FDyActorScriptStatus.h>
#include <Dy/Component/Internal/Script/FDyWidgetScriptStatus.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyScriptType;
class FDyUiWidget;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDyScript
/// @brief
///
class MDyScript final : public IDySingleton<MDyScript>
{
  MDY_SINGLETON_DERIVED(MDyScript);
  MDY_SINGLETON_PROPERTIES(MDyScript);
public:
  /// @brief Get reference of lua instance.
  /// @return lua instance l-value reference.
  MDY_NODISCARD sol::state& GetLuaInstance() noexcept;

  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefWidget
  /// @param iIsAwakened
  MDY_NODISCARD FDyWidgetScriptState* 
  CreateWidgetScript(_MIN_ const std::string& iScriptSpecifier, _MIN_ FDyUiWidget& iRefWidget, _MIN_ bool iIsAwakened);

  /// @brief
  void TryMoveInsertWidgetScriptToMainContainer();
  
  /// @brief Create widget script. \n
  /// @TODO IMPLEMENT LUA VERSION. 
  /// @param iScriptSpecifier
  /// @param iRefActor
  /// @param iIsAwakened
  MDY_NODISCARD FDyActorScriptState* 
  CreateActorScript(_MIN_ const std::string& iScriptSpecifier, _MIN_ FDyActor& iRefActor, _MIN_ bool iIsAwakened);

  /// @brief Update widget script.
  void UpdateWidgetScript(_MIN_ TF32 dt);
  /// @brief Update widget script if only script present type is type.
  void UpdateWidgetScript(_MIN_ TF32 dt, _MIN_ EDyScriptState type);

  /// @brief Update actor script.
  void UpdateActorScript(_MIN_ TF32 dt);
  /// @brief Update actor script if only script present type is type.
  void UpdateActorScript(_MIN_ TF32 dt, _MIN_ EDyScriptState type);

private:
  sol::state mLua;

  using TDyWidgetScriptList = std::vector<std::unique_ptr<FDyWidgetScriptState>>;
  TDyWidgetScriptList mInsertWidgetScriptList = {};
  TDyWidgetScriptList mWidgetScriptList       = {};
  TDyWidgetScriptList mGCedWidgetScriptList   = {};

  /// Activated CDyScript component list.
  /// this list must not be invalidated when iterating list, but except for unenrolling.
  using TDyActorScriptList = std::vector<std::unique_ptr<FDyActorScriptState>>;
  TDyActorScriptList  mInsertActorScriptList  = {};
  TDyActorScriptList  mActorScriptList        = {};
  TDyActorScriptList  mGCedActorScriptList    = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H