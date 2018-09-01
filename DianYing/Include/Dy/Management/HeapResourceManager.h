#ifndef GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H
#define GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H
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

#include <unordered_map>

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Core/Component/Internal/ShaderType.h>
#include <Dy/Core/Component/Internal/TextureType.h>

#include <Dy/Core/Component/Resource/ShaderResource.h>
#include <Dy/Core/Component/Resource/TextureResource.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Core/Component/Resource/ModelResource.h>

//!
//! Forward declaration
//!


//!
//! Implementation
//!

namespace dy
{

class MDyResource final : public dy::ISingleton<MDyResource>
{
  MDY_SINGLETON_DERIVED(MDyResource);
  MDY_SINGLETON_PROPERTIES(MDyResource);

  template <typename TType>
  using THashList = std::unordered_map<std::string, TType>;
public:
  ///
  /// @brief Create shader resource on cpu and gpu.
  /// @param[in] shaderName name to create shader, must be same with shader information name.
  ///
  [[nodiscard]]
  EDySuccess CreateShaderResource(const std::string& shaderName);

  ///
  /// @brief Create texture resource on gpu.
  /// @param[in] textureName name to create texture, must be same with texture information name.
  ///
  [[nodiscard]]
  EDySuccess CreateTextureResource(const std::string& textureName);

  ///
  /// @brief Create material resource on cpu and gpu.
  /// @param[in] materialName name to create material which must be same with material information name.
  /// and material informaiton to be a target should be capable of create all resource related to material.
  ///
  [[nodiscard]]
  EDySuccess CreateMaterialResource(const std::string& materialName);

  ///
  /// @brief Create model resource on cpu and gpu.
  /// @param[in] modelName name to create model which must be same with model information name.
  /// Model resource just populate geometry information, not material information.
  /// User have to instantiate material information using Info::PopulateMaterialInfo(populateMaterialDescriptor);
  ///
  [[nodiscard]]
  EDySuccess CreateModelResource(const std::string& modelName);

  ///
  /// @brief
  ///
  [[nodiscard]]
  CDyShaderResource* GetShaderResource(const std::string& shaderName);

  ///
  /// @brief
  ///
  [[nodiscard]]
  CDyTextureResource* GetTextureResource(const std::string& textureName);

  ///
  /// @brief
  ///
  [[nodiscard]]
  CDyMaterialResource* GetMaterialResource(const std::string& materialName);

  ///
  /// @brief
  ///
  [[nodiscard]]
  CDyModelResource* GetModelResource(const std::string& modelName);

private:
  THashList<std::unique_ptr<CDyShaderResource>>   mOnBoardShaderLists;
  THashList<std::unique_ptr<CDyTextureResource>>  mOnBoardTextureLists;
  THashList<std::unique_ptr<CDyMaterialResource>> mOnBoardMaterialLists;
  THashList<std::unique_ptr<CDyModelResource>>    mOnBoardModelLists;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H