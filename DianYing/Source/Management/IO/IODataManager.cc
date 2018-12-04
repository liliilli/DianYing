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
#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>
#include <filesystem>

namespace
{

MDY_SET_IMMUTABLE_STRING(kDyDataInformation,            "MDyIOData");

MDY_SET_IMMUTABLE_STRING(kErrorShaderNameNotSpecified,  "{} | Failed to create shader information. Shader name is not specified.");
MDY_SET_IMMUTABLE_STRING(kErrorShaderFragmentEmpty,     "{} | Failed to create shader information. Shader fragments are empty.");
MDY_SET_IMMUTABLE_STRING(kErrorShaderFramgmentRawCodeEmpty,
    "{}::{} | Failed to create shader information. Shader fragment's mIsEnabledRawLoadShaderCode is true but not specified mShaderRawCode.");
MDY_SET_IMMUTABLE_STRING(kErrorShaderFramgmentPathEmpty,
    "{}::{} | Failed to create shader information. Shader fragment's path is not specified so can not arbitary fragment file.");

MDY_SET_IMMUTABLE_STRING(kErrorTextureNameNotSpecified, "{} | Failed to create texture information. Texture name is not specified.");
MDY_SET_IMMUTABLE_STRING(kErrorTextureLocalPathEmpty,   "{} | Failed to create texture information. \
Texture local path is empty though absolute path flag is false.");
MDY_SET_IMMUTABLE_STRING(kErrorTextureAbsolPathEmpty,   "{} | Failed to create texture information. \
Texture absolute path is empty though absolute path flag is true.");
MDY_SET_IMMUTABLE_STRING(kErrorTextureTypeNone,         "{} | Failed to create texture information. Texture type must not be 'None'");

MDY_SET_IMMUTABLE_STRING(kErrorMaterialNameNotSpecified, "{}::{} | Failed to create material information. Material name is not speicified.");
MDY_SET_IMMUTABLE_STRING(kErrorBindingShaderNameEmpty,   "{}::{} | Failed to create material information. \
Shader name is not speicified even though flag is not set. | Material name : {}");

MDY_SET_IMMUTABLE_STRING(kErrorModelNameEmpty,          "{}::{} | Failed to create model information. Model name is not speicified.");
MDY_SET_IMMUTABLE_STRING(kErrorModelPathEmpty,          "{}::{} | Failed to create model information. Model path is not speicified. | Model name : {}");

} /// ::unnamed namespace

