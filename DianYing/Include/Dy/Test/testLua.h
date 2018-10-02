#ifndef GUARD_DY_TEST_TESTLUA_H
#define GUARD_DY_TEST_TESTLUA_H
#include <Dy/Test/TestMacroSetting.h>

#if defined(MDY_FLAG_TEST_ENABLED)
#define SOL_CHECK_ARGUMENT 1

namespace dy::test
{

//!
//! Hello world
//!

void DyLuaHelloWorld();
void DyLuaFundamental01();
void DyLuaFundamental02();

//!
//! Variable
//!

void DyLuaVariablesReading();
void DyLuaVariablesLookup();
void DyLuaVariablesOptionalLookup();
void DyLuaVariableGetOrLoopup();
void DyLuaVariableWriting();
void DyLuaReferenceModify();

//!
//! Functions and You!
//!

void DyLuaFunctionRegisteringCpp();
void DyLuaFunctionMemberFunction();
void DyLuaFunctionTemplateFunction();
void DyLuaFunctionGetFunctionFromLua();
void DyLuaFunctionProtectedFunction();
void DyLuaFunctionMultipleReturnFromLua();
void DyLuaFunctionMultipleReturnFromCpp();
void DyLuaFunctionAnyReturn();

//!
//! C++ in Lua
//!

void DyLuaCppPlayerScript();
void DyLuaCppDoNotOwnershipPointer();
void DyLuaCppOwnershipPointerSafely();
void DyLuaCppSafeNullptr();

/// Entry point of lua testing
void DyLuaEntryPoint();

} /// ::dy namespace

///
/// @macro MDY_TEST_LUA_FUNCTIONALITY
/// @brief Lua test function helper macro. Only enabled when MDY_FLAG_TEST_ENABLED is enabled.
///
#define MDY_TEST_LUA_FUNCTIONALITY() ::dy::test::DyLuaEntryPoint()

#else /// IF NOT MDY_FLAG_TEST_ENABLED

///
/// @macro MDY_TEST_LUA_FUNCTIONALITY
/// @brief Lua test function helper macro. Only enabled when MDY_FLAG_TEST_ENABLED is enabled.
///
#define MDY_TEST_LUA_FUNCTIONALITY() \
  static_assert(false, R"dy(Macro flag "MDY_FLAG_TEST_ENABLED" must be enabled when call MDY_TEST_LUA_FUNCTIONALITY macro.)dy")

#endif /// MDY_FLAG_TEST_ENABLED
#endif /// GUARD_DY_TEST_TESTLUA_H
