#include <precompiled.h>
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

/// Header file
#include <Dy/Management/IO/MIOMeta.h>

#include <optional>
#include <regex>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Core/Thread/SIOConnectionHelper.h>
#include <Dy/Core/GEngine.h>

#include <Dy/Helper/System/Pointer.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperString.h>
#include <Dy/Helper/Library/HelperRegex.h>

#include <Dy/Management/MSetting.h>
#include <Dy/Management/MScript.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetLayoutMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>
#include <Dy/Meta/Information/MetaInfoModel.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Meta/Information/MetaInfoAttachment.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>
#include <Dy/Meta/Information/MetaInfoModelMesh.h>
#include <Dy/Meta/Information/MetaInfoModelSkeleton.h>
#include <Dy/Meta/Information/MetaInfoModelAnim.h>
#include <Dy/Meta/Information/MetaInfoSound.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Information/MetaInfoFont.h>
#include <Dy/Meta/Information/PrefabMetaInformation.h>
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>
#include <Dy/Meta/Information/MetaInfoRenderPipeline.h>
#include <Dy/Meta/Information/MetaInfoRenderItem.h>
#include <Dy/Meta/Resource/PLevelInstanceMetaInfo.h>

//!
//! Local tranlation unit variables
//!

namespace
{

//!
//! Script Meta information header list
//!

MDY_SET_IMMUTABLE_STRING(sCategoryList,     "List");

//!
//! Prefab Meta information header list
//!

MDY_SET_IMMUTABLE_STRING(sCategoryMeta,         "Meta");
MDY_SET_IMMUTABLE_STRING(sCategoryObjectList,   "ObjectList");

std::unique_ptr<dy::PDyMetaWidgetRootDescriptor> 
DyCreateWidgetMetaInformation(_MIN_ const nlohmann::json& jsonAtlas)
{
  using namespace dy;
  ///
  /// @brief  Check widget meta information list.
  /// @param  atlas Valid json atlas instance.
  /// @return If succeeded, return DY_SUCCESS an
  ///
  static auto CheckWidgetCategories = [](_MIN_ const nlohmann::json& atlas) -> EDySuccess
  {
    using namespace json;
    if (json::HasJsonKey(atlas, sCategoryMeta) == false)       { return DY_FAILURE; }
    if (json::HasJsonKey(atlas, sCategoryObjectList) == false) { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  ///
  /// @brief  Check meta widget component information header list.
  /// @param  atlas Valid json atlas instance.
  /// @return If succeeded, return DY_SUCCESS flag for representing success.
  ///
  static auto CheckComponentCommonHeaders = [](_MIN_ const nlohmann::json& atlas) -> EDySuccess
  {
    using namespace json;
    if (json::HasJsonKey(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Name) == false) { return DY_FAILURE; }
    if (json::HasJsonKey(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Type) == false) { return DY_FAILURE; }
    if (json::HasJsonKey(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Parent) == false) { return DY_FAILURE; }
    if (json::HasJsonKey(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Details) == false) { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (const auto flag = CheckWidgetCategories(jsonAtlas); flag == DY_FAILURE)
  {
    DyPushLogCritical("Json log : {}", jsonAtlas.dump(2));
    MDY_UNEXPECTED_BRANCH(); return nullptr;
  }

  // (3) Make widget component meta information instance sequencially.
  const auto& componentAtlas = jsonAtlas[(sCategoryObjectList)];
  MIOMeta::THashMap<PDyMetaWidgetChildableBaseDesc::TElementType> tempWidgetObjectMap = {};

  for (const auto& componentInfo : componentAtlas)
  { // Check Header list integrity
    MDY_ASSERT_MSG(CheckComponentCommonHeaders(componentInfo) == DY_SUCCESS, "Failed to check common headers of item.");

    const auto componentType = json::GetValueFrom<EDyWidgetComponentType>(componentInfo, PDyMetaWidgetCommonBaseDesc::sHeader_Type);
    switch (componentType)
    {
    case EDyWidgetComponentType::Text:
    { // Text component
      auto instance = PDyMetaWidgetTextDescriptor::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mUiObjectSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair<>(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
      instance = nullptr;
    } break;
    case EDyWidgetComponentType::HorizontalLayout:
    { // Horizontal layout component
      auto instance = PDyMetaWidgetHorizontalLayout::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mUiObjectSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair<>(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
    } break;
    case EDyWidgetComponentType::VerticalLayout:
    { // Vertical layout component
      auto instance = PDyMetaWidgetVerticalLayout::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mUiObjectSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair<>(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
    } break;
    case EDyWidgetComponentType::BasicGaugeBar:
    { // Basic gauge bar component. (plain color, without material and any effects).
      auto instance = PDyMetaWidgetBarDescriptor::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mUiObjectSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
    } break;
    case EDyWidgetComponentType::Image:
    { // Image component.
      auto instance = PDyMetaWidgetImageDescriptor::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mUiObjectSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
    } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  // (4) Make a tree by iterating unordered_map, tempComponentMetaList.
  for (auto it = tempWidgetObjectMap.begin(); it != tempWidgetObjectMap.end(); ++it)
  {
    if (auto& item = it->second.second; item->mParentSpecifierName.empty() == false)
    { // Find parent's object instance iterator pointer.
      auto parentIt = tempWidgetObjectMap.find(item->mParentSpecifierName);
      MDY_ASSERT_MSG(parentIt != tempWidgetObjectMap.end(), "Unexpected error occurred.");

      // Move pair to parent's children map and erase iterator of it.
      auto& parentInstance = parentIt->second.second;
      parentInstance->mChildComponentList.try_emplace(it->first, std::move(it->second));
      it = tempWidgetObjectMap.erase(it);
    }
  }

  // (5) Make root widget meta instance and retrieve information from headers.
  //     ...and Make heap.
  auto rootInstance = PDyMetaWidgetRootDescriptor::CreateMetaInformation(jsonAtlas[(sCategoryMeta)]);
  for (auto it = tempWidgetObjectMap.begin(); it != tempWidgetObjectMap.end(); ++it)
  {
    auto [_, result] = rootInstance->mChildComponentList.try_emplace(it->first, std::move(it->second));
    MDY_ASSERT_MSG(result == true, "Unexpected error occurred.");
  }

  return rootInstance;
}

void MoveMetaPrefabIntoParentRecursively(
    _MINOUT_ dy::TPrefabMetaInfoList& p, 
    _MIN_ const std::vector<std::string>& parentSpecifierList,
    _MIN_ const TU32 level,
    _MINOUT_ dy::TPrefabMetaInfoList::value_type& object)
{
  for (auto& parentObject : p)
  { 
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(object), "Unexpected error occurred");
    if (parentObject == object)           { continue; }
    if (MDY_CHECK_ISEMPTY(parentObject))  { continue; }

    if (parentObject->mSpecifierName == parentSpecifierList[level])
    { // 
      if (parentSpecifierList.size() == level + 1)
      { // Move it and return.
        parentObject->mChildrenList.emplace_back(std::move(object));
        return;
      }
      else
      { // Call function recursively.
        return MoveMetaPrefabIntoParentRecursively(p, parentSpecifierList, level + 1, object);
      }
    }
  }

  MDY_UNEXPECTED_BRANCH();
  return;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

class MIOMeta::Impl final : public ISingleton<MIOMeta>
{
public:
  template <typename TType>
  using THashMap = std::unordered_map<std::string, TType>;
  using TResourceSpecifierList = std::vector<DDyResourceName>;

  const PDyScriptInstanceMetaInfo&
  GetScriptMetaInformation(const std::string& iScriptName) const;

  const THashMap<PDyScriptInstanceMetaInfo>&
  GetRefGlobalScriptMetaInfoContainer() const noexcept;

  const PDyPrefabInstanceMetaInfo&
  GetPrefabMetaInformation(const std::string& iPrefabName) const;

  const PDyMetaFontInformation&
  GetFontMetaInformation(const std::string& iFontName) const;

  const PDyMetaWidgetRootDescriptor&
  GetWidgetMetaInformation(const std::string& iWidgetMetaName) const;

  const PDyGLShaderInstanceMetaInfo&
  GetShaderMetaInformation(const std::string& iShaderName) const;
  /// @brief  Get builtin mesh instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyMeshInstanceMetaInfo&
  GetBtMeshMetaInformation(const std::string& specifier) const;
  /// @brief  Get model instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyModelInstanceMetaInfo&
  GetModelMetaInformation(const std::string& specifier) const;
  /// @brief  Get model instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyModelSkelInstanceMetaInfo&
  GetModelSkeletonMetaInformation(const std::string& specifier) const;
  /// @brief  Get model animation scrap meta information from container.
  /// @return Return value is undefined if not found.
  const PDyModelAnimInstanceMetaInfo&
  GetModelAnimScrapMetaInformation(const std::string& specifier) const;
  /// @brief  Get texture instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyTextureInstanceMetaInfo&
  GetTextureMetaInformation(const std::string& specifier) const;
  /// @brief  Get material instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyMaterialInstanceMetaInfo&
  GetMaterialMetaInformation(const std::string& specifier) const;
  /// @brief  Get framebuffer instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyGlFrameBufferInstanceMetaInfo&
  GetGlFrameBufferMetaInformation(const std::string& specifier) const;
  /// @brief  Get attachment instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDyGlAttachmentInstanceMetaInfo&
  GetGLAttachmentMetaInformation(const std::string& specifier) const;
  /// @brief  Get sound instance meta information from container.
  /// @return Return value is undefined if not found.
  const PDySoundInstanceMetaInfo&
  GetSoundMetaInformation(const std::string& specifier) const;

  /// @brief Get loading widget pointer if exist, or return nullptr (nothing) value
  const PDyMetaWidgetRootDescriptor*
  MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)() const noexcept;

  /// @brief Check loading widget is exist and ready.
  /// @return If found, return true or false.
  bool IsLoadingWidgetMetaInfoExist() const noexcept;

  /// @brief Get booting loading resource specifier list. \n
  /// This function must not be called twice.
  void MDY_PRIVATE(PopulateBootResourceSpecifierList)() const noexcept;

  /// @brief Get global loading resource specifier list. \n
  /// THis function must not be called twice. 
  void MDY_PRIVATE(PopulateGlobalResourceSpecifierList)() const noexcept;

  void MDY_PRIVATE(InitiateMetaInformation)();
  void MDY_PRIVATE(InitiateMetaInformationComp)(const nlohmann::json& iJson);

  EDySuccess pReadScriptResourceMetaInformation(const std::string& metaFilePath);
  EDySuccess pReadPrefabResourceMetaInformation(const std::string& metaFilePath);
  EDySuccess pReadWidgetResourceMetaInformation(const std::string& metaFilePath);
  EDySuccess pReadLevelResourceMetaInformation (const std::string& metaFilepath);

  EDySuccess pReadFontResourceMetaInformation     (const std::string& metaFilePath);
  EDySuccess pReadModelResourceMetaInformation    (const std::string& metaFilePath);
  EDySuccess pReadModelMeshResourceMetaInformation(const std::string& metaFilePath);
  EDySuccess pReadModelSkeletonMetaInformation    (const std::string& metaFilePath);
  EDySuccess pReadModelAnimationMetaInformation   (const std::string& metaFilePath);
  EDySuccess pReadTextureResourceMetaInformation  (const std::string& metaFilePath);
  EDySuccess pReadShaderResourceMetaInformation   (const std::string& metaFilePath);
  EDySuccess pReadMaterialResourceMetaInformation (const std::string& metaFilePath);
  EDySuccess pReadSoundResourceMetaInformation    (const std::string& metaFilePath);

  EDySuccess pReadScriptMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadPrefabMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadWidgetMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadFontMetaAtlas  (const nlohmann::json& iJson);
  EDySuccess pReadLevelMetaAtlas (const nlohmann::json& iJson);
  EDySuccess pReadShaderMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadSoundMetaAtlas    (const nlohmann::json& iJson);
  EDySuccess pReadTextureMetaAtlas  (const nlohmann::json& iJson);
  EDySuccess pReadMaterialMetaAtlas (const nlohmann::json& iJson);
  EDySuccess pReadModelAnimMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadModelMeshMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadModelSkelMetaAtlas(const nlohmann::json& iJson);
  EDySuccess pReadModelMetaAtlas    (const nlohmann::json& iJson);

  EDySuccess pfAddWidgetMetaInformation (const std::string& metaInformationString);
  EDySuccess pfAddScriptMetaInformation (const PDyScriptInstanceMetaInfo& metaInfo);
  EDySuccess pfAddGLShaderMetaInfo      (const PDyGLShaderInstanceMetaInfo& metaInfo);
  EDySuccess pfAddBuiltinMeshMetaInfo   (const PDyMeshInstanceMetaInfo& metaInfo);
  EDySuccess pfAddModelMetaInfo         (const PDyModelInstanceMetaInfo& metaInfo);
  EDySuccess pfAddTextureMetaInfo       (const PDyTextureInstanceMetaInfo& metaInfo);
  EDySuccess pfAddMaterialMetaInfo      (const PDyMaterialInstanceMetaInfo& metaInfo);
  EDySuccess pfAddGLAttachmentMetaInfo  (const PDyGlAttachmentInstanceMetaInfo& metaInfo);
  EDySuccess pfAddGLFrameBufferMetaInfo (const PDyGlFrameBufferInstanceMetaInfo& metaInfo);
  EDySuccess MDY_PRIVATE(AddLoadingWidgetMetaInformation)(const std::string& widgetMetaInfo);

  EDySuccess MDY_PRIVATE(AddBootResourceSpecifierList)(const TResourceSpecifierList& list);
  EDySuccess MDY_PRIVATE(AddGlobalResourceSpecifierList)(const TResourceSpecifierList& list);

  /// Level meta information map.
  //THashMap<PDyLevelConstructMetaInfo>   mLevelInfoMap   = {};
  THashMap<PLevelInstanceMetaInfo>      mLevelMetaInfo = {};
  /// Script meta information map.
  THashMap<PDyScriptInstanceMetaInfo>   mScriptMetaInfo = {};
  /// @brief Global script meta information hash-map.
  THashMap<PDyScriptInstanceMetaInfo>   mGlobalScriptMetaInfo = {};
  /// Font meta information map.
  THashMap<PDyMetaFontInformation>      mFontMetaInfo   = {};
  /// GL shader meta information map.
  THashMap<PDyGLShaderInstanceMetaInfo> mShaderMetaInfo = {};
  /// Model meta information map.
  THashMap<PDyModelInstanceMetaInfo>    mModelMetaInfo  = {};
  /// Model Mesh meta information map.
  THashMap<PDyMeshInstanceMetaInfo>     mModelMeshMetaInfo = {};
  /// Model Skeleton meta information map.
  THashMap<PDyModelSkelInstanceMetaInfo>mModelSkeletonMetaInfo = {};
  /// Model Skeleton meta information map.
  THashMap<PDyModelAnimInstanceMetaInfo>mModelAnimScrapMetaInfo = {};
  /// Texture meta information map.
  THashMap<PDyTextureInstanceMetaInfo>  mTextureMetaInfo= {};
  /// Material meta information map.
  THashMap<PDyMaterialInstanceMetaInfo> mMaterialMetaInfo = {};
  /// Render pipeline meta information map.
  THashMap<PDyRenderPipelineInstanceMetaInfo> mRenderPipelineMetaInfo = {};
  /// Render item meta information map.
  THashMap<PDyRenderItemInstanceMetaInfo> mRenderIteMIOMeta = {};
  /// @brief Sound meta information map.
  THashMap<PDySoundInstanceMetaInfo>    mSoundMetaInfo = {};

  //! Internal

  /// Attahchment meta information map
  THashMap<PDyGlAttachmentInstanceMetaInfo>   mAttachmentMetaInfo = {};
  /// Framebuffer object meta information map
  THashMap<PDyGlFrameBufferInstanceMetaInfo>  mFrameBufferMetaInfo = {};

  /// Resource specifier name list for loading in boot sequence of MDySync.
  TResourceSpecifierList              mBootResourceSpecifierList    = {};
  std::vector<TResourceSpecifierList> mGlobalResourceSpecifierList  = {};  

  //!
  //! Hierarchial meta information containers.
  //!

  /// Prefab meta information map. (Temporary use std::unique_ptr, for reconstructing children tree)
  THashMap<std::unique_ptr<PDyPrefabInstanceMetaInfo>>   mPrefabMetaInfo = {};
  /// Widget meta information map.
  THashMap<std::unique_ptr<PDyMetaWidgetRootDescriptor>> mWidgetMetaInfo = {};

  /// Loading widget meta information map.
  std::unique_ptr<PDyMetaWidgetRootDescriptor> mLoadingWidgetMetaInfo = MDY_INITIALIZE_NULL;
};

} /// ::dy namespace
#include <Dy/Management/Inline/MMetaInfoImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MIOMeta::pfInitialize() 
{ 
  this->mInternal = new Impl();
  return DY_SUCCESS; 
}

EDySuccess MIOMeta::pfRelease()
{
  delete this->mInternal; this->mInternal = nullptr;
  return DY_SUCCESS;
}

const PLevelInstanceMetaInfo* 
MIOMeta::GetLevelMetaInformation(const std::string& iLevelName) const noexcept
{
  if (this->IsLevelMetaInformation(iLevelName) == false)
  {
    return nullptr;
  }

  return &this->mInternal->mLevelMetaInfo.at(iLevelName);
}

std::vector<std::pair<std::string, const PDyScriptInstanceMetaInfo*>> 
MIOMeta::GetGlobalScriptMetaInfos() const noexcept
{
  std::vector<std::pair<std::string, const PDyScriptInstanceMetaInfo*>> resultList;

  const auto& container = this->mInternal->GetRefGlobalScriptMetaInfoContainer();
  for (const auto& [scriptName, scriptMeta] : container)
  {
    resultList.emplace_back(std::make_pair(scriptName, &scriptMeta));
  }

  return resultList;
}

const PDyScriptInstanceMetaInfo& 
MIOMeta::GetScriptMetaInformation(const std::string& specifierName) const
{
  return this->mInternal->GetScriptMetaInformation(specifierName);
}

const PDyPrefabInstanceMetaInfo& 
MIOMeta::GetPrefabMetaInformation(const std::string& iPrefabName) const
{
  return this->mInternal->GetPrefabMetaInformation(iPrefabName);
}

const PDyMetaFontInformation& 
MIOMeta::GetFontMetaInformation(const std::string& iFontName) const
{
  return this->mInternal->GetFontMetaInformation(iFontName);
}

const PDyMetaWidgetRootDescriptor& 
MIOMeta::GetWidgetMetaInformation(const std::string& iSpecifierName) const
{
  return this->mInternal->GetWidgetMetaInformation(iSpecifierName);
}

const PDyGLShaderInstanceMetaInfo& 
MIOMeta::GetGLShaderMetaInformation(const std::string& iShaderName) const
{
  return this->mInternal->GetShaderMetaInformation(iShaderName);
}

const PDyMeshInstanceMetaInfo& 
MIOMeta::GetBtMeshMetaInformation(const std::string& iMeshName) const
{
  return this->mInternal->GetBtMeshMetaInformation(iMeshName);
}

const PDyModelInstanceMetaInfo& 
MIOMeta::GetModelMetaInformation(const std::string& iModelName) const
{
  return this->mInternal->GetModelMetaInformation(iModelName);
}

const PDyModelSkelInstanceMetaInfo& 
MIOMeta::GetModelSkeletonMetaInformation(const std::string& iSkeletonName) const
{
  return this->mInternal->GetModelSkeletonMetaInformation(iSkeletonName);
}

const PDyModelAnimInstanceMetaInfo& 
MIOMeta::GetModelAnimScrapMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetModelAnimScrapMetaInformation(specifier);
}

const PDyTextureInstanceMetaInfo& 
MIOMeta::GetTextureMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetTextureMetaInformation(specifier);
}

const PDyMaterialInstanceMetaInfo& 
MIOMeta::GetMaterialMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetMaterialMetaInformation(specifier);
}

const PDyGlFrameBufferInstanceMetaInfo& 
MIOMeta::GetGlFrameBufferMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetGlFrameBufferMetaInformation(specifier);
}

const PDyGlAttachmentInstanceMetaInfo& 
MIOMeta::GetGLAttachmentMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetGLAttachmentMetaInformation(specifier);
}

const PDySoundInstanceMetaInfo& MIOMeta::GetSoundMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetSoundMetaInformation(specifier);
}

const PDyRenderPipelineInstanceMetaInfo& 
MIOMeta::GetRenderPipeline(const std::string& iRenderPipelineSpecifier) const
{
  MDY_ASSERT_FORCE(this->IsRenderPipelineExist(iRenderPipelineSpecifier) == true);
  return this->mInternal->mRenderPipelineMetaInfo.at(iRenderPipelineSpecifier);
}

const PDyRenderItemInstanceMetaInfo& 
MIOMeta::GetRenderItem(const std::string& iRenderItemSpecifier) const
{
  MDY_ASSERT_FORCE(this->IsRenderItemExist(iRenderItemSpecifier) == true);
  return this->mInternal->mRenderIteMIOMeta.at(iRenderItemSpecifier);
}

MDY_NODISCARD const PDyMetaWidgetRootDescriptor* 
MIOMeta::MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)() const noexcept
{
  return this->mInternal->__TryGetLoadingWidgetMetaLoading();
}

bool MIOMeta::IsGLShaderMetaInfoExist(const std::string & specifier) const noexcept
{
  return Contains(this->mInternal->mShaderMetaInfo, specifier);
}

bool MIOMeta::IsMeshMetaInfoExist(const std::string & specifier) const noexcept
{
  return Contains(this->mInternal->mModelMeshMetaInfo, specifier);
}

bool MIOMeta::IsLevelMetaInformation(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mLevelMetaInfo, specifier);
}

