#ifndef GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H
#define GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H
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

#include <Dy/Helper/Wrapper/DMutexUniqueHashMap.h>
#include <Dy/Core/Resource/Information/FDyShaderInformation.h>
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>
#include <Dy/Core/Resource/Information/FDyModelInformation.h>
#include <Dy/Core/Resource/Information/FDyModelSkeletonInformation.h>
#include <Dy/Core/Resource/Information/FDyMaterialInformation.h>
#include <Dy/Core/Resource/Information/FDyAttachmentInformation.h>
#include <Dy/Core/Resource/Information/FDyFrameBufferInformation.h>
#include <Dy/Core/Resource/Information/FDyMeshInformation.h>
#include <Dy/Core/Resource/Information/FDyModelAnimScrapInformation.h>
#include <Dy/Core/Resource/Information/FDySoundInformation.h>
#include <Dy/Core/Resource/Type/TemplateRescInfoType.h>
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

/// @class MIORescInfo
/// @brief IO Resource Data information Manager.
class MIORescInfo final : public ISingleton<MIORescInfo>
{
  MDY_SINGLETON_PROPERTIES(MIORescInfo);
  MDY_SINGLETON_DERIVED(MIORescInfo);
public:
  template <EResourceType TType>
  MDY_NODISCARD typename __TDyRscInfo<TType>::type* GetPtrInformation(const std::string& specifier) noexcept
  {
    if constexpr (TType == EResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Mesh)    { return this->__mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Skeleton){ return this->__mModelSkeletonContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Material){ return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Sound){ return this->__mSoundContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLAttachment)  { return this->__mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) { return this->__mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::AnimationScrap){ return this->__mModelAnimScrapContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

  template <EResourceType TType>
  MDY_NODISCARD const typename __TDyRscInfo<TType>::type* GetPtrInformation(const std::string& specifier) const noexcept
  {
    if constexpr (TType == EResourceType::GLShader)     { return this->__mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Texture) { return this->__mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Mesh)    { return this->__mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Model)   { return this->__mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Skeleton){ return this->__mModelSkeletonContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Material){ return this->__mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Sound)   { return this->__mSoundContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLAttachment)  { return this->__mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) { return this->__mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::AnimationScrap){ return this->__mModelAnimScrapContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

private:
  /// @brief Insert result from TDyIO (I/O GC-IN Phase)
  void InsertResult(_MIN_ EResourceType type, _MIN_ void* ptrrawInstance);

  /// @brief Try remove information instance with iSpecifier name and iType. \n
  /// If not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess MDY_PRIVATE(TryRemove)(_MIN_ const std::string& iSpcifier, _MIN_ EResourceType iType);

  template <typename TType>
  using __THashMap = DMutexUniqueHashMap<std::string, TType>;

  __THashMap<FDyShaderInformation>  __mShaderContainer    = {};
  __THashMap<FDyTextureInformation> __mTextureContainer   = {};
  __THashMap<FDyMeshInformation>    __mMeshContainer      = {};
  __THashMap<FDyModelInformation>   __mModelContainer     = {};
  __THashMap<FDyModelSkeletonInformation>   __mModelSkeletonContainer = {};
  __THashMap<FDyModelAnimScrapInformation>  __mModelAnimScrapContainer = {};
  __THashMap<FDyMaterialInformation>__mMaterialContainer  = {};
  __THashMap<FDyAttachmentInformation> __mAttachmentContainer   = {};
  __THashMap<FDyFrameBufferInformation> __mFrameBufferContainer = {};
  __THashMap<FDySoundInformation> __mSoundContainer = {};

  friend class TDyIO;
  friend class FDyIOGC;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H