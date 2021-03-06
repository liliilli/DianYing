#ifndef GUARD_DY_MANAGEMENT_METAINFOMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_METAINFOMANAGER_IMPL_INL
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

namespace dy
{

inline const PDyScriptInstanceMetaInfo& 
MIOMeta::Impl::GetScriptMetaInformation(const std::string& iScriptName) const
{
  return this->mScriptMetaInfo.at(iScriptName);
}

inline const std::unordered_map<std::string, PDyScriptInstanceMetaInfo>& 
MIOMeta::Impl::GetRefGlobalScriptMetaInfoContainer() const noexcept
{
  return this->mGlobalScriptMetaInfo;
}

inline const PDyPrefabInstanceMetaInfo& 
MIOMeta::Impl::GetPrefabMetaInformation(const std::string& iPrefabName) const
{
  return *this->mPrefabMetaInfo.at(iPrefabName);
}

inline const PDyMetaFontInformation& 
MIOMeta::Impl::GetFontMetaInformation(const std::string& iFontName) const
{
  return this->mFontMetaInfo.at(iFontName);
}

inline const PDyMetaWidgetRootDescriptor& 
MIOMeta::Impl::GetWidgetMetaInformation(const std::string& iWidgetMetaName) const
{
  return *this->mWidgetMetaInfo.at(iWidgetMetaName);
}

inline const PDyGLShaderInstanceMetaInfo&
MIOMeta::Impl::GetShaderMetaInformation(const std::string& iShaderName) const
{
  return this->mShaderMetaInfo.at(iShaderName);
}

inline const PDyMeshInstanceMetaInfo& 
MIOMeta::Impl::GetBtMeshMetaInformation(const std::string& specifier) const
{
  return this->mModelMeshMetaInfo.at(specifier);
}

inline const PDyModelInstanceMetaInfo& 
MIOMeta::Impl::GetModelMetaInformation(const std::string& specifier) const
{
  return this->mModelMetaInfo.at(specifier);
}

inline const PDyModelSkelInstanceMetaInfo& 
MIOMeta::Impl::GetModelSkeletonMetaInformation( const std::string& specifier) const
{
  return this->mModelSkeletonMetaInfo.at(specifier);
}

inline const PDyModelAnimInstanceMetaInfo& 
MIOMeta::Impl::GetModelAnimScrapMetaInformation(const std::string& specifier) const
{
  return this->mModelAnimScrapMetaInfo.at(specifier);
}

inline const PDyTextureInstanceMetaInfo& 
MIOMeta::Impl::GetTextureMetaInformation(const std::string& specifier) const
{
  return this->mTextureMetaInfo.at(specifier);
}

inline const PDyMaterialInstanceMetaInfo& 
MIOMeta::Impl::GetMaterialMetaInformation(const std::string& specifier) const
{
  return this->mMaterialMetaInfo.at(specifier);
}

inline const PDyGlFrameBufferInstanceMetaInfo& 
MIOMeta::Impl::GetGlFrameBufferMetaInformation(const std::string& specifier) const
{
  return this->mFrameBufferMetaInfo.at(specifier);
}

inline const PDyGlAttachmentInstanceMetaInfo& 
MIOMeta::Impl::GetGLAttachmentMetaInformation( const std::string& specifier) const
{
  return this->mAttachmentMetaInfo.at(specifier);
}

inline const PDySoundInstanceMetaInfo& 
MIOMeta::Impl::GetSoundMetaInformation( const std::string& specifier) const
{
  return this->mSoundMetaInfo.at(specifier);
}

inline const PDyMetaWidgetRootDescriptor* 
MIOMeta::Impl::MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)() const noexcept
{
  if (this->IsLoadingWidgetMetaInfoExist() == false) { return nullptr; }
  return this->mLoadingWidgetMetaInfo.get();
}

inline bool MIOMeta::Impl::IsLoadingWidgetMetaInfoExist() const noexcept
{
  return this->mLoadingWidgetMetaInfo != nullptr;
}

inline void MIOMeta::Impl::MDY_PRIVATE(PopulateBootResourceSpecifierList)() const noexcept
{
  static bool mIsCalled = false;
  MDY_ASSERT_MSG(mIsCalled == false, "This function must not be called twice.");

  SIOConnectionHelper::PopulateResourceList(
      this->mBootResourceSpecifierList, 
      EResourceScope::Global,
      []() { TEMP_CAST(gEngine)->SetNextGameStatus(EGlobalGameState::FirstLoading); }
  );

  mIsCalled = true;
}

