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

//!
//! Forward declaration
//!

namespace dy
{
MDY_INTERFACE __IBinderBase;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @struct DDyIOReferenceInstance
/// @brief Internal reference instance for checking GC'ing and verifying resource validation.
///
struct DDyIOReferenceInstance final
{
  using TConditionCallback = bool(*)();

  std::string         mSpecifierName = {};
  EDyResourceStyle    mResourcecStyle = EDyResourceStyle::NoneError;
  EResourceType     mResourceType   = EResourceType::NoneError;
  EResourceScope            mScope          = EResourceScope::Global;

  bool                mIsResourceValid    = false;
  TConditionCallback  mConditionCallback  = nullptr;
  void*               mPtrInstance        = nullptr;               

  std::vector<const __IBinderBase*> mPtrBoundBinderList = {};

  DDyIOReferenceInstance() = default;
  /// @brief Constructor without binding object ptr. \n
  /// In this case, Reference count would be 0 and nothing.
  DDyIOReferenceInstance(_MIN_ const std::string& specifier, _MIN_ EDyResourceStyle style, _MIN_ EResourceType type, _MIN_ EResourceScope scope)
    : mSpecifierName(specifier), mResourcecStyle(style), mResourceType(type), mScope(scope)
  {};

  /// @brief Bind binder instance pointer address to this RI.
  /// Be careful of duplicating address pointer.
  void AttachBinder(_MIN_ const __IBinderBase* iPtrBase) noexcept;

  /// @brief Unbind binder instance pointer address from this RI.
  /// If RI's scope is temporal and valid, GC candidate flag will be set up.
  void DetachBinder(_MIN_ const __IBinderBase* iPtrBase) noexcept;

  /// @brief Set resource valid. `iPtrInstance` must be valid pointer type of each RI type.
  /// and, `mIsResourceValid` will be true.
  void SetValid(_MIN_ void*& iPtrInstance) noexcept;
  /// @brief
  void SetNotValid() noexcept;

  /// @brief Check this RI is need to be gced so moved GC container.
  MDY_NODISCARD bool IsNeedToBeGced() const noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H