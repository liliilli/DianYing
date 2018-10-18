#include <precompiled.h>
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

/// Header file
#include <Dy/Management/ScriptManager.h>
#include <Dy/Management/LoggingManager.h>

//!
//! Local function & forward declaration
//!

namespace dy
{

///
MDY_SET_IMMUTABLE_STRING(sCDyScriptFrame,
R"dy(
LDyScript = {
    new = function(self, v)
      o = v or {}
      setmetatable(o, self)
      self.__index = self
      return o
    end

  , Initiate = function(self) end
  , Start = function(self) end
  , Update = function(self, dt) end
  , Destroy = function(self) end
  , OnEnabled = function(self) end
  , OnDisabled = function(self) end
}
)dy");

///
/// @brief  Initialize MDyLog functionality to lua to use it.
/// @param  lua Lua binding instance
///
void DyInitializeMDyLog(_MIO_ sol::state& lua)
{
  /// Binding lua
  lua.new_enum<dy::EDyLogLevel>("EDyLogLevel",
    {
      { "Trace",        EDyLogLevel::Trace },
      { "Debug",        EDyLogLevel::Debug },
      { "Information",  EDyLogLevel::Information },
      { "Warning",      EDyLogLevel::Warning },
      { "Error",        EDyLogLevel::Error },
      { "Critical",     EDyLogLevel::Critical }
    }
  );

  /// When binding lua, destructor must be public and can be seen from outside.
  lua.new_usertype<dy::MDyLog>("MDyLog",
      "new",            sol::no_constructor,
      "GetInstance",    &MDyLog::GetInstance,
      "PushLog",        &MDyLog::PushLog<>,
      "SetVisibleLevel",&MDyLog::SetVisibleLevel
  );
}

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyScript::pfInitialize()
{
  mLua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

  /// Manager binding
  DyInitializeMDyLog(mLua);
  mLua.safe_script(sCDyScriptFrame);

  mLua.safe_script(R"dy(
DerivedScript = LDyScript:new()

function DerivedScript:Initiate()
  print("Hello world!")
  MDyLog:GetInstance():PushLog(EDyLogLevel.Critical, "Good bye world! by Dy.")
  MDyLog:GetInstance():PushLog(EDyLogLevel.Debug, "Good bye world! by Dy. Another version")
end
)dy");

  mLua.safe_script(R"dy(
instance = DerivedScript:new()
instance:Initiate()
)dy");

  return DY_SUCCESS;
}

EDySuccess MDyScript::pfRelease()
{
  return DY_SUCCESS;
}

} /// ::dy namespace