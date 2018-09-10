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
#include <Dy/Management/HeapResourceManager.h>

#include <Dy/Management/DataInformationManager.h>
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

EDySuccess MDyHeapResource::CreateShaderResource(const std::string& shaderName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo   = MDyDataInformation::GetInstance();
  const DDyShaderInformation* shaderInfo = manInfo.GetShaderInformation(shaderName);
  if (shaderInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find shader in information list. | Shader name : {}",
                  "MDyHeapResource", "CreateShaderResource", shaderName);
    return DY_FAILURE;
  }

  // Create memory space for new shader resource.
  auto [it, result] = this->mOnBoardShaderLists.try_emplace(shaderName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Shader name : {}",
                       "MDyHeapResource", "CreateShaderResource", shaderName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderResource = std::make_unique<CDyShaderResource>();
  if (const auto success = shaderResource->pfInitializeResource(*shaderInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create shader resource. | Shader resource name : {}",
                  "MDyHeapResource", "CreateShaderResource", shaderName);
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  it->second.swap(shaderResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Shader resource name : {}",
                       "MDyHeapResource", "CreateShaderResource", shaderName);
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  shaderInfo->__pfLinkShaderResourcePtr(it->second.get());
  it->second->__pfLinkShaderInformationPtr(const_cast<DDyShaderInformation*>(shaderInfo));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyHeapResource", "CreateShaderResource", "Shader", shaderName);
  return DY_SUCCESS;
}

EDySuccess MDyHeapResource::CreateTextureResource(const std::string& textureName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo   = MDyDataInformation::GetInstance();
  const DDyTextureInformation* textureInfo = manInfo.GetTextureInformation(textureName);
  if (textureInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find texture in information list. | Texture name : {}",
                  "MDyHeapResource", "CreateTextureResource", textureName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardTextureLists.try_emplace(textureName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred. | Texture name : {}",
                       "MDyHeapResource", "CreateTextureResource", textureName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource> textureResource = std::make_unique<CDyTextureResource>();
  if (const auto success = textureResource->pfInitializeTextureResource(*textureInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create texture resource properly. | Texture resource name : {}",
                  "MDyHeapResource", "CreateTextureResource", textureName);
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Texture resource name : {}",
                       "MDyHeapResource", "CreateTextureResource", textureName);
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  textureInfo->__pfLinkTextureResource(it->second.get());
  it->second->__pfLinkTextureInformationPtr(const_cast<DDyTextureInformation*>(textureInfo));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyHeapResource", "CreateTextureResource", "Texture", textureName);
  return DY_SUCCESS;
}

EDySuccess MDyHeapResource::CreateMaterialResource(const std::string& materialName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo = MDyDataInformation::GetInstance();
  const DDyMaterialInformation* materialInfo = manInfo.GetMaterialInformation(materialName);
  if (materialInfo == nullptr)
  {
    MDY_LOG_ERROR("{} | Failed to find material in information list. Material name : {}",
                  "MDyHeapResource::CreateMaterialResource", materialName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardMaterialLists.try_emplace(materialName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{} | Unexpected error occurred. Material name : {}",
                       "MDyHeapResource::CreateMaterialResource", materialName);
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
                         "MDyHeapResource::CreateMaterialResource", materialInformation.mShaderName);
      return DY_FAILURE;
    }
  }

  // Verify texture resource instance also.
  if (materialInformation.mTextureNames.size() > kTextureCountLimit)
  {
    MDY_LOG_CRITICAL_D("{} | Texture size must not be bigger than {}. | Material Name : {} | Material texture size : {}",
                       "MDyHeapResource::CreateMaterialResource", kTextureCountLimit, materialName, materialInformation.mTextureNames.size());
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
                           "MDyHeapResource::CreateMaterialResource", textureName);
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
    mParamterDescriptor.mShaderTuple    = DDyMaterialShaderTuple{
        materialInformation.mShaderName,
        this->GetShaderResource(materialInformation.mShaderName)
    };
    for (const auto& textureName : materialInformation.mTextureNames)
    {
      mParamterDescriptor.mTextureTuples.emplace_back(
        decltype(mParamterDescriptor.mTextureTuples)::value_type\
        {textureName, this->GetTextureResource(textureName)}
      );
    }
  }

  // Create texture resource and insert to empty memory space.
  auto materialResource = std::make_unique<CDyMaterialResource>();
  if (const auto success = materialResource->pfInitializeMaterialResource(mParamterDescriptor);
      success == DY_FAILURE)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Failed to create material resource in heap. | Material resource name : {}",
                       "MDyHeapResource", "CreateMaterialResource", materialName);
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  it->second.swap(materialResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Material resource name : {}",
                       "MDyHeapResource", "CreateMaterialResource", materialName);
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  materialInfo->__pfSetMaterialResourceLink(it->second.get());
  it->second  ->__pfLinkMaterialInformation(const_cast<DDyMaterialInformation*>(materialInfo));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyHeapResource", "CreateMaterialResource", "Material", materialName);
  return DY_SUCCESS;
}

