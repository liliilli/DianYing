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
#include <Dy/Management/IO/IOResourceManager.h>

#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

constexpr int32_t kTextureCountLimit = 16;

} /// ::unnamed namespace

namespace dy
{

//!
//! Create resource functions.
//!

EDySuccess MDyIOResource::CreateShaderResource(const std::string& shaderName)
{
  // Get information from MDyIOData manager.
  const auto& manInfo   = MDyIOData::GetInstance();
  const DDyShaderInformation* shaderInfo = manInfo.GetShaderInformation(shaderName);
  if (shaderInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find shader in information list. | Shader name : {}", "MDyIOResource", "CreateShaderResource", shaderName);
    return DY_FAILURE;
  }

  // Create memory space for new shader resource.
  auto [it, result] = this->mOnBoardShaderLists.try_emplace(shaderName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Shader name : {}", "MDyIOResource", "CreateShaderResource", shaderName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderResource = std::make_unique<CDyShaderResource>();
  if (const auto success = shaderResource->pfInitializeResource(*shaderInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create shader resource. | Shader resource name : {}", "MDyIOResource", "CreateShaderResource", shaderName);
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  it->second.swap(shaderResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Shader resource name : {}", "MDyIOResource", "CreateShaderResource", shaderName);
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  shaderInfo->__pfSetShaderResourceLink   (DyMakeNotNull(it->second.get()));
  it->second->__pfSetShaderInformationLink(DyMakeNotNull(const_cast<DDyShaderInformation*>(shaderInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource", "CreateShaderResource", "Shader", shaderName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::CreateTextureResource(const std::string& textureName)
{ // Get information from MDyIOData manager.
  const auto& manInfo   = MDyIOData::GetInstance();
  const DDyTextureInformation* textureInfo = manInfo.GetTextureInformation(textureName);
  if (textureInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find texture in information list. | Texture name : {}",
                  "MDyIOResource", "CreateTextureResource", textureName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardTextureLists.try_emplace(textureName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Texture name : {}",
                       "MDyIOResource", "CreateTextureResource", textureName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource> textureResource = std::make_unique<CDyTextureResource>();
  if (const auto success = textureResource->pfInitializeTextureResource(*textureInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create texture resource properly. | Texture resource name : {}",
                  "MDyIOResource", "CreateTextureResource", textureName);
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Texture resource name : {}",
                       "MDyIOResource", "CreateTextureResource", textureName);
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  textureInfo->__pfLinkTextureResource      (it->second.get());
  it->second->__pfSetTextureInformationLink (DyMakeNotNull(const_cast<DDyTextureInformation*>(textureInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource", "CreateTextureResource", "Texture", textureName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::CreateTextureResourceWithChunk(_MIN_ const PDyTextureConstructionBufferChunkDescriptor& desc)
{ // Get information from MDyIOData manager.
  auto [it, result] = this->mOnBoardTextureLists.try_emplace(desc.mTextureSpecifierName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Texture name : {}",
                       "MDyIOResource", "CreateTextureResource", desc.mTextureSpecifierName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource> textureResource = std::make_unique<CDyTextureResource>();
  if (const auto success = textureResource->pfInitializeTextureResourceWithChunk(desc);
      success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create texture resource properly. | Texture resource name : {}",
                  "MDyIOResource", "CreateTextureResource", desc.mTextureSpecifierName);
    this->mOnBoardTextureLists.erase(desc.mTextureSpecifierName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Texture resource name : {}",
                       "MDyIOResource", "CreateTextureResource", desc.mTextureSpecifierName);
    this->mOnBoardTextureLists.erase(desc.mTextureSpecifierName);
    return DY_FAILURE;
  }

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource", "CreateTextureResource", "Texture", desc.mTextureSpecifierName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::CreateMaterialResource(const std::string& materialName)
{
  // Get information from MDyIOData manager.
  const auto& manInfo = MDyIOData::GetInstance();
  const DDyMaterialInformation* materialInfo = manInfo.GetMaterialInformation(materialName);
  if (materialInfo == nullptr)
  {
    MDY_LOG_ERROR("{} | Failed to find material in information list. Material name : {}",
                  "MDyIOResource::CreateMaterialResource", materialName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardMaterialLists.try_emplace(materialName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occurred. Material name : {}",
                       "MDyIOResource::CreateMaterialResource", materialName);
    return DY_FAILURE;
  }

  // Specify shader is already created.
  // If not, create shader in advance but return error code when cannot.
  const auto& materialInformation = materialInfo->GetInformation();

  if (const auto* shaderResource = this->GetShaderResource(materialInformation.mShaderName);
      shaderResource == nullptr)
  {
    if (this->CreateShaderResource(materialInformation.mShaderName) == DY_FAILURE)
    {
      MDY_LOG_CRITICAL_D("{} | Could not create shader resource. | Shader resource name : {}",
                         "MDyIOResource::CreateMaterialResource", materialInformation.mShaderName);
      return DY_FAILURE;
    }
  }

  // Verify texture resource instance also.
  if (materialInformation.mTextureNames.size() > kTextureCountLimit)
  {
    MDY_LOG_CRITICAL_D("{} | Texture size must not be bigger than {}. | Material Name : {} | Material texture size : {}",
                       "MDyIOResource::CreateMaterialResource", kTextureCountLimit, materialName, materialInformation.mTextureNames.size());
    return DY_FAILURE;
  }

  for (const auto& textureName : materialInformation.mTextureNames)
  {
    const auto* textureResource = this->GetTextureResource(textureName);
    if (textureResource == nullptr)
    {
      const auto err = this->CreateTextureResource(textureName);
      if (err == DY_FAILURE)
      {
        MDY_LOG_CRITICAL_D("{} | Could not create texture resource. | Texture resource name : {}",
                           "MDyIOResource::CreateMaterialResource", textureName);
        return DY_FAILURE;
      }
    }
  }

  //!
  //! Make material resource descriptor.
  //!
  PDyMaterialResourceDescriptor mParamterDescriptor;
  {
    mParamterDescriptor.mMaterialName   = materialInformation.mMaterialName;
    mParamterDescriptor.mBlendMode      = materialInformation.mBlendMode;
    mParamterDescriptor.mShaderTuple    = DDyMaterialShaderTuple{materialInformation.mShaderName};

    for (const auto& textureName : materialInformation.mTextureNames)
    {
      mParamterDescriptor.mTextureTuples.emplace_back(textureName);
    }
  }

  // Create texture resource and insert to empty memory space.
  auto materialResource = std::make_unique<CDyMaterialResource>();
  if (const auto success = materialResource->pfInitializeMaterialResource(mParamterDescriptor);
      success == DY_FAILURE)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Failed to create material resource in heap. | Material resource name : {}",
                       "MDyIOResource", "CreateMaterialResource", materialName);
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  it->second.swap(materialResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Material resource name : {}",
                       "MDyIOResource", "CreateMaterialResource", materialName);
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  materialInfo->__pfSetMaterialResourceLink(DyMakeNotNull(it->second.get()));
  it->second  ->__pfSetMaterialInformationLink(DyMakeNotNull(const_cast<DDyMaterialInformation*>(materialInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource", "CreateMaterialResource", "Material", materialName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::CreateSoundResource(const std::string& soundName)
{
  // Get information from MDyIOData manager.
  const DDySoundInformation* soundInformation = MDyIOData::GetInstance().GetSoundInformation(soundName);
  if (soundInformation == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find sound in information list. | Sound name : {}", "MDyIOResource", "CreateSoundResource", soundName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardSoundLists.try_emplace(soundName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Sound name : {}", "MDyIOResource", "CreateSoundResource", soundName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  auto soundResource = std::make_unique<CDySoundResource>();
  if (const auto success = soundResource->pfInitializeResource(*soundInformation); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create sound resource properly. | Sound resource name : {}", "MDyIOResource", "CreateSoundResource", soundName);

    this->mOnBoardTextureLists.erase(soundName);
    return DY_FAILURE;
  }

  it->second.swap(soundResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Sound resource name : {}", "MDyIOResource", "CreateSoundResource", soundName);

    this->mOnBoardTextureLists.erase(soundName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  soundInformation->__pfSetSoundResourceLink  (DyMakeNotNull(it->second.get()));
  it->second->__pfSetSoundInformationLink     (DyMakeNotNull(const_cast<DDySoundInformation*>(soundInformation)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}", "MDyIOResource", "CreateSoundResource", "Sound", soundName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::CreateModelResource(const std::string& modelName)
{
  // Get information from MDyIOData manager.
  const auto& manInfo   = MDyIOData::GetInstance();
  const auto* modelInfo = manInfo.GetModelInformation(modelName);
  if (modelInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find model in information list. | Model name : {}",
                  "MDyIOResource", "CreateModelResource", modelName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardModelLists.try_emplace(modelName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in creating memory space. | Model name : {}",
                       "MDyIOResource", "CreateModelResource", modelName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  auto modelResource = std::make_unique<CDyModelResource>();
  if (const auto success = modelResource->pInitializeModelResource(*modelInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create model resource properly. | Model resource name : {}",
                  "MDyIOResource", "CreateModelResource", modelName);
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  it->second.swap(modelResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Model resource name : {}",
                       "MDyIOResource", "CreateModelResource", modelName);
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  modelInfo ->__pfSetModelResourceLink(DyMakeNotNull(it->second.get()));
  it->second->__pfSetModelInformationLink(DyMakeNotNull(const_cast<DDyModelInformation*>(modelInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource", "CreateModelResource", "Model", modelName);
  return DY_SUCCESS;
}

//!
//! Get resource functions
//!

CDyShaderResource* MDyIOResource::GetShaderResource(const std::string& shaderName)
{
  const auto it = this->mOnBoardShaderLists.find(shaderName);
  if (it == this->mOnBoardShaderLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource", "GetShaderResource", "Shader", shaderName);
    return nullptr;
  }

  return it->second.get();
}

CDyTextureResource* MDyIOResource::GetTextureResource(const std::string& textureName)
{
  const auto it = this->mOnBoardTextureLists.find(textureName);
  if (it == this->mOnBoardTextureLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource", "GetTextureResource", "Texture", textureName);
    return nullptr;
  }

  return it->second.get();
}

CDyMaterialResource* MDyIOResource::GetMaterialResource(const std::string& materialName)
{
  const auto it = this->mOnBoardMaterialLists.find(materialName);
  if (it == this->mOnBoardMaterialLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource", "GetMaterialResource", "Material", materialName);
    return nullptr;
  }

  return it->second.get();
}

CDyModelResource* MDyIOResource::GetModelResource(const std::string& modelName)
{
  const auto it = this->mOnBoardModelLists.find(modelName);
  if (it == this->mOnBoardModelLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource", "GetModelResource", "Model", modelName);
    return nullptr;
  }

  return it->second.get();
}

CDySoundResource* MDyIOResource::GetSoundResource(const std::string& soundName)
{
  const auto it = this->mOnBoardSoundLists.find(soundName);
  if (it == this->mOnBoardSoundLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}", "MDyIOResource", "GetSoundResource", "Sound", soundName);
    return nullptr;
  }

  return it->second.get();
}

EDySuccess MDyIOResource::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyIOResource::pfRelease()
{
  this->mOnBoardShaderLists.clear();
  this->mOnBoardTextureLists.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace