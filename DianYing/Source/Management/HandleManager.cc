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
#include <Dy/Management/HandleManager.h>

namespace dy
{

EDySuccess MDyHandle::pfInitialize()
{
  return DY_SUCCESS;
}

EDySuccess MDyHandle::pfRelease()
{
  this->mOnBoardShaderLists.clear();

  return DY_SUCCESS;
}

EDySuccess MDyHandle::CreateShaderResource(const std::string& shaderName, const PDyShaderConstructionDescriptor& shaderDescriptor)
{
  auto [it, result] = this->mOnBoardShaderLists.try_emplace(shaderName, nullptr);
  if (!result)
  {
    return DY_FAILURE;
  }

  std::unique_ptr<CDyShaderComponent> shaderResource = std::make_unique<CDyShaderComponent>();
  if (const auto success = shaderResource->pInitializeShaderProgram(shaderDescriptor);
      success == DY_FAILURE)
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

  return DY_SUCCESS;
}

EDySuccess MDyHandle::CreateTextureResource(const std::string& textureName, const PDyTextureConstructionDescriptor& textureDescriptor)
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

CDyShaderComponent* MDyHandle::GetShaderResource(const std::string& shaderName)
{
  const auto it = this->mOnBoardShaderLists.find(shaderName);
  if (it == this->mOnBoardShaderLists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

CDyTextureComponent* MDyHandle::GetTextureResource(const std::string& textureName)
{
  const auto it = this->mOnBoardTextureLists.find(textureName);
  if (it == this->mOnBoardTextureLists.end())
  {
    return nullptr;
  }

  return it->second.get();
}

} /// ::dy namespace