bool MIOMeta::IsModelMetaInfoExist(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mModelMetaInfo, specifier);
}

bool MIOMeta::IsModelSkeletonMetaInfoExist(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mModelSkeletonMetaInfo, specifier);
}

bool MIOMeta::IsModelAnimScrapMetaInfoExist(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mModelAnimScrapMetaInfo, specifier);
}

bool MIOMeta::IsTextureMetaInfoExist(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mTextureMetaInfo, specifier);
}

bool MIOMeta::IsMaterialMetaInfoExist(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mMaterialMetaInfo, specifier);
}

bool MIOMeta::IsWidgetMetaInfoExist(const std::string& specifier) const noexcept
{
  return Contains(this->mInternal->mWidgetMetaInfo, specifier);
}

bool MIOMeta::IsScriptMetaInformationExist(const std::string& specifierName) const noexcept
{
  return Contains(this->mInternal->mScriptMetaInfo, specifierName);
}

bool MIOMeta::IsPrefabMetaInformationExist(const std::string& specifierName) const noexcept
{
  return Contains(this->mInternal->mPrefabMetaInfo, specifierName);
}

bool MIOMeta::IsFontMetaInformationExist(const std::string& specifierName) const noexcept
{
  return Contains(this->mInternal->mFontMetaInfo, specifierName);
}

