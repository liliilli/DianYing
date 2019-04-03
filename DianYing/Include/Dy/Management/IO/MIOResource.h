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

#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Resource/Resource/AResourceTextureBase.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Core/Resource/Resource/FResourceModel.h>
#include <Dy/Core/Resource/Resource/FResourceMaterial.h>
#include <Dy/Core/Resource/Resource/AResourceAttachmentBase.h>
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>

#include <Dy/Core/Resource/Type/XTemplateRescInfoType.h>
#include <Dy/Helper/Wrapper/DMutexUniqueHashMap.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Meta/Type/EResourceType.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EResourceType;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class MIOResource
/// @brief IO Resource actual resource part manager.
class MIOResource final : public ISingleton<MIOResource>
{
  MDY_SINGLETON_PROPERTIES(MIOResource);
  MDY_SINGLETON_DERIVED(MIOResource);
public:
  template <EResourceType TType>
  MDY_NODISCARD typename __TResourceType<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier)
  {
    if constexpr (TType == EResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Mesh)    { return this->__mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Material){ return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLAttachment)  { return this->__mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) { return this->__mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

  template <EResourceType TType>
  MDY_NODISCARD const typename __TResourceType<TType>::type* GetPtrInformation(_MIN_ const std::string& specifier) const noexcept
  {
    if constexpr (TType == EResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Mesh)    { return this->__mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Material) { return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLAttachment)  { return this->__mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) { return this->__mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

private:
  /// @brief Insert result from TRescIO (I/O GC-IN Phase)
  void InsertResult(_MIN_ EResourceType type, _MIN_ void* ptrrawInstance);

  /// @brief Try remove information instance with iSpecifier name and iType. \n
  /// If not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess MDY_PRIVATE(TryRemove)(_MIN_ const std::string& iSpcifier, _MIN_ EResourceType iType);

  template <typename TType>
  using __THashMap = DMutexUniqueHashMap<std::string, TType>;

  __THashMap<FResourceShader>   __mShaderContainer    = {};
  __THashMap<AResourceTextureBase>  __mTextureContainer   = {};
  __THashMap<FResourceMesh>     __mMeshContainer      = {};
  __THashMap<FResourceModel>    __mModelContainer     = {};
  __THashMap<FResourceMaterial> __mMaterialContainer  = {};
  __THashMap<AResourceAttachmentBase>   __mAttachmentContainer  = {};
  __THashMap<AResourceFrameBufferBase>  __mFrameBufferContainer = {};

  friend class TRescIO;
  friend class FRescIOGC;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H