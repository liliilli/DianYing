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
#include <Dy/Core/Resource/Information/FInformationShader.h>
#include <Dy/Core/Resource/Information/FInformationTexture.h>
#include <Dy/Core/Resource/Information/FInformationModel.h>
#include <Dy/Core/Resource/Information/FInformationModelSkeleton.h>
#include <Dy/Core/Resource/Information/FInformationMaterial.h>
#include <Dy/Core/Resource/Information/FInformationAttachment.h>
#include <Dy/Core/Resource/Information/FInformationFrameBuffer.h>
#include <Dy/Core/Resource/Information/FInformationMesh.h>
#include <Dy/Core/Resource/Information/FInformationModelAnimScrap.h>
#include <Dy/Core/Resource/Information/FInformationSound.h>
#include <Dy/Core/Resource/Type/XTemplateRescInfoType.h>
#include <Dy/Meta/Type/EResourceType.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>

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
  MDY_NODISCARD typename __TDyRscInfo<TType>::type* 
  GetPtrInformation(const std::string& specifier) noexcept
  {
    if constexpr (TType == EResourceType::GLShader)     { return this->mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Texture) { return this->mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Mesh)    { return this->mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Model)   { return this->mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Skeleton){ return this->mModelSkeletonContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Material){ return this->mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Sound)   { return this->mSoundContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLAttachment)  { return this->mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) { return this->mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::AnimationScrap){ return this->mModelAnimScrapContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

  template <EResourceType TType>
  MDY_NODISCARD const typename __TDyRscInfo<TType>::type* 
  GetPtrInformation(const std::string& specifier) const noexcept
  {
    if constexpr (TType == EResourceType::GLShader)     { return this->mShaderContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Texture) { return this->mTextureContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Mesh)    { return this->mMeshContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Model)   { return this->mModelContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Skeleton){ return this->mModelSkeletonContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Material){ return this->mMaterialContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::Sound)   { return this->mSoundContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLAttachment)  { return this->mAttachmentContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::GLFrameBuffer) { return this->mFrameBufferContainer.TryGetInstancePtr(specifier); }
    else if constexpr (TType == EResourceType::AnimationScrap){ return this->mModelAnimScrapContainer.TryGetInstancePtr(specifier); }
    else { MDY_UNEXPECTED_BRANCH_BUT_RETURN(nullptr); }
  }

private:
  /// @brief Insert result from TRescIO (I/O GC-IN Phase)
  void InsertResult(EResourceType type, void* ptrrawInstance);

  /// @brief Try remove information instance with iSpecifier name and iType. \n
  /// If not exist, return EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess __TryRemove(const std::string& iSpcifier, EResourceType iType);

  template <typename TType>
  using THashMap = DMutexUniqueHashMap<std::string, TType>;

  THashMap<FInformationShader>  mShaderContainer    = {};
  THashMap<FInformationTexture> mTextureContainer   = {};
  THashMap<FInformationMesh>    mMeshContainer      = {};
  THashMap<FInformationModel>   mModelContainer     = {};
  THashMap<FInformationModelSkeleton>   mModelSkeletonContainer = {};
  THashMap<FInformationModelAnimScrap>  mModelAnimScrapContainer = {};
  THashMap<FInformationMaterial>mMaterialContainer  = {};
  THashMap<FInformationAttachment> mAttachmentContainer   = {};
  THashMap<FInformationFrameBuffer> mFrameBufferContainer = {};
  THashMap<FInformationSound> mSoundContainer = {};

  friend class TRescIO;
  friend class FRescIOGC;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_IO_IODATAMANANGER_20181113_H