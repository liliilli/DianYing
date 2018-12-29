#ifndef GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H
#define GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Helper/Pointer.h>
#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Core/Thread/SDyIOBindingHelper.h>
#include <Dy/Core/Resource/Type/EDyLazy.h>
#include <Dy/Core/Resource/Type/FDyBinderBase.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>

namespace dy
{

/// @struct __TDyResourceBinderBase
/// @brief Binder base class for each supporting resource type.
template <EDyResourceType TType>
struct __TDyResourceBinderBase : public __FDyBinderBase
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__TDyResourceBinderBase);
  using TPtrResource      = const typename __TResourceType<TType>::type*;
  using TTryGetReturnType = std::optional<TPtrResource>;

  /// @brief Release binder instance and detach it from specified Reference Instance.
  virtual ~__TDyResourceBinderBase()
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrResource)) { MDY_CALL_ASSERT_SUCCESS(this->pTryDetachResource()); }
  }

  TPtrResource operator->() noexcept        { return this->mPtrResource; }
  TPtrResource operator->() const noexcept  { return this->mPtrResource; }

  /// @brief Check resource is binded to binder handle.
  MDY_NODISCARD bool IsResourceExist() const noexcept override final
  {
    return MDY_CHECK_ISNOTNULL(this->mPtrResource);
  }

  /// @brief Get resource pointer which is not nullable.
  MDY_NODISCARD TPtrResource Get() const noexcept
  {
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mPtrResource), "Resource pointer address must not be null when use it.");
    return this->mPtrResource;
  }

protected:
  __TDyResourceBinderBase(_MIN_ const std::string& iSpecifierName) : mSpecifierName{iSpecifierName} {};
  __TDyResourceBinderBase() {};

  /// @brief Require resource.
  MDY_NODISCARD EDySuccess pTryRequireResource() noexcept
  {
    MDY_ASSERT(this->mSpecifierName.empty() == false, "Resource specifier name must be valid to require resource.");
    auto ptrResult = SDyIOBindingHelper::TryRequireResource<TType>(this->mSpecifierName, this);
    if (ptrResult.has_value() == false) { return DY_FAILURE; }

    this->mPtrResource = ptrResult.value();
    return DY_SUCCESS;
  }

  /// @brief Try detach resource. \n
  /// If pointer of resource is null (so, not bound to anything), just return DY_FAILURE.
  /// If detach is succeeded, return DY_SUCCESS. and `mPtrResource` will be nullptr agian.
  MDY_NODISCARD EDySuccess pTryDetachResource() noexcept
  {
    if (MDY_CHECK_ISNULL(this->mPtrResource)) { return DY_FAILURE; }

    MDY_CALL_ASSERT_SUCCESS(SDyIOBindingHelper::TryDetachResource<TType>(this->mSpecifierName, this));
    this->mPtrResource = nullptr;
    return DY_SUCCESS;
  }

  /// @brief Set new specifier name. this function must be called in lazy resource type.
  void pSetSpecifierName(_MIN_ const std::string& iNewSpecifier) noexcept
  {
    this->mSpecifierName = iNewSpecifier;
  }

private:
  /// @brief Try update resource pointer of this type with ptr when RI is being valid. \n
  /// `iPtr` must be convertible to specialized __TDyResourceBinderBase `Type`.
  void TryUpdateResourcePtr(_MIN_ const void* iPtr) noexcept override final
  {
    this->mPtrResource = static_cast<TPtrResource>(iPtr);
  }

  /// @brief Try detach resource pointer of this type with ptr when RI is being GCed.
  void TryDetachResourcePtr() noexcept override final { this->mPtrResource = nullptr; }

  std::string   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  TPtrResource  mPtrResource    = MDY_INITIALIZE_NULL;
};

///
/// @class TDyResourceBinder
/// @brief Resource binder class.
///
template <EDyResourceType TType, EDyLazy TIsLazy>
class TDyResourceBinder;

///
/// @class TDyResourceBinder
/// @brief Not lazy version of `TDyResourceBinder`.
///
template <EDyResourceType TType>
class TDyResourceBinder<TType, EDyLazy::No> final : public __TDyResourceBinderBase<TType>
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);

  TDyResourceBinder(_MIN_ const std::string& specifier) : TSuper{specifier}
  {
    MDY_NOTUSED auto _ = TSuper::pTryRequireResource();
  }
  ~TDyResourceBinder() = default;
};

template <EDyResourceType TType>
using TDyResourceBinderInstant    = TDyResourceBinder<TType, EDyLazy::No>;
using TDyIResourceBinderShader    = TDyResourceBinderInstant<EDyResourceType::GLShader>;
using TDyIResourceBinderMesh      = TDyResourceBinderInstant<EDyResourceType::Mesh>;
using TDyIResourceBinderModel     = TDyResourceBinderInstant<EDyResourceType::Model>;
using TDyIResourceBinderTexture   = TDyResourceBinderInstant<EDyResourceType::Texture>;
using TDyIResourceBinderMaterial  = TDyResourceBinderInstant<EDyResourceType::Material>;
using TDyIResourceBinderAttachment  = TDyResourceBinderInstant<EDyResourceType::GLAttachment>;
using TDyIResourceBinderFrameBuffer = TDyResourceBinderInstant<EDyResourceType::GLFrameBuffer>;

///
/// @class TDyResourceBinder
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
///
template <EDyResourceType TType>
class TDyResourceBinder<TType, EDyLazy::Yes> final : public __TDyResourceBinderBase<TType>
{
private:
  using TSuper = __TDyResourceBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);
  TDyResourceBinder() = default;
  ~TDyResourceBinder() = default;

  /// @brief Try require resource with specifier name in given EDyResourceType.
  /// If resource is already bound to binder handle, detach it first and newly bind another resource into it.
  void TryRequireResource(_MIN_ const std::string& specifier)
  {
    TSuper::pSetSpecifierName(specifier);
    MDY_NOTUSED auto _1 = TSuper::pTryDetachResource();
    if (const auto flag = TSuper::pTryRequireResource();
        flag == DY_FAILURE)
    {
      /// @TODO IMEPLEMENT TEMPORAL RESOURCE BINDING. AND WAITING.
      MDY_UNEXPECTED_BRANCH();
    }
  }
};

template <EDyResourceType TType>
using TDyResourceBinderLazy = TDyResourceBinder<TType, EDyLazy::Yes>;
using TDyLResourceBinderShader    = TDyResourceBinderLazy<EDyResourceType::GLShader>;
using TDyLResourceBinderMesh      = TDyResourceBinderLazy<EDyResourceType::Mesh>;
using TDyLResourceBinderModel     = TDyResourceBinderLazy<EDyResourceType::Model>;
using TDyLResourceBinderTexture   = TDyResourceBinderLazy<EDyResourceType::Texture>;
using TDyLResourceBinderMaterial  = TDyResourceBinderLazy<EDyResourceType::Material>;
using TDyLResourceBinderAttachment  = TDyResourceBinderLazy<EDyResourceType::GLAttachment>;
using TDyLResourceBinderFrameBuffer = TDyResourceBinderLazy<EDyResourceType::GLFrameBuffer>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H