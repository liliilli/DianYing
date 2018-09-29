#pragma once
#define SOL_CHECK_ARGUMENT 1
#include <sol2/sol.hpp>

namespace dy
{

void DyLuaHelloWorld();
void DyLuaFundamental01();
void DyLuaFundamental02();

/// Entry point of lua testing
void DyLuaEntryPoint()
{
  DyLuaHelloWorld();
  DyLuaFundamental01();
  DyLuaFundamental02();
}

/// Just hello world!
void DyLuaHelloWorld()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);

  int value = lua.script("return 54");
  MDY_LOG_CRITICAL_D("Hello world Lua! : {}", value);
}

/// Test 01
void DyLuaFundamental01()
{
  sol::state lua;
  int x = 0;
  lua.set_function("Beep", [&x]()
  {
    ++x;
  });
  lua.script("Beep()");
  PHITOS_ASSERT(x == 1, "LUA TEST FAILED");

  sol::function fnBeep = lua["Beep"];
  (void)fnBeep();
  PHITOS_ASSERT(x == 2, "LUA TEST FAILED");
}

void DyLuaFundamental02()
{
  struct DDyVars
  {
    TI32 mBoop = 0;

    [[nodiscard]] TI32 Bop() const
    {
      return this->mBoop + 1;
    }
  };

  sol::state lua;
  lua.new_usertype<DDyVars>(
      "DDyVars",
      "mBoop", &DDyVars::mBoop,
      "Bop", &DDyVars::Bop
  );
  lua.script(R"dy(
Beep = DDyVars.new()
Beep.mBoop = 1
bopValue = Beep:Bop()
)dy");

  DDyVars& beep = lua["Beep"];
  TI32 bopValue = lua["bopValue"];

  PHITOS_ASSERT(beep.mBoop == 1, "LUA TEST FAILED");
  PHITOS_ASSERT(lua.get<DDyVars>("Beep").mBoop == 1, "LUA TEST FAILED");
  PHITOS_ASSERT(beep.Bop() == 2, "LUA TEST FAILED");
  PHITOS_ASSERT(bopValue = 2, "LUA TEST FAILED");

  MDY_LOG_CRITICAL("DyLuaFundamental02 Success");
}

} /// ::dy namespace
