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
  const CDyShaderInformation* shaderInfo = manInfo.pfGetShaderInformation(shaderName);
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
  const CDyTextureInformation* textureInfo = manInfo.pfGetTextureInformation(textureName);
  if (!textureInfo)
  {
    return DY_FAILURE;
  }

  auto [it, result] = this->mOnBoardTextureLists.try_emplace(textureName, nullptr);
  if (!result)
  {
    return DY_FAILURE;
  }

  std::unique_ptr<CDyTextureResource> textureResource = std::make_unique<CDyTextureResource>();
 if (const auto success = textureResource->pfInitializeResource(*textureInfo); success == DY_FAILURE)
  {
    this->mOnBoardShaderLists.erase(textureName);
    return DY_FAILURE;
  }

  it->second.swap(textureResource);
  if (!it->second)
  {
    this->mOnBoardShaderLists.erase(textureName);
    return DY_FAILURE;
  }

  // At last, setting pointers to each other.
  textureInfo->__pfSetNextLevel(it->second.get());
  it->second->__pfSetPrevLevel(const_cast<CDyTextureInformation*>(textureInfo));
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