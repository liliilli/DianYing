#pragma once
#define SOL_CHECK_ARGUMENT 1

namespace dy::test
{

void DyLuaHelloWorld();
void DyLuaFundamental01();
void DyLuaFundamental02();
void DyLuaVariablesReading();
void DyLuaVariablesLookup();
void DyLuaVariablesOptionalLookup();
void DyLuaVariableGetOrLoopup();
void DyLuaVariableWriting();
void DyLuaReferenceModify();

/// Entry point of lua testing
void DyLuaEntryPoint();

} /// ::dy namespace
