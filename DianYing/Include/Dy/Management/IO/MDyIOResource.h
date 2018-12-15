#ifndef GUARD_DY_MANAGEMENT_IO_MDYIORESOURCE_20181113_H
#define GUARD_DY_MANAGEMENT_IO_MDYIORESOURCE_20181113_H
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

#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>
#include <Dy/Helper/Wrapper/DDyMutexUniqueHashMap.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Meta/Type/EDyResourceType.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyResourceType;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDyIOResource
/// @brief IO Resource part manager (newly implemented)
///
class MDyIOResource final : public IDySingleton<MDyIOResource>
{
  MDY_SINGLETON_PROPERTIES(MDyIOResource);
  MDY_SINGLETON_DERIVED(MDyIOResource);
public:
  template <EDyResourceType TType>
  MDY_NODISCARD typename __TResourceType<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier)
  {
    if constexpr (TType == EDyResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Texture) { return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

  template <EDyResourceType TType>
  MDY_NODISCARD const typename __TResourceType<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier) const noexcept
  {
    if constexpr (TType == EDyResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Texture) { return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

private:
  /// @brief Insert result from TDyIO (I/O GC-IN Phase)
  void InsertResult(_MIN_ EDyResourceType type, _MIN_ void* ptrrawInstance);

  template <typename TType>
  using __THashMap = DDyMutexUniqueHashMap<std::string, TType>;

  __THashMap<FDyShaderResource>   __mShaderContainer   = {};
  __THashMap<FDyTextureResource>  __mTextureContainer  = {};
  __THashMap<FDyModelResource>    __mModelContainer    = {};
  __THashMap<FDyMaterialResource> __mMaterialContainer = {};

  friend class TDyIO;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H