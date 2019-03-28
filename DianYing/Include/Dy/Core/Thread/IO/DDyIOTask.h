#ifndef GUARD_DY_CORE_THREAD_DDYIOTASK_H
#define GUARD_DY_CORE_THREAD_DDYIOTASK_H
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

#include <Dy/Core/Resource/Type/EResourceScope.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Core/Thread/IO/EDyIOTask.h>

#include <Dy/Helper/Type/DClamp.h>

namespace dy
{

///
/// @struct DDyIOTask
/// @brief Task for populating information or resources to TDyIO thread.
///
struct DDyIOTask final
{
private:
  using TPriority = DClamp<TU8, 0, 255>;
public:
  /// Must be moved except for Requiring.
  std::string       mSpecifierName  = {};
  EDyResourceStyle  mResourcecStyle = EDyResourceStyle::NoneError;
  EResourceType   mResourceType   = EResourceType::NoneError;
  EResourceScope          mScope          = EResourceScope::Global;
  EDyObject         mBoundObjectStyle    = EDyObject::NoneError;
  TPriority         mTaskPriority   = 0;
  bool              mIsResourceDeferred = false;

  void*             mPtrBoundObject = nullptr;
  std::any          mRawInstanceForUsingLater;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_DDYIOTASK_H