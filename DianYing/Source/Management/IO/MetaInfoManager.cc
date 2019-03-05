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
#include <Dy/Management/IO/MetaInfoManager.h>

#include <optional>
#include <regex>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/DyEngine.h>

#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/HelperString.h>
#include <Dy/Helper/Library/HelperRegex.h>

#include <Dy/Management/SettingManager.h>
#include <Dy/Management/ScriptManager.h>
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
#include <Dy/Meta/Information/FontMetaInformation.h>
#include <Dy/Meta/Information/PrefabMetaInformation.h>
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>

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
    if (DyCheckHeaderIsExist(atlas, sCategoryMeta) == DY_FAILURE)       { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sCategoryObjectList) == DY_FAILURE) { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  ///
  /// @brief  Check meta widget component information header list.
  /// @param  atlas Valid json atlas instance.
  /// @return If succeeded, return DY_SUCCESS flag for representing success.
  ///
  static auto CheckComponentCommonHeaders = [](_MIN_ const nlohmann::json& atlas) -> EDySuccess
  {
    if (DyCheckHeaderIsExist(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Name) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Type) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Parent) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, PDyMetaWidgetCommonBaseDesc::sHeader_Details) == DY_FAILURE) { return DY_FAILURE; }
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
  MDyMetaInfo::THashMap<PDyMetaWidgetChildableBaseDesc::TElementType> tempWidgetObjectMap = {};

  for (const auto& componentInfo : componentAtlas)
  { // Check Header list integrity
    MDY_ASSERT_MSG(CheckComponentCommonHeaders(componentInfo) == DY_SUCCESS, "Failed to check common headers of item.");

    const auto componentType = DyJsonGetValueFrom<EDyWidgetComponentType>(componentInfo, PDyMetaWidgetCommonBaseDesc::sHeader_Type);
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

class MDyMetaInfo::Impl final : public IDySingleton<MDyMetaInfo>
{
public:
  template <typename TType>
  using THashMap = std::unordered_map<std::string, TType>;
  using TResourceSpecifierList = std::vector<DDyResourceName>;

  const PDyLevelConstructMetaInfo& 
  GetLevelMetaInformation(const std::string& iLevelName) const noexcept;

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
  THashMap<PDyLevelConstructMetaInfo>   mLevelInfoMap   = {};
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

EDySuccess MDyMetaInfo::pfInitialize() 
{ 
  this->mInternal = new Impl();
  return DY_SUCCESS; 
}

EDySuccess MDyMetaInfo::pfRelease()
{
  delete this->mInternal; this->mInternal = nullptr;
  return DY_SUCCESS;
}

const PDyLevelConstructMetaInfo* 
MDyMetaInfo::GetLevelMetaInformation(const std::string& iLevelName) const noexcept
{
  return &this->mInternal->GetLevelMetaInformation(iLevelName);
}

std::vector<std::pair<std::string, const PDyScriptInstanceMetaInfo*>> 
MDyMetaInfo::GetGlobalScriptMetaInfos() const noexcept
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
MDyMetaInfo::GetScriptMetaInformation(const std::string& specifierName) const
{
  return this->mInternal->GetScriptMetaInformation(specifierName);
}

const PDyPrefabInstanceMetaInfo& 
MDyMetaInfo::GetPrefabMetaInformation(const std::string& iPrefabName) const
{
  return this->mInternal->GetPrefabMetaInformation(iPrefabName);
}

const PDyMetaFontInformation& 
MDyMetaInfo::GetFontMetaInformation(const std::string& iFontName) const
{
  return this->mInternal->GetFontMetaInformation(iFontName);
}

const PDyMetaWidgetRootDescriptor& 
MDyMetaInfo::GetWidgetMetaInformation(const std::string& iSpecifierName) const
{
  return this->mInternal->GetWidgetMetaInformation(iSpecifierName);
}

const PDyGLShaderInstanceMetaInfo& 
MDyMetaInfo::GetGLShaderMetaInformation(const std::string& iShaderName) const
{
  return this->mInternal->GetShaderMetaInformation(iShaderName);
}

const PDyMeshInstanceMetaInfo& 
MDyMetaInfo::GetBtMeshMetaInformation(const std::string& iMeshName) const
{
  return this->mInternal->GetBtMeshMetaInformation(iMeshName);
}

const PDyModelInstanceMetaInfo& 
MDyMetaInfo::GetModelMetaInformation(const std::string& iModelName) const
{
  return this->mInternal->GetModelMetaInformation(iModelName);
}

const PDyModelSkelInstanceMetaInfo& 
MDyMetaInfo::GetModelSkeletonMetaInformation(const std::string& iSkeletonName) const
{
  return this->mInternal->GetModelSkeletonMetaInformation(iSkeletonName);
}

const PDyModelAnimInstanceMetaInfo& 
MDyMetaInfo::GetModelAnimScrapMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetModelAnimScrapMetaInformation(specifier);
}

const PDyTextureInstanceMetaInfo& 
MDyMetaInfo::GetTextureMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetTextureMetaInformation(specifier);
}