bool MIOMeta::IsAttachmentMetaInfoExist(const std::string& specifierName) const noexcept
{
  return Contains(this->mInternal->mAttachmentMetaInfo, specifierName);
}

bool MIOMeta::IsFrameBufferMetaInfoExist(const std::string& speicfierName) const noexcept
{
  return Contains(this->mInternal->mFrameBufferMetaInfo, speicfierName);
}

bool MIOMeta::IsSoundMetaInfoExist(const std::string& specifierName) const noexcept
{
  return Contains(this->mInternal->mSoundMetaInfo, specifierName);
}

bool MIOMeta::IsRenderPipelineExist(const std::string& iRenderPipelineName) const noexcept
{
  return Contains(this->mInternal->mRenderPipelineMetaInfo, iRenderPipelineName);
}

bool MIOMeta::IsRenderItemExist(const std::string& iRenderItemName) const noexcept
{
  return Contains(this->mInternal->mRenderIteMIOMeta, iRenderItemName);
}

bool MIOMeta::IsLoadingWidgetMetaInfoExist() const noexcept
{
  return this->mInternal->mLoadingWidgetMetaInfo != nullptr;
}

void MIOMeta::MDY_PRIVATE(PopulateBootResourceSpecifierList)() const noexcept
{
  this->mInternal->MDY_PRIVATE(PopulateBootResourceSpecifierList)();
}

