#ifndef GUARD_DY_CORE_RESOURCE_TYPE_TDYRESOURCEBINDERBASE_H
#define GUARD_DY_CORE_RESOURCE_TYPE_TDYRESOURCEBINDERBASE_H
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

#include <Dy/Core/Resource/Type/FDyBinderBase.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>
#include <Dy/Core/Thread/SDyIOBindingHelper.h>
#include <Dy/Meta/Type/EDyResourceType.h>

namespace dy
{

/// @struct __TDyResourceBinderBase
/// @brief Binder base class for each supporting resource type.
template <EDyResourceType TType>
struct __TDyResourceBinderBase : public __FDyBinderBase
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__TDyResourceBinderBase);
  using TPtrResource = typename __TResourceType<TType>::type*;

  /// @brief Release binder instance and detach it from specified Reference Instance.
  virtual ~__TDyResourceBinderBase();

  TPtrResource operator->() noexcept        
  { 
    return this->mPtrResource; 
  }

  const TPtrResource operator->() const noexcept  
  { 
    return this->mPtrResource; 
  }

  /// @brief Get resource pointer which is not nullable.
  MDY_NODISCARD TPtrResource Get() const noexcept;
  
  /// @brief Check resource is binded to binder handle.
  MDY_NODISCARD bool IsResourceExist() const noexcept override final;

protected:
  __TDyResourceBinderBase() = default;

  /// @brief Require resource.
  MDY_NODISCARD EDySuccess pTryRequireResource(_MIN_ const std::string& iNewSpecifier) noexcept;

  /// @brief Try detach resource. \n
  /// If pointer of resource is null (so, not bound to anything), just return DY_FAILURE.
  /// If detach is succeeded, return DY_SUCCESS. and `mPtrResource` will be nullptr agian.
  MDY_NODISCARD EDySuccess pTryDetachResource() noexcept;

    /// @brief Try update resource pointer of this type with ptr when RI is being valid. \n
  /// `iPtr` must be convertible to specialized __TDyResourceBinderBase `Type`.
  void TryUpdateResourcePtr(_MIN_ const void* iPtr) noexcept override final;

  /// @brief Try detach resource pointer of this type with ptr when RI is being GCed.
  void TryDetachResourcePtr() noexcept override final; 

  std::string   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  std::string   mDelayedSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  TPtrResource  mPtrResource    = MDY_INITIALIZE_NULL;
};

template <EDyResourceType TType>
__TDyResourceBinderBase<TType>::~__TDyResourceBinderBase()
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrResource)) 
  { 
    MDY_CALL_ASSERT_SUCCESS(this->pTryDetachResource()); 
  }
}

template <EDyResourceType TType>
typename __TDyResourceBinderBase<TType>::TPtrResource 
__TDyResourceBinderBase<TType>::Get() const noexcept
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mPtrResource), "Resource pointer address must not be null when use it.");
  return this->mPtrResource;
}

template <EDyResourceType TType>
EDySuccess __TDyResourceBinderBase<TType>::pTryRequireResource
(_MIN_ const std::string& iNewSpecifier) noexcept
{
  MDY_ASSERT(iNewSpecifier.empty() == false, "Resource specifier name must be valid to require resource.");

  auto ptrResult = SDyIOBindingHelper::TryRequireResource<TType>(iNewSpecifier, this);
  if (ptrResult.has_value() == false) 
  { 
    this->mDelayedSpecifierName = iNewSpecifier;
    return DY_FAILURE; 
  }
  else
  {
    const auto noError = this->pTryDetachResource();
    this->mPtrResource = const_cast<TPtrResource>(ptrResult.value());
    return DY_SUCCESS;
  }
}

template <EDyResourceType TType>
EDySuccess __TDyResourceBinderBase<TType>::pTryDetachResource() noexcept
{
  // Checking 
  if (MDY_CHECK_ISNULL(this->mPtrResource)) { return DY_FAILURE; }

  // Detach
  MDY_CALL_ASSERT_SUCCESS(SDyIOBindingHelper::TryDetachResource<TType>(this->mSpecifierName, this));
  this->mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  this->mPtrResource    = nullptr;
  return DY_SUCCESS;
}

template <EDyResourceType TType>
bool __TDyResourceBinderBase<TType>::IsResourceExist() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrResource);
}

template <EDyResourceType TType>
void __TDyResourceBinderBase<TType>::TryUpdateResourcePtr(const void* iPtr) noexcept
{
  // If there is something already bound to this instance, detach this from resource.
  MDY_CALL_BUT_NOUSE_RESULT(this->pTryDetachResource());

  this->mPtrResource          = static_cast<TPtrResource>(const_cast<void*>(iPtr));
  this->mSpecifierName        = this->mDelayedSpecifierName;
  this->mDelayedSpecifierName = MDY_INITIALIZE_EMPTYSTR;
}

template <EDyResourceType TType>
void __TDyResourceBinderBase<TType>::TryDetachResourcePtr() noexcept
{ 
  this->mPtrResource = nullptr; 
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TDYRESOURCEBINDERBASE_H