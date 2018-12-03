#ifndef GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H
#define GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H
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

#include <atomic>
#include <Dy/Core/Thread/EDyIOTask.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Resource/Type/EDyScope.h>

namespace dy
{

///
/// @struct DDyIOPtrBoundObject
/// @brief Specifies pointer and type of bound object from outside world.
///
struct DDyIOPtrBoundObject final
{
  EDyObject mObjectStyle    = EDyObject::NoneError;
  void*     mPtrBoundObject = nullptr;
};

///
/// @struct DDyIOReferenceInstance
/// @brief Internal reference instance for checking GC'ing and verifying resource validation.
///
struct DDyIOReferenceInstance final
{
  using TConditionCallback = bool(*)();

  std::string         mSpecifierName = {};
  EDyResourceStyle    mResourcecStyle = EDyResourceStyle::NoneError;
  EDyResourceType     mResourceType   = EDyResourceType::NoneError;
  EDyScope            mScope          = EDyScope::Global;

  std::atomic<TU32>   mReferenceCount     = 0;
  bool                mIsNeedToBeGarbageCollected = false;
  bool                mIsResourceValid    = false;
  TConditionCallback  mConditionCallback  = nullptr;
  std::vector<DDyIOPtrBoundObject> mPtrBoundObjectList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H