const PDyMaterialInstanceMetaInfo& 
MDyMetaInfo::GetMaterialMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetMaterialMetaInformation(specifier);
}

const PDyGlFrameBufferInstanceMetaInfo& 
MDyMetaInfo::GetGlFrameBufferMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetGlFrameBufferMetaInformation(specifier);
}

const PDyGlAttachmentInstanceMetaInfo& 
MDyMetaInfo::GetGLAttachmentMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetGLAttachmentMetaInformation(specifier);
}

const PDySoundInstanceMetaInfo& MDyMetaInfo::GetSoundMetaInformation(const std::string& specifier) const
{
  return this->mInternal->GetSoundMetaInformation(specifier);
}

MDY_NODISCARD const PDyMetaWidgetRootDescriptor* 
MDyMetaInfo::MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)() const noexcept
{
  return this->mInternal->__TryGetLoadingWidgetMetaLoading();
}

bool MDyMetaInfo::IsGLShaderMetaInfoExist(const std::string & specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mShaderMetaInfo, specifier);
}

bool MDyMetaInfo::IsMeshMetaInfoExist(const std::string & specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mModelMeshMetaInfo, specifier);
}

bool MDyMetaInfo::IsLevelMetaInformation(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mLevelInfoMap, specifier);
}

bool MDyMetaInfo::IsModelMetaInfoExist(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mModelMetaInfo, specifier);
}

bool MDyMetaInfo::IsModelSkeletonMetaInfoExist(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mModelSkeletonMetaInfo, specifier);
}

bool MDyMetaInfo::IsModelAnimScrapMetaInfoExist(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mModelAnimScrapMetaInfo, specifier);
}

bool MDyMetaInfo::IsTextureMetaInfoExist(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mTextureMetaInfo, specifier);
}

bool MDyMetaInfo::IsMaterialMetaInfoExist(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mMaterialMetaInfo, specifier);
}

bool MDyMetaInfo::IsWidgetMetaInfoExist(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mInternal->mWidgetMetaInfo, specifier);
}

bool MDyMetaInfo::IsScriptMetaInformationExist(const std::string& specifierName) const noexcept
{
  return DyIsMapContains(this->mInternal->mScriptMetaInfo, specifierName);
}

bool MDyMetaInfo::IsPrefabMetaInformationExist(const std::string& specifierName) const noexcept
{
  return DyIsMapContains(this->mInternal->mPrefabMetaInfo, specifierName);
}

bool MDyMetaInfo::IsFontMetaInformationExist(const std::string& specifierName) const noexcept
{
  return DyIsMapContains(this->mInternal->mFontMetaInfo, specifierName);
}

bool MDyMetaInfo::IsAttachmentMetaInfoExist(const std::string& specifierName) const noexcept
{
  return DyIsMapContains(this->mInternal->mAttachmentMetaInfo, specifierName);
}

bool MDyMetaInfo::IsFrameBufferMetaInfoExist(const std::string& speicfierName) const noexcept
{
  return DyIsMapContains(this->mInternal->mFrameBufferMetaInfo, speicfierName);
}

bool MDyMetaInfo::IsSoundMetaInfoExist(const std::string& specifierName) const noexcept
{
  return DyIsMapContains(this->mInternal->mSoundMetaInfo, specifierName);
}

bool MDyMetaInfo::IsLoadingWidgetMetaInfoExist() const noexcept
{
  return this->mInternal->mLoadingWidgetMetaInfo != nullptr;
}

