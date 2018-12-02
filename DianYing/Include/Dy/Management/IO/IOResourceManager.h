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
/// @todo IMPLEMENT THREAD-SAFE & BINDING-SAFE HEAP RESOURCE DELETION FUNCTIONS
///

#include <unordered_map>

#include <Dy/Management/Interface/ISingletonCrtp.h>

#include <Dy/Core/Resource/Resource/ShaderResource.h>
#include <Dy/Core/Resource/Resource/TextureResource.h>
#include <Dy/Core/Resource/Resource/MaterialResource.h>
#include <Dy/Core/Resource/Resource/ModelResource.h>
#include <Dy/Core/Resource/Resource/SoundResource.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyScope;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDyIOResource
/// @brief Manages heap resources which have actual memroy resource information for operation.
///
class MDyIOResource final : public dy::IDySingleton<MDyIOResource>
{
  MDY_SINGLETON_DERIVED(MDyIOResource);
  MDY_SINGLETON_PROPERTIES(MDyIOResource);

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
  /// @brief Create material resource on cpu and gpu.
  /// @param[in] materialName name to create material which must be same with material information name.
  /// and material informaiton to be a target should be capable of create all resource related to material.
  ///
  [[nodiscard]]
  EDySuccess CreateMaterialResource(const std::string& materialName);

  ///
  /// @brief Create sound resource on cpu.
  /// @param[in] soundName name to create sound which must be same with sound information name.
  ///
  [[nodiscard]]
  EDySuccess CreateSoundResource(const std::string& soundName);

  ///
  /// @brief Create model resource on cpu and gpu.
  /// @param[in] modelName name to create model which must be same with model information name.
  /// Model resource just populate geometry information, not material information.
  /// User have to instantiate material information using Info::PopulateMaterialInfo(populateMaterialDescriptor);
  ///
  [[nodiscard]]
  EDySuccess CreateModelResource(const std::string& modelName);

  ///
  /// @brief Create texture resource on gpu.
  /// @param specifierName name to create texture, must be same with texture information specifier name. \n
  /// and Same name texture information must be valid on time.
  /// @param scope Lifetime scope for resource
  ///
  MDY_NODISCARD EDySuccess CreateTextureResource(_MIN_ const std::string& specifierName, _MIN_ const EDyScope scope);

  ///
  /// @brief Get valid shader resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyShaderResource* GetShaderResource(const std::string& shaderName);

  ///
  /// @brief Get valid texture resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyTextureResource* GetTextureResource(const std::string& textureName);

  ///
  /// @brief Get valid material resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyMaterialResource* GetMaterialResource(const std::string& materialName);

  ///
  /// @brief Get valid model resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyModelResource* GetModelResource(const std::string& modelName);

  ///
  /// @brief
  ///
  [[nodiscard]]
  CDySoundResource* GetSoundResource(const std::string& soundName);

  ///
  /// @brief Create texture resource on gpu.
  /// @param[in] textureName name to create texture, must be same with texture information name.
  ///
  MDY_NODISCARD EDySuccess CreateTextureResource_Deprecated(const std::string& textureName);

  ///
  /// @brief Create texture resource on gpu, with PDyTextureConstructionBufferChunkDescriptor.
  /// @param desc Texture construction buffer chunk descriptor for creating resource instance.
  /// @return Success flag.
  ///
  MDY_NODISCARD EDySuccess CreateTextureResourceWithChunk_Deprecated(_MIN_ const PDyTextureConstructionBufferChunkDescriptor& desc);

private:
  THashList<std::unique_ptr<CDyShaderResource>>   mOnBoardShaderLists;
  THashList<std::unique_ptr<CDyTextureResource>>  mOnBoardTextureLists;
  THashList<std::unique_ptr<CDyMaterialResource>> mOnBoardMaterialLists;
  THashList<std::unique_ptr<CDyModelResource>>    mOnBoardModelLists;
  THashList<std::unique_ptr<CDySoundResource>>    mOnBoardSoundLists;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H