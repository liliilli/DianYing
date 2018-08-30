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

EDySuccess MDyDataInformation::CreateShaderInformation(const std::string& shaderName, const PDyShaderConstructionDescriptor& shaderDescriptor)
{
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

EDySuccess MDyDataInformation::DeleteShaderInformation(const std::string& shaderName)
{
  auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    return DY_FAILURE;
  }

  // IF mShaderInformation is being used by another resource instance?
  // then, return DY_FAILURE or remove it.
  assert(false);

  return DY_SUCCESS;
}

const CDyShaderInformation* MDyDataInformation::pfGetShaderInformation(const std::string& shaderName) const noexcept
{
  const auto iterator = mShaderInformation.find(shaderName);
  if (iterator == mShaderInformation.end())
  {
    // @todo Error log message
    return nullptr;
  }

  return iterator->second.get();
}

} /// ::dy namespace