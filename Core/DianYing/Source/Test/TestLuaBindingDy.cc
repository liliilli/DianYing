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
#include <Dy/Test/testLua.h>
#if defined(MDY_FLAG_TEST_ENABLED)

#include <sol2/sol.hpp>
#include <Dy/Helper/GlobalType.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Binding/LuaScriptFrame.h>

namespace dy::test
{

void DyLuaBindingDyTest()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  lua.new_enum<EDyLogLevel>("EDyLogLevel",
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
  lua.new_usertype<MDyLog>("MDyLog",
      "new", sol::no_constructor,
      "GetInstance", &MDyLog::GetInstance,
      "PushLogString", &MDyLog::PushLog<>,
      "PushLog", &MDyLog::PushLog<TI32>
  );

  lua.safe_script(R"dy(
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

  lua.safe_script(R"dy(
DerivedScript = LDyScript:new()

function DerivedScript:Initiate()
  print("Hello world!")
  MDyLog:GetInstance():PushLog(EDyLogLevel.Critical, "Good bye world! by Dy. {0}", 17)
  MDyLog:GetInstance():PushLogString(EDyLogLevel.Debug, "Good bye world! by Dy. Another version")
end
)dy");

  lua.safe_script(R"dy(
instance = DerivedScript:new()
instance:Initiate()
)dy");

#ifdef false
  // Read user customed type from lua script file, you bind things using the `new_usertype` and `set_usertype` methods as shown below.
  // These methods are on both `table` and `state(_view)`.
  lua.new_usertype<LDyScript>("LDyScript",
      // 3 constructors
      sol::constructors<LDyScript(sol::this_state)>(),
      // Functions
      "Initiate",   &LDyScript::Initiate,
      "Start",      &LDyScript::Start,
      "Update",     &LDyScript::Update,
      "Destroy",    &LDyScript::Destroy,
      "OnEnabled",  &LDyScript::OnEnabled,
      "OnDisabled", &LDyScript::OnDisabled
  );
  lua.safe_script(R"dy(

)dy");

  lua.script(R"dy(
Account = {balance = 0}

function Account:new (o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function Account:deposit (v)
  self.balance = self.balance + v
end

function Account:withdraw (v)
  if v > self.balance then error"insufficient funds" end
  self.balance = self.balance - v
end
)dy");

  lua.script(R"dy(
SpecialAccount = Account:new()

function SpecialAccount:withdraw (v)
  if v - self.balance >= self:getLimit() then
    error"insufficient funds"
  end
  self.balance = self.balance - v
end

function SpecialAccount:getLimit ()
  return self.balance * 0.10
end

s = SpecialAccount:new{limit=1000.00}
s:deposit(100.0)
)dy");

  lua.script(R"dy(
Instance = DerivedScriptClass.new()
Instance:Initiate()
)dy");

  lua.safe_script(R"dy(
LDyScript = {

}
)dy");

  lua.safe_script(R"dy(
LDyScript = {
    new = function(self, v)
      o = v or {}
      setmetatable(o, self)
      self.__index = self
      return o
    end

  , Initiate = function(self)
      print("Hello world!", self.mValue)
      self.mValue = self.mValue + 50
    end

  , mValue = 50
}

DerivedScript = LDyScript:new{
    Initiate = function(self)
       print("Bye world!", self.mValue)
       self.mValue = self.mValue - 50
    end

  , Update = function(self, dt)
      print(dt)
    end

  , mValue = 1000
}

a = LDyScript:new()
LDyScript = nil
a:Initiate()
a:Initiate()
a:Initiate()
a:Initiate()

b = DerivedScript:new()
b:Initiate()
b:Initiate()
b:Initiate()
b:Initiate()
b:Update(0.0167)
)dy");
#endif
}

} /// ::dy::test namespace

#endif