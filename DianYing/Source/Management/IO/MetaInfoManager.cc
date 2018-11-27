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

//!
//! Prefab Meta information header list
//!

MDY_SET_IMMUTABLE_STRING(sCategoryMeta,         "Meta");
MDY_SET_IMMUTABLE_STRING(sCategoryObjectList,   "ObjectList");
MDY_SET_IMMUTABLE_STRING(sHeaderType,           "Type");

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
  MDY_CALL_ASSERT_SUCCESS(this->pReadSceneResourceMetaInformation (metaPath.mSceneMetaPath));

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfRelease()
{
  this->mLevelInfoMap.clear();
  this->mScriptMetaInfo.clear();
  return DY_SUCCESS;
}

const PDyLevelConstructMetaInfo* MDyMetaInfo::GetLevelMetaInformation(const std::string& levelName) const noexcept
{
  const auto it = this->mLevelInfoMap.find(levelName);
  if (it == this->mLevelInfoMap.end())  { return nullptr; }
  else                                  { return &it->second; }
}

const PDyScriptInstanceMetaInfo& MDyMetaInfo::GetScriptMetaInformation(_MIN_ const std::string& specifierName) const
{
  return this->mScriptMetaInfo.at(specifierName);
}

const PDyPrefabInstanceMetaInfo& MDyMetaInfo::GetPrefabMetaInformation(_MIN_ const std::string& specifierName) const
{
  MDY_ASSERT(DyIsMapContains(this->mPrefabMetaInfo, specifierName) == true, "");
  return *this->mPrefabMetaInfo.at(specifierName);
}

const PDyMetaFontInformation& MDyMetaInfo::GetFontMetaInformation(_MIN_ const std::string& specifierName) const
{
  return this->mFontMetaInfo.at(specifierName);
}

const PDyMetaWidgetRootDescriptor& MDyMetaInfo::GetWidgetMetaInformation(_MIN_ const std::string& specifierName) const
{
  MDY_ASSERT(DyIsMapContains(this->mWidgetMetaInfo, specifierName) == true, "");
  return *this->mWidgetMetaInfo.at(specifierName);
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

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Validity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlasFromFile(metaFilePath);
  MDY_ASSERT(opJsonAtlas.has_value() == true, "Failed to read prefab resource mta information.");

  // Check specified category are exist.
  const nlohmann::json& jsonAtlas = opJsonAtlas.value();
  MDY_CALL_ASSERT_SUCCESS(CheckPrefabMetaCategory(jsonAtlas));

  // Make prefab meta information instance sequencially.
  const auto& prefabAtlas = jsonAtlas.at(MSVSTR(sCategoryObjectList));
  THashMap<PDyPrefabInstanceMetaInfo::TChildNameList> childrenNameMetaList = {};

  for (const auto& prefabInfo : prefabAtlas)
  {
    auto metaInfoPtr  = PDyPrefabInstanceMetaInfo::CreateMetaInformation(prefabInfo);
    auto [it, result] = this->mPrefabMetaInfo.try_emplace(metaInfoPtr->mSpecifierName, std::move(metaInfoPtr));
    MDY_ASSERT(result == true, "Unexpected error occurred.");
  }

  // Reconstruct list that parent have children recursively. O(N^2)
  for (auto& [string, metaInfoPtr] : this->mPrefabMetaInfo)
  {
    using TChildNameList      = PDyPrefabInstanceMetaInfo::TChildNameList;
    using TChildMetaInfoList  = PDyPrefabInstanceMetaInfo::TChildMetaInfoList;
    // (1) Validity Test
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
{ // (1) Validity Test
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

EDySuccess MDyMetaInfo::pfAddScriptMetaInformation(_MIN_ const PDyScriptInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT(DyIsMapContains(this->mScriptMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated script name is exist.");
  this->mScriptMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pReadFontResourceMetaInformation(_MIN_ const std::string& metaFilePath)
{ // (1) Validity Test
  const auto opJsonAtlas = DyGetJsonAtlasFromFile(metaFilePath);
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

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pReadSceneResourceMetaInformation(_MIN_ const std::string& metaFilepath)
{
  // (1) Validity test
  const auto opJsonAtlas = DyGetJsonAtlasFromFile(metaFilepath);
  if (opJsonAtlas.has_value() == false) { return DY_FAILURE; }

  // (2)
  for (const auto& sceneAtlas : opJsonAtlas.value())
  {
    auto desc = sceneAtlas.get<PDyLevelConstructMetaInfo>();
    auto [it, isSucceeded] = this->mLevelInfoMap.try_emplace(desc.mMetaCategory.mLevelName, std::move(desc));
    MDY_ASSERT(isSucceeded == true, "Unexpected error occurred.");
  }

  return DY_SUCCESS;
}

} /// ::dy namespace