void MDyMetaInfo::MDY_PRIVATE(PopulateBootResourceSpecifierList)() const noexcept
{
  this->mInternal->MDY_PRIVATE(PopulateBootResourceSpecifierList)();
}

void MDyMetaInfo::MDY_PRIVATE(PopulateGlobalResourceSpecifierList)() const noexcept
{
  this->mInternal->MDY_PRIVATE(PopulateGlobalResourceSpecifierList)();
}

void MDyMetaInfo::MDY_PRIVATE(InitiateMetaInformation)()
{
  this->mInternal->__InitiateMetaInformation();
}

void MDyMetaInfo::MDY_PRIVATE(InitiateMetaInformationComp)(const nlohmann::json& iJson)
{
  this->mInternal->__InitiateMetaInformationComp(iJson);
}

EDySuccess MDyMetaInfo::pReadScriptResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadScriptResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadPrefabResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadPrefabMetaAtlas(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadFontResourceMetaInformation(const std::string& metaFilePath)
{ 
  return this->mInternal->pReadFontResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadModelResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadModelMeshResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelMeshResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadModelSkeletonMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelSkeletonMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadModelAnimationMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadModelAnimationMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadTextureResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadTextureResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadShaderResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadShaderResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadMaterialResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadMaterialResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadLevelResourceMetaInformation(const std::string& metaFilepath)
{
  return this->mInternal->pReadLevelResourceMetaInformation(metaFilepath);
}

EDySuccess MDyMetaInfo::pReadWidgetResourceMetaInformation(const std::string& metaFilePath)
{ 
  return this->mInternal->pReadWidgetResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadSoundResourceMetaInformation(const std::string& metaFilePath)
{
  return this->mInternal->pReadSoundResourceMetaInformation(metaFilePath);
}

EDySuccess MDyMetaInfo::pReadScriptMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadScriptMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadPrefabMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadPrefabMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadWidgetMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadWidgetMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadFontMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadFontMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadLevelMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadLevelMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadShaderMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadShaderMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadSoundMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadSoundMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadTextureMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadTextureMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadMaterialMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadMaterialMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadModelAnimMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelAnimMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadModelMeshMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelMeshMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadModelSkelMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelSkelMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pReadModelMetaAtlas(const nlohmann::json& iJson)
{
  return this->mInternal->pReadModelMetaAtlas(iJson);
}

EDySuccess MDyMetaInfo::pfAddWidgetMetaInformation(const std::string& metaInformationString)
{
  return this->mInternal->pfAddWidgetMetaInformation(metaInformationString);
}

EDySuccess MDyMetaInfo::MDY_PRIVATE(AddLoadingWidgetMetaInformation)(const std::string& widgetMetaInfo)
{
  return this->mInternal->MDY_PRIVATE(AddLoadingWidgetMetaInformation)(widgetMetaInfo);
}

EDySuccess MDyMetaInfo::pfAddScriptMetaInformation(const PDyScriptInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddScriptMetaInformation(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddGLShaderMetaInfo(const PDyGLShaderInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddGLShaderMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddBuiltinMeshMetaInfo(const PDyMeshInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddBuiltinMeshMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddModelMetaInfo(const PDyModelInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddModelMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddTextureMetaInfo(const PDyTextureInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddTextureMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddMaterialMetaInfo(const PDyMaterialInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddMaterialMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddGLAttachmentMetaInfo(const PDyGlAttachmentInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddGLAttachmentMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::pfAddGLFrameBufferMetaInfo(const PDyGlFrameBufferInstanceMetaInfo& metaInfo)
{
  return this->mInternal->pfAddGLFrameBufferMetaInfo(metaInfo);
}

EDySuccess MDyMetaInfo::MDY_PRIVATE(AddBootResourceSpecifierList)(const TResourceSpecifierList& list)
{
  return this->mInternal->__AddBootResourceSpecifierList(list);
}

EDySuccess MDyMetaInfo::MDY_PRIVATE(AddGlobalResourceSpecifierList)(const TResourceSpecifierList& list)
{
  return this->mInternal->__AddGlobalResourceSpecifierList(list);
}

} /// ::dy namespace