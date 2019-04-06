#ifndef GUARD_DY_INTERFACE_INTERNAL_IONCOLLIDERBASEDELEGATORTMP_INL
#define GUARD_DY_INTERFACE_INTERNAL_IONCOLLIDERBASEDELEGATORTMP_INL
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
#include <Dy/Component/Abstract/AActorCppScript.h>
#include <Dy/Management/MLog.h>

namespace dy
{
  
template <typename TPredicate>
IOnColliderBaseDelegator<TPredicate>::IOnColliderBaseDelegator()
{
  static_assert(
    IsInheritancedFrom<TPredicate, ISignatureCollisionDelegate> == true, 
    "Failed to create delegator type"); 
}

template <typename TPredicate>
IOnColliderBaseDelegator<TPredicate>::~IOnColliderBaseDelegator() = default;

template <typename TPredicate>
template <typename TScriptType>
DCollisionCallbackItem<TPredicate>* 
IOnColliderBaseDelegator<TPredicate>::AddCallback(
  TScriptType& iRefController,
  typename TPredicate::template RawType<TScriptType> iPredicate)
{
  static_assert(
    IsInheritancedFrom<TScriptType, AActorCppScript> == true, 
    "TScriptType is not inheritenced from AActorCppScript.");

  using namespace std::placeholders;
  if (this->IsBoundCallback(iPredicate) == true)
  { 
    DyPushLogDebugWarning("You are binding callback predicate that is already bound to rigidbody.");
    return nullptr; 
  }

  // https://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
  const void* addr = (void*&)iPredicate;

  // Make function first.
  this->mBoundCallbackList.emplace_back(std::bind(iPredicate, &iRefController, _1, _2, _3, _4, _5), addr, iRefController);
  // Bind back() to script instance.
  return &this->mBoundCallbackList.back();
}

template <typename TPredicate>
template <typename TScriptType>
bool IOnColliderBaseDelegator<TPredicate>::IsBoundCallback(
  typename TPredicate::template RawType<TScriptType> iPredicate)
{
  static_assert(
    IsInheritancedFrom<TScriptType, AActorCppScript> == true, 
    "TScriptType is not inheritenced from AActorCppScript.");

  const void* addr = (void*&)iPredicate;
  // Check by using iteration.
  for (const auto& item : mBoundCallbackList) { if (item.mSpecificationId == addr) { return true; } }
  // If not found anyone, just return false.
  return false;
}

template <typename TPredicate>
template <typename TScriptType>
EDySuccess IOnColliderBaseDelegator<TPredicate>::RemoveCallback(
  typename TPredicate::template RawType<TScriptType> iPredicate)
{
  static_assert(
    IsInheritancedFrom<TScriptType, AActorCppScript> == true, 
    "TScriptType is not inheritenced from AActorCppScript.");

  const void* addr = (void*&)iPredicate;
  auto lambda = [addr](const auto& item) { return item.mSpecificationId == addr; };
  if (ContainsIf(this->mBoundCallbackList, lambda) == false) { return DY_FAILURE; }

  EraseRemoveIf(this->mBoundCallbackList, lambda);
  return DY_SUCCESS;
}

template <typename TPredicate>
EDySuccess IOnColliderBaseDelegator<TPredicate>::RemoveCallback(const void* iId)
{
  auto lambda = [iId](const auto& item) { return item.mSpecificationId == iId; };
  if (ContainsIf(this->mBoundCallbackList, lambda) == false) { return DY_FAILURE; }

  EraseRemoveIf(this->mBoundCallbackList, lambda);
  return DY_SUCCESS;
}

template <typename TPredicate>
void IOnColliderBaseDelegator<TPredicate>::RemoveAll()
{
  for (auto& item : this->mBoundCallbackList)
  {
    const auto flag = item.mPtrController->MDY_PRIVATE(DetachCollisionCbHandle)(item.mSpecificationId);
    MDY_ASSERT_MSG(flag == DY_SUCCESS, "Unexpected error occurred.");
  }
  this->mBoundCallbackList.clear();
}

template <typename TPredicate>
std::vector<DCollisionCallbackItem<TPredicate>>& 
IOnColliderBaseDelegator<TPredicate>::GetBoundCallbackList()
{
  return this->mBoundCallbackList;
}

} /// ::dy namespace

#endif /// GUARD_DY_INTERFACE_INTERNAL_IONCOLLIDERBASEDELEGATORTMP_INL