namespace dy
{

EDySuccess MDyIOData::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyIOData::pfInitialize().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyIOData::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyIOData::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyIOData::CreateShaderInformation_Deprecated(const std::string& shaderSpecifierName, MDY_NOTUSED EDyScope scope)
{
  ///
  /// @brief Check compatibility to Dy shader rendering system. \n
  /// Must have either at least compute shader, or vertex shader.
  ///
  static auto CheckValidity = [](_MIN_ const PDyGLShaderInstanceMetaInfo& metaInfo)
  {
    if (metaInfo.mSpecifierName.empty() == true)
    {
      MDY_LOG_CRITICAL_D(kErrorShaderNameNotSpecified, kDyDataInformation);
      return DY_FAILURE;
    }

    if (metaInfo.mSourceType == EDyResourceSource::Builtin)
    { // If shader meta information is builtin meta info, verify mBuiltinBuffer.
      if (metaInfo.mIsComputeShader == true)
      {
        MDY_ASSERT(
            metaInfo.GetFragment(EDyShaderFragmentType::Compute).mBuiltinBuffer.empty() == false,
            "Compute shader must not be empty. Unexpected error occurred.");
      }
      else
      {
        MDY_ASSERT(
            metaInfo.GetFragment(EDyShaderFragmentType::Vertex).mBuiltinBuffer.empty() == false,
            "Vertex shader must not be empty. Unexpected error occurred.");
      }
    }
    else
    {
      if (metaInfo.mIsComputeShader == true)
      { // If shader meta information is external, verify file path.
        MDY_ASSERT(
          metaInfo.GetFragment(EDyShaderFragmentType::Compute).mExternalFilePath.empty() == false,
          "Compute shader must not be specified with empty path. Unexpected error occurred.");
      }
      else
      {
        MDY_ASSERT(
          metaInfo.GetFragment(EDyShaderFragmentType::Vertex).mExternalFilePath.empty() == false,
          "Vertex shader must not be specified with empty path. Unexpected error occurred.");
      }
    }
    return DY_SUCCESS;
  };

  ///
  /// @brief Make shader information and bind to `Dy` shader rendering system.
  ///
  static auto InsertShaderInformation = [](_MIN_ const PDyGLShaderInstanceMetaInfo& metaInfo,
                                           _MIO_ THeapHash<DDyShaderInformation>& shaderMap)
  {
    // Create information space.
    const auto& shaderName    = metaInfo.mSpecifierName;
    auto [it, creationResult] = shaderMap.try_emplace(shaderName, nullptr);
    if (creationResult == false)
    { // Something is already in or memory oob.
      MDY_LOG_CRITICAL("{} | Unexpected error happened during create memory for shader information {}.",
                       "MDyIOData::CreateShaderInformation_Deprecated().", shaderName);
      return DY_FAILURE;
    }

    // Make resource in heap, and insert it to empty memory space.
    auto shaderInfoSmtPtr = std::make_unique<DDyShaderInformation>(metaInfo);
    it->second.swap(shaderInfoSmtPtr);

    if (MDY_CHECK_ISEMPTY(it->second))
    {
      MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Unexpected error occurred.");
      MDY_LOG_CRITICAL("{} | Unexpected error happened during swapping shader information {}.",
                       "MDyIOData::CreateShaderInformation_Deprecated().", shaderName);
      shaderMap.erase(shaderName);
      return DY_FAILURE;
    }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  const auto& metaManager     = MDyMetaInfo::GetInstance();
  const auto& shaderMetaInfo  = metaManager.GetGLShaderMetaInformation(shaderSpecifierName);

  MDY_ASSERT(CheckValidity(shaderMetaInfo) == DY_SUCCESS, "Validity check must not be failed.");

  // Find if duplicated name is exist on information list.
  if (DyIsMapContains(this->mShaderInformation, shaderMetaInfo.mSpecifierName) == true)
  {
    MDY_LOG_WARNING_D("{} | {} is already found in mShaderInformation list.",
        "MDyIOData::CreateShaderInformation_Deprecated.", shaderMetaInfo.mSpecifierName);
    return DY_FAILURE;
  }

  // Create information space.
  MDY_CALL_ASSERT_SUCCESS(InsertShaderInformation(shaderMetaInfo, this->mShaderInformation));
  MDY_LOG_CRITICAL(R"dy("{}" shader information Created.)dy", shaderMetaInfo.mSpecifierName);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::CreateModelInformation_Deprecated(const std::string& modelSpecifierName, MDY_NOTUSED EDyScope scope)
{
  /// @brief Check compatibility of instance into Dy model binding structure.
  static auto CheckValidity = [](_MIN_ const PDyModelInstanceMetaInfo& metaInfo)
  {
    MDY_ASSERT(metaInfo.mSpecifierName.empty() == false, "Model specifier name must be specified.");

    if (metaInfo.mSourceType == EDyResourceSource::Builtin)
    { // If model meta information is builtin meta info, verify mPtrBuiltinModelBuffer.
      MDY_ASSERT(MDY_CHECK_ISNOTNULL(metaInfo.mPtrBuiltinModelBuffer), "Model buffer must not be nulled.");
    }
    else
    { // If model meta information is external, verify emptiness of file path and check exist.
      MDY_ASSERT(metaInfo.mExternalModelPath.empty() == false, "Model external file path must not be empty.");
      MDY_ASSERT(std::filesystem::exists(metaInfo.mExternalModelPath) == true, "Model file path must be valid.");
    }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  const auto& metaManager = MDyMetaInfo::GetInstance();
  const auto& metaInfo    = metaManager.GetModelMetaInformation(modelSpecifierName);
  MDY_ASSERT(CheckValidity(metaInfo) == DY_SUCCESS, "Validity check must not be failed.");

  decltype(this->mModelInformation)::iterator it;
  {
    std::lock_guard<std::mutex> mt(this->mTemporalMutex);
    if (DyIsMapContains(this->mModelInformation, metaInfo.mSpecifierName) == true)
    {
      MDY_LOG_WARNING_D("{} | Resource is already found. Name : {}", "MDyIOData", metaInfo.mSpecifierName);
      return DY_FAILURE;
    }

    // Check there is already in the information map, if not, make memory space to insert it.
    bool creationResult = false;
    std::tie(it, creationResult) = mModelInformation.try_emplace(metaInfo.mSpecifierName, nullptr);
    MDY_ASSERT(creationResult == true, "Unexpected error occurred.");
  }

  // Make resource in heap, and insert it to empty memory space.
  auto materialInformation = std::make_unique<DDyModelInformation>(metaInfo);
  it->second.swap(materialInformation);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Unexpected error occurred.");
  return DY_SUCCESS;
}

EDySuccess MDyIOData::CreateTextureInformation_Deprecated(const std::string& textureSpecifier, MDY_NOTUSED EDyScope scope)
{
  /// @brief Check compatibility of instance into Dy model binding structure.
  static auto CheckValidity = [](_MIN_ const PDyTextureInstanceMetaInfo& metaInfo)
  {
    MDY_ASSERT(metaInfo.mSpecifierName.empty() == false, "Texture specifier name must be specified.");

    if (metaInfo.mSourceType == EDyResourceSource::Builtin)
    { // If texture meta information is builtin meta info, verify mPtrBuiltinModelBuffer.
      MDY_ASSERT(MDY_CHECK_ISNOTNULL(metaInfo.mPtrBuiltinBuffer), "Texture buffer must not be nulled.");
    }
    else
    { // If texture meta information is external, verify emptiness of file path and check exist.
      MDY_ASSERT(metaInfo.mExternalFilePath.empty() == false, "Texture external file path must not be empty.");
      MDY_ASSERT(std::filesystem::exists(metaInfo.mExternalFilePath) == true, "Texture file path must be valid.");
    }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  const auto& metaManager = MDyMetaInfo::GetInstance();
  const auto& metaInfo    = metaManager.GetTextureMetaInformation(textureSpecifier);
  MDY_ASSERT(CheckValidity(metaInfo) == DY_SUCCESS, "Validity check must not be failed.");

  // Find if duplicated texture name is exist in information list.
  const auto& textureName = metaInfo.mSpecifierName;
  if (DyIsMapContains(this->mTextureInformation, textureName) == true)
  {
    MDY_LOG_WARNING_D("{}::{} | {} is already found in mTextureInformation list.", kDyDataInformation, "CreateTextureInformation_Deprecated", textureName);
    return DY_FAILURE;
  }

  auto [it, creationResult] = this->mTextureInformation.try_emplace(textureName, nullptr);
  MDY_ASSERT(creationResult == true, "Unexpected erorr occurred.");

  // Make resource in heap, and insert it to empty memory space.
  auto textureInformation = std::make_unique<DDyTextureInformation>(metaInfo);
  it->second.swap(textureInformation);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Unexpected error occurred.");
  MDY_LOG_CRITICAL("{} | \"{}\" texture information Created.", textureName);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::CreateMaterialInformation_Deprecated(_MIN_ const std::string& materialSpecifier, MDY_NOTUSED EDyScope scope)
{
  /// @brief Check metarial meta information whether specifier name and shader exists. \n
  /// Texture list does not have to be not empty. (intentional)
  static auto CheckValidity = [](_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo)
  {
    MDY_ASSERT(metaInfo.mSpecifierName.empty() == false, "Material specifier name must be specified.");
    MDY_ASSERT(metaInfo.mShaderSpecifier.empty() == false, "Material shader specifier must be specified.");

    const auto& metaManager = MDyMetaInfo::GetInstance();
    MDY_ASSERT(metaManager.IsGLShaderMetaInfoExist(metaInfo.mShaderSpecifier) == true, "Shader must be valid.");

    for (const auto& textureSpecifier : metaInfo.mTextureNames)
    {
      if (textureSpecifier.empty() == true) { continue; }
      MDY_ASSERT(metaManager.IsTextureMetaInfoExist(textureSpecifier) == true, "Texture must be valid.");
    }
    return DY_SUCCESS;
  };

#ifdef false
  struct DMaterialInfoDependenciesPtrList final
  {
    using TTextureInfoList = std::array<DDyTextureInformation*, 16>;

    DDyShaderInformation* mPtrShaderInfo = MDY_INITIALIZE_NULL;
    TTextureInfoList      mPtrTextureInfos = {};
  };
#endif

  static auto GetDependenciesOfMaterialInfo = [this, scope](_MIN_ const PDyMaterialInstanceMetaInfo& metaInfo)
  {
    //DMaterialInfoDependenciesPtrList result{};

    if (this->IsShaderInformationExist(metaInfo.mShaderSpecifier) == false)
    {
      MDY_CALL_ASSERT_SUCCESS(this->CreateShaderInformation_Deprecated(metaInfo.mShaderSpecifier, scope));
    }

    const auto* shaderInfo = this->GetShaderInformation(metaInfo.mShaderSpecifier);
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(shaderInfo), "Unexpected error occurred.");
    //result.mPtrShaderInfo = shaderInfo;
#ifdef false
    if (shaderInfo->GetScope() < scope) { shaderInfo->SetScope(scope); this->ResumeFromGCCandidate(shaderInfo); }
#endif

    for (const auto& textureSpecifier : metaInfo.mTextureNames)
    {
      if (textureSpecifier.empty() == true) { continue; }
      if (this->IsTextureInformationExist(textureSpecifier) == false)
      {
        MDY_CALL_ASSERT_SUCCESS(this->CreateTextureInformation_Deprecated(textureSpecifier, scope));
      }
      const auto* textureInfo = this->GetTextureInformation(textureSpecifier);
      MDY_ASSERT(MDY_CHECK_ISNOTNULL(textureInfo), "Unexpected error occurred.");
  #ifdef false
      if (textureInfo->GetScope() < scope) { textureInfo->SetScope(scope); this->ResumeFromGCCandidate(textureInfo); }
  #endif
    }
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  const auto& metaManager = MDyMetaInfo::GetInstance();
  const auto& metaInfo = metaManager.GetMaterialMetaInformation(materialSpecifier);
  MDY_ASSERT(CheckValidity(metaInfo) == DY_SUCCESS, "Validity check must not be failed.");

  // Check there is duplicated material name is exist in information list.
  const auto& materialName = metaInfo.mSpecifierName;
  if (DyIsMapContains(this->mMaterialInformation, materialName) == true)
  {
    MDY_LOG_WARNING_D("{}::{} | {} is already found in mMaterialInformation list.", kDyDataInformation, "CreateMaterialInformation_Deprecated", materialName);
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mMaterialInformation.try_emplace(materialName, nullptr);
  MDY_ASSERT(creationResult == true, "Unexpected error occurred.");

  // Make resource in heap, and insert it to empty memory space.
  GetDependenciesOfMaterialInfo(metaInfo);
  auto materialInformation = std::make_unique<DDyMaterialInformation>(metaInfo);
  it->second.swap(materialInformation);

  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Unexpected error occurred.");
  return DY_SUCCESS;
}

EDySuccess MDyIOData::CreateSoundInformation(const PDySoundConstructionDescriptor& soundDescriptor)
{
  ///
  /// @function CheckIntegerityOfDescriptor
  /// @brief Check sound escriptor is good to fit in.
  ///
  static auto CheckIntegerityOfDescriptor = [](const PDySoundConstructionDescriptor& soundDescriptor) -> EDySuccess
  {
    if (soundDescriptor.mSoundName.empty()) return DY_FAILURE;
    if (soundDescriptor.mSoundPath.empty()) return DY_FAILURE;

    return DY_SUCCESS;
  };

  if (CheckIntegerityOfDescriptor(soundDescriptor) == DY_FAILURE)
  {
    /// @TODO OUTPUT FAILURE MESSAGE WITH PROPER ERROR FLAG
    return DY_FAILURE;
  };

  const auto& soundName = soundDescriptor.mSoundName;
  decltype(this->mSoundInformation)::iterator it;
  {
    std::lock_guard<std::mutex> mt(this->mTemporalMutex);
    if (mSoundInformation.find(soundName) != mSoundInformation.end())
    {
      MDY_LOG_WARNING_D("{} | Resource is already found. Name : {}", "MDyIOData", soundName);
      return DY_FAILURE;
    }

    // Check there is already in the information map, if not, make memory space to insert it.
    bool creationResult = false;
    std::tie(it, creationResult) = mSoundInformation.try_emplace(soundName, nullptr);
    if (!creationResult)
    {
      MDY_LOG_CRITICAL_D("{} | Failed to create information resource memory space. Name : {}", "MDyIOData", soundName);
      return DY_FAILURE;
    }
  }

  // Make resource in heap, and insert it to empty memory space.
  auto soundInfo = std::make_unique<DDySoundInformation>(soundDescriptor);
  if (it->second.swap(soundInfo); !it->second)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occured. Name : {}", "MDyIOData", soundName);
    this->mSoundInformation.erase(soundName);
    return DY_FAILURE;
  }

  return DY_SUCCESS;;
}

std::optional<std::string> MDyIOData::PopulateMaterialInformation(
    const std::string& materialName,
    const PDyMaterialPopulateDescriptor& materialPopulateDescriptor)
{
  // Check if baseMaterial called materialName is already on information list.
  const auto* baseMaterial = GetMaterialInformation(materialName);
  if (baseMaterial == nullptr)
  {
    MDY_LOG_CRITICAL_D("{} | Failed to getting information of base material. base material name : {}", "MDyIOData::PopulateMaterialInformation", materialName);
    return std::nullopt;
  }

  // Setup meterial populate descriptor from parameter descriptor and error checking.
  PDyMaterialPopulateDescriptor actualMaterialPopDesc = materialPopulateDescriptor;
  if (!actualMaterialPopDesc.mIsEnabledMaterialCustomNameOverride)
  {
    std::string newMaterialName {fmt::format("ov_{0}", materialName)};
    if (actualMaterialPopDesc.mIsEnabledShaderOverride)
    {
      newMaterialName.append(fmt::format("{0}{1}", 's', actualMaterialPopDesc.mOverrideShaderName));
    }
    const auto id = baseMaterial->__pfEnrollAndGetNextDerivedMaterialIndex(newMaterialName);
    actualMaterialPopDesc.mMaterialOverrideName = newMaterialName + std::to_string(id);
  }
  else
  {
    if (actualMaterialPopDesc.mMaterialOverrideName.empty())
    {
      MDY_LOG_ERROR("{} | Empty material override name is prohibitted.", "MDyIOData::PopulateMaterialInformation");
      return std::nullopt;
    }
    if (GetMaterialInformation(actualMaterialPopDesc.mMaterialOverrideName))
    {
      MDY_LOG_ERROR("{} | Override name is already posed by any of material information.", "MDyIOData::PopulateMaterialInformation");
      return std::nullopt;
    };
  }

  // Check there is already in the information map.
  auto [infoIt, result] = this->mMaterialInformation.try_emplace(actualMaterialPopDesc.mMaterialOverrideName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occured.", "MDyIOData::PopulateMaterialInformation");
    return std::nullopt;
  }

  // Let baseMaterial have created populated derived material and move ownership to list.
  auto populateDerivedSmtPtr = baseMaterial->__pfPopulateMaterialWith(actualMaterialPopDesc);
  infoIt->second.swap(populateDerivedSmtPtr);
  if (!infoIt->second)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occured on swapping.", "MDyIOData::PopulateMaterialInformation");
    return std::nullopt;
  }

  MDY_LOG_INFO("{}::{} | Populated material from base material {}. | Populated material name : {}",
               kDyDataInformation, "PopulateMaterialInformation", materialName, actualMaterialPopDesc.mMaterialOverrideName);
  return actualMaterialPopDesc.mMaterialOverrideName;
}

//!
//! Delete functions
//!

EDySuccess MDyIOData::DeleteShaderInformation(const std::string& shaderName, bool isForced)
{
  const auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove shader information. Have not shader information. | {} : {}",
                    "MDyIOData", "DeleteShaderInformation", "Shader Name", shaderName);
    return DY_FAILURE;
  }

  // IF mShaderInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove shader information. Must remove resource first. | {} : {}",
                    "MDyIOData", "DeleteShaderInformation", "Shader lName", shaderName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mShaderInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::DeleteTextureInformation(const std::string& textureName, bool isForced)
{
  const auto iterator = mTextureInformation.find(textureName);
  if (iterator == mTextureInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove texture information. Have not texture information. | {} : {}",
                    "MDyIOData", "DeleteTextureInformation", "Texture Name", textureName);
    return DY_FAILURE;
  }

  // IF mTextureInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove material information. Must remove resource first. | {} : {}",
                    "MDyIOData", "DeleteMaterialInformation", "Material lName", textureName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mTextureInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::DeleteMaterialInformation(const std::string& materialName, bool isForced)
{
  const auto iterator = this->mMaterialInformation.find(materialName);
  if (iterator == this->mMaterialInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove material information. Have not material information. | {} : {}",
                    "MDyIOData", "DeleteMaterialInformation", "Material Name", materialName);
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove material information. Must remove resource first. | {} : {}",
                    "MDyIOData", "DeleteMaterialInformation", "Material lName", materialName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mMaterialInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::DeleteModelInformation(const std::string& modelName, bool isAllRemoveSubresource, bool isForced)
{
  const auto iterator = mModelInformation.find(modelName);
  if (iterator == mModelInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove model information. Have not model information. | {} : {}",
                    "MDyIOData", "DeleteModelInformation", "ModelName", modelName);
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE, otherwise remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove model information. Must remove resource first. | {} : {}",
                    "MDyIOData", "DeleteModelInformation", "ModelName", modelName);
    return DY_FAILURE;
  }

  // First, release all information and related subresource instances from system
  // only when isAllRemoveSubresource true.
  if (isAllRemoveSubresource)
  {
    const auto& materialNameList = iterator->second->GetBindedMaterialNameLists();
    for (const auto& materialName : materialNameList)
    {
      this->DeleteMaterialInformation(materialName);
    }
  }

  // And remove model information!
  mModelInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::DeleteSoundInformation(const std::string& soundName, bool isForced)
{
  const auto iterator = mSoundInformation.find(soundName);
  if (iterator == mSoundInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove sound information. Have not sound information. | {} : {}",
                    "MDyIOData", "DeleteSoundInformation", "Sound Name", soundName);
    return DY_FAILURE;
  }

  // IF mTextureInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove sound information. Must remove resource first. | {} : {}",
                    "MDyIOData", "DeleteSoundInformation", "Sound Name", soundName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mSoundInformation.erase(iterator);
  return DY_SUCCESS;
}

//!
//! Get function.
//!

const DDyShaderInformation* MDyIOData::GetShaderInformation(const std::string& shaderName) const noexcept
{
  const auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    MDY_LOG_WARNING("{}::{} | Failed to find shader information. | Shader name : {}",
                    kDyDataInformation, "GetTextureInformation", shaderName);
    return nullptr;
  }

  return iterator->second.get();
}

const DDyTextureInformation* MDyIOData::GetTextureInformation(const std::string& textureName) const noexcept
{
  const auto iterator = mTextureInformation.find(textureName);
  if (iterator == mTextureInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find texture information. | Texture name : {}",
                    "MDyIOData::GetTextureInformation", textureName);
    return nullptr;
  }

  return iterator->second.get();
}

const DDyMaterialInformation* MDyIOData::GetMaterialInformation(const std::string& materialName) const noexcept
{
  const auto iterator = mMaterialInformation.find(materialName);
  if (iterator == mMaterialInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find material information. | Material name : {}",
                    "MDyIOData::GetMaterialInformation", materialName);
    return nullptr;
  }

  return iterator->second.get();
}

const DDyModelInformation* MDyIOData::GetModelInformation(const std::string& modelName) const noexcept
{
  std::lock_guard<std::mutex> mt(this->mTemporalMutex);

  const auto iterator = this->mModelInformation.find(modelName);
  if (iterator == this->mModelInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find model information. | {} : {}",
                    "MDyIOData", "Model name", modelName);
    return nullptr;
  }

  return iterator->second.get();
}

const DDySoundInformation* MDyIOData::GetSoundInformation(const std::string& soundName) const noexcept
{
  std::lock_guard<std::mutex> mt(this->mTemporalMutex);

  const auto iterator = this->mSoundInformation.find(soundName);
  if (iterator == this->mSoundInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find model information. | {} : {}", "MDyIOData", "Sound name", soundName);
    return nullptr;
  }

  return iterator->second.get();
}

//!
//! Deprecated
//!

#ifdef false
EDySuccess MDyIOData::CreateTextureInformation_Deprecated(const PDyTextureInstanceMetaInfo& textureDescriptor)
{
  ///
  /// @function
  /// @brief
  ///
  static auto CheckIntegerityOfDescriptor = [](const PDyTextureInstanceMetaInfo& textureDescriptor)
  {
    // Integrity test
    if (textureDescriptor.mSpecifierName.empty())
    {
      MDY_LOG_CRITICAL_D(kErrorTextureNameNotSpecified, kDyDataInformation);
      return DY_FAILURE;
    }
    if (textureDescriptor.mIsEnabledAbsolutePath_Deprecated && textureDescriptor.mTextureFileAbsolutePath_Deprecated.empty())
    {
      MDY_LOG_CRITICAL_D(kErrorTextureAbsolPathEmpty, kDyDataInformation);
      return DY_FAILURE;
    }
    else if (textureDescriptor.mExternalFilePath.empty())
    {
      MDY_LOG_CRITICAL_D(kErrorTextureLocalPathEmpty, kDyDataInformation);
      return DY_FAILURE;
    }
    if (textureDescriptor.mTextureType == EDyTextureStyleType::None)
    {
      MDY_LOG_CRITICAL_D(kErrorTextureTypeNone, kDyDataInformation);
      return DY_FAILURE;
    }

    return DY_SUCCESS;
  };

  ///
  /// @callback TaskInsertShaderInformation
  /// @brief
  ///
  static auto TaskInsertTextureInformation = [](const PDyTextureInstanceMetaInfo& textureDescriptor,
                                                THeapHash<DDyTextureInformation>& textureMap)
  {
    // Check there is already in the information map.
    const auto& textureName  = textureDescriptor.mSpecifierName;
    auto[it, creationResult] = textureMap.try_emplace(textureName, nullptr);
    if (!creationResult)
    {
      MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during create memory for texture information. Texture name : {}.",
        kDyDataInformation, "CreateTextureInformation_Deprecated", textureName);
      return DY_FAILURE;
    }

    // Make resource in heap, and insert it to empty memory space.
    auto textureInformation = std::make_unique<DDyTextureInformation>(textureDescriptor);
    it->second.swap(textureInformation);
    if (!it->second)
    {
      MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during swapping texture information {}.",
        kDyDataInformation, "CreateTextureInformation_Deprecated", textureName);
      textureMap.erase(textureName);
      return DY_FAILURE;
    }

    return DY_SUCCESS;
  };

  if (CheckIntegerityOfDescriptor(textureDescriptor) == DY_FAILURE)
  {
    return DY_FAILURE;
  }

  // Find if duplicated texture name is exist in information list.
  const auto& textureName = textureDescriptor.mSpecifierName;
  if (mTextureInformation.find(textureName) != mTextureInformation.end())
  {
    MDY_LOG_WARNING_D("{}::{} | {} is already found in mTextureInformation list.", kDyDataInformation, "CreateTextureInformation_Deprecated", textureName);
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  if (TaskInsertTextureInformation(textureDescriptor, this->mTextureInformation) == DY_FAILURE)
  {
    return DY_FAILURE;
  }

  MDY_LOG_CRITICAL("{} | \"{}\" texture information Created.", textureName);
  return DY_SUCCESS;
}

