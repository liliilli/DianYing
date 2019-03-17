#ifndef GUARD_DY_BINDING_LUASCRIPTFRAME_H
#define GUARD_DY_BINDING_LUASCRIPTFRAME_H
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

#include <sol2/sol.hpp>

namespace dy
{

/// @class FLuaScriptFrame
/// @brief Lua script frame for CDyScript.
class FLuaScriptFrame
{
public:
  FLuaScriptFrame(sol::this_state L) :
      mCustomizedInitiate(sol::make_reference<sol::function>(L.lua_state(), &FLuaScriptFrame::Initiate)),
      mCustomizedStart(sol::make_reference<sol::function>(L.lua_state(), &FLuaScriptFrame::Start)),
      mCustomizedUpdate(sol::make_reference<sol::function>(L.lua_state(), &FLuaScriptFrame::Update)),
      mCustomizedDestroy(sol::make_reference<sol::function>(L.lua_state(), &FLuaScriptFrame::Destroy)),
      mCustomizedOnEnabled(sol::make_reference<sol::function>(L.lua_state(), &FLuaScriptFrame::OnEnabled)),
      mCustomizedOnDisabled(sol::make_reference<sol::function>(L.lua_state(), &FLuaScriptFrame::OnDisabled))
  {}

  ///
  /// @brief
  /// This method will be called only one time, when ScriptFrame is attached to
  /// object, and actived first time in game applicaiton.
  ///
  /// This is just optional, you can do not to override Initiate() method
  /// when there is no need.
  ///
  void Initiate() {};

  ///
  /// @brief
  /// This method must be called in Initialization time of script bound to object.
  /// And all derived scripts have to override this,
  /// even though not have components to set up.
  ///
  void Start() {};

  ///
  /// @brief
  /// This method updates information of script instance bound to specific object.
  /// Must be overriden and implemented by derived script class.
  ///
  void Update(float dt) {};

  ///
  /// @brief
  /// This method will be called only one time, when the object are destroyed.
  /// Object's destuctor call all destroy() methods in all scripts bound to object to be vanished.
  /// This is optional method, you could leave it vacant.
  ///
  void Destroy() {};

  ///
  /// @brief
  /// This method will be called
  /// if only bound object is activated from deactivation.
  ///
  void OnEnabled() {};

  ///
  /// @brief
  /// This method will be called
  /// If only bound object is desabled from activation.
  ///
  void OnDisabled() {};

  sol::function mCustomizedInitiate;
  sol::function mCustomizedStart;
  sol::function mCustomizedUpdate;
  sol::function mCustomizedDestroy;
  sol::function mCustomizedOnEnabled;
  sol::function mCustomizedOnDisabled;
};

} /// ::dy namespace

#endif /// GUARD_DY_BINDING_LUASCRIPTFRAME_H