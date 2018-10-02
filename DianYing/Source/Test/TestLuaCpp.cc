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

//!
//! Local translation unit data
//!

namespace
{

struct FDyPlayer
{
private:
  TI32 mHp       = MDY_INITIALIZE_DEFINT;
  TI32 mBullets  = MDY_INITIALIZE_DEFINT;
  TI32 mSpeed    = MDY_INITIALIZE_DEFINT;

public:
  ///
  FDyPlayer() : FDyPlayer(3, 100) {};
  ///
  FDyPlayer(TI32 ammo, TI32 hitPoints) : mHp{hitPoints}, mBullets(ammo) {};
  ///
  FDyPlayer(TI32 ammo) : FDyPlayer{ammo, 100} {};

  MDY_GETSET(mHp);
  MDY_GETSET(mBullets);
  MDY_GETSET(mSpeed);

  FORCEINLINE void Boost() noexcept
  {
    this->mSpeed += 10;
  }

  MDY_NODISCARD bool Shoot() noexcept
  {
    if (this->mBullets > 0)
    {
      this->mBullets -= 1;
      return true;
    }
    else return false;
  }
};

///
/// @struct DDyMyType
///
struct DDyMyType
{
  void Stuff()
  {
    MDY_LOG_CRITICAL("YOSHI!");
  };
};

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy::test
{

void DyLuaCppPlayerScript()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  // Note that you can set a userdata before you register a usertype,
  // and it will still carry the right metatable if you register if later
  lua["p2"] = FDyPlayer{0};

  // Read user customed type from lua script file, you bind things using the `new_usertype` and `set_usertype` methods as shown below.
  // These methods are on both `table` and `state(_view)`.
  lua.new_usertype<FDyPlayer>("FDyPlayer",
      // 3 constructors
      sol::constructors<FDyPlayer(), FDyPlayer(TI32), FDyPlayer(TI32, TI32)>(),
      // Propeties
      "mBullets", sol::property(&FDyPlayer::GetmBullets,  &FDyPlayer::SetmBullets),
      "mHp",      sol::property(&FDyPlayer::GetmHp,       &FDyPlayer::SetmHp),
      "mSpeed",   sol::property(&FDyPlayer::GetmSpeed,    &FDyPlayer::SetmSpeed),
      // Functions
      "Shoot",    &FDyPlayer::Shoot,
      "Boost",    &FDyPlayer::Boost
  );

  lua.script_file("./TestResource/Lua/player_script.lua");

  // This is a powerful way to allow reuse of C++ code from Lua beyond just registering functions,
  // and should get you on your way to having more complex classes and data structures.
  // In this case you need more customization than just usertypes, however, you can customize Sol to
  // behave more fit to your desires by using desired customization and extension structures.
  MDY_LOG_CRITICAL("{} | SUCCESS", "DyLuaCppPlayerScript");
}

void DyLuaCppDoNotOwnershipPointer()
{
  sol::state lua;

  // DO NOT TRY THIS.
  // Sol will not take ownership of raw pointers: raw poitners do not own anything.
  // Sol wil not delete raw pointers, because they do not own anything.
#ifdef false
  // AHHHHHHHHHHHH
  lua["DangledMyType"] = []() -> DDyMyType*
  {
    return new DDyMyType();
  };

  // AAAHHH!!!
  lua.set("DoNotThisAgain", new DDyMyType());

  // AAAAAAAHHHHHH!!!?
  lua["WHATAREYOUDOIN"] = new DDyMyType();
#endif

  MDY_LOG_CRITICAL("{} | SUCCESS", "DyLuaCppPlayerScript");
}

void DyLuaCppOwnershipPointerSafely()
{
  sol::state lua;
  lua.open_libraries(sol::lib::base);

  // :ok:
  if constexpr (true) // WAY 1
    lua["MyFunc"] = []() -> std::unique_ptr<DDyMyType>
    {
      return std::make_unique<DDyMyType>();
    };
  else if constexpr (false) // WAY 2
    lua["MyFunc"] = []() -> std::shared_ptr<DDyMyType>
    {
      return std::make_shared<DDyMyType>();
    };
  else if constexpr (false) // WAY 3
    lua["MyFunc"] = []() -> DDyMyType
    {
      return DDyMyType();
    };

  lua.set("Something", std::unique_ptr<DDyMyType>(new DDyMyType()));

  std::shared_ptr<DDyMyType> myShared = std::make_shared<DDyMyType>();
  lua.set("SomethingElse", myShared);

  auto myUnique = std::make_unique<DDyMyType>();
  lua["OtherThing"] = std::move(myUnique);

  lua["MyStaticFunc"] = []() -> DDyMyType&
  {
    static DDyMyType mt;
    return mt;
  };

  MDY_LOG_CRITICAL("{} | SUCCESS", "DyLuaCppOwnershipPointerSafely");
}

void DyLuaCppSafeNullptr()
{
  sol::state lua;

  // BUT THIS IS STILL BAD DONT DO THIS
  // RETURN A EMPTY UNIQUE_PTR OR DO IT OTHER WAY.
#ifdef false
  lua["MyFunc"] = []() -> DDyMyType*
  {
    return nullptr;
  }
#endif

  lua["MyFunc2"] = []() -> std::unique_ptr<DDyMyType>
  { // default-constructs as a nullptr, gets pushed as nil to Lua
    // same happens for std::shared_ptr
    return std::unique_ptr<DDyMyType>();
  };

  // it will set 'Something' to nil. and Delete it on next GC if there's no more reference.
  lua.set("Something", nullptr);
  // Also fine
  lua["SomethingElse"] = nullptr;

  MDY_LOG_CRITICAL("{} | SUCCESS", "DyLuaCppOwnershipPointerSafely");
}

} /// ::dy::test namespace

#endif /// MDY_FLAG_TEST_ENABLED