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

namespace dy
{

EDySuccess MDyDataInformation::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::pfRelease()
{
  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::CreateShaderInformation(const PDyShaderConstructionDescriptor& shaderDescriptor)
{
  const auto& shaderName = shaderDescriptor.mShaderName;
  if (mShaderInformation.find(shaderName) != mShaderInformation.end())
  {
    return DY_FAILURE;
  }

  auto [it, creationResult] = mShaderInformation.try_emplace(shaderName, nullptr);
  if (!creationResult) {
    // Something is already in.
    return DY_FAILURE;
  }

  // Make resource in heap, and insert it to empty memory space.
  auto shaderInformation = std::make_unique<CDyShaderInformation>(shaderDescriptor);
  it->second.swap(shaderInformation);
  if (!it->second)
  {
    this->mShaderInformation.erase(shaderName);
    return DY_FAILURE;
  }

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

EDySuccess MDyDataInformation::DeleteShaderInformation(const std::string& shaderName)
{
  const auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    return DY_FAILURE;
  }

  // IF mShaderInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  assert(false);

  return DY_SUCCESS;
}

EDySuccess MDyDataInformation::DeleteTextureInformation(const std::string& textureName)
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

EDySuccess MDyDataInformation::DeleteMaterialInformation(const std::string& materialName)
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

EDySuccess MDyDataInformation::DeleteModelInformation(const std::string& modelName, bool isAllRemoveSubresource)
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