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
#include <Dy/Management/IO/IOResourceManager_Deprecated.h>

#include <Dy/Management/IO/IODataManager_Deprecated.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Meta/Type/EDyResourceType.h>

namespace
{

constexpr int32_t kTextureCountLimit = 16;

} /// ::unnamed namespace

namespace dy
{

//!
//! Create resource functions.
//!

void MDyIOResource_Deprecated::InsertResult(_MIN_ EDyResourceType type,_MIN_ void* rawInstance) noexcept
{
  switch (type)
  {
  case EDyResourceType::GLShader:
  { // TEMPORAL
    MDY_SYNC_LOCK_GUARD(this->mTemporalIOInsertDeleteGetMutex);
    auto ptr = std::unique_ptr<CDyShaderResource_Deprecated>(static_cast<CDyShaderResource_Deprecated*>(rawInstance));
    this->mOnBoardShaderLists.try_emplace(ptr->mShaderName, std::move(ptr));
  } break;
  case EDyResourceType::Texture:
  { // TEMPORAL
    MDY_SYNC_LOCK_GUARD(this->mTemporalIOInsertDeleteGetMutex);
    auto ptr = std::unique_ptr<CDyTextureResource_Deprecated>(static_cast<CDyTextureResource_Deprecated*>(rawInstance));
    this->mOnBoardTextureLists.try_emplace(ptr->mTextureName, std::move(ptr));
  } break;
  case EDyResourceType::Model:
  case EDyResourceType::Material:
    MDY_NOT_IMPLEMENTED_ASSERT();
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

EDySuccess MDyIOResource_Deprecated::CreateShaderResource_Deprecated(const std::string& shaderName)
{
  // Get information from MDyIOData_Deprecated manager.
  const auto& manInfo   = MDyIOData_Deprecated::GetInstance();
  const DDyShaderInformation_Deprecated* shaderInfo = manInfo.GetShaderInformation(shaderName);
  if (shaderInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find shader in information list. | Shader name : {}", "MDyIOResource_Deprecated", "CreateShaderResource_Deprecated", shaderName);
    return DY_FAILURE;
  }

  // Create memory space for new shader resource.
  auto [it, result] = this->mOnBoardShaderLists.try_emplace(shaderName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Shader name : {}", "MDyIOResource_Deprecated", "CreateShaderResource_Deprecated", shaderName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderResource = std::make_unique<CDyShaderResource_Deprecated>();
  if (const auto success = shaderResource->pfInitializeResource(*shaderInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create shader resource. | Shader resource name : {}", "MDyIOResource_Deprecated", "CreateShaderResource_Deprecated", shaderName);
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  it->second.swap(shaderResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Shader resource name : {}", "MDyIOResource_Deprecated", "CreateShaderResource_Deprecated", shaderName);
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  shaderInfo->__pfSetShaderResourceLink   (DyMakeNotNull(it->second.get()));
  it->second->__pfSetShaderInformationLink(DyMakeNotNull(const_cast<DDyShaderInformation_Deprecated*>(shaderInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource_Deprecated", "CreateShaderResource_Deprecated", "Shader", shaderName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::CreateTextureResource_Deprecated(const std::string& textureName)
{ // Get information from MDyIOData_Deprecated manager.
  const auto& manInfo   = MDyIOData_Deprecated::GetInstance();
  const DDyTextureInformation_Deprecated* textureInfo = manInfo.GetTextureInformation(textureName);
  if (textureInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find texture in information list. | Texture name : {}",
                  "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", textureName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardTextureLists.try_emplace(textureName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Texture name : {}",
                       "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", textureName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource_Deprecated> textureResource = std::make_unique<CDyTextureResource_Deprecated>();
  if (const auto success = textureResource->pfInitializeTextureResource(*textureInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create texture resource properly. | Texture resource name : {}",
                  "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", textureName);
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Texture resource name : {}",
                       "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", textureName);
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  textureInfo->__pfLinkTextureResource      (it->second.get());
  it->second->__pfSetTextureInformationLink (DyMakeNotNull(const_cast<DDyTextureInformation_Deprecated*>(textureInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", "Texture", textureName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::CreateTextureResourceWithChunk_Deprecated(_MIN_ const PDyTextureConstructionBufferChunkDescriptor& desc)
{ // Get information from MDyIOData_Deprecated manager.
  auto [it, result] = this->mOnBoardTextureLists.try_emplace(desc.mTextureSpecifierName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Texture name : {}",
                       "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", desc.mTextureSpecifierName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource_Deprecated> textureResource = std::make_unique<CDyTextureResource_Deprecated>();
  if (const auto success = textureResource->pfInitializeTextureResourceWithChunk(desc);
      success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create texture resource properly. | Texture resource name : {}",
                  "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", desc.mTextureSpecifierName);
    this->mOnBoardTextureLists.erase(desc.mTextureSpecifierName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Texture resource name : {}",
                       "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", desc.mTextureSpecifierName);
    this->mOnBoardTextureLists.erase(desc.mTextureSpecifierName);
    return DY_FAILURE;
  }

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", "Texture", desc.mTextureSpecifierName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::CreateMaterialResource_Deprecated(const std::string& materialName)
{
  // Get information from MDyIOData_Deprecated manager.
  const auto& manInfo = MDyIOData_Deprecated::GetInstance();
  const DDyMaterialInformation_Deprecated* materialInfo = manInfo.GetMaterialInformation(materialName);
  if (materialInfo == nullptr)
  {
    MDY_LOG_ERROR("{} | Failed to find material in information list. Material name : {}",
                  "MDyIOResource_Deprecated::CreateMaterialResource_Deprecated", materialName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardMaterialLists.try_emplace(materialName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occurred. Material name : {}",
                       "MDyIOResource_Deprecated::CreateMaterialResource_Deprecated", materialName);
    return DY_FAILURE;
  }

  // Specify shader is already created.
  // If not, create shader in advance but return error code when cannot.
  const auto& materialInformation = materialInfo->GetInformation();

  if (const auto* shaderResource = this->GetShaderResource(materialInformation.mShaderSpecifier);
      shaderResource == nullptr)
  {
    if (this->CreateShaderResource_Deprecated(materialInformation.mShaderSpecifier) == DY_FAILURE)
    {
      MDY_LOG_CRITICAL_D("{} | Could not create shader resource. | Shader resource name : {}",
                         "MDyIOResource_Deprecated::CreateMaterialResource_Deprecated", materialInformation.mShaderSpecifier);
      return DY_FAILURE;
    }
  }

  // Verify texture resource instance also.
  if (materialInformation.mTextureNames_Deprecated.size() > kTextureCountLimit)
  {
    MDY_LOG_CRITICAL_D("{} | Texture size must not be bigger than {}. | Material Name : {} | Material texture size : {}",
                       "MDyIOResource_Deprecated::CreateMaterialResource_Deprecated", kTextureCountLimit, materialName, materialInformation.mTextureNames_Deprecated.size());
    return DY_FAILURE;
  }

  for (const auto& textureName : materialInformation.mTextureNames_Deprecated)
  {
    const auto* textureResource = this->GetTextureResource(textureName);
    if (textureResource == nullptr)
    {
      const auto err = this->CreateTextureResource_Deprecated(textureName);
      if (err == DY_FAILURE)
      {
        MDY_LOG_CRITICAL_D("{} | Could not create texture resource. | Texture resource name : {}",
                           "MDyIOResource_Deprecated::CreateMaterialResource_Deprecated", textureName);
        return DY_FAILURE;
      }
    }
  }

  //!
  //! Make material resource descriptor.
  //!
  PDyMaterialResourceDescriptor mParamterDescriptor;
  {
    mParamterDescriptor.mMaterialName   = materialInformation.mSpecifierName;
    mParamterDescriptor.mBlendMode      = materialInformation.mBlendMode;
    mParamterDescriptor.mShaderTuple    = DDyMaterialShaderTuple{materialInformation.mShaderSpecifier};

    for (const auto& textureName : materialInformation.mTextureNames_Deprecated)
    {
      mParamterDescriptor.mTextureTuples.emplace_back(textureName);
    }
  }

  // Create texture resource and insert to empty memory space.
  auto materialResource = std::make_unique<CDyMaterialResource_Deprecated>();
  if (const auto success = materialResource->pfInitializeMaterialResource(mParamterDescriptor);
      success == DY_FAILURE)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Failed to create material resource in heap. | Material resource name : {}",
                       "MDyIOResource_Deprecated", "CreateMaterialResource_Deprecated", materialName);
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  it->second.swap(materialResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Material resource name : {}",
                       "MDyIOResource_Deprecated", "CreateMaterialResource_Deprecated", materialName);
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  materialInfo->__pfSetMaterialResourceLink(DyMakeNotNull(it->second.get()));
  it->second  ->__pfSetMaterialInformationLink(DyMakeNotNull(const_cast<DDyMaterialInformation_Deprecated*>(materialInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource_Deprecated", "CreateMaterialResource_Deprecated", "Material", materialName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::CreateSoundResource_Deprecated(const std::string& soundName)
{
  // Get information from MDyIOData_Deprecated manager.
  const DDySoundInformation_Deprecated* soundInformation = MDyIOData_Deprecated::GetInstance().GetSoundInformation(soundName);
  if (soundInformation == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find sound in information list. | Sound name : {}", "MDyIOResource_Deprecated", "CreateSoundResource_Deprecated", soundName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardSoundLists.try_emplace(soundName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Sound name : {}", "MDyIOResource_Deprecated", "CreateSoundResource_Deprecated", soundName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  auto soundResource = std::make_unique<CDySoundResource_Deprecated>();
  if (const auto success = soundResource->pfInitializeResource(*soundInformation); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create sound resource properly. | Sound resource name : {}", "MDyIOResource_Deprecated", "CreateSoundResource_Deprecated", soundName);

    this->mOnBoardTextureLists.erase(soundName);
    return DY_FAILURE;
  }

  it->second.swap(soundResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Sound resource name : {}", "MDyIOResource_Deprecated", "CreateSoundResource_Deprecated", soundName);

    this->mOnBoardTextureLists.erase(soundName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  soundInformation->__pfSetSoundResourceLink  (DyMakeNotNull(it->second.get()));
  it->second->__pfSetSoundInformationLink     (DyMakeNotNull(const_cast<DDySoundInformation_Deprecated*>(soundInformation)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}", "MDyIOResource_Deprecated", "CreateSoundResource_Deprecated", "Sound", soundName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::CreateModelResource_Deprecated(const std::string& modelName)
{
  // Get information from MDyIOData_Deprecated manager.
  const auto& manInfo   = MDyIOData_Deprecated::GetInstance();
  const auto* modelInfo = manInfo.GetModelInformation(modelName);
  if (modelInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find model in information list. | Model name : {}",
                  "MDyIOResource_Deprecated", "CreateModelResource_Deprecated", modelName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardModelLists.try_emplace(modelName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in creating memory space. | Model name : {}",
                       "MDyIOResource_Deprecated", "CreateModelResource_Deprecated", modelName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  auto modelResource = std::make_unique<CDyModelResource_Deprecated>();
  if (const auto success = modelResource->pInitializeModelResource(*modelInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create model resource properly. | Model resource name : {}",
                  "MDyIOResource_Deprecated", "CreateModelResource_Deprecated", modelName);
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  it->second.swap(modelResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Model resource name : {}",
                       "MDyIOResource_Deprecated", "CreateModelResource_Deprecated", modelName);
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  modelInfo ->__pfSetModelResourceLink(DyMakeNotNull(it->second.get()));
  it->second->__pfSetModelInformationLink(DyMakeNotNull(const_cast<DDyModelInformation_Deprecated*>(modelInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyIOResource_Deprecated", "CreateModelResource_Deprecated", "Model", modelName);
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::CreateTextureResource_Deprecated(_MIN_ const std::string& specifierName, _MIN_ MDY_NOTUSED const EDyScope scope)
{
  const DDyTextureInformation_Deprecated* textureInfo = MDyIOData_Deprecated::GetInstance().GetTextureInformation(specifierName);
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(textureInfo), "Texture information must be valid before resource population.");

  auto [it, isCreated] = this->mOnBoardTextureLists.try_emplace(specifierName, nullptr);
  MDY_ASSERT(isCreated == true, "Unexpected error occurred.");

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource_Deprecated> textureResource = std::make_unique<CDyTextureResource_Deprecated>();
  MDY_CALL_ASSERT_SUCCESS(textureResource->pfInitializeTextureResource(*textureInfo));

  it->second.swap(textureResource);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(it->second), "Unexpected error occurred.");

  // At last, setting pointers to each other.
  textureInfo->__pfLinkTextureResource(it->second.get());
  it->second->__pfSetTextureInformationLink(DyMakeNotNull(const_cast<DDyTextureInformation_Deprecated*>(textureInfo)));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}", "MDyIOResource_Deprecated", "CreateTextureResource_Deprecated", "Texture", specifierName);
  return DY_SUCCESS;
}

//!
//! Get resource functions
//!

CDyShaderResource_Deprecated* MDyIOResource_Deprecated::GetShaderResource(const std::string& shaderName)
{
  const auto it = this->mOnBoardShaderLists.find(shaderName);
  if (it == this->mOnBoardShaderLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource_Deprecated", "GetShaderResource", "Shader", shaderName);
    return nullptr;
  }

  return it->second.get();
}

CDyTextureResource_Deprecated* MDyIOResource_Deprecated::GetTextureResource(const std::string& textureName)
{
  const auto it = this->mOnBoardTextureLists.find(textureName);
  if (it == this->mOnBoardTextureLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource_Deprecated", "GetTextureResource", "Texture", textureName);
    return nullptr;
  }

  return it->second.get();
}

CDyMaterialResource_Deprecated* MDyIOResource_Deprecated::GetMaterialResource(const std::string& materialName)
{
  const auto it = this->mOnBoardMaterialLists.find(materialName);
  if (it == this->mOnBoardMaterialLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource_Deprecated", "GetMaterialResource", "Material", materialName);
    return nullptr;
  }

  return it->second.get();
}

CDyModelResource_Deprecated* MDyIOResource_Deprecated::GetModelResource(const std::string& modelName)
{
  const auto it = this->mOnBoardModelLists.find(modelName);
  if (it == this->mOnBoardModelLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyIOResource_Deprecated", "GetModelResource", "Model", modelName);
    return nullptr;
  }

  return it->second.get();
}

CDySoundResource_Deprecated* MDyIOResource_Deprecated::GetSoundResource(const std::string& soundName)
{
  const auto it = this->mOnBoardSoundLists.find(soundName);
  if (it == this->mOnBoardSoundLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}", "MDyIOResource_Deprecated", "GetSoundResource", "Sound", soundName);
    return nullptr;
  }

  return it->second.get();
}

EDySuccess MDyIOResource_Deprecated::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyIOResource_Deprecated::pfRelease()
{
  this->mOnBoardShaderLists.clear();
  this->mOnBoardTextureLists.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace