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
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kDyDataInformation,            "MDyDataInformation");

MDY_SET_IMMUTABLE_STRING(kErrorShaderNameNotSpecified,  "{} | Failed to create shader information. Shader name is not specified.");
MDY_SET_IMMUTABLE_STRING(kErrorShaderFragmentEmpty,     "{} | Failed to create shader information. Shader fragments are empty.");

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

EDySuccess MDyDataInformation::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyDataInformation::pfInitialize().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyDataInformation::pfRelease().", "FunctionCall");
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateShaderInformation(const PDyShaderConstructionDescriptor& shaderDescriptor)
{
  // Integrity test
  if (shaderDescriptor.mShaderName.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorShaderNameNotSpecified, kDyDataInformation);
    return DY_FAILURE;
  }
  if (shaderDescriptor.mShaderFragments.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorShaderFragmentEmpty, kDyDataInformation);
    return DY_FAILURE;
  }

  // Find if duplicated name is exist on information list.
  const auto& shaderName = shaderDescriptor.mShaderName;
  if (mShaderInformation.find(shaderName) != mShaderInformation.end())
  {
    MDY_LOG_WARNING_D("{} | {} is already found in mShaderInformation list.",
                      "MDyDataInformation::CreateShaderInformation().", shaderName);
    return DY_FAILURE;
  }

  // Create information space.
  auto [it, creationResult] = mShaderInformation.try_emplace(shaderName, nullptr);
  if (!creationResult) {
    // Something is already in or memory oob.
    MDY_LOG_CRITICAL("{} | Unexpected error happened during create memory for shader information {}.",
                     "MDyDataInformation::CreateShaderInformation().", shaderName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderInformation = std::make_unique<DDyShaderInformation>(shaderDescriptor);
  it->second.swap(shaderInformation);
  if (!it->second)
  {
    MDY_LOG_CRITICAL("{} | Unexpected error happened during swapping shader information {}.",
                     "MDyDataInformation::CreateShaderInformation().", shaderName);
    this->mShaderInformation.erase(shaderName);
    return DY_FAILURE;
  }

  MDY_LOG_CRITICAL("{} | \"{}\" shader information Created.", shaderName);
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateTextureInformation(const PDyTextureConstructionDescriptor& textureDescriptor)
{
  // Integrity test
  if (textureDescriptor.mTextureName.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorTextureNameNotSpecified, kDyDataInformation);
    return DY_FAILURE;
  }
  if (textureDescriptor.mIsEnabledAbsolutePath && textureDescriptor.mTextureFileAbsolutePath.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorTextureAbsolPathEmpty, kDyDataInformation);
    return DY_FAILURE;
  }
  else if (textureDescriptor.mTextureFileLocalPath.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorTextureLocalPathEmpty, kDyDataInformation);
    return DY_FAILURE;
  }
  if (textureDescriptor.mTextureType == EDyTextureStyleType::None)
  {
    MDY_LOG_CRITICAL_D(kErrorTextureTypeNone, kDyDataInformation);
    return DY_FAILURE;
  }

  // Find if duplicated texture name is exist in information list.
  const auto& textureName = textureDescriptor.mTextureName;
  if (mTextureInformation.find(textureName) != mTextureInformation.end())
  {
    MDY_LOG_WARNING_D("{}::{} | {} is already found in mTextureInformation list.", kDyDataInformation, "CreateTextureInformation", textureName);
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mTextureInformation.try_emplace(textureName, nullptr);
  if (!creationResult)
  {
    MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during create memory for texture information. Texture name : {}.",
                     kDyDataInformation, "CreateTextureInformation", textureName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto textureInformation = std::make_unique<DDyTextureInformation>(textureDescriptor);
  it->second.swap(textureInformation);
  if (!it->second)
  {
    MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during swapping texture information {}.",
                     kDyDataInformation, "CreateTextureInformation", textureName);
    this->mTextureInformation.erase(textureName);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateMaterialInformation(const PDyMaterialConstructionDescriptor& materialDescriptor)
{
  // Integrity test
  if (materialDescriptor.mMaterialName.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorMaterialNameNotSpecified, kDyDataInformation, "CreateMaterialInformation");
    return DY_FAILURE;
  }
  if (!materialDescriptor.mIsShaderLazyInitialized && materialDescriptor.mShaderName.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorBindingShaderNameEmpty, kDyDataInformation, "CreateMaterialInformation", materialDescriptor.mMaterialName);
    return DY_FAILURE;
  }

  // Check there is duplicated material name is exist in information list.
  const auto& materialName = materialDescriptor.mMaterialName;
  if (mMaterialInformation.find(materialName) != mMaterialInformation.end())
  {
    MDY_LOG_WARNING_D("{}::{} | {} is already found in mMaterialInformation list.", kDyDataInformation, "CreateMaterialInformation", materialName);
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mMaterialInformation.try_emplace(materialName, nullptr);
  if (!creationResult)
  {
    MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during create memory for material information. | Material name : {}.",
                     kDyDataInformation, "CreateMaterialInformation", materialName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto materialInformation = std::make_unique<DDyMaterialInformation>(materialDescriptor);
  it->second.swap(materialInformation);
  if (!it->second)
  {
    MDY_LOG_CRITICAL("{}::{} | Unexpected error happened during swapping texture information {}.",
                     kDyDataInformation, "CreateMaterialInformation", materialName);
    this->mMaterialInformation.erase(materialName);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateModelInformation(const PDyModelConstructionDescriptor& modelDescriptor)
{
  // Integrity test
  if (modelDescriptor.mModelName.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorModelNameEmpty, kDyDataInformation, "CreateModelInformation");
    throw std::runtime_error("Model name is not specified.");
  }
  const auto& modelName = modelDescriptor.mModelName;
  if (modelDescriptor.mModelPath.empty())
  {
    MDY_LOG_CRITICAL_D(kErrorModelPathEmpty, kDyDataInformation, "CreateModelInformation", modelName);
    throw std::runtime_error("Model path is not specified.");
  }

  decltype(this->mModelInformation)::iterator it;
  {
    std::lock_guard<std::mutex> mt(this->mTemporalMutex);
    if (mModelInformation.find(modelName) != mModelInformation.end())
    {
      MDY_LOG_WARNING_D("{} | Resource is already found. Name : {}", "MDyDataInformation", modelName);
      return DY_FAILURE;
    }

    // Check there is already in the information map, if not, make memory space to insert it.
    bool creationResult = false;
    std::tie(it, creationResult) = mModelInformation.try_emplace(modelName, nullptr);
    if (!creationResult)
    {
      MDY_LOG_CRITICAL_D("{} | Failed to create resource memory space. Name : {}", "MDyDataInformation", modelName);
      return DY_FAILURE;
    }
  }

  // Make resource in heap, and insert it to empty memory space.
  auto materialInformation = std::make_unique<DDyModelInformation>(modelDescriptor);
  if (it->second.swap(materialInformation); !it->second)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occured. Name : {}", "MDyDataInformation", modelName);
    this->mModelInformation.erase(modelName);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

std::optional<std::string> MDyDataInformation::PopulateMaterialInformation(
    const std::string& materialName,
    const PDyMaterialPopulateDescriptor& materialPopulateDescriptor)
{
  // Check if baseMaterial called materialName is already on information list.
  const auto* baseMaterial = GetMaterialInformation(materialName);
  if (baseMaterial == nullptr)
  {
    MDY_LOG_CRITICAL_D("{} | Failed to getting information of base material. base material name : {}", "MDyDataInformation::PopulateMaterialInformation", materialName);
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
      MDY_LOG_ERROR("{} | Empty material override name is prohibitted.", "MDyDataInformation::PopulateMaterialInformation");
      return std::nullopt;
    }
    if (GetMaterialInformation(actualMaterialPopDesc.mMaterialOverrideName))
    {
      MDY_LOG_ERROR("{} | Override name is already posed by any of material information.", "MDyDataInformation::PopulateMaterialInformation");
      return std::nullopt;
    };
  }

  // Check there is already in the information map.
  auto [infoIt, result] = this->mMaterialInformation.try_emplace(actualMaterialPopDesc.mMaterialOverrideName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occured.", "MDyDataInformation::PopulateMaterialInformation");
    return std::nullopt;
  }

  // Let baseMaterial have created populated derived material and move ownership to list.
  auto populateDerivedSmtPtr = baseMaterial->__pfPopulateWith(actualMaterialPopDesc);
  infoIt->second.swap(populateDerivedSmtPtr);
  if (!infoIt->second)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occured on swapping.", "MDyDataInformation::PopulateMaterialInformation");
    return std::nullopt;
  }

  MDY_LOG_INFO("{}::{} | Populated material from base material {}. | Populated material name : {}",
               kDyDataInformation, "PopulateMaterialInformation", materialName, actualMaterialPopDesc.mMaterialOverrideName);
  return actualMaterialPopDesc.mMaterialOverrideName;
}

//!
//! Delete functions
//!

EDySuccess MDyDataInformation::DeleteShaderInformation(const std::string& shaderName, bool isForced)
{
  const auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove shader information. Have not shader information. | {} : {}",
                    "MDyDataInformation", "DeleteShaderInformation", "Shader Name", shaderName);
    return DY_FAILURE;
  }

  // IF mShaderInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove shader information. Must remove resource first. | {} : {}",
                    "MDyDataInformation", "DeleteShaderInformation", "Shader lName", shaderName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mShaderInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteTextureInformation(const std::string& textureName, bool isForced)
{
  const auto iterator = mTextureInformation.find(textureName);
  if (iterator == mTextureInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove texture information. Have not texture information. | {} : {}",
                    "MDyDataInformation", "DeleteTextureInformation", "Texture Name", textureName);
    return DY_FAILURE;
  }

  // IF mTextureInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove material information. Must remove resource first. | {} : {}",
                    "MDyDataInformation", "DeleteMaterialInformation", "Material lName", textureName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mTextureInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteMaterialInformation(const std::string& materialName, bool isForced)
{
  const auto iterator = this->mMaterialInformation.find(materialName);
  if (iterator == this->mMaterialInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove material information. Have not material information. | {} : {}",
                    "MDyDataInformation", "DeleteMaterialInformation", "Material Name", materialName);
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove material information. Must remove resource first. | {} : {}",
                    "MDyDataInformation", "DeleteMaterialInformation", "Material lName", materialName);
    return DY_FAILURE;
  }

  // And remove material information!
  this->mMaterialInformation.erase(iterator);
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteModelInformation(const std::string& modelName, bool isAllRemoveSubresource, bool isForced)
{
  const auto iterator = mModelInformation.find(modelName);
  if (iterator == mModelInformation.end())
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove model information. Have not model information. | {} : {}",
                    "MDyDataInformation", "DeleteModelInformation", "ModelName", modelName);
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE, otherwise remove it.
  if (iterator->second->IsBeingBindedToResource() && !isForced)
  {
    MDY_LOG_ERROR_D("{}::{} | Failed to remove model information. Must remove resource first. | {} : {}",
                    "MDyDataInformation", "DeleteModelInformation", "ModelName", modelName);
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

//!
//! Get function.
//!

const DDyShaderInformation* MDyDataInformation::GetShaderInformation(const std::string& shaderName) const noexcept
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

const DDyTextureInformation* MDyDataInformation::GetTextureInformation(const std::string& textureName) const noexcept
{
  const auto iterator = mTextureInformation.find(textureName);
  if (iterator == mTextureInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find texture information. | Texture name : {}",
                    "MDyDataInformation::GetTextureInformation", textureName);
    return nullptr;
  }

  return iterator->second.get();
}

const DDyMaterialInformation* MDyDataInformation::GetMaterialInformation(const std::string& materialName) const noexcept
{
  const auto iterator = mMaterialInformation.find(materialName);
  if (iterator == mMaterialInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find material information. | Material name : {}",
                    "MDyDataInformation::GetMaterialInformation", materialName);
    return nullptr;
  }

  return iterator->second.get();
}

const DDyModelInformation* MDyDataInformation::GetModelInformation(const std::string& modelName) const noexcept
{
  std::lock_guard<std::mutex> mt(this->mTemporalMutex);

  const auto iterator = this->mModelInformation.find(modelName);
  if (iterator == this->mModelInformation.end())
  {
    MDY_LOG_WARNING("{} | Failed to find model information. | {} : {}",
                    "MDyDataInformation", "Model name", modelName);
    return nullptr;
  }

  return iterator->second.get();
}

} /// ::dy namespace