void MIOMeta::MDY_PRIVATE(PopulateGlobalResourceSpecifierList)() const noexcept
{
  this->mInternal->MDY_PRIVATE(PopulateGlobalResourceSpecifierList)();
}

void MIOMeta::MDY_PRIVATE(InitiateMetaInformation)()
{
  this->mInternal->__InitiateMetaInformation();
}

void MIOMeta::MDY_PRIVATE(InitiateMetaInformationComp)(const nlohmann::json& iJson)
{
  this->mInternal->__InitiateMetaInformationComp(iJson);
}

EDySuccess MIOMeta::pReadScriptResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadScriptResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadPrefabResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadPrefabMetaAtlas(metaFilePath);
}

EDySuccess MIOMeta::pReadFontResourceMetaInformation(const std::string& metaFilePath)
{ 
  return this->mInternal->pReadFontResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadModelResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadModelMeshResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelMeshResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadModelSkeletonMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelSkeletonMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadModelAnimationMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelAnimationMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadTextureResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadTextureResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadShaderResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadShaderResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadMaterialResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadMaterialResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadLevelResourceMetaInformation(const std::string& metaFilepath)
{
  return this->mInternal->pReadLevelResourceMetaInformation(metaFilepath);
}

EDySuccess MIOMeta::pReadWidgetResourceMetaInformation(const std::string& metaFilePath)
{ 
  return this->mInternal->pReadWidgetResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadSoundResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadSoundResourceMetaInformation(metaFilePath);
}

EDySuccess MIOMeta::pReadScriptMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadScriptMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadPrefabMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadPrefabMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadWidgetMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadWidgetMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadFontMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadFontMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadLevelMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadLevelMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadShaderMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadShaderMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadSoundMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadSoundMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadTextureMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadTextureMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadMaterialMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadMaterialMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadModelAnimMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelAnimMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadModelMeshMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelMeshMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadModelSkelMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelSkelMetaAtlas(iJson);
}

