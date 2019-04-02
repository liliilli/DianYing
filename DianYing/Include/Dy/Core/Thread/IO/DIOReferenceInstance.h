#ifndef GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H
#define GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H
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

#include <atomic>
#include <Dy/Core/Thread/IO/EDyIOTask.h>
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Core/Resource/Type/EResourceScope.h>
#include <Dy/Core/Resource/Internal/XHandleAliases.h>
#include <Dy/Helper/Internal/FHandler.h>

//!
//! Forward declaration
//!

namespace dy
{
MDY_INTERFACE IBinderBase;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct DIOReferenceInstance
/// @brief Internal reference instance for checking GC'ing and verifying resource validation.
struct DIOReferenceInstance final
{
  using TConditionCallback = bool(*)();

  std::string     mSpecifierName;
  EResourceStyle  mResourcecStyle = EResourceStyle::NoneError;
  EResourceType   mResourceType   = EResourceType::NoneError;
  EResourceScope  mScope          = EResourceScope::Global;

  bool               mIsResourceValid   = false;
  TConditionCallback mConditionCallback = nullptr;
  void*              mPtrInstance       = nullptr;               

  mutable std::mutex mContainerMutex;

  DIOReferenceInstance() = default;
  /// @brief Constructor without binding object ptr. \n
  /// In this case, Reference count would be 0 and nothing.
  DIOReferenceInstance(
    const std::string& specifier, 
    EResourceStyle style, EResourceType type, EResourceScope scope)
    : mSpecifierName(specifier), 
      mResourcecStyle(style), mResourceType(type), mScope(scope)
  {};

  /// @brief Bind binder instance pointer address to this RI.
  /// Be careful of duplicating address pointer.
  void AttachBinder(IBinderBase& iRefBase) noexcept;
  /// @brief Unbind binder instance pointer address from this RI.
  /// If RI's scope is temporal and valid, GC candidate flag will be set up.
  void DetachBinder(IBinderBase& iRefBase) noexcept;

  /// @brief Set resource valid. `iPtrInstance` must be valid pointer type of each RI type.
  /// and, `mIsResourceValid` will be true.
  void SetValid(void*& iPtrInstance) noexcept;

  /// @brief
  void SetNotValid() noexcept;

  /// @brief Check this RI is need to be gced so moved GC container.
  MDY_NODISCARD bool HaveToBeGCed() const noexcept;

  /// @brief Check handler is being bound to this.
  bool IsBeingBound() const noexcept;

private:
  FResourceHandler mInternalHandler;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H