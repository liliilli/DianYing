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
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Component/Internal/Physics/DCollisionCallbackItem.h>

//!
//! Forward declaration
//!

namespace dy
{
class AActorCppScript;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct IOnColliderBaseDelegator
/// @brief Base delegator type for control & handling callback of collision type.
template <typename TPredicate>
MDY_INTERFACE IOnColliderBaseDelegator
{
protected:
  using TFuncType = std::function<typename TPredicate::WrapType>;

public:
  IOnColliderBaseDelegator();
  virtual ~IOnColliderBaseDelegator() = 0;

  /// @brief Add callback of member function that satisfy condition as collision callback function.
  template <typename TScriptType>
  DCollisionCallbackItem<TPredicate>* 
  AddCallback(TScriptType& iRefController, typename TPredicate::template RawType<TScriptType> iPredicate);

  /// @brief Check.
  template <typename TScriptType>
  MDY_NODISCARD bool IsBoundCallback(typename TPredicate::template RawType<TScriptType> iPredicate);

  /// @brief Remove callback function binding handler.
  template <typename TScriptType>
  EDySuccess RemoveCallback(typename TPredicate::template RawType<TScriptType> iPredicate);

  /// @brief Remove callback function binding handler. (internal)
  EDySuccess RemoveCallback(const void* iId);

  /// @brief Remove all callback binding.
  void RemoveAll();

  /// @brief Get bound callback list.
  std::vector<DCollisionCallbackItem<TPredicate>>& GetBoundCallbackList();

protected:
  /// @brief Bound callback list.
  std::vector<DCollisionCallbackItem<TPredicate>> mBoundCallbackList; 
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERFACE_PHYSICS_IDyonColliderBaseDelegator_H
#include <Dy/Component/Interface/Internal/IOnColliderBaseDelegatorTmp.inl>