EDySuccess MIOMeta::pReadModelMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelMetaAtlas(iJson);
}

EDySuccess MIOMeta::pfAddPrefabMetaInfo(const std::string_view& iMetaInfo)
{
  const nlohmann::json jsonAtlas = nlohmann::json::parse(iMetaInfo);
  auto prefabMetaInfo = PDyPrefabInstanceMetaInfo::CreateMetaInformation(jsonAtlas);

  const auto name = prefabMetaInfo->mSpecifierName;
  auto [_, isSucceeded] = this->mInternal->mPrefabMetaInfo.try_emplace(name, std::move(prefabMetaInfo));
  if (isSucceeded == false) { return DY_FAILURE; }

  return DY_SUCCESS;
}

EDySuccess MIOMeta::pfAddFontMetaInfo(const PBuiltinMetaFontInfo& iMetaInfo)
{
  this->mInternal->mFontMetaInfo.try_emplace(
    iMetaInfo.mIdentifier, 
    static_cast<PDyMetaFontInformation>(iMetaInfo));

  return DY_SUCCESS;
}

EDySuccess MIOMeta::pfAddWidgetMetaInformation(const std::string& metaInformationString)
{
  return this->mInternal->pfAddWidgetMetaInformation(metaInformationString);
}

EDySuccess MIOMeta::MDY_PRIVATE(AddLoadingWidgetMetaInformation)(const std::string& widgetMetaInfo)
{
  return this->mInternal->MDY_PRIVATE(AddLoadingWidgetMetaInformation)(widgetMetaInfo);
}

