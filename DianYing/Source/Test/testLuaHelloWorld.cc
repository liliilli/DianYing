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
#include <sol2/sol.hpp>
#include <Dy/Helper/GlobalType.h>
#include <Dy/Management/LoggingManager.h>

namespace dy::test
{

/// Just hello world!
void DyLuaHelloWorld()
{
  // The first line creates the lua_State and will hold onto it for the duration of the scope
  // its declared in (e.g., from the opening { to the closing }).
  // It will automatically close / cleanup that lua state when it gets destructed.
  sol::state lua;

  // The second line opens a single lua-provided library, “base”.
  // There are several other libraries that come with lua that you can open by default,
  // and those are included in the sol::lib enumeration
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

} /// ::dy::test namespace