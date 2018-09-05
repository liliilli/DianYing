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
  const auto& shaderName = shaderDescriptor.mShaderName;
  if (mShaderInformation.find(shaderName) != mShaderInformation.end())
  {
    MDY_LOG_WARNING_D("{} | {} is already found in mShaderInformation list.",
                      "MDyDataInformation::CreateShaderInformation().", shaderName);
    return DY_FAILURE;
  }

  auto [it, creationResult] = mShaderInformation.try_emplace(shaderName, nullptr);
  if (!creationResult) {
    // Something is already in or memory oob.
    MDY_LOG_CRITICAL("{} | Unexpected error happened during create memory for shader information {}.",
                     "MDyDataInformation::CreateShaderInformation().", shaderName);
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderInformation = std::make_unique<CDyShaderInformation>(shaderDescriptor);
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
  const auto& textureName = textureDescriptor.mTextureName;
  if (mTextureInformation.find(textureName) != mTextureInformation.end())
  {
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mTextureInformation.try_emplace(textureName, nullptr);
  if (!creationResult) {

    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto textureInformation = std::make_unique<CDyTextureInformation>(textureDescriptor);
  it->second.swap(textureInformation);
  if (!it->second)
  {
    this->mTextureInformation.erase(textureName);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateMaterialInformation(const PDyMaterialConstructionDescriptor& materialDescriptor)
{
  const auto& materialName = materialDescriptor.mMaterialName;
  if (mMaterialInformation.find(materialName) != mMaterialInformation.end())
  {
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mMaterialInformation.try_emplace(materialName, nullptr);
  if (!creationResult) {

    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto materialInformation = std::make_unique<DDyMaterialInformation>(materialDescriptor);
  it->second.swap(materialInformation);
  if (!it->second)
  {
    this->mMaterialInformation.erase(materialName);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateModelInformation(const PDyModelConstructionDescriptor& modelDescriptor)
{
  const auto& materialName = modelDescriptor.mModelName;
  if (mModelInformation.find(materialName) != mModelInformation.end())
  {
    return DY_FAILURE;
  }

  // Check there is already in the information map.
  auto [it, creationResult] = mModelInformation.try_emplace(materialName, nullptr);
  if (!creationResult) {

    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  try
  {
    auto materialInformation = std::make_unique<DDyModelInformation>(modelDescriptor);
    it->second.swap(materialInformation);
    if (!it->second)
    {
      this->mModelInformation.erase(materialName);
      return DY_FAILURE;
    }
  }
  catch (const std::runtime_error& e)
  {
#if defined(_WIN32)
    const auto* log = e.what();
    const int32_t neededSize = MultiByteToWideChar(CP_UTF8, 0, log, static_cast<int32_t>(std::strlen(log)), nullptr, 0);

    std::wstring wstrTo( neededSize, 0 );
    MultiByteToWideChar(CP_UTF8, 0, log, static_cast<int32_t>(std::strlen(log)), &wstrTo[0], neededSize);
    MessageBox(nullptr, wstrTo.c_str(), L"Error", MB_OK | MB_ICONERROR);
#endif
    assert(false);
  }

  return DY_SUCCESS;
}

std::optional<std::string> MDyDataInformation::PopulateMaterialInformation(
    const std::string& materialName,
    const PDyMaterialPopulateDescriptor& materialPopulateDescriptor)
{
  // Check if baseMaterial called materialName is already on information list.
  const auto* baseMaterial = GetMaterialInformation(materialName);
  if (!baseMaterial)
  {
    // @todo error log
    return std::nullopt;
  }

  // Setup meterial populate descriptor from parameter descriptor.
  // and error checking.
  PDyMaterialPopulateDescriptor actualMaterialPopDesc = materialPopulateDescriptor;
  if (!actualMaterialPopDesc.mIsEnabledMaterialCustomNameOverride)
  {
    std::string newName {fmt::format("ov_{0}", materialName)};
    if (actualMaterialPopDesc.mIsEnabledShaderOverride)
    {
      newName.append(fmt::format("{0}{1}", 's', actualMaterialPopDesc.mOverrideShaderName));
    }
    const auto id = baseMaterial->__pfEnrollAndGetNextDerivedMaterialIndex(newName);
    actualMaterialPopDesc.mMaterialOverrideName = newName + std::to_string(id);
  }
  else
  {
    if (actualMaterialPopDesc.mMaterialOverrideName.empty())
    {
      // @todo error log "Empty name is prohibited."
      return std::nullopt;
    }
    if (GetMaterialInformation(actualMaterialPopDesc.mMaterialOverrideName))
    {
      // @todo error log "OverrideName is already posed by material instance."
      return std::nullopt;
    };
  }

  // Check there is already in the information map.
  auto [infoIt, result] = this->mMaterialInformation.try_emplace(actualMaterialPopDesc.mMaterialOverrideName, nullptr);
  if (!result)
  {
    // @todo error log
    return std::nullopt;
  }

  // Let baseMaterial have created populated derived material and move ownership to list.
  auto populateDerivedSmtPtr = baseMaterial->__pfPopulateWith(actualMaterialPopDesc);
  infoIt->second.swap(populateDerivedSmtPtr);
  if (!infoIt->second)
  {
    // @todo error log
    return std::nullopt;
  }
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
    return DY_FAILURE;
  }

  if (!isForced)
  {

  }
  // IF mShaderInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  assert(false);

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteTextureInformation(const std::string& textureName, bool isForced)
{
  const auto iterator = mTextureInformation.find(textureName);
  if (iterator == mTextureInformation.end())
  {
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  assert(false);

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteMaterialInformation(const std::string& materialName, bool isForced)
{
  const auto iterator = mTextureInformation.find(materialName);
  if (iterator == mTextureInformation.end())
  {
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  assert(false);

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteModelInformation(const std::string& modelName, bool isAllRemoveSubresource, bool isForced)
{
  const auto iterator = mTextureInformation.find(modelName);
  if (iterator == mTextureInformation.end())
  {
    return DY_FAILURE;
  }

  // IF mMaterialInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  assert(false);

  // First, release all information and related subresource instances from system
  // only when isAllRemoveSubresource true.
  if (isAllRemoveSubresource)
  {

  }

  // And remove model information!

  return DY_SUCCESS;
}

//!
//! Get function.
//!

const CDyShaderInformation* MDyDataInformation::GetShaderInformation(const std::string& shaderName) const noexcept
{
  const auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    // @todo Error log message
    return nullptr;
  }

  return iterator->second.get();
}

const CDyTextureInformation* MDyDataInformation::GetTextureInformation(const std::string& textureName) const noexcept
{
  const auto iterator = mTextureInformation.find(textureName);
  if (iterator == mTextureInformation.end())
  {
    // @todo Error log message
    return nullptr;
  }

  return iterator->second.get();
}

const DDyMaterialInformation* MDyDataInformation::GetMaterialInformation(const std::string& materialName) const noexcept
{
  const auto iterator = mMaterialInformation.find(materialName);
  if (iterator == mMaterialInformation.end())
  {
    // @todo Error log message
    return nullptr;
  }

  return iterator->second.get();
}

const DDyModelInformation* MDyDataInformation::GetModelInformation(const std::string& modelName) const noexcept
{
  const auto iterator = mModelInformation.find(modelName);
  if (iterator == mModelInformation.end())
  {
    // @todo Error log message
    return nullptr;
  }

  return iterator->second.get();
}

} /// ::dy namespace