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

#include <Dy/Core/Resource/Information_Deprecated/MaterialInformation_Deprecated.h>
#include <Dy/Core/Resource/Information_Deprecated/ShaderInformation_Deprecated.h>
#include <Dy/Core/Resource/Information_Deprecated/TextureInformation_Deprecated.h>
#include <Dy/Core/Resource/Information_Deprecated/ModelInformation_Deprecated.h>
#include <Dy/Core/Resource/Information_Deprecated/SoundInformation_Deprecated.h>
#include <Dy/Helper/ThreadPool.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Core/Resource/Type/EDyScope.h>

namespace dy
{
enum class EDyResourceType;
struct PDyModelInstanceMetaInfo;

///
/// @class MDyIOData_Deprecated
/// @brief Manages data information which are needed to create heap instances such as CDy-Resource.
///
class MDyIOData_Deprecated final : public IDySingleton<MDyIOData_Deprecated>
{
  MDY_SINGLETON_DERIVED(MDyIOData_Deprecated);
  MDY_SINGLETON_PROPERTIES(MDyIOData_Deprecated);
public:
  /// @brief Insert result into IO Data.
  void InsertResult(_MIN_ EDyResourceType type, _MIN_ void* rawResultInstance) noexcept;

  ///
  /// @brief Create shader information with scope. This function must be succeeded.
  /// @param shaderSpecifierName specifier name of GL shader must be valid on runtime.
  /// @param scope Information lifetime scope.
  ///
  MDY_NODISCARD EDySuccess CreateShaderInformation_Deprecated(_MIN_ const std::string& shaderSpecifierName, _MIN_ EDyScope scope);

  ///
  /// @brief Create model information with scope. This function must be succeeded.
  /// @param modelSpecifierName specifier name of GL shader must be valid on runtime.
  /// @param scope Information lifetime scope.
  ///
  MDY_NODISCARD EDySuccess CreateModelInformation_Deprecated(_MIN_ const std::string& modelSpecifierName, _MIN_ EDyScope scope);

  ///
  /// @brief Create texture information with scope. This function must be succeeded.
  /// @param textureSpecifier specifier name of texture must be valid on runtime.
  /// @param scope Information lifetime scope.
  ///
  MDY_NODISCARD EDySuccess CreateTextureInformation_Deprecated(_MIN_ const std::string& textureSpecifier, _MIN_ EDyScope scope);

  /// @brief Create material information with scope. This function must be succeeded.
  /// When creating information of material, checks validity texture and shader is exist on meta information.
  /// @param materialSpecifier specifier name of material must be valid on runtime.
  /// @param scope Informatino lifetime scope.
  ///
  MDY_NODISCARD EDySuccess CreateMaterialInformation_Deprecated(_MIN_ const std::string& materialSpecifier, _MIN_ EDyScope scope);

  /// @brief Check shader is exist now on MDyIOData_Deprecated.
  MDY_NODISCARD bool IsShaderInformationExist(_MIN_ const std::string& shaderSpecifier) const noexcept
  {
    return DyIsMapContains(this->mShaderInformation, shaderSpecifier);
  }

  /// @brief Texture shader is exist now on MDyIOData_Deprecated.
  MDY_NODISCARD bool IsTextureInformationExist(_MIN_ const std::string& textureSpecifier) const noexcept
  {
    return DyIsMapContains(this->mTextureInformation, textureSpecifier);
  }

  /// @brief Create sound information,
  EDySuccess CreateSoundInformation(const PDySoundConstructionDescriptor& soundDescriptor);

  ///
  /// @brief Get shader information.
  /// @return Valid shader information pointer reference, or nullptr when not found.
  ///
  MDY_NODISCARD const DDyShaderInformation_Deprecated* GetShaderInformation(const std::string& shaderName) const noexcept;

  ///
  /// @brief Get texture information.
  /// @return Valid texture information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const DDyTextureInformation_Deprecated* GetTextureInformation(const std::string& textureName) const noexcept;

  ///
  /// @brief Get material information.
  /// @return Valid material information pointer reference, or nullptr when not found.
  ///
  [[nodiscard]]
  const DDyMaterialInformation_Deprecated* GetMaterialInformation(const std::string& materialName) const noexcept;

  ///
  /// @brief Get model information.
  /// @return Valid model information poiter reference, or nullptr when not found.
  ///
  const DDyModelInformation_Deprecated* GetModelInformation(const std::string& modelName) const noexcept;

  ///
  /// @brief Get sound information.
  /// @return Valid model information pointer reference, or nullptr when not found.
  ///
  const DDySoundInformation_Deprecated* GetSoundInformation(const std::string& soundName) const noexcept;

  ///
  /// @brief Destroy shader information. Runtime instances binded to specified shader information
  /// will have nullptr or default shader resource instead.
  ///
  EDySuccess DeleteShaderInformation(const std::string& shaderName, bool isForced = false);

  ///
  /// @brief Destroy texture information. Runtime instances bindned to specified texture information
  /// will have nullptr or default builtin texture resource instead.
  ///
  EDySuccess DeleteTextureInformation(const std::string& textureName, bool isForced = false);

  ///
  /// @brief Destroy material information. Runtime instances which are binded to speicifed material information
  /// will have nullptr or default builtin material resource instead.
  ///
  EDySuccess DeleteMaterialInformation(const std::string& materialName, bool isForced = false);

  ///
  /// @brief Destroy model information. Runtime instances which are binded to specified model information
  /// will have nullptr or defualt builtin model resource instaned.
  /// And if you check isAllRemoveSubresource true, material and texture related to this are also
  /// released from system.
  ///
  EDySuccess DeleteModelInformation(const std::string& modelName, bool isAllRemoveSubresource = false, bool isForced = false);

  ///
  /// @brief Destroy sound information. Runtime instances which are binded to speicfied sound information
  /// will have nullptr or default builtin sound resource instead.
  ///
  EDySuccess DeleteSoundInformation(const std::string& soundName, bool isForced = false);

  ///
  /// @brief Populate derived material information.
  ///
  std::optional<std::string> PopulateMaterialInformation(
      const std::string& materialName,
      const PDyMaterialPopulateDescriptor& materialPopulateDescriptor);

private:
  template <typename TInformationType>
  using THeapHash = std::unordered_map<std::string, std::unique_ptr<TInformationType>>;

  THeapHash<DDyShaderInformation_Deprecated>    mShaderInformation;
  THeapHash<DDyTextureInformation_Deprecated>   mTextureInformation;
  THeapHash<DDyMaterialInformation_Deprecated>  mMaterialInformation;
  THeapHash<DDyModelInformation_Deprecated>     mModelInformation;
  THeapHash<DDySoundInformation_Deprecated>     mSoundInformation;

  mutable std::mutex                 mTemporalMutex;
  mutable std::mutex mTemporalIOInsertDeleteGetMutex;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_MANAGEMENT_RESOURCE_MANAGER_H