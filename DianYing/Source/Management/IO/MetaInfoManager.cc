#include <precompiled.h>
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

/// Header file
#include <Dy/Management/IO/MetaInfoManager.h>

#include <optional>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Element/Helper/DescriptorHelperFunctions.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetLayoutMetaInformation.h>

//!
//! Local tranlation unit variables
//!

namespace
{

//!
//! Error message template list
//!

MDY_SET_IMMUTABLE_STRING(sErrorSameName, "MDyMetaInfo::pReadScriptResourceMetaInformation | Same script specifier detected. Unexpected error! | Script Name : {}");
MDY_SET_IMMUTABLE_STRING(sErrorAmbiguousFlag, "MDyMetaInfo::pReadScriptResourceMetaInformation | Ambiguous flag for using either path or innate code. | Script Name : {}");

//!
//! Script Meta information header list
//!

MDY_SET_IMMUTABLE_STRING(sCategoryList,     "List");
MDY_SET_IMMUTABLE_STRING(sHeaderScriptName, "ScriptName");
MDY_SET_IMMUTABLE_STRING(sHeaderScriptPath, "ScriptPath");
MDY_SET_IMMUTABLE_STRING(sHeaderScriptCode, "ScriptCode");
MDY_SET_IMMUTABLE_STRING(sHeaderIsUsingScriptPath,        "IsUsingScriptPath");
MDY_SET_IMMUTABLE_STRING(sHeaderIsUsingScriptInnateCode,  "IsUsingScriptInnateCode");

//!
//! Prefab Meta information header list
//!

MDY_SET_IMMUTABLE_STRING(sCategoryMeta,         "Meta");
MDY_SET_IMMUTABLE_STRING(sCategoryObjectList,   "ObjectList");
MDY_SET_IMMUTABLE_STRING(sHeaderSpecifierName,  "SpecifierName");
MDY_SET_IMMUTABLE_STRING(sHeaderType,           "Type");
MDY_SET_IMMUTABLE_STRING(sHeaderChildrenName,   "ChildrenName");
MDY_SET_IMMUTABLE_STRING(sHeaderIsInitiallyActivated, "IsInitiallyActivated");
MDY_SET_IMMUTABLE_STRING(sHeaderComponentList,  "ComponentList");

std::unique_ptr<dy::PDyMetaWidgetRootDescriptor> DyCreateWidgetMetaInformation(_MIN_ const nlohmann::json& jsonAtlas)
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
    MDY_LOG_CRITICAL("Json log : {}", jsonAtlas.dump(2));
    MDY_UNEXPECTED_BRANCH(); return nullptr;
  }

  // (3) Make widget component meta information instance sequencially.
  const auto& componentAtlas = jsonAtlas[MSVSTR(sCategoryObjectList)];
  MDyMetaInfo::THashMap<PDyMetaWidgetChildableBaseDesc::TElementType> tempWidgetObjectMap = {};

  for (const auto& componentInfo : componentAtlas)
  { // Check Header list integrity
    MDY_ASSERT(CheckComponentCommonHeaders(componentInfo) == DY_SUCCESS, "Failed to check common headers of item.");

    const auto componentType = DyGetValue<EDyWidgetComponentType>(componentInfo, PDyMetaWidgetCommonBaseDesc::sHeader_Type);
    switch (componentType)
    {
    case EDyWidgetComponentType::Text:
    { // Text component
      auto instance = PDyMetaWidgetTextDescriptor::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mComponentSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair<>(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
    } break;
    case EDyWidgetComponentType::HorizontalLayout:
    { // Horizontal layout component
      auto instance = PDyMetaWidgetHorizontalLayout::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mComponentSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair<>(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
      );
    } break;
    case EDyWidgetComponentType::VerticalLayout:
    { // Vertical layout component
      auto instance = PDyMetaWidgetVerticalLayout::CreateMetaInformation(componentInfo);
      const auto specifierName = instance->mComponentSpecifierName;
      tempWidgetObjectMap.try_emplace(
          specifierName,
          std::make_pair<>(componentType, DyConvertUniquePtrTo<PDyMetaWidgetCommonBaseDesc>(std::move(instance)))
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
      MDY_ASSERT(parentIt != tempWidgetObjectMap.end(), "Unexpected error occurred.");

      // Move pair to parent's children map and erase iterator of it.
      auto& parentInstance = parentIt->second.second;
      parentInstance->mChildComponentList.try_emplace(it->first, std::move(it->second));
      it = tempWidgetObjectMap.erase(it);
    }
  }

  // (5) Make root widget meta instance and retrieve information from headers.
  //     ...and Make heap.
  auto rootInstance = PDyMetaWidgetRootDescriptor::CreateMetaInformation(jsonAtlas[MSVSTR(sCategoryMeta)]);
  for (auto it = tempWidgetObjectMap.begin(); it != tempWidgetObjectMap.end(); ++it)
  {
    auto [_, result] = rootInstance->mChildComponentList.try_emplace(it->first, std::move(it->second));
    MDY_ASSERT(result == true, "Unexpected error occurred.");
  }

  return rootInstance;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyMetaInfo::pfInitialize()
{
  const auto& metaPath = MDySetting::GetInstance().GetMetaPathSettingInformation();

  MDY_CALL_ASSERT_SUCCESS(this->pReadFontResourceMetaInformation  (metaPath.mFontMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadScriptResourceMetaInformation(metaPath.mScriptMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadPrefabResourceMetaInformation(metaPath.mPrefabMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadWidgetResourceMetaInformation(metaPath.mWidgetMetaPath));

  // @TODO scene meta information reading from meta file.
  const auto opJsonAtlas = DyGetJsonAtlasFromFile(metaPath.mSceneMetaPath);
  if (opJsonAtlas.has_value() == false) { return DY_FAILURE; }

  PDyLevelConstructDescriptor desc = PDyLevelConstructDescriptor::CreateDescriptor(opJsonAtlas.value());
  auto [it, result] = this->mLevelInfoMap.try_emplace(desc.mLevelName, desc);
  if (result == false)
  {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfRelease()
{
  this->mLevelInfoMap.clear();
  this->mScriptMetaInfo.clear();
  return DY_SUCCESS;
}

const PDyLevelConstructDescriptor* MDyMetaInfo::GetLevelMetaInformation(const std::string& levelName) const noexcept
{
  const auto it = this->mLevelInfoMap.find(levelName);
  if (it == this->mLevelInfoMap.end())  { return nullptr; }
  else                                  { return &it->second; }
}

const PDyScriptInstanceMetaInfo& MDyMetaInfo::GetScriptMetaInformation(_MIN_ const std::string& specifierName) const
{
  return this->mScriptMetaInfo.at(specifierName);
}

const PDyMetaFontInformation& MDyMetaInfo::GetFontMetaInformation(_MIN_ const std::string& specifierName) const
{
  return this->mFontMetaInfo.at(specifierName);
}

EDySuccess MDyMetaInfo::pReadScriptResourceMetaInformation(_MIN_ const std::string& metaFilePath)
{
  // Validity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlasFromFile(metaFilePath);
  MDY_ASSERT(opJsonAtlas.has_value() == true, "Failed to read script resource meta information. File path is not exist.");

  // Check "List" Category is exist.
  const nlohmann::json& jsonAtlas = opJsonAtlas.value();
  MDY_ASSERT(DyCheckHeaderIsExist(jsonAtlas, sCategoryList) == DY_SUCCESS, "Unexpecte error occurred.");

  const auto& scriptResourceListAtlas = jsonAtlas.at(MSVSTR(sCategoryList));
  for (const auto& scriptResource : scriptResourceListAtlas)
  {
    auto metaInfo = scriptResource.get<PDyScriptInstanceMetaInfo>();

    // Check Duplicated script specfier integrity
    MDY_ASSERT(DyIsMapContains(this->mScriptMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated script specifier not permitted.");
    MDY_ASSERT(std::filesystem::exists(metaInfo.mFilePath), "File not exist.");

    auto [it, isSucceeded] = this->mScriptMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
    MDY_ASSERT(isSucceeded == true, "Unexpected error occurred.");
  }

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pReadPrefabResourceMetaInformation(const std::string& metaFilePath)
{
  ///
  /// @brief  Check prefab meta information list.
  /// @param  atlas Valid json atlas instance.
  /// @return If succeeded, return DY_SUCCESS an
  ///
  static auto CheckPrefabMetaCategory = [](_MIN_ const nlohmann::json& atlas) -> EDySuccess
  {
    if (DyCheckHeaderIsExist(atlas, sCategoryMeta) == DY_FAILURE)       { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sCategoryObjectList) == DY_FAILURE) { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  ///
  /// @brief  Check meta script information header list.
  /// @param  atlas Valid json atlas instance.
  /// @return If succeeded, return DY_SUCCESS flag for representing success.
  ///
  static auto CheckPrefabObjectListHeaders = [](_MIN_ const nlohmann::json& atlas) -> EDySuccess
  {
    if (DyCheckHeaderIsExist(atlas, sHeaderSpecifierName) == DY_FAILURE)  { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderType) == DY_FAILURE)           { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderChildrenName) == DY_FAILURE)   { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderIsInitiallyActivated) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderComponentList) == DY_FAILURE)  { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  ///
  /// @function GetMetaComponentInformation
  /// @brief
  /// @param validComponentAtlas
  /// @param desc
  ///
  static auto GetMetaComponentInformation = [](_MIN_ const auto& validComponentAtlas, _MOUT_ PDyPrefabMetaInformation& desc)
  {
    for (const nlohmann::json& componentMetaInfo : validComponentAtlas)
    {
      const auto typeEnum = DyGetComponentTypeFrom(DyGetValue<std::string>(componentMetaInfo, sHeaderType));
      switch (typeEnum)
      {
      default: MDY_UNEXPECTED_BRANCH(); break;
      case EDyComponentMetaType::Transform:
      {
        const auto meta = CreateTransformMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::Script:
      {
        const auto meta = CreateScriptMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::DirectionalLight:
      {
        const auto meta = CreateDirectionalLightMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::ModelFilter:
      {
        const auto meta = CreateModelFilterMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::ModelRenderer:
      {
        const auto meta = CreateModelRendererMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::Camera:
      {
        const auto meta = CreateCameraMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      }
    }
  };

  ///
  /// @function
  /// @brief
  /// @param
  /// @return
  /// @TODO SCRIPT THIS
  ///
  static auto GetChildrenNameList = [](_MIN_ const nlohmann::json& atlas) -> PDyPrefabMetaInformation::TChildNameList
  {
    PDyPrefabMetaInformation::TChildNameList list = {};
    for (const auto& childAtlas : atlas)
    {
      list.emplace_back(childAtlas.get<std::string>());
    }
    return list;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Integrity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlasFromFile(metaFilePath);
  if (opJsonAtlas.has_value() == false)
  {
    MDY_ASSERT(opJsonAtlas.has_value() == true, "Failed to read prefab resource meta information. File path is not exist.");
    return DY_FAILURE;
  }

  // Check specified category are exist.
  const nlohmann::json& jsonAtlas = opJsonAtlas.value();
  if (const auto flag = CheckPrefabMetaCategory(jsonAtlas); flag == DY_FAILURE)
  {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

  // Make prefab meta information instance sequencially.
  const auto& prefabAtlas = jsonAtlas.at(MSVSTR(sCategoryObjectList));
  THashMap<PDyPrefabMetaInformation::TChildNameList> childrenNameMetaList = {};

  for (const auto& prefabInfo : prefabAtlas)
  { // Check Header list integrity
    const auto flag = CheckPrefabObjectListHeaders(prefabInfo);
    MDY_ASSERT(flag == DY_SUCCESS, "flag == DY_FAILURE");

    auto metaInfoPtr  = std::make_unique<PDyPrefabMetaInformation>();
    auto& metaInfo    = *metaInfoPtr;
    metaInfo.mSpecifierName           = DyGetValue<std::string>(prefabInfo, sHeaderSpecifierName);
    metaInfo.mIsInitiallyActivated    = DyGetValue<bool>(prefabInfo, sHeaderIsInitiallyActivated);
    metaInfo.mPrefabType              = DyGetMetaObjectTypeFrom(DyGetValue<std::string>(prefabInfo, sHeaderType));

    // Set component meta dependency information to PDyPrefabMetaInformation;
    const auto& componentMap = prefabInfo.at(MSVSTR(sHeaderComponentList));
    GetMetaComponentInformation(componentMap, metaInfo);

    // And Read children name and save first.
    childrenNameMetaList.try_emplace(metaInfo.mSpecifierName, GetChildrenNameList(prefabInfo.at(MSVSTR(sHeaderChildrenName))));

    auto [it, result] = this->mPrefabMetaInfo.try_emplace(metaInfo.mSpecifierName, std::move(metaInfoPtr));
    if (result == false)
    {
      MDY_UNEXPECTED_BRANCH();
      return DY_FAILURE;
    }
  }

  // Reconstruct list that parent have children recursively. O(N^2)
  for (auto& [string, metaInfoPtr] : this->mPrefabMetaInfo)
  {
    using TChildNameList      = PDyPrefabMetaInformation::TChildNameList;
    using TChildMetaInfoList  = PDyPrefabMetaInformation::TChildMetaInfoList;
    // Integrity Test
    if (MDY_CHECK_ISEMPTY(metaInfoPtr))                                   { continue; }
    if (childrenNameMetaList.find(string) == childrenNameMetaList.end())  { continue; };

    const TChildNameList& childrenNameList = childrenNameMetaList[string];
    if (childrenNameList.empty()) { continue; }

    // Get children meta information from childrenNameList.
    for (const auto& childName : childrenNameList)
    {
      auto childIt = this->mPrefabMetaInfo.find(childName);
      if (childIt == this->mPrefabMetaInfo.end()) { MDY_UNEXPECTED_BRANCH(); }

      metaInfoPtr->mChildrenList.emplace_back(std::move(childIt->second));
    }
  }

  // Erase vacant.
  for (auto it = this->mPrefabMetaInfo.begin(); it != this->mPrefabMetaInfo.end();)
  {
    if (MDY_CHECK_ISEMPTY(it->second))  { it = this->mPrefabMetaInfo.erase(it); }
    else                                { ++it; }
  }

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pReadWidgetResourceMetaInformation(_MIN_ const std::string& metaFilePath)
{
  // (1) Integrity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlasFromFile(metaFilePath);
  MDY_ASSERT(opJsonAtlas.has_value() == true, "Must be valid json atlas from file path.");

  auto& listAtlas = opJsonAtlas.value();
  for (const auto& widgetMeta : listAtlas)
  {
    auto rootInstance = DyCreateWidgetMetaInformation(widgetMeta);
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(rootInstance), "Widget root instance must not be empty.");
    this->mWidgetMetaInfo.try_emplace(rootInstance->mWidgetSpecifierName, std::move(rootInstance));
  }
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddWidgetMetaInformation(_MIN_ const std::string& metaInformationString)
{
  nlohmann::json jsonAtlas = nlohmann::json::parse(metaInformationString);
  auto rootInstance = DyCreateWidgetMetaInformation(jsonAtlas);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(rootInstance), "Widget root instance must not be empty.");

  this->mWidgetMetaInfo.try_emplace(rootInstance->mWidgetSpecifierName, std::move(rootInstance));
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pReadFontResourceMetaInformation(_MIN_ const std::string& metaFilePath)
{ // (1) Validity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlasFromFile(metaFilePath);
  if (opJsonAtlas.has_value() == false)
  {
    MDY_ASSERT(opJsonAtlas.has_value() == true, "Failed to read font meta information. File is not exist.");
    return DY_FAILURE;
  }

  // (2)
  const nlohmann::json& jsonAtlas = opJsonAtlas.value();
  for (auto it = jsonAtlas.cbegin(); it != jsonAtlas.cend(); ++it)
  { // Create font meta information instance from each json atlas.
    auto [_, isSucceeded] = this->mFontMetaInfo.try_emplace(
        it.key(),
        PDyMetaFontInformation::CreateWithJson(it.value())
    );
    MDY_ASSERT(isSucceeded == true, "Font meta information creation must be succeeded.");
  }

  return DY_SUCCESS;;
}

} /// ::dy namespace