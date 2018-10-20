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
#include <Dy/Management/MetaInfoManager.h>

#include <optional>
#include <nlohmann/json.hpp>

#include <Dy/Helper/JsonHelper.h>
#include <Dy/Helper/Constant/StringSettingFile.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Element/Helper/DescriptorHelperFunctions.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>

//!
//! Local tranlation unit variables
//!

namespace dy
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


///
/// @brief  Find "Header" String is exist on given json atlas.
/// @param  atlas Valid immutable json atlas instance.
/// @param  string Header string to verify.
/// @return If found, return DY_SUCCESS or DY_FAILURE.
///
MDY_NODISCARD EDySuccess
DyCheckHeaderIsExist(_MIN_ const nlohmann::json& atlas, _MIN_ const std::string_view& string) noexcept
{
  if (atlas.find(MSVSTR(string)) == atlas.end())  { return DY_FAILURE; }
  else                                            { return DY_SUCCESS; }
}

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

EDySuccess MDyMetaInfo::pfInitialize()
{
  const auto opJsonAtlas = DyGetJsonAtlas(MSVSTR(gTestPath));
  if (opJsonAtlas.has_value() == false) { return DY_FAILURE; }

  { // @TODO TEST READING (FIXED PATH) script file from meta file.
    const auto flag = this->pReadScriptResourceMetaInformation(MSVSTR(gScriptResourceMetaInfo));
    if (flag == DY_FAILURE)
    {
      MDY_UNEXPECTED_BRANCH();
      return DY_FAILURE;
    }
  }

  { // @TODO TEST READING (FIXED PATH) prefab file from meta file.
    const auto flag = this->pReadPrefabResourceMetaInformation(MSVSTR(gPrefabResourceMetaInfo));
    if (flag == DY_FAILURE)
    {
      MDY_UNEXPECTED_BRANCH();
      return DY_FAILURE;
    }
  }

  // @TODO scene meta information reading from meta file.
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

MDY_NODISCARD const PDyMetaScriptInformation & MDyMetaInfo::GetScriptMetaInformation(_MIN_ const std::string & specifierName) const
{
  return this->mScriptMetaInfo.at(specifierName);
}

EDySuccess MDyMetaInfo::pReadScriptResourceMetaInformation(_MIN_ const std::string& metaFilePath)
{
  // Integrity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlas(metaFilePath);
  if (opJsonAtlas.has_value() == false)
  {
    MDY_ASSERT(opJsonAtlas.has_value() == true, "Failed to read script resource meta information. File path is not exist.");
    return DY_FAILURE;
  }

  // Check "List" Category is exist.
  const nlohmann::json& jsonAtlas = opJsonAtlas.value();
  if (DyCheckHeaderIsExist(jsonAtlas, sCategoryList) == DY_FAILURE)
  {
    MDY_UNEXPECTED_BRANCH();
    return DY_FAILURE;
  }

  // Lambda functions

  ///
  /// @brief  Check meta script information header list.
  /// @param  atlas
  /// @return If succeeded, return DY_SUCCESS flag for representing success.
  /// @TODO SCRIPT THIS
  ///
  static auto CheckMetaScriptResourceHeaders = [](_MIN_ const nlohmann::json& atlas) -> EDySuccess
  {
    if (DyCheckHeaderIsExist(atlas, sHeaderScriptName) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderScriptPath) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderScriptCode) == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderIsUsingScriptPath)        == DY_FAILURE) { return DY_FAILURE; }
    if (DyCheckHeaderIsExist(atlas, sHeaderIsUsingScriptInnateCode)  == DY_FAILURE) { return DY_FAILURE; }
    return DY_SUCCESS;
  };

  ///
  /// @brief
  /// @param atlas
  /// @param code
  /// @TODO SCRIPT THIS
  ///
  static auto GetInnatedScriptCodeFrom = [](_MIN_ const nlohmann::json& atlas, _MOUT_ std::string& code)
  {
    for (const auto& stringAtlas : atlas) { code += stringAtlas.get<std::string>(); }
  };

  // FunctionBody ∨
  const auto& scriptResourceListAtlas = jsonAtlas.at(MSVSTR(sCategoryList));
  for (const auto& scriptResource : scriptResourceListAtlas)
  { // Check Header list integrity
    const auto flag = CheckMetaScriptResourceHeaders(scriptResource);
    MDY_ASSERT(flag == DY_SUCCESS, "flag == DY_FAILURE");

    PDyMetaScriptInformation metaInfo   = {};

    const auto scriptSpecifierString    = DyGetValue<std::string>(scriptResource, sHeaderScriptName);
    const auto isUsingScriptPath        = DyGetValue<bool>(scriptResource, sHeaderIsUsingScriptPath);
    const auto isUsingScriptInnateCode  = DyGetValue<bool>(scriptResource, sHeaderIsUsingScriptInnateCode);
    metaInfo.mName = scriptSpecifierString;

    // Check Duplicated script specfier integrity
    if (this->mScriptMetaInfo.find(scriptSpecifierString) != this->mScriptMetaInfo.end())
    {
      MDY_LOG_CRITICAL(sErrorSameName, scriptSpecifierString);
      return DY_FAILURE;
    }

    // Check ambiguous flag integrity test
    if ((isUsingScriptPath == true && isUsingScriptInnateCode == true) ||
        (isUsingScriptPath == false && isUsingScriptInnateCode == false))
    {
      MDY_LOG_CRITICAL(sErrorAmbiguousFlag, scriptSpecifierString);
      return DY_FAILURE;
    }

    if (isUsingScriptPath == true)
    { // If using path, just do it.
      const auto scriptPath       = DyGetValue<std::string>(scriptResource, sHeaderScriptPath);
      metaInfo.mIsUsingScriptPath = true;
      metaInfo.mScriptPath        = scriptPath;
    }
    else if (isUsingScriptInnateCode == true)
    { // If using innate code, retrieve string code chunk from inside.
      std::string scriptCode = MDY_INITILAIZE_EMPTYSTR;
      GetInnatedScriptCodeFrom(scriptResource.at(MSVSTR(sHeaderScriptCode)), scriptCode);
      metaInfo.mIsUsingScriptInnateCode = true;
      metaInfo.mScriptCode              = scriptCode;
    }

    auto [it, result] = this->mScriptMetaInfo.try_emplace(metaInfo.mName, metaInfo);
    if (result == false)
    {
      MDY_UNEXPECTED_BRANCH();
      return DY_FAILURE;
    }
  }

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pReadPrefabResourceMetaInformation(const std::string& metaFilePath)
{
  // Lambda functions
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
  /// @TODO SCRIPT THIS
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
  /// @TODO SCRIPT THIS
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

  // FunctionBody ∨

  // Integrity Test
  const std::optional<nlohmann::json> opJsonAtlas = DyGetJsonAtlas(metaFilePath);
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

  // Reconstruct list that parent have children recursively.
  // O(N^2)
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

} /// ::dy namespace