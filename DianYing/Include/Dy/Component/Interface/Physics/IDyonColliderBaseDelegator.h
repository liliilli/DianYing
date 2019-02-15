#ifndef GUARD_DY_COMPONENT_INTERFACE_PHYSICS_IDyonColliderBaseDelegator_H
#define GUARD_DY_COMPONENT_INTERFACE_PHYSICS_IDyonColliderBaseDelegator_H
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

#include <functional>
#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Component/Internal/Physics/DDyCollisionCallbackItem.h>
#include <Dy/Component/Abstract/ADyActorCppScript.h>

//!
//! Forward declaration
//!

namespace dy
{
class ADyActorCppScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct IDyonColliderBaseDelegator
/// @brief Base delegator type for control & handling callback of collision type.
template <typename TPredicate>
MDY_INTERFACE IDyonColliderBaseDelegator
{
protected:
  using TFuncType = std::function<typename TPredicate::WrapType>;

public:
  IDyonColliderBaseDelegator() { static_assert(IsInheritancedFrom<TPredicate, ISignatureCollisionDelegate> == true, "Failed to create delegator type"); }
  virtual ~IDyonColliderBaseDelegator() = 0;

  /// @brief Add callback of member function that satisfy condition as collision callback function.
  template <typename TScriptType>
  DDyCollisionCallbackItem<TPredicate>* 
  AddCallback(_MIN_ TScriptType& iRefController, _MIN_ typename TPredicate::template RawType<TScriptType> iPredicate)
  {
    static_assert(IsInheritancedFrom<TScriptType, ADyActorCppScript> == true, "TScriptType is not inheritenced from ADyActorCppScript.");
    using namespace std::placeholders;
    if (this->IsBoundCallback(iPredicate) == true)
    { 
      MDY_LOG_WARNING_D("You are binding callback predicate that is already bound to rigidbody.");
      return nullptr; 
    }

    // https://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    const void* addr = (void*&)iPredicate;

    // Make function first.
    this->mBoundCallbackList.emplace_back(std::bind(iPredicate, &iRefController, _1, _2, _3, _4, _5, _6), addr, iRefController);
    // Bind back() to script instance.
    return &this->mBoundCallbackList.back();
  }

  /// @brief Check.
  template <typename TScriptType>
  MDY_NODISCARD bool IsBoundCallback(_MIN_ typename TPredicate::template RawType<TScriptType> iPredicate)
  {
    static_assert(IsInheritancedFrom<TScriptType, ADyActorCppScript> == true, "TScriptType is not inheritenced from ADyActorCppScript.");
    const void* addr = (void*&)iPredicate;

    // Check by using iteration.
    for (const auto& item : mBoundCallbackList) { if (item.mSpecificationId == addr) { return true; } }
    // If not found anyone, just return false.
    return false;
  }

  /// @brief Remove callback function binding handler.
  template <typename TScriptType>
  EDySuccess RemoveCallback(_MIN_ typename TPredicate::template RawType<TScriptType> iPredicate)
  {
    static_assert(IsInheritancedFrom<TScriptType, ADyActorCppScript> == true, "TScriptType is not inheritenced from ADyActorCppScript.");
    const void* addr = (void*&)iPredicate;

    auto lambda = [addr](const auto& item) { return item.mSpecificationId == addr; };
    if (Contains(this->mBoundCallbackList, lambda) == false) { return DY_FAILURE; }

    DyEraseRemoveIf(this->mBoundCallbackList, lambda);
    return DY_SUCCESS;
  }

  /// @brief Remove callback function binding handler. (internal)
  EDySuccess RemoveCallback(_MIN_ const void* iId)
  {
    auto lambda = [iId](const auto& item) { return item.mSpecificationId == iId; };
    if (Contains(this->mBoundCallbackList, lambda) == false) { return DY_FAILURE; }

    DyEraseRemoveIf(this->mBoundCallbackList, lambda);
    return DY_SUCCESS;
  }

  /// @brief
  void RemoveAll();

protected:
  /// @brief Bound callback list.
  std::vector<DDyCollisionCallbackItem<TPredicate>> mBoundCallbackList; 
};

template <typename TPredicate>
IDyonColliderBaseDelegator<TPredicate>::~IDyonColliderBaseDelegator() = default;

template <typename TPredicate>
void IDyonColliderBaseDelegator<TPredicate>::RemoveAll()
{
  for (auto& item : this->mBoundCallbackList)
  {
    const auto flag = item.mPtrController->MDY_PRIVATE_SPECIFIER(DetachCollisionCbHandle)(item.mSpecificationId);
    MDY_ASSERT(flag == DY_SUCCESS, "Unexpected error occurred.");
  }
  this->mBoundCallbackList.clear();
}

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERFACE_PHYSICS_IDyonColliderBaseDelegator_H