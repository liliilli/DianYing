#ifndef GUARD_DY_CORE_RESOURCE_TYPE_TDYINFORMATIONBINDER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_TDYINFORMATIONBINDER_H
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

#include <Dy/Meta/Type/EResourceType.h>
#include <Dy/Core/Thread/SDyIOBindingHelper.h>
#include <Dy/Core/Resource/Type/IBinderBase.h>
#include <Dy/Core/Resource/Type/XTemplateRescInfoType.h>

namespace dy
{

/// @struct TInformationBinderBase
/// @brief Binder base class for each supporting information resource type.
template <EResourceType TType>
struct TInformationBinderBase : public IBinderBase
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TInformationBinderBase);
  using TPtrResource      = const typename __TDyRscInfo<TType>::type*;
  using TTryGetReturnType = std::optional<TPtrResource>;

  /// @brief Release binder instance and detach it from specified Reference Instance.
  virtual ~TInformationBinderBase()
  {
    if (this->mPtrResource != nullptr) 
    { 
      MDY_CALL_ASSERT_SUCCESS(this->pTryDetachResource()); 
    }
  }

  TPtrResource operator->() noexcept        { return this->mPtrResource; }
  TPtrResource operator->() const noexcept  { return this->mPtrResource; }

  /// @brief Check resource is binded to binder handle.
  bool IsResourceExist() const noexcept override final
  {
    return MDY_CHECK_ISNOTNULL(this->mPtrResource);
  }

  /// @brief Get resource pointer which is not nullable.
  TPtrResource Get() const noexcept
  {
    MDY_ASSERT_MSG(
      this->mPtrResource != nullptr, 
      "Resource pointer address must not be null when use it.");
    return this->mPtrResource;
  }

protected:
  TInformationBinderBase(const std::string& iSpecifierName) 
    : mSpecifierName{iSpecifierName} {};
  TInformationBinderBase() {};

  /// @brief Require resource.
  EDySuccess pTryRequireResource(const std::string& iNewSpecifier) noexcept
  {
    MDY_ASSERT_MSG(
      iNewSpecifier.empty() == false, 
      "Resource specifier name must be valid to require resource.");

    auto ptrResult = SDyIOBindingHelper::TryRequireInformation<TType>(iNewSpecifier, *this);
    if (ptrResult.has_value() == false) 
    { 
      this->mDelayedSpecifierName = iNewSpecifier;
      return DY_FAILURE; 
    }
    else
    {
      const auto noError = this->pTryDetachResource();
      this->mPtrResource = ptrResult.value();
      return DY_SUCCESS;
    }
  }

  /// @brief Try detach resource. \n
  /// If pointer of resource is null (so, not bound to anything), just return DY_FAILURE.
  /// If detach is succeeded, return DY_SUCCESS. and `mPtrResource` will be nullptr agian.
  EDySuccess pTryDetachResource() noexcept
  {
    if (this->mPtrResource == nullptr) { return DY_FAILURE; }

    MDY_CALL_ASSERT_SUCCESS(SDyIOBindingHelper::TryDetachInformation<TType>(this->mSpecifierName, *this));
    this->mSpecifierName.clear();
    this->mPtrResource = nullptr;
    return DY_SUCCESS;
  }

  /// @brief Set new specifier name. this function must be called in lazy resource type.
  void pSetSpecifierName(const std::string& iNewSpecifier) noexcept
  {
    this->mSpecifierName = iNewSpecifier;
  }

protected:
  /// @brief Try update resource pointer of this type with ptr when RI is being valid. \n
  /// `iPtr` must be convertible to specialized TResourceBinderBase `Type`.
  void TryUpdateResourcePtr(const void* iPtr) noexcept override final
  {
  // If there is something already bound to this instance, detach this from resource.
    MDY_CALL_BUT_NOUSE_RESULT(this->pTryDetachResource());

    this->mPtrResource = static_cast<TPtrResource>(const_cast<void*>(iPtr));
    this->mSpecifierName = this->mDelayedSpecifierName;
    this->mDelayedSpecifierName.clear();
  }

  /// @brief Try detach resource pointer of this type with ptr when RI is being GCed.
  void TryDetachResourcePtr() noexcept override final 
  { 
    this->mPtrResource = nullptr; 
  }

  std::string   mSpecifierName;
  std::string   mDelayedSpecifierName;
  TPtrResource  mPtrResource = nullptr;
};

/// @class TDyInformationBinder
/// @brief Not lazy version of `TDyInformationBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
template <EResourceType TType>
class TDyInformationBinder final : public TInformationBinderBase<TType>
{
private:
  using TSuper = TInformationBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyInformationBinder);
  TDyInformationBinder(const std::string& iNewSpecifier) 
    : TSuper{iNewSpecifier} 
  { 
    this->TryRequireResource(iNewSpecifier); 
  }

  TDyInformationBinder() = default;
  virtual ~TDyInformationBinder() = default;

  /// @brief Try require resource with specifier name in given EResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(const std::string& iNewSpecifier)
  {
    // If resource is already exist and bound by something. 
    // Let it be until new resource is bounded soon.
    // If flag == DY_FAILURE, iNewSpecifier will be stored as deferred resource specifier.
    if (const auto flag = TSuper::pTryRequireResource(iNewSpecifier); flag == DY_SUCCESS) 
    { 
      this->mSpecifierName = iNewSpecifier;
      this->Process(); 
    }
  }
};

using TDyInformationBinderShader     = TDyInformationBinder<EResourceType::GLShader>;
using TDyInformationBinderMesh       = TDyInformationBinder<EResourceType::Mesh>;
using TDyInformationBinderModel      = TDyInformationBinder<EResourceType::Model>;
using TDyInformationBinderTexture    = TDyInformationBinder<EResourceType::Texture>;
using TDyInformationBinderMaterial   = TDyInformationBinder<EResourceType::Material>;
using TDyInformationBinderAttachment = TDyInformationBinder<EResourceType::GLAttachment>;
using TDyInformationBinderFrameBuffer= TDyInformationBinder<EResourceType::GLFrameBuffer>;
using TDyInformationBinderSkeleton   = TDyInformationBinder<EResourceType::Skeleton>;
using TDyInformationBinderAnimScrap  = TDyInformationBinder<EResourceType::AnimationScrap>;
using TDyInformationBinderSound      = TDyInformationBinder<EResourceType::Sound>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TDYINFORMATIONBINDER_H