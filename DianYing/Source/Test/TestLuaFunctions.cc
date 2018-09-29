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

#include <sol2/sol.hpp>
#include <Dy/Helper/GlobalType.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

std::string MyFunction(TI32 a, const std::string& b)
{
  return fmt::format(R"dy({0}D{1})dy", b, a);
}

struct DDyClass
{
  DDyClass(TI32 x) : mA{x} {}

  TI32 mA = 0;

  TI32 Func()
  {
    ++this->mA;
    return this->mA;
  }
};

template <typename TA, typename TB>
auto TempAdd(const TA& a, const TB& b)
{
  return a + b;
}

sol::object FancySophiscatedFunction(sol::object a, sol::object b, sol::this_state s)
{
  sol::state_view lua(s);

  if (a.is<int>() && b.is<int>())
  {
    return sol::make_object(lua, a.as<int>() + b.as<int>());
  }
  else if (a.is<bool>())
  {
    bool doTriple = a.as<bool>();
    return sol::make_object(lua, b.as<double>() * (doTriple ? 3 : 1));
  }

  return sol::make_object(lua, sol::nil);
}

} /// ::unnamed namespace

namespace dy::test
{

void DyLuaFunctionRegisteringCpp()
{
  sol::state lua;
  if constexpr (true) // WAY 1
  {
    lua["MyFunction"] = MyFunction;
  }
  else if constexpr (false) // WAY 2
  {
    lua.set("MyFunction", MyFunction);
  }
  else if constexpr (false) // WAY 3
  {
    lua.set_function("MyFunction", MyFunction);
  }

  lua.script(R"dy(
someStr = MyFunction(65536, 'Da')
)dy");

  const std::string returnStr = lua["someStr"];
  MDY_LOG_CRITICAL("{} | someStr : {}", "DyLuaFunctionRegisteringCpp", returnStr);
}

void DyLuaFunctionMemberFunction()
{
  sol::state lua;

  // Here, we are binding the member function with a class instance.
  // It will call the function of the given class instance.
  lua.set_function("DDyClassInstantedFunc", &DDyClass::Func, DDyClass{0});

  // We dont pass a class instance here.
  // The function will need you to pass an instance of "DDyClass" to it in lua to work,
  // as shown below.
  lua.set_function("DDyClassNotInstancedFunc", &DDyClass::Func);

  // With a pre-bound instance:
  lua.script(R"dy(
firstValue  = DDyClassInstantedFunc()
secondValue = DDyClassInstantedFunc()
)dy");

  lua.set("Object", DDyClass{24});
  lua.script(R"dy(
thirdValue  = DDyClassNotInstancedFunc(Object)
fourthValue = DDyClassNotInstancedFunc(Object)
)dy");

  PHITOS_ASSERT(lua["firstValue"].get<TI32>()  == 1,  "MUST SUCCESS");
  PHITOS_ASSERT(lua["secondValue"].get<TI32>() == 2,  "MUST SUCCESS");
  PHITOS_ASSERT(lua["thirdValue"].get<TI32>()  == 25, "MUST SUCCESS");
  PHITOS_ASSERT(lua["fourthValue"].get<TI32>() == 26, "MUST SUCCESS");

  // Member class functions and member class variables will both be turned into functions
  // when set in this manner.
  // You can get intuitive variable with the obj.a = value access after this section
  // when you learn about usertypes to have C++ in Lua,
  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionMemberFunction");
}

void DyLuaFunctionTemplateFunction()
{
  sol::state lua;

  lua["TempFunc_TI32"] = TempAdd<TI32, TI32>;
  lua["TempFunc_String"] = TempAdd<std::string, std::string>;
  lua.script(R"dy(
MyNum = TempFunc_TI32(1, 13172)
)dy");

  const TI32 myNum = lua["MyNum"];

  lua.script(R"dy(
MyStr = TempFunc_String('bark bark', ' woof woof')
)dy");

  const std::string myStr = lua["MyStr"];

  MDY_LOG_CRITICAL("DyLuaFunctionTemplateFunction | {} : {}", "MyNum", myNum);
  MDY_LOG_CRITICAL("DyLuaFunctionTemplateFunction | {} : {}", "MyStr", myStr);

  // As a side note,
  // binding functions with default parameters does not magically bind multiple versions
  // of the function to be called with the default parameters.
  // You must instead use sol::overload.

  // As a side note,
  // please make sure to understand Make sure you understand the implications of binding
  // a lambda/callable struct in the various ways and what it means for your code!
  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionTemplateFunction");
}

void DyLuaFunctionGetFunctionFromLua()
{
  sol::state lua;
  lua.script(R"dy(
  function fFunc (a)
    return a + 5
  end
)dy");

  // Get and immediately call
  const TI32 x = lua["fFunc"](30);
  PHITOS_ASSERT(x == 35, "MUST SUCCESS");

  sol::function f = lua["fFunc"];
  const TI32 y = f(340);
  PHITOS_ASSERT(y == 370, "MUST SUCCESS");

  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionGetFunctionFromLua");
}

void DyLuaFunctionProtectedFunction()
{
  sol::state lua;
  lua.script(R"dy(
  function fHandler (iMessage)
    return "Handled this message: " .. iMessage
  end

  function fFunc (iA)
    if iA < 0 then
      error("negative number detected")
    end
    return iA + 5
  end
)dy");

  sol::protected_function f = lua["fFunc"];
  f.error_handler = lua["fHandler"];

  for (TI32 i = -500; i <= 500; i += 50)
  {
    sol::protected_function_result result = f(i);
    if (result.valid())
    { // IF SUCCESSED
      const TI32 x = result.get<TI32>();
      MDY_LOG_CRITICAL("{} | x = {}", "DyLuaFunctionProtectedFunction", x);
    }
    else
    { // IF FAILED
      const sol::error  lErr = result;
      const std::string lWhat = lErr.what();
      // @TODO ERROR MESSAGE DOES NOT DISPLAY PROPERLY. INSTEAD ERROR MESSAGE OF INTERNAL LUA.
      MDY_LOG_CRITICAL("{} | Failed to execute function. {}", \
                       "DyLuaFunctionProtectedFunction", lWhat);
    }
  }

  // You can get anything that’s a callable in Lua,
  // including C++ functions you bind using set_function or similar.
  // sol::protected_function behaves similarly to sol::function,
  // but has a error_handler variable you can set to a Lua function.
  // This catches all errors and runs them through the error-handling function:
  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionProtectedFunction");
}

void DyLuaFunctionMultipleReturnFromLua()
{
  sol::state lua;
  lua.script(R"dy(
function fReturn (iA, iB, iC)
  return iA, iB, iC
end
)dy");

  std::tuple<TI32, TI32, TI32> result = lua["fReturn"](1, 2, 3);

  MDY_LOG_CRITICAL("{} | iA = {}", "DyLuaFunctionMultipleReturnFromLua", std::get<0>(result));
  MDY_LOG_CRITICAL("{} | iB = {}", "DyLuaFunctionMultipleReturnFromLua", std::get<1>(result));
  MDY_LOG_CRITICAL("{} | iC = {}", "DyLuaFunctionMultipleReturnFromLua", std::get<2>(result));

  TI32 a;
  TI32 b;
  std::string c;

  sol::tie(a, b, c) = lua["fReturn"](1, 2, "Bark");

  MDY_LOG_CRITICAL("{} | a = {}", "DyLuaFunctionMultipleReturnFromLua", a);
  MDY_LOG_CRITICAL("{} | b = {}", "DyLuaFunctionMultipleReturnFromLua", b);
  MDY_LOG_CRITICAL("{} | c = {}", "DyLuaFunctionMultipleReturnFromLua", c);

  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionMultipleReturnFromLua");
}

void DyLuaFunctionMultipleReturnFromCpp()
{
  sol::state lua;

  lua["fFunc"] = [](TI32 a, TI32 b, sol::object c)
  { // sol::object can be anything here: just pass it through.
    return std::make_tuple(a, b, c);
  };

  std::tuple<TI32, TI32, TI32> result = lua["fFunc"](1, 2, 3);
  MDY_LOG_CRITICAL("{} | iA = {}", "DyLuaFunctionMultipleReturnFromCpp", std::get<0>(result));
  MDY_LOG_CRITICAL("{} | iB = {}", "DyLuaFunctionMultipleReturnFromCpp", std::get<1>(result));
  MDY_LOG_CRITICAL("{} | iC = {}", "DyLuaFunctionMultipleReturnFromCpp", std::get<2>(result));

  std::tuple<TI32, TI32, std::string> result2 = lua["fFunc"](1, 2, "Arf?");
  MDY_LOG_CRITICAL("{} | iA = {}", "DyLuaFunctionMultipleReturnFromCpp", std::get<0>(result2));
  MDY_LOG_CRITICAL("{} | iB = {}", "DyLuaFunctionMultipleReturnFromCpp", std::get<1>(result2));
  MDY_LOG_CRITICAL("{} | iC = {}", "DyLuaFunctionMultipleReturnFromCpp", std::get<2>(result2));

  TI32 a = MDY_NOT_INITIALIZED_0;
  TI32 b = MDY_NOT_INITIALIZED_0;
  std::string c;
  sol::tie(a, b, c) = lua["fFunc"](1, 2, "Meow");

  MDY_LOG_CRITICAL("{} | a = {}", "DyLuaFunctionMultipleReturnFromCpp", a);
  MDY_LOG_CRITICAL("{} | b = {}", "DyLuaFunctionMultipleReturnFromCpp", b);
  MDY_LOG_CRITICAL("{} | c = {}", "DyLuaFunctionMultipleReturnFromCpp", c);

  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionMultipleReturnFromCpp");
}

void DyLuaFunctionAnyReturn()
{
  sol::state lua;
  lua["fFunc"] = FancySophiscatedFunction;

  const int     result = lua["fFunc"](1, 2);
  MDY_LOG_CRITICAL("{} | result = {}", "DyLuaFunctionAnyReturn", result);
  const double  result2 = lua["fFunc"](false, 2.5);
  MDY_LOG_CRITICAL("{} | result2 = {}", "DyLuaFunctionAnyReturn", result2);

  lua.script(R"dy(
  result3 = fFunc(true, 5.5)
)dy");
  const double  result3 = lua["result3"];
  MDY_LOG_CRITICAL("{} | result3 = {}", "DyLuaFunctionAnyReturn", result3);

  // This covers almost everything you need to know about Functions and
  // how they interact with Sol.
  // For some advanced tricks and neat things, check out sol::this_state and sol::variadic_args.
  //
  // The next stop in this tutorial is about C++ types (usertypes) in Lua!
  // If you need a bit more information about functions in the C++ side and
  // how to best utilize arguments from C++, see this note.
  MDY_LOG_CRITICAL("{} | Success", "DyLuaFunctionAnyReturn");
}

} /// ::dy::test namespace