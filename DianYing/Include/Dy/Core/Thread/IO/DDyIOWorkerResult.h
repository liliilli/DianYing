#ifndef GUARD_DY_THREAD_IO_DDYIOWORKERRESULT_H
#define GUARD_DY_THREAD_IO_DDYIOWORKERRESULT_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <any>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>

namespace dy
{

///
/// @struct DDyIOWorkerResult
/// @brief Result type of worker task processing.
///
struct DDyIOWorkerResult final
{
  std::string       mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  EDyResourceStyle  mResourceStyle  = EDyResourceStyle::NoneError;
  EResourceType   mResourceType   = EResourceType::NoneError;
  bool              mIsHaveDeferredTask = false;
  // @TODO NEED TO CHANGE TYPE TO MORE LIABLE TYPE.
  void*             mSmtPtrResultInstance;
};

} /// ::dy namespace

#endif /// GUARD_DY_THREAD_IO_DDYIOWORKERRESULT_H