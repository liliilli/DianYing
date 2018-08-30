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
  if (const auto success = shaderResource->pfInitializeShaderResource(*shaderInfo); success == DY_FAILURE)
  {
    this->mOnBoardShaderLists.erase(shaderName);
    return DY_FAILURE;
  }
  else
  {
    it->second.swap(shaderResource);
    if (!it->second)
    {
      this->mOnBoardShaderLists.erase(shaderName);
      return DY_FAILURE;
    }
  }

  // At last, setting pointers to each other.
  shaderInfo->pfSetNextLevel(it->second.get());
  it->second->pfSetPrevLevel(const_cast<CDyShaderInformation*>(shaderInfo));
  return DY_SUCCESS;
}

EDySuccess MDyResource::CreateTextureResource(const std::string& textureName, const PDyTextureConstructionDescriptor& textureDescriptor)
{
  auto [it, result] = this->mOnBoardTextureLists.try_emplace(textureName, nullptr);
  if (!result)
  {
    return DY_FAILURE;
  }

  std::unique_ptr<CDyTextureComponent> textureResource = std::make_unique<CDyTextureComponent>();
  if (const auto success = textureResource->pInitializeTextureResource(textureDescriptor);
      success == DY_FAILURE)
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

  return DY_SUCCESS;
}

CDyShaderResource* MDyResource::GetShaderResource(const std::string& shaderName)
{
  const auto it = this->mOnBoardShaderLists.find(shaderName);
  if (it == this->mOnBoardShaderLists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

CDyTextureComponent* MDyResource::GetTextureResource(const std::string& textureName)
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
  return DY_SUCCESS;
}

} /// ::dy namespace