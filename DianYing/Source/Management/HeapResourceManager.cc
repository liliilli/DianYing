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

namespace dy
{

//!
//! Create resource functions.
//!

EDySuccess MDyResource::CreateShaderResource(const std::string& shaderName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo   = MDyDataInformation::GetInstance();
  const CDyShaderInformation* shaderInfo = manInfo.GetShaderInformation(shaderName);
  if (!shaderInfo)
  {
    return DY_FAILURE;
  }

  // Create memory space for new shader resource.
  auto [it, result] = this->mOnBoardShaderLists.try_emplace(shaderName, nullptr);
  if (!result)
  {
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderResource = std::make_unique<CDyShaderResource>();
  if (const auto success = shaderResource->pfInitializeResource(*shaderInfo); success == DY_FAILURE)
  {
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  it->second.swap(shaderResource);
  if (!it->second)
  {
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  shaderInfo->__pfSetNextLevel(it->second.get());
  it->second->__pfSetPrevLevel(const_cast<CDyShaderInformation*>(shaderInfo));
  return DY_SUCCESS;
}

EDySuccess MDyResource::CreateTextureResource(const std::string& textureName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo   = MDyDataInformation::GetInstance();
  const CDyTextureInformation* textureInfo = manInfo.GetTextureInformation(textureName);
  if (!textureInfo)
  {
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardTextureLists.try_emplace(textureName, nullptr);
  if (!result)
  {
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  std::unique_ptr<CDyTextureResource> textureResource = std::make_unique<CDyTextureResource>();
  if (const auto success = textureResource->pfInitializeResource(*textureInfo); success == DY_FAILURE)
  {
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    this->mOnBoardTextureLists.erase(textureName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  textureInfo->__pfSetNextLevel(it->second.get());
  it->second->__pfSetPrevLevel(const_cast<CDyTextureInformation*>(textureInfo));
  return DY_SUCCESS;
}

EDySuccess MDyResource::CreateMaterialResource(const std::string& materialName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo = MDyDataInformation::GetInstance();
  const DDyMaterialInformation* materialInfo = manInfo.GetMaterialInformation(materialName);
  if (!materialInfo)
  {
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardMaterialLists.try_emplace(materialName, nullptr);
  if (!result)
  {
    /*
     * std::cout << "Failure : " << "Unexpected error";
     */
    return DY_FAILURE;
  }

  // Specify shader is already created.
  // If not, create shader in advance but return error code when cannot.
  const auto& information = materialInfo->GetInformation();
  const auto* shaderResource = this->GetShaderResource(information.mShaderName);
  if (!shaderResource)
  {
    if (this->CreateShaderResource(information.mShaderName) == DY_FAILURE)
    {
      /*
       * std::cout << "Failure : " << "Could not create shader which name is " << information.mShaderName;
       */
      return DY_FAILURE;
    }
  }

  // Verify texture resource instance also.
  if (information.mTextureName.size() > 16)
  {
    /*
     * std::cout << "Failure : " << "Texture size must not be bigger than 16.";
     */
    return DY_FAILURE;
  }

  for (const auto& textureName : information.mTextureName)
  {
    const auto* textureResource = this->GetTextureResource(textureName);
    if (!textureResource)
    {
      if (this->CreateTextureResource(textureName) == DY_FAILURE)
      {
        /*
         * std::cout << "Failure : " << "Could not create shader which name is " << textureName;
         */
        return DY_FAILURE;
      }
    }
  }

  // Make material resource descriptor.
  PDyMaterialResourceDescriptor mParamterDescriptor;
  {
    mParamterDescriptor.mMaterialName   = information.mMaterialName;
    mParamterDescriptor.mBlendMode      = information.mBlendMode;
    mParamterDescriptor.mShaderTuple    = DDyMaterialShaderTuple{
        information.mShaderName,
        this->GetShaderResource(information.mShaderName)
    };
    for (const auto& textureName : information.mTextureName)
    {
      mParamterDescriptor.mTextureTuples.emplace_back(
        decltype(mParamterDescriptor.mTextureTuples)::value_type\
        {textureName, this->GetTextureResource(textureName)}
      );
    }
  }

  // Create texture resource and insert to empty memory space.
  auto materialResource = std::make_unique<CDyMaterialResource>();
  if (const auto success = materialResource->pInitializeMaterial(mParamterDescriptor);
      success == DY_FAILURE)
  {
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  it->second.swap(materialResource);
  if (!it->second)
  {
    this->mOnBoardMaterialLists.erase(materialName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  materialInfo->__pfSetNextLevel(it->second.get());
  it->second  ->__pfSetPrevLevel(const_cast<DDyMaterialInformation*>(materialInfo));
  return DY_SUCCESS;
}

EDySuccess MDyResource::CreateModelResource(const std::string& modelName)
{
  // Get information from MDyDataInformation manager.
  const auto& manInfo   = MDyDataInformation::GetInstance();
  const auto* modelInfo = manInfo.GetModelInformation(modelName);
  if (!modelInfo)
  {
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardModelLists.try_emplace(modelName, nullptr);
  if (!result)
  {
    /*
     * std::cout << "Failure : " << "Unexpected error";
     */
    return DY_FAILURE;
  }

  // Create texture resource and insert to empty memory space.
  auto modelResource = std::make_unique<CDyModelResource>();
  if (const auto success = modelResource->pInitializeModel(*modelInfo); success == DY_FAILURE)
  {
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  it->second.swap(modelResource);
  if (!it->second)
  {
    this->mOnBoardModelLists.erase(modelName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  modelInfo ->__pfSetNextLevel(it->second.get());
  it->second->__pfSetPrevLevel(const_cast<DDyModelInformation*>(modelInfo));
  return DY_SUCCESS;
}

//!
//! Get resource functions
//!

CDyShaderResource* MDyResource::GetShaderResource(const std::string& shaderName)
{
  const auto it = this->mOnBoardShaderLists.find(shaderName);
  if (it == this->mOnBoardShaderLists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

CDyTextureResource* MDyResource::GetTextureResource(const std::string& textureName)
{
  const auto it = this->mOnBoardTextureLists.find(textureName);
  if (it == this->mOnBoardTextureLists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

CDyMaterialResource* MDyResource::GetMaterialResource(const std::string& materialName)
{
  const auto it = this->mOnBoardMaterialLists.find(materialName);
  if (it == this->mOnBoardMaterialLists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

EDySuccess MDyResource::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyResource::pfRelease()
{
  this->mOnBoardShaderLists.clear();
  this->mOnBoardTextureLists.clear();
  return DY_SUCCESS;
}

} /// ::dy namespace