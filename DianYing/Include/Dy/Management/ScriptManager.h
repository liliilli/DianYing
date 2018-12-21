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
#include <Dy/Component/UI/CDyWidgetScriptCpp.h>
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

  /// @brief
  void UpdateWidget(_MIN_ TF32 dt);

private:
  sol::state mLua;

  using TDyWidgetScriptList = std::vector<std::unique_ptr<FDyWidgetScriptState>>;
  TDyWidgetScriptList mInsertWidgetScriptList = {};
  TDyWidgetScriptList mWidgetScriptList       = {};
  TDyWidgetScriptList mGCedWidgetScriptList   = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_SCRIPTMANAGER_H