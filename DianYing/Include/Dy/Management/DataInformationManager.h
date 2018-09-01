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

#include <unordered_map>

#include <Dy/Core/Component/Information/MaterialInformation.h>
#include <Dy/Core/Component/Information/ShaderInformation.h>
#include <Dy/Core/Component/Information/TextureInformation.h>
#include <Dy/Core/Component/Information/ModelInformation.h>

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
  EDySuccess CreateShaderInformation(const PDyShaderConstructionDescriptor& shaderDescriptor);

  ///
  /// @brief Create texture information.
  ///
  EDySuccess CreateTextureInformation(const PDyTextureConstructionDescriptor& textureDescriptor);

  ///
  /// @brief Create material information.
  ///
  EDySuccess CreateMaterialInformation(const PDyMaterialConstructionDescriptor& materialDescriptor);

  ///
  /// @brief Crate model information.
  ///
  EDySuccess CreateModelInformation(const PDyModelConstructionDescriptor& modelDescriptor);

  ///
  /// @brief Get shader information.
  /// @return Valid shader information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const CDyShaderInformation* GetShaderInformation(const std::string& shaderName) const noexcept;

  ///
  /// @brief Get texture information.
  /// @return Valid texture information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const CDyTextureInformation* GetTextureInformation(const std::string& textureName) const noexcept;

  ///
  /// @brief Get material information.
  /// @return Valid material information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const DDyMaterialInformation* GetMaterialInformation(const std::string& materialName) const noexcept;

  ///
  /// @brief Get model information.
  /// @return Valid model information poiter reference, or nullptr when not found.
  ///
  const DDyModelInformation* GetModelInformation(const std::string& modelName) const noexcept;

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

  ///
  /// @brief Destroy material information. Runtime instances which are binded to speicifed material information
  /// will have nullptr or default builtin material resource instead.
  ///
  EDySuccess DeleteMaterialInformation(const std::string& materialName);

  ///
  /// @brief Destroy model information. Runtime instances which are binded to specified model information
  /// will have nullptr or defualt builtin model resource instaned.
  /// And if you check isAllRemoveSubresource true, material and texture related to this are also
  /// released from system.
  ///
  EDySuccess DeleteModelInformation(const std::string& modelName, bool isAllRemoveSubresource = false);

private:
  template <typename TInformationType>
  using THeapHash = std::unordered_map<std::string, std::unique_ptr<TInformationType>>;

  THeapHash<CDyShaderInformation>    mShaderInformation;
  THeapHash<CDyTextureInformation>   mTextureInformation;
  THeapHash<DDyMaterialInformation>  mMaterialInformation;
  THeapHash<DDyModelInformation>     mModelInformation;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H