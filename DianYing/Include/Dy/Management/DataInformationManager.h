#ifndef GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H
#define GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>

#include <Dy/Core/Component/Information/ShaderInformation.h>
#include <unordered_map>
#include "Dy/Core/Component/Information/TextureInformation.h"

namespace dy
{

class MDyDataInformation final : public ISingleton<MDyDataInformation>
{
  MDY_SINGLETON_DERIVED(MDyDataInformation);
  MDY_SINGLETON_PROPERTIES(MDyDataInformation);
public:
  ///
  /// @brief Create shader information.
  ///
  EDySuccess CreateShaderInformation(const std::string& shaderName, const PDyShaderConstructionDescriptor& shaderDescriptor);

  ///
  /// @brief Create texture information.
  ///
  EDySuccess CreateTextureInformation(const std::string& textureName, const PDyTextureConstructionDescriptor& textureDescriptor);

  ///
  /// @brief Destroy shader information. Runtime instances binded to specified shader information
  /// will have nullptr or default shader resource instead.
  ///
  EDySuccess DeleteShaderInformation(const std::string& shaderName);

  ///
  /// @brief Destroy texture information. Runtime instances bindned to specified texture information
  /// will have nullptr or default builtin texture resource instead.
  ///
  EDySuccess DeleteTextureInformation(const std::string& textureName);

private:
  ///
  /// @brief Get shader information.
  /// @return Valid shader information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const CDyShaderInformation* pfGetShaderInformation(const std::string& shaderName) const noexcept;

  ///
  /// @brief Get texture information.
  /// @return Valid texture information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const CDyTextureInformation* pfGetTextureInformation(const std::string& textureName) const noexcept;

  std::unordered_map<std::string, std::unique_ptr<CDyShaderInformation>>  mShaderInformation;
  std::unordered_map<std::string, std::unique_ptr<CDyTextureInformation>> mTextureInformation;

  friend class MDyResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H