EDySuccess MDyIOData::CreateMaterialInformation_Deprecated(const PDyMaterialInstanceMetaInfo& materialDescriptor)
{
  // Integrity test
  if (materialDescriptor.mSpecifierName.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorMaterialNameNotSpecified, kDyDataInformation, "CreateMaterialInformation_Deprecated");
    return DY_FAILURE;
  }
  if (!materialDescriptor.mIsShaderLazyInitialized_Deprecated && materialDescriptor.mShaderSpecifier.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorBindingShaderNameEmpty, kDyDataInformation, "CreateMaterialInformation_Deprecated", materialDescriptor.mSpecifierName);
    return DY_FAILURE;
  }

  // Check there is duplicated material name is exist in information list.
  const auto& materialName = materialDescriptor.mSpecifierName;
  if (mMaterialInformation.find(materialName) != mMaterialInformation.end())
  {
    MDY_LOG_WARNING_D("{}::{} | {} is already found in mMaterialInformation list.", kDyDataInformation, "CreateMaterialInformation_Deprecated", materialName);
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mMaterialInformation.try_emplace(materialName, nullptr);
  if (!creationResult)
  {
    MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during create memory for material information. | Material name : {}.",
                     kDyDataInformation, "CreateMaterialInformation_Deprecated", materialName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto materialInformation = std::make_unique<DDyMaterialInformation>(materialDescriptor);
  it->second.swap(materialInformation);
  if (!it->second)
  {
    MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during swapping texture information {}.",
                     kDyDataInformation, "CreateMaterialInformation_Deprecated", materialName);
    this->mMaterialInformation.erase(materialName);
    return DY_FAILURE;
  }

  MDY_LOG_INFO("{}::{} | Create material information. | Material name : {}.", kDyDataInformation, "CreateMaterialInformation_Deprecated", materialName);
  return DY_SUCCESS;
}
#endif

} /// ::dy namespace