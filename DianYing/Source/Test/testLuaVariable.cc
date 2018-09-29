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

void DyLuaVariablesReading()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("./TestResource/Lua/variables.lua");

  // Can get nested variables.
  const bool isFullScreen = lua["config"]["fullscreen"];
  // the type "sol::state" can behaves exactly like a table.
  sol::table config       = lua["config"];

  sol::table resolution = config.get<sol::table>("resolution");
  // Table and state can have multiple things pulled out of it too.
  // @TODO DOESN'T WORK BUT TUPLE WORKS WELL :(
#ifdef false
  std::pair<TI32, TI32>   xyResolution    = resolution.get<TI32, TI32>("x", "y");
#endif
  // Also tuple too!
  std::tuple<TI32, TI32>  xyResolutionTup = resolution.get<TI32, TI32>("x", "y");

  MDY_LOG_CRITICAL("isFullScreen : {}", isFullScreen);
  MDY_LOG_CRITICAL("xyResolution Tuple : {}, {}", std::get<0>(xyResolutionTup), std::get<1>(xyResolutionTup));
#ifdef false
  MDY_LOG_CRITICAL("xyResolution Pair  : {}, {}", xyResolution.first, xyResolution.second);
#endif

  MDY_LOG_CRITICAL("DyLuaVariablesReading Success");
}

 void DyLuaVariablesLookup()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("./TestResource/Lua/variables.lua");

  // Can get nested variables.
  auto isFullScreen = lua["config"]["invalidData"];
  if (isFullScreen.valid())
  {
    PHITOS_UNEXPECTED_BRANCH();
  }
  else
  {
    MDY_LOG_CRITICAL("DyLuaVariablesLookup Success. config.invalidData is not exist on table.");
  }
}

void DyLuaVariablesOptionalLookup()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("./TestResource/Lua/variables.lua");

  // Can bind value to sol::optional too. (WARNING : NOT std::optional! this leads to UB)
  {
    sol::optional<TF32> var = lua["config"]["invalidData"];
    if (var) { PHITOS_UNEXPECTED_BRANCH(); }
    else
    {
      MDY_LOG_CRITICAL("DyLuaVariablesOptionalLookup | config.invalidData is not exist on table.");
    }
  }

  {
    sol::optional<bool> var = lua["config"]["fullscreen"];
    if (!var) { PHITOS_UNEXPECTED_BRANCH(); }
    else
    {
      MDY_LOG_CRITICAL("DyLuaVariablesOptionalLookup | config.fullscreen {}", var.value());
    }
  }

  {
    sol::optional<int> var = lua["config"]["fullscreen"];
    if (var) { PHITOS_UNEXPECTED_BRANCH(); }
    else
    {
      MDY_LOG_CRITICAL("DyLuaVariablesOptionalLookup | config.fullscreen is not a integer");
    }
  }

  {
    sol::optional<std::tuple<TI32, TI32>> var = lua["config"]["resolution"];
    if (var) { PHITOS_UNEXPECTED_BRANCH(); }
    else
    {
      MDY_LOG_CRITICAL("DyLuaVariablesOptionalLookup | config.resolution is exist but the way of retrieving values is wrong.");
    }
  }
}

void DyLuaVariableGetOrLoopup()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  lua.script_file("./TestResource/Lua/variables.lua");

  int isWrongSoDefaulted = lua["config"]["fullscreen"].get_or(24);
  bool isNotDefaulted    = lua["config"]["fullscreen"];

  MDY_LOG_CRITICAL("DyLuaVariableGetOrLoopup | isWrongSoDefaulted : {}",  isWrongSoDefaulted);
  MDY_LOG_CRITICAL("DyLuaVariableGetOrLoopup | isNotDefaulted : {}",      isNotDefaulted);
}

void DyLuaVariableWriting()
{
  sol::state lua;

  // To support print() etc.
  lua.open_libraries(sol::lib::base);

  // define variable in global table.
  lua["bark"] = 50;

  // a table being created in the global table
  lua["some_table"] = lua.create_table_with(
      "key0", 24,
      "key1", 25,
      lua["bark"], "the key is 50 and this string is its value."
  );

  lua.script(R"dy(
print(some_table[50])
print(some_table["key0"])
print(some_table["key1"])

-- a lua comment : access a global in a lua script with the _G table.
print(_G["bark"])
)dy");

  MDY_LOG_CRITICAL("DyLuaVariableWriting Success.");
}

void DyLuaReferenceModify()
{
  {
    sol::state lua;
    auto barkkey = lua["Bark"];
    PHITOS_ASSERT(!barkkey.valid(), "MUST SUCCESS");

    barkkey = 50;
    PHITOS_ASSERT(barkkey.valid(), "MUST SUCCESS");
  }

  {
    sol::state lua;
    lua["Bark"] = 50;
    PHITOS_ASSERT(lua["Bark"].valid(), "MUST SUCCESS");

    lua["Bark"] = sol::nil;
    PHITOS_ASSERT(!lua["Bark"].valid(), "MUST SUCCESS");
  }

  MDY_LOG_CRITICAL("DyLuaReferenceModify Success.");
}

} /// ::dy::test namespace