EDySuccess MIOMeta::pfAddScriptMetaInformation(const PDyScriptInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddScriptMetaInformation(metaInfo);
}

EDySuccess MIOMeta::pfAddGLShaderMetaInfo(const PDyGLShaderInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddGLShaderMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddBuiltinMeshMetaInfo(const PDyMeshInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddBuiltinMeshMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddModelMetaInfo(const PDyModelInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddModelMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddTextureMetaInfo(const PDyTextureInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddTextureMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddMaterialMetaInfo(const PDyMaterialInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddMaterialMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddGLAttachmentMetaInfo(const PDyGlAttachmentInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddGLAttachmentMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddGLFrameBufferMetaInfo(const PDyGlFrameBufferInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddGLFrameBufferMetaInfo(metaInfo);
}

EDySuccess MIOMeta::pfAddRenderPipelineMetaInfo(const PDyRenderPipelineInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT_MSG(
    Contains(this->mInternal->mRenderPipelineMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated render pipeline name is exist.");

  this->mInternal->mRenderPipelineMetaInfo.try_emplace(
    metaInfo.mSpecifierName, 
    metaInfo);
  return DY_SUCCESS;
}

EDySuccess MIOMeta::pfAddRenderIteMIOMeta(const PDyRenderItemInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT_MSG(
    Contains(this->mInternal->mRenderIteMIOMeta, metaInfo.mSpecifierName) == false, 
    "Duplicated render pipeline name is exist.");

  this->mInternal->mRenderIteMIOMeta.try_emplace(
    metaInfo.mSpecifierName, 
    metaInfo);
  return DY_SUCCESS;
}

EDySuccess MIOMeta::MDY_PRIVATE(AddBootResourceSpecifierList)(const TResourceSpecifierList& list)
{
  return this->mInternal->__AddBootResourceSpecifierList(list);
}

EDySuccess MIOMeta::MDY_PRIVATE(AddGlobalResourceSpecifierList)(const TResourceSpecifierList& list)
{
  return this->mInternal->__AddGlobalResourceSpecifierList(list);
}

} /// ::dy namespace