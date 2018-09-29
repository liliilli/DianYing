#pragma once
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

/// Entry point of lua testing
void DyLuaEntryPoint();

} /// ::dy namespace
