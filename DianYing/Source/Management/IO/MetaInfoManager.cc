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
#include <regex>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <Dy/DyMacroSetting.h>

#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/DyEngine.h>

#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/HelperString.h>
#include <Dy/Helper/Library/HelperRegex.h>

#include <Dy/Management/SettingManager.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetLayoutMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>

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

void MoveMetaPrefabIntoParentRecursively(
    _MINOUT_ dy::TPrefabMetaInfoList& p, 
    _MIN_ const std::vector<std::string>& parentSpecifierList,
    _MIN_ const TU32 level,
    _MINOUT_ dy::TPrefabMetaInfoList::value_type& object)
{
  for (auto& parentObject : p)
  { 
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(object), "Unexpected error occurred");
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

EDySuccess MDyMetaInfo::pfInitialize()
{
  const auto& metaPath = MDySetting::GetInstance().GetMetaPathSettingInformation();
  reflect::RDyBuiltinResource::BindBuiltinResourcesToMetaManager();

  #if defined(MDY_FLAG_LOAD_COMPRESSED_DATAFILE) == false
  {
    MDY_CALL_ASSERT_SUCCESS(this->pReadFontResourceMetaInformation  (metaPath.mFontMetaPath));
    MDY_CALL_ASSERT_SUCCESS(this->pReadScriptResourceMetaInformation(metaPath.mScriptMetaPath));
    MDY_CALL_ASSERT_SUCCESS(this->pReadPrefabResourceMetaInformation(metaPath.mPrefabMetaPath));
    MDY_CALL_ASSERT_SUCCESS(this->pReadWidgetResourceMetaInformation(metaPath.mWidgetMetaPath));
    MDY_CALL_ASSERT_SUCCESS(this->pReadSceneResourceMetaInformation (metaPath.mSceneMetaPath));
  }
  #else
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  }
  #endif

  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfRelease()
{
  this->mLevelInfoMap.clear();
  this->mScriptMetaInfo.clear();
  this->mFontMetaInfo.clear();
  this->mShaderMetaInfo.clear();
  this->mBtMeshMetaInfo.clear();
  this->mModelMetaInfo.clear();
  this->mTextureMetaInfo.clear();
  this->mMaterialMetaInfo.clear();
  this->mAttachmentMetaInfo.clear();
  this->mFrameBufferMetaInfo.clear();
  this->mBootResourceSpecifierList.clear();
  this->mGlobalResourceSpecifierList.clear();
  return DY_SUCCESS;
}

const PDyLevelConstructMetaInfo* MDyMetaInfo::GetLevelMetaInformation(_MIN_ const std::string& levelName) const noexcept
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

const PDyGLShaderInstanceMetaInfo& MDyMetaInfo::GetGLShaderMetaInformation(_MIN_ const std::string& specifier) const
{
  MDY_ASSERT(this->IsGLShaderMetaInfoExist(specifier) == true, "GLShader given specifier name is not exist.");
  return this->mShaderMetaInfo.at(specifier);
}

const PDyBtMeshInstanceMetaInfo & MDyMetaInfo::GetBtMeshMetaInformation(_MIN_ const std::string & specifier) const
{
  MDY_ASSERT(this->IsMeshMetaInfoExist(specifier) == true, "Bt Mesh given specifier name is not exist.");
  return this->mBtMeshMetaInfo.at(specifier);
}

const PDyModelInstanceMetaInfo& MDyMetaInfo::GetModelMetaInformation(const std::string& specifier) const
{
  MDY_ASSERT(this->IsModelMetaInfoExist(specifier) == true, "Model given specifier name is not exist.");
  return this->mModelMetaInfo.at(specifier);
}

const PDyTextureInstanceMetaInfo& MDyMetaInfo::GetTextureMetaInformation(const std::string& specifier) const
{
  MDY_ASSERT(this->IsTextureMetaInfoExist(specifier) == true, "Texture given specifier name is not exist.");
  return this->mTextureMetaInfo.at(specifier);
}

const PDyMaterialInstanceMetaInfo& MDyMetaInfo::GetMaterialMetaInformation(const std::string& specifier) const
{
  MDY_ASSERT(this->IsMaterialMetaInfoExist(specifier) == true, "Material given specifier name is not exist.");
  return this->mMaterialMetaInfo.at(specifier);
}

const PDyGlFrameBufferInstanceMetaInfo& MDyMetaInfo::GetGlFrameBufferMetaInformation(_MIN_ const std::string& specifier) const
{
  return this->mFrameBufferMetaInfo.at(specifier);
}

const PDyGlAttachmentInstanceMetaInfo& MDyMetaInfo::GetGLAttachmentMetaInformation(_MIN_ const std::string& specifier) const
{
  return this->mAttachmentMetaInfo.at(specifier);
}

MDY_NODISCARD const PDyMetaWidgetRootDescriptor* 
MDyMetaInfo::MDY_PRIVATE_SPECIFIER(TryGetLoadingWidgetMetaLoading)() const noexcept
{
  if (this->IsLoadingWidgetMetaInfoExist() == false) { return nullptr; }
  return this->mLoadingWidgetMetaInfo.get();
}

bool MDyMetaInfo::IsGLShaderMetaInfoExist(_MIN_ const std::string & specifier) const noexcept
{
  return DyIsMapContains(this->mShaderMetaInfo, specifier);
}

bool MDyMetaInfo::IsMeshMetaInfoExist(_MIN_ const std::string & specifier) const noexcept
{
  return DyIsMapContains(this->mBtMeshMetaInfo, specifier);
}

bool MDyMetaInfo::IsLevelMetaInformation(const std::string& specifier) const noexcept
{
  return DyIsMapContains(this->mLevelInfoMap, specifier);
}

bool MDyMetaInfo::IsLoadingWidgetMetaInfoExist() const noexcept
{
  return MDY_CHECK_ISNOTEMPTY(this->mLoadingWidgetMetaInfo);
}

void MDyMetaInfo::MDY_PRIVATE_SPECIFIER(PopulateBootResourceSpecifierList)() const noexcept
{
  static bool mIsCalled = false;
  MDY_ASSERT(mIsCalled == false, "This function must not be called twice.");

  SDyIOConnectionHelper::PopulateResourceList(
      this->mBootResourceSpecifierList, 
      EDyScope::Global,
      []() { DyEngine::GetInstance().SetNextGameStatus(EDyGlobalGameStatus::FirstLoading); }
  );
  mIsCalled = true;
}

void MDyMetaInfo::MDY_PRIVATE_SPECIFIER(PopulateGlobalResourceSpecifierList)() const noexcept
{
  static bool mIsCalled = false;
  MDY_ASSERT(mIsCalled == false, "This function must not be called twice.");

  // Global resource list consists of many sub-global resource list from each global resource script.
  SDyIOConnectionHelper::PopulateResourceList(
      mGlobalResourceSpecifierList, 
      EDyScope::Global,
      []() { DyEngine::GetInstance().SetNextGameStatus(EDyGlobalGameStatus::Loading); }
  );
  mIsCalled = true;
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
  /// @brief  Check prefab meta information list.
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
  MDY_ASSERT(opJsonAtlas.has_value() == true, "Failed to read prefab resource meta information.");

  const auto& jsonAtlas = opJsonAtlas.value();
  MDY_CALL_ASSERT_SUCCESS(CheckPrefabMetaCategory(jsonAtlas));

  // Make prefab meta information instance sequencially.
  const auto& prefabAtlas = jsonAtlas.at(MSVSTR(sCategoryObjectList));
  TPrefabMetaInfoList prefabObjectList = {};
  for (const auto& prefabInfo : prefabAtlas)
  {
    prefabObjectList.emplace_back(PDyPrefabInstanceMetaInfo::CreateMetaInformation(prefabInfo));
  }

  // (2) Make object list tree.
  for (auto& object : prefabObjectList)
  {
    if (MDY_CHECK_ISEMPTY(object)) { continue; }
    if (object->mPrefabType == EDyMetaObjectType::Actor
    &&  object->mCommonProperties.mParentSpecifierName.empty() == false)
    { // If object type is Actor, and have parents specifier name as dec
      // Try move object into any parent's children list.
      const auto list = DyRegexCreateObjectParentSpecifierList(object->mCommonProperties.mParentSpecifierName);
      MoveMetaPrefabIntoParentRecursively(prefabObjectList, list, 0, object);
    }
  }

  // (3) Realign object meta list.
  for (auto& ptrsmtPrefabObject : prefabObjectList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtPrefabObject)) { continue; }
    const auto name = ptrsmtPrefabObject->mSpecifierName;
    this->mPrefabMetaInfo.try_emplace(name, std::move(ptrsmtPrefabObject));
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

EDySuccess MDyMetaInfo::MDY_PRIVATE_SPECIFIER(AddLoadingWidgetMetaInformation)(_MIN_ const std::string& widgetMetaInfo)
{
  const nlohmann::json jsonAtlas = nlohmann::json::parse(widgetMetaInfo);
  auto rootInstance = DyCreateWidgetMetaInformation(jsonAtlas);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(rootInstance), "Widget root instance must not be empty.");

  mLoadingWidgetMetaInfo = std::move(rootInstance);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddScriptMetaInformation(_MIN_ const PDyScriptInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT(DyIsMapContains(this->mScriptMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated script name is exist.");
  this->mScriptMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddGLShaderMetaInfo(_MIN_ const PDyGLShaderInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT(DyIsMapContains(this->mShaderMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated gl shader name is exist.");
  this->mShaderMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddBuiltinMeshMetaInfo(_MIN_ const PDyBtMeshInstanceMetaInfo& metaInfo)
{
#if defined(_DEBUG) == true
  if (metaInfo.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel == false)
  {
    MDY_ASSERT(metaInfo.mCustomMeshBuffer.empty() == false, "Builtin mesh must be valid if using customized binding.");
  }
#endif
  MDY_ASSERT(DyIsMapContains(this->mBtMeshMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated Mesh name is exist.");
  this->mBtMeshMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddModelMetaInfo(_MIN_ const PDyModelInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT(DyIsMapContains(this->mModelMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated model name is exist.");
  this->mModelMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddTextureMetaInfo(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT(DyIsMapContains(this->mTextureMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated texture name is exist.");
  this->mTextureMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddMaterialMetaInfo(_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT(DyIsMapContains(this->mMaterialMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated material name is exist.");
  this->mMaterialMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddGLAttachmentMetaInfo(_MIN_ const PDyGlAttachmentInstanceMetaInfo& metaInfo)
{
#if defined(_DEBUG) == true
  MDY_ASSERT(metaInfo.mSpecifierName.empty() != true, "Attachment specifier name must be specifed.");
  MDY_ASSERT(metaInfo.mBufferFormat != EDyRenderBufferInternalFormat::NoneError, "Attachment format must not none.");
  MDY_ASSERT(metaInfo.mAttachmentSize.X > 0
          && metaInfo.mAttachmentSize.Y > 0, "Attachment size must be positive.");
#endif

  MDY_ASSERT(DyIsMapContains(this->mAttachmentMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated attachment name is exist.");
  this->mAttachmentMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::pfAddGLFrameBufferMetaInfo(const PDyGlFrameBufferInstanceMetaInfo& metaInfo)
{
#if defined(_DEBUG) == true
  // Validation test check.
  MDY_ASSERT(metaInfo.mSpecifierName.empty() != true, "Attachment specifier name must be specifed.");
  for (const auto& [name, type] : metaInfo.mColorAttachmentList) 
  { // Check color attachment.
    MDY_ASSERT(name.empty() == false, "Color attachment specifier name must not be empty.");
    MDY_ASSERT(type != EDyGlAttachmentType::Depth
            && type != EDyGlAttachmentType::NoneError, "Color attachment type must be `::ColorX`");
    MDY_ASSERT(metaInfo.mFrameBufferSize.X > 0
            && metaInfo.mFrameBufferSize.Y > 0, "Framebuffer size must be positive.");
  }
#endif

  MDY_ASSERT(DyIsMapContains(this->mFrameBufferMetaInfo, metaInfo.mSpecifierName) == false, "Duplicated framebuffer name is exist.");
  auto [it, isSuccessful] = this->mFrameBufferMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  MDY_ASSERT(isSuccessful == true, "Setting up framebuffer meta info must be successful.");

  auto& [specifier, instance] = *it;
  if (instance.mIsUsingDepthBuffer == true && instance.mDepthAttachmentSpecifier.empty() == true)
  { // If frame buffer will use depth buffer but not specified anything, just create default framebuffer depth attachment meta info.
    // and bind it.
    PDyGlAttachmentInstanceMetaInfo defaultDepthBuffer;
    {
      defaultDepthBuffer.mSpecifierName   = fmt::format("{}_D_{}", specifier, DyGetRandomString(5));
      defaultDepthBuffer.mBufferFormat    = EDyRenderBufferInternalFormat::DEPTH32;
      defaultDepthBuffer.mSourceType      = EDyResourceSource::Builtin;
      defaultDepthBuffer.mAttachmentSize  = instance.mFrameBufferSize;
    }
    
    MDY_CALL_ASSERT_SUCCESS(this->pfAddGLAttachmentMetaInfo(defaultDepthBuffer));
    instance.mDepthAttachmentSpecifier = defaultDepthBuffer.mSpecifierName;
  }

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

EDySuccess MDyMetaInfo::MDY_PRIVATE_SPECIFIER(AddBootResourceSpecifierList)(_MIN_ const TResourceSpecifierList& list)
{
  this->mBootResourceSpecifierList = list;
  return DY_SUCCESS;
}

EDySuccess MDyMetaInfo::MDY_PRIVATE_SPECIFIER(AddGlobalResourceSpecifierList)(_MIN_ const TResourceSpecifierList& list)
{
  if (list.empty() == false) { this->mGlobalResourceSpecifierList.emplace_back(list); }
  return DY_SUCCESS;
}

} /// ::dy namespace