inline void MIOMeta::Impl::MDY_PRIVATE(PopulateGlobalResourceSpecifierList)() const noexcept
{
  static bool mIsCalled = false;
  MDY_ASSERT_MSG_FORCE(mIsCalled == false, "This function must not be called twice.");

  // Global resource list consists of many sub-global resource list from each global resource script.
  SIOConnectionHelper::PopulateResourceList(mGlobalResourceSpecifierList, EResourceScope::Global,
    [] 
    { 
      // Create global scripts.
      auto& scriptManager = MScript::GetInstance();
      scriptManager.CreateGlobalScriptInstances();
      scriptManager.CallonStartGlobalScriptList();
      TEMP_CAST(gEngine)->SetNextGameStatus(EGlobalGameState::Loading); 
    }
  );
  mIsCalled = true;
}

inline void MIOMeta::Impl::MDY_PRIVATE(InitiateMetaInformation)()
{
  const auto& metaPath = MSetting::GetInstance().GetMetaPathSettingInformation();
  reflect::RBuiltinResources::BindBuiltinResourcesToMetaManager();

  MDY_CALL_ASSERT_SUCCESS(this->pReadFontResourceMetaInformation    (metaPath.mFontMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelMeshResourceMetaInformation(metaPath.mModelMeshMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelResourceMetaInformation   (metaPath.mModelMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelSkeletonMetaInformation   (metaPath.mModelSkeletonMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelAnimationMetaInformation  (metaPath.mModelAnimMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadTextureResourceMetaInformation (metaPath.mTextureMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadShaderResourceMetaInformation  (metaPath.mGLShaderMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadMaterialResourceMetaInformation(metaPath.mMaterialMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadSoundResourceMetaInformation   (metaPath.mSoundMetaPath));

  MDY_CALL_ASSERT_SUCCESS(this->pReadScriptResourceMetaInformation(metaPath.mScriptMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadPrefabResourceMetaInformation(metaPath.mPrefabMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadWidgetResourceMetaInformation(metaPath.mWidgetMetaPath));
  MDY_CALL_ASSERT_SUCCESS(this->pReadLevelResourceMetaInformation (metaPath.mLevelMetaPath));
}

inline void MIOMeta::Impl::MDY_PRIVATE(InitiateMetaInformationComp)(const nlohmann::json& iJson)
{
  reflect::RBuiltinResources::BindBuiltinResourcesToMetaManager();

  MDY_CALL_ASSERT_SUCCESS(this->pReadFontMetaAtlas(iJson["Font"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadShaderMetaAtlas(iJson["Shader"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadSoundMetaAtlas(iJson["Sound"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadTextureMetaAtlas(iJson["Texture"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadMaterialMetaAtlas(iJson["Material"]));

  MDY_CALL_ASSERT_SUCCESS(this->pReadModelAnimMetaAtlas(iJson["ModelAnim"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelMeshMetaAtlas(iJson["ModelMesh"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelSkelMetaAtlas(iJson["ModelSkel"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadModelMetaAtlas(iJson["Model"]));

  MDY_CALL_ASSERT_SUCCESS(this->pReadPrefabMetaAtlas(iJson["Prefab"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadScriptMetaAtlas(iJson["Script"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadWidgetMetaAtlas(iJson["Widget"]));
  MDY_CALL_ASSERT_SUCCESS(this->pReadLevelMetaAtlas (iJson["Level"]));
}

inline EDySuccess MIOMeta::Impl::pReadScriptResourceMetaInformation(const std::string& metaFilePath)
{
  // Validity Test
  const std::optional<nlohmann::json> opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG(opJsonAtlas.has_value() == true, "Failed to read script resource meta information. File path is not exist.");

  // Check "List" Category is exist.
  const nlohmann::json& jsonAtlas = opJsonAtlas.value();
  return this->pReadScriptMetaAtlas(jsonAtlas);
}

inline EDySuccess MIOMeta::Impl::pReadPrefabResourceMetaInformation(const std::string& metaFilePath)
{
  /// @brief Check prefab meta information list.
  static auto CheckPrefabMetaCategory = []( const nlohmann::json& atlas) -> EDySuccess
  {
    using namespace json;
    if (json::HasJsonKey(atlas, sCategoryMeta) == false)       { return EDySuccess::DY_FAILURE; }
    if (json::HasJsonKey(atlas, sCategoryObjectList) == false) { return EDySuccess::DY_FAILURE; }
    return EDySuccess::DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Validity Test
  const std::optional<nlohmann::json> opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read prefab resource meta information.");

  const auto& jsonAtlas = opJsonAtlas.value();
  MDY_CALL_ASSERT_SUCCESS(CheckPrefabMetaCategory(jsonAtlas));
  return this->pReadPrefabMetaAtlas(jsonAtlas);
}

inline EDySuccess MIOMeta::Impl::pReadFontResourceMetaInformation(const std::string& metaFilePath)
{ 
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read font meta information. File is not exist.");
  // (2) Get information from buffer.
  return this->pReadFontMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadModelResourceMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read Model meta information. File is not exist.");

  // (2) Get information from buffer.
  return this->pReadModelMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadModelMeshResourceMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read model meta information. File is not exist.");

  // (2) Get information from buffer.
  return this->pReadModelMeshMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadModelSkeletonMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read model skeleton information. File is not exist.");

  // (2) Get information from buffer.
  return this->pReadModelSkelMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadModelAnimationMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read model animation sequence information. File is not exist.");

  // (2) Get information from buffer.
  return this->pReadModelAnimMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadTextureResourceMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read texture meta information. File is not exist.");

  // (2) Insert each item.
  return this->pReadTextureMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadShaderResourceMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read shader meta information. File is not exist.");
  // (2) Insert each item.
  return this->pReadShaderMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadMaterialResourceMetaInformation(const std::string& metaFilePath)
{
  // (1) Validity Test
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read material meta information. File is not exist.");
  // (2) Insert each item.
  return this->pReadMaterialMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadLevelResourceMetaInformation(const std::string& metaFilepath)
{
  const auto opJsonAtlas = json::GetAtlasFromFile(metaFilepath);
  MDY_ASSERT_MSG_FORCE(
    opJsonAtlas.has_value() == true, 
    "Failed to read scene meta information. File is not exist.");

  return this->pReadLevelMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadWidgetResourceMetaInformation(const std::string& metaFilePath)
{ 
  const std::optional<nlohmann::json> opJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG(
    opJsonAtlas.has_value() == true, 
    "Must be valid json atlas from file path.");

  return this->pReadWidgetMetaAtlas(opJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadSoundResourceMetaInformation(const std::string& metaFilePath)
{
  const auto optJsonAtlas = json::GetAtlasFromFile(metaFilePath);
  MDY_ASSERT_MSG(
    optJsonAtlas.has_value() == true, 
    "Must be valid json atlas from file path.");

  return this->pReadSoundMetaAtlas(optJsonAtlas.value());
}

inline EDySuccess MIOMeta::Impl::pReadScriptMetaAtlas(const nlohmann::json& iJson)
{
  // Check "List" Category is exist.
  MDY_ASSERT(json::HasJsonKey(iJson, sCategoryList) == true);

  const auto& scriptResourceListAtlas = iJson[(sCategoryList)];
  for (const auto& scriptResource : scriptResourceListAtlas)
  {
    auto metaInfo = scriptResource.get<PDyScriptInstanceMetaInfo>();

    // Check Duplicated script specfier integrity
    MDY_ASSERT_MSG(
      Contains(this->mScriptMetaInfo, metaInfo.mSpecifierName) == false, 
      "Duplicated script specifier not permitted.");
    MDY_ASSERT_MSG(
      std::filesystem::exists(metaInfo.mFilePath) == true, 
      "File not exist.");

    auto [it, isSucceeded] = this->mScriptMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
    MDY_ASSERT_MSG(isSucceeded == true, "Unexpected error occurred.");
  }

  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadPrefabMetaAtlas(const nlohmann::json& iJson)
{
  // Make prefab meta information instance sequencially.
  const auto& prefabAtlas = iJson.at((sCategoryObjectList));

  TPrefabMetaInfoList prefabObjectList = {};
  for (const auto& prefabInfo : prefabAtlas)
  {
    prefabObjectList.emplace_back(PDyPrefabInstanceMetaInfo::CreateMetaInformation(prefabInfo));
  }

  // (2) Make object list tree.
  for (auto& object : prefabObjectList)
  {
    if (MDY_CHECK_ISEMPTY(object)) { continue; }
    if (object->mPrefabType == EWorldObjectType::Actor
    &&  object->mCommonProperties.mParentSpecifierName.empty() == false)
    { // If object type is Actor, and have parents specifier name as dec
      // Try move object into any parent's children list.
      const auto list = regex::CreateObjectParentSpecifierList(object->mCommonProperties.mParentSpecifierName);
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
  return EDySuccess::DY_SUCCESS; 
}

inline EDySuccess MIOMeta::Impl::pReadWidgetMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& widgetMeta : iJson)
  {
    auto rootInstance = DyCreateWidgetMetaInformation(widgetMeta);
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(rootInstance), "Widget root instance must not be empty.");
    this->mWidgetMetaInfo.try_emplace(rootInstance->mWidgetSpecifierName, std::move(rootInstance));
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadFontMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& fontItem : iJson)
  { 
    // Create font meta information instance from each json atlas.
    const auto fontInfo = json::GetValue<PDyMetaFontInformation>(fontItem);
    auto [_, isSucceeded] = this->mFontMetaInfo.try_emplace(fontInfo.mSpecifierName, fontInfo);
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Font meta information creation must be succeeded.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadLevelMetaAtlas(const nlohmann::json& iJson)
{
  auto levelList = iJson.items();
  for (auto it = levelList.begin(); it != levelList.end(); ++it)
  {
    auto metaInfo = it.value().get<PLevelInstanceMetaInfo>();
    metaInfo.mLevelName = it.key();

    auto [_, isSucceeded] = this->mLevelMetaInfo.try_emplace(metaInfo.mLevelName, metaInfo);
    MDY_ASSERT(isSucceeded == true);
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadShaderMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<PDyGLShaderInstanceMetaInfo>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mShaderMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadSoundMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<decltype(mSoundMetaInfo)::value_type::second_type>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mSoundMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadTextureMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<PDyTextureInstanceMetaInfo>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mTextureMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadMaterialMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<PDyMaterialInstanceMetaInfo>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mMaterialMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadModelAnimMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<decltype(mModelAnimScrapMetaInfo)::value_type::second_type>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mModelAnimScrapMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadModelMeshMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<PDyMeshInstanceMetaInfo>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mModelMeshMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadModelSkelMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<decltype(mModelSkeletonMetaInfo)::value_type::second_type>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mModelSkeletonMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pReadModelMetaAtlas(const nlohmann::json& iJson)
{
  for (const auto& item : iJson.items())
  {
    auto desc = item.value().get<PDyModelInstanceMetaInfo>();
    desc.mSpecifierName = item.key();

    auto [it, isSucceeded] = this->mModelMetaInfo.try_emplace(desc.mSpecifierName, std::move(desc));
    MDY_ASSERT_MSG_FORCE(isSucceeded == true, "Unexpected error occurred.");
  }
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddWidgetMetaInformation(const std::string& metaInformationString)
{
  const nlohmann::json jsonAtlas = nlohmann::json::parse(metaInformationString);
  auto rootInstance = DyCreateWidgetMetaInformation(jsonAtlas);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(rootInstance), "Widget root instance must not be empty.");

  this->mWidgetMetaInfo.try_emplace(rootInstance->mWidgetSpecifierName, std::move(rootInstance));
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::MDY_PRIVATE(AddLoadingWidgetMetaInformation)(const std::string& widgetMetaInfo)
{
  const nlohmann::json jsonAtlas = nlohmann::json::parse(widgetMetaInfo);
  auto rootInstance = DyCreateWidgetMetaInformation(jsonAtlas);
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(rootInstance), "Widget root instance must not be empty.");

  mLoadingWidgetMetaInfo = std::move(rootInstance);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddScriptMetaInformation(const PDyScriptInstanceMetaInfo& metaInfo)
{
  if (metaInfo.mScriptMode == decltype(metaInfo.mScriptMode)::Global)
  { // If script mode is `Global`, insert it to separated container.
    MDY_ASSERT_MSG_FORCE(
      Contains(this->mGlobalScriptMetaInfo, metaInfo.mSpecifierName) == false, 
      "Duplicated global script name is exist.");

    this->mGlobalScriptMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  }
  else
  {
    MDY_ASSERT_MSG_FORCE(
      Contains(this->mScriptMetaInfo, metaInfo.mSpecifierName) == false, 
      "Duplicated general script name is exist.");

    this->mScriptMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  }

  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddGLShaderMetaInfo(const PDyGLShaderInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT_MSG(
    Contains(this->mShaderMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated gl shader name is exist.");

  this->mShaderMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddBuiltinMeshMetaInfo(const PDyMeshInstanceMetaInfo& metaInfo)
{
#if defined(NDEBUG) == false 
  if (metaInfo.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel == false)
  {
    MDY_ASSERT_MSG(
      metaInfo.mCustomMeshBuffer.empty() == false, 
      "Builtin mesh must be valid if using customized binding.");
  }
#endif
  MDY_ASSERT_MSG(
    Contains(this->mModelMeshMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated Mesh name is exist.");

  this->mModelMeshMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddModelMetaInfo(const PDyModelInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT_MSG(
    Contains(this->mModelMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated model name is exist.");

  this->mModelMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddTextureMetaInfo(const PDyTextureInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT_MSG(
    Contains(this->mTextureMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated texture name is exist.");

  this->mTextureMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddMaterialMetaInfo(const PDyMaterialInstanceMetaInfo& metaInfo)
{
  MDY_ASSERT_MSG(
    Contains(this->mMaterialMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated material name is exist.");

  this->mMaterialMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddGLAttachmentMetaInfo(const PDyGlAttachmentInstanceMetaInfo& metaInfo)
{
#if defined(_DEBUG) == true
  MDY_ASSERT_MSG(
    metaInfo.mSpecifierName.empty() != true, 
    "Attachment specifier name must be specifed.");
  MDY_ASSERT_MSG(
    metaInfo.mBufferFormat != EGlBufferDataInternalFormat::NoneError, 
    "Attachment format must not none.");
  MDY_ASSERT_MSG(
    metaInfo.mAttachmentSize.X > 0 && metaInfo.mAttachmentSize.Y > 0, 
    "Attachment size must be positive.");
#endif

  MDY_ASSERT_MSG(
    Contains(this->mAttachmentMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated attachment name is exist.");

  this->mAttachmentMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::pfAddGLFrameBufferMetaInfo(const PDyGlFrameBufferInstanceMetaInfo& metaInfo)
{

#if defined(_DEBUG) == true
  // Validation test check.
  MDY_ASSERT_MSG(
    metaInfo.mSpecifierName.empty() != true, 
    "Attachment specifier name must be specifed.");
  for (const auto& [name, type] : metaInfo.mColorAttachmentList) 
  { // Check color attachment.
    MDY_ASSERT_MSG(
      name.empty() == false, 
      "Color attachment specifier name must not be empty.");
    MDY_ASSERT_MSG(
      type != EDyGlAttachmentType::Depth && type != EDyGlAttachmentType::NoneError, 
      "Color attachment type must be `::ColorX`");
    MDY_ASSERT_MSG(
      metaInfo.mFrameBufferSize.X > 0 && metaInfo.mFrameBufferSize.Y > 0, 
      "Framebuffer size must be positive.");
  }
#endif

  MDY_ASSERT_MSG(
    Contains(this->mFrameBufferMetaInfo, metaInfo.mSpecifierName) == false, 
    "Duplicated framebuffer name is exist.");

  auto [it, isSuccessful] = this->mFrameBufferMetaInfo.try_emplace(metaInfo.mSpecifierName, metaInfo);
  MDY_ASSERT_MSG(isSuccessful == true, "Setting up framebuffer meta info must be successful.");

  auto& [specifier, instance] = *it;
  if (instance.mIsUsingDepthBuffer == true && instance.mDepthAttachmentSpecifier.empty() == true)
  { // If frame buffer will use depth buffer but not specified anything, 
    // just create default framebuffer depth attachment meta info.
    // and bind it.
    PDyGlAttachmentInstanceMetaInfo defaultDepthBuffer;
    {
      defaultDepthBuffer.mSpecifierName   = fmt::format("{}_D_{}", specifier, CreateRandomString(5));
      defaultDepthBuffer.mBufferFormat    = EGlBufferDataInternalFormat::DEPTH32;
      defaultDepthBuffer.mSourceType      = EDyResourceSource::Builtin;
      defaultDepthBuffer.mAttachmentSize  = instance.mFrameBufferSize;
    }
    
    MDY_CALL_ASSERT_SUCCESS(this->pfAddGLAttachmentMetaInfo(defaultDepthBuffer));
    instance.mDepthAttachmentSpecifier = defaultDepthBuffer.mSpecifierName;
  }

  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::MDY_PRIVATE(AddBootResourceSpecifierList)(const TResourceSpecifierList& list)
{
  this->mBootResourceSpecifierList = list;
  return EDySuccess::DY_SUCCESS;
}

inline EDySuccess MIOMeta::Impl::MDY_PRIVATE(AddGlobalResourceSpecifierList)(const TResourceSpecifierList& list)
{
  if (list.empty() == false) { this->mGlobalResourceSpecifierList.emplace_back(list); }
  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_METAINFOMANAGER_IMPL_INL