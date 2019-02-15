#ifndef GUARD_DY_COMPONENT_INTERNAL_PHYSICS_DDyCollisionCallbackItem_H
#define GUARD_DY_COMPONENT_INTERNAL_PHYSICS_DDyCollisionCallbackItem_H
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

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class ADyActorCppScript;
} /// ::dy namesapce

//!
//! Implementation
//!

namespace dy
{

/// @struct DDyCollisionCallbackItem
/// @brief 
template <typename TPredicate>
struct DDyCollisionCallbackItem final
{
protected:
  using TFuncType = std::function<typename TPredicate::WrapType>;

public:
  DDyCollisionCallbackItem(TFuncType iFunction, const void* id, ADyActorCppScript& iPtrScript) :
      mCallbackFunction(iFunction),
      mPtrController(&iPtrScript),
      mSpecificationId(id)
  { 

  }

  TFuncType                   mCallbackFunction = nullptr;
  NotNull<ADyActorCppScript*> mPtrController;
  const void*                 mSpecificationId = nullptr;
  bool                        mIsCallable = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_PHYSICS_DDyCollisionCallbackItem_H