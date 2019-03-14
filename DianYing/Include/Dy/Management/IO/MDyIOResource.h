#ifndef GUARD_DY_MANAGEMENT_IO_MDYIORESOURCE_20181113_H
#define GUARD_DY_MANAGEMENT_IO_MDYIORESOURCE_20181113_H
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

#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>

#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>
#include <Dy/Helper/Wrapper/DMutexUniqueHashMap.h>
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
    else if constexpr (TType == EDyResourceType::Mesh)    { return this->__mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Material){ return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::GLAttachment)  { return this->__mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::GLFrameBuffer) { return this->__mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

  template <EDyResourceType TType>
  MDY_NODISCARD const typename __TResourceType<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier) const noexcept
  {
    if constexpr (TType == EDyResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Mesh)    { return this->__mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::Material) { return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::GLAttachment)  { return this->__mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EDyResourceType::GLFrameBuffer) { return this->__mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

private:
  /// @brief Insert result from TDyIO (I/O GC-IN Phase)
  void InsertResult(_MIN_ EDyResourceType type, _MIN_ void* ptrrawInstance);

  /// @brief Try remove information instance with iSpecifier name and iType. \n
  /// If not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess MDY_PRIVATE(TryRemove)(_MIN_ const std::string& iSpcifier, _MIN_ EDyResourceType iType);

  template <typename TType>
  using __THashMap = DMutexUniqueHashMap<std::string, TType>;

  __THashMap<FDyShaderResource>   __mShaderContainer    = {};
  __THashMap<FDyTextureResource>  __mTextureContainer   = {};
  __THashMap<FDyMeshResource>     __mMeshContainer      = {};
  __THashMap<FDyModelResource>    __mModelContainer     = {};
  __THashMap<FDyMaterialResource> __mMaterialContainer  = {};
  __THashMap<FDyAttachmentResource>   __mAttachmentContainer  = {};
  __THashMap<FDyFrameBufferResource>  __mFrameBufferContainer = {};

  friend class TDyIO;
  friend class FDyIOGC;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H