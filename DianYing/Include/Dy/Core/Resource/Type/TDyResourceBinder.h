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

#include <Dy/Meta/Type/EDyResourceType.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>

namespace dy
{

enum class EDyLazy : bool
{
  No,
  Yes
};

template <EDyResourceType TType>
struct __TDyBinderBase
{
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(__TDyBinderBase);
  using TPtrResource      = const typename __TResourceType<TType>::type*;
  using TTryGetReturnType = std::optional<TPtrResource>;

  __TDyBinderBase(_MIN_ const std::string& iSpecifierName) : mSpecifierName{iSpecifierName} {};
  __TDyBinderBase() {};

  /// @brief Release binder instance and detach it from specified Reference Instance.
  virtual ~__TDyBinderBase()
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrResource)) { MDY_NOT_IMPLEMENTED_ASSERT(); }
  }

  MDY_NODISCARD EDySuccess TryRequireResource() const noexcept
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
    return DY_FAILURE;
  }

  MDY_NODISCARD bool IsResourceExist() const noexcept
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
    return false;
  }

  MDY_NODISCARD TPtrResource Get() const noexcept
  {
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mPtrResource), "Resource pointer address must not be null when use it.");
    return this->mPtrResource;
  }

protected:
  /// @brief Set new specifier name. this function must be called in lazy resource type.
  void SetSpecifierName(_MIN_ const std::string& iNewSpecifier) noexcept
  {
    this->mSpecifierName = iNewSpecifier;
  }

private:
  /// @brief
  MDY_NODISCARD TTryGetReturnType TryGetResourcePtr() noexcept
  {
    MDY_ASSERT(this->mSpecifierName.empty() == false, "Specifier name must not be empty.");

    if constexpr (TType == EDyResourceType::Model)
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
    }
    else if constexpr (TType == EDyResourceType::GLShader)
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
    }
    else if constexpr (TType == EDyResourceType::Material)
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
    }
    else if constexpr (TType == EDyResourceType::Texture)
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
      return MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
    }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt); }
  }

  std::string   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  TPtrResource  mPtrResource    = MDY_INITIALIZE_NULL;
};

///
/// @class TDyResourceBinder
/// @brief
///
template <EDyResourceType TType, EDyLazy TIsLazy>
class TDyResourceBinder;

///
/// @class TDyResourceBinder
/// @brief Not lazy version of `TDyResourceBinder`.
///
template <EDyResourceType TType>
class TDyResourceBinder<TType, EDyLazy::No> final : public __TDyBinderBase<TType>
{
private:
  using TSuper = __TDyBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);

  TDyResourceBinder(_MIN_ const std::string& specifier) : TSuper{specifier}
  {
    MDY_NOTUSED auto _ = TSuper::TryRequireResource();
  }
  ~TDyResourceBinder() = default;
};

template <EDyResourceType TType>
using TDyResourceBinderInstant = TDyResourceBinder<TType, EDyLazy::No>;

///
/// @class TDyResourceBinder
/// @brief Not lazy version of `TDyResourceBinder`.
/// User have to require resource manually. but detach is held by automatically. (RAII)
///
template <EDyResourceType TType>
class TDyResourceBinder<TType, EDyLazy::Yes> final
{
private:
  using TSuper = __TDyBinderBase<TType>;
public:
  MDY_NOT_COPYABLE_MOVEABLE_PROPERTIES(TDyResourceBinder);

  TDyResourceBinder() {};
  ~TDyResourceBinder() = default;

  void TryBindResource(_MIN_ const std::string& specifier)
  {

  }
};

template <EDyResourceType TType>
using TDyResourceBinderLazy = TDyResourceBinder<TType, EDyLazy::No>;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_FDYRESOURCEBINDER_H