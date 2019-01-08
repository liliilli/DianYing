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
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Resource/Type/EDyScope.h>

//!
//! Forward declaration
//!

namespace dy
{
MDY_INTERFACE __FDyBinderBase;
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
  EDyResourceType     mResourceType   = EDyResourceType::NoneError;
  EDyScope            mScope          = EDyScope::Global;

  bool                mIsResourceValid    = false;
  TConditionCallback  mConditionCallback  = nullptr;
  void*               mPtrInstance        = nullptr;               

  std::vector<const __FDyBinderBase*> mPtrBoundBinderList = {};

  DDyIOReferenceInstance() = default;
  /// @brief Constructor without binding object ptr. \n
  /// In this case, Reference count would be 0 and nothing.
  DDyIOReferenceInstance(_MIN_ const std::string& specifier, _MIN_ EDyResourceStyle style, _MIN_ EDyResourceType type, _MIN_ EDyScope scope)
    : mSpecifierName(specifier), mResourcecStyle(style), mResourceType(type), mScope(scope)
  {};

  /// @brief Bind binder instance pointer address to this RI.
  /// Be careful of duplicating address pointer.
  void AttachBinder(_MIN_ const __FDyBinderBase* iPtrBase) noexcept;

  /// @brief Unbind binder instance pointer address from this RI.
  /// If RI's scope is temporal and valid, GC candidate flag will be set up.
  void DetachBinder(_MIN_ const __FDyBinderBase* iPtrBase) noexcept;

  /// @brief Set resource valid. `iPtrInstance` must be valid pointer type of each RI type.
  /// and, `mIsResourceValid` will be true.
  void SetValid(_MIN_ void*& iPtrInstance) noexcept;
  /// @brief
  void SetNotValid() noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_DDYIOREFERENCEINSTANCE_H