EDySuccess MDyHeapResource::CreateModelResource(const std::string& modelName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo   = MDyDataInformation::GetInstance();
  const auto* modelInfo = manInfo.GetModelInformation(modelName);
  if (modelInfo == nullptr)
  {
    MDY_LOG_ERROR("{}::{} | Failed to find model in information list. | Model name : {}",
                  "MDyHeapResource", "CreateModelResource", modelName);
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardModelLists.try_emplace(modelName, nullptr);
  if (!result)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in creating memory space. | Model name : {}",
                       "MDyHeapResource", "CreateModelResource", modelName);
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  auto modelResource = std::make_unique<CDyModelResource>();
  if (const auto success = modelResource->pInitializeModelResource(*modelInfo); success == DY_FAILURE)
  {
    MDY_LOG_ERROR("{}::{} | Cannot create model resource properly. | Model resource name : {}",
                  "MDyHeapResource", "CreateModelResource", modelName);
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  it->second.swap(modelResource);
  if (!it->second)
  {
    MDY_LOG_CRITICAL_D("{}::{} | Unexpected error occurred in swapping. | Model resource name : {}",
                       "MDyHeapResource", "CreateModelResource", modelName);
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  modelInfo ->__pfSetModelResourceLink(it->second.get());
  it->second->__pfLinkModelInformationPtr(const_cast<DDyModelInformation*>(modelInfo));

  MDY_LOG_INFO("{0}::{1} | Create {2} resource. | {2} resource name : {3}",
               "MDyHeapResource", "CreateModelResource", "Model", modelName);
  return DY_SUCCESS;
}

//!
//! Get resource functions
//!

CDyShaderResource* MDyHeapResource::GetShaderResource(const std::string& shaderName)
{
  const auto it = this->mOnBoardShaderLists.find(shaderName);
  if (it == this->mOnBoardShaderLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyHeapResource", "GetShaderResource", "Shader", shaderName);
    return nullptr;
  }

  return it->second.get();
}

CDyTextureResource* MDyHeapResource::GetTextureResource(const std::string& textureName)
{
  const auto it = this->mOnBoardTextureLists.find(textureName);
  if (it == this->mOnBoardTextureLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyHeapResource", "GetTextureResource", "Texture", textureName);
    return nullptr;
  }

  return it->second.get();
}

CDyMaterialResource* MDyHeapResource::GetMaterialResource(const std::string& materialName)
{
  const auto it = this->mOnBoardMaterialLists.find(materialName);
  if (it == this->mOnBoardMaterialLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyHeapResource", "GetMaterialResource", "Material", materialName);
    return nullptr;
  }

  return it->second.get();
}

CDyModelResource* MDyHeapResource::GetModelResource(const std::string& modelName)
{
  const auto it = this->mOnBoardModelLists.find(modelName);
  if (it == this->mOnBoardModelLists.end())
  {
    MDY_LOG_WARNING("{0}::{1} | Failed to find {2} resource. | {2} resource name : {3}",
                    "MDyHeapResource", "GetModelResource", "Model", modelName);
    return nullptr;
  }

  return it->second.get();
}

EDySuccess MDyHeapResource::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyHeapResource::pfRelease()
{
  this->mOnBoardShaderLists.clear();
  this->mOnBoardTextureLists.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace