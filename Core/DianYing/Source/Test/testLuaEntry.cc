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

namespace dy::test
{

void DyLuaEntryPoint()
{
#ifdef false
  DyLuaHelloWorld();
  DyLuaFundamental01();
  DyLuaFundamental02();
  DyLuaVariablesReading();
  DyLuaVariablesLookup();
  DyLuaVariablesOptionalLookup();
  DyLuaVariableGetOrLoopup();
  DyLuaVariableWriting();
  DyLuaReferenceModify();
  DyLuaFunctionRegisteringCpp();
  DyLuaFunctionMemberFunction();
  DyLuaFunctionTemplateFunction();
  DyLuaFunctionProtectedFunction();
  DyLuaFunctionMultipleReturnFromLua();
  DyLuaFunctionMultipleReturnFromCpp();
  DyLuaFunctionAnyReturn();
  DyLuaCppPlayerScript();
  DyLuaCppOwnershipPointerSafely();
#endif
  DyLuaBindingDyTest();
}

} /// ::dy::test namespace

#endif /// MDY_FLAG_TEST_ENABLED