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

#include <Dy/Core/Resource/Resource_Deprecated/ShaderResource_Deprecated.h>
#include <Dy/Core/Resource/Resource_Deprecated/TextureResource_Deprecated.h>
#include <Dy/Core/Resource/Resource_Deprecated/MaterialResource_Deprecated.h>
#include <Dy/Core/Resource/Resource_Deprecated/ModelResource_Deprecated.h>
#include <Dy/Core/Resource/Resource_Deprecated/SoundResource_Deprecated.h>

//!
//! Forward declaration
//!

namespace dy
{
enum class EDyResourceType;
enum class EDyScope;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDyIOResource_Deprecated
/// @brief Manages heap resources which have actual memroy resource information for operation.
///
class MDyIOResource_Deprecated final : public dy::IDySingleton<MDyIOResource_Deprecated>
{
  MDY_SINGLETON_DERIVED(MDyIOResource_Deprecated);
  MDY_SINGLETON_PROPERTIES(MDyIOResource_Deprecated);

  template <typename TType>
  using THashList = std::unordered_map<std::string, TType>;
public:
  /// @brief
  void InsertResult(_MIN_ EDyResourceType type, _MIN_ void* rawInstance) noexcept;

  ///
  /// @brief Create shader resource on cpu and gpu.
  /// @param[in] shaderName name to create shader, must be same with shader information name.
  ///
  [[nodiscard]]
  EDySuccess CreateShaderResource_Deprecated(const std::string& shaderName);

  ///
  /// @brief Create material resource on cpu and gpu.
  /// @param[in] materialName name to create material which must be same with material information name.
  /// and material informaiton to be a target should be capable of create all resource related to material.
  ///
  [[nodiscard]]
  EDySuccess CreateMaterialResource_Deprecated(const std::string& materialName);

  ///
  /// @brief Create sound resource on cpu.
  /// @param[in] soundName name to create sound which must be same with sound information name.
  ///
  [[nodiscard]]
  EDySuccess CreateSoundResource_Deprecated(const std::string& soundName);

  ///
  /// @brief Create model resource on cpu and gpu.
  /// @param[in] modelName name to create model which must be same with model information name.
  /// Model resource just populate geometry information, not material information.
  /// User have to instantiate material information using Info::PopulateMaterialInfo(populateMaterialDescriptor);
  ///
  [[nodiscard]]
  EDySuccess CreateModelResource_Deprecated(const std::string& modelName);

  ///
  /// @brief Create texture resource on gpu.
  /// @param specifierName name to create texture, must be same with texture information specifier name. \n
  /// and Same name texture information must be valid on time.
  /// @param scope Lifetime scope for resource
  ///
  MDY_NODISCARD EDySuccess CreateTextureResource_Deprecated(_MIN_ const std::string& specifierName, _MIN_ const EDyScope scope);

  ///
  /// @brief Get valid shader resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyShaderResource_Deprecated* GetShaderResource(const std::string& shaderName);

  ///
  /// @brief Get valid texture resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyTextureResource_Deprecated* GetTextureResource(const std::string& textureName);

  ///
  /// @brief Get valid material resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyMaterialResource_Deprecated* GetMaterialResource(const std::string& materialName);

  ///
  /// @brief Get valid model resource instance. If not find, just return nullptr as failure sign.
  ///
  [[nodiscard]]
  CDyModelResource_Deprecated* GetModelResource(const std::string& modelName);

  ///
  /// @brief
  ///
  [[nodiscard]]
  CDySoundResource_Deprecated* GetSoundResource(const std::string& soundName);

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
  THashList<std::unique_ptr<CDyShaderResource_Deprecated>>   mOnBoardShaderLists;
  THashList<std::unique_ptr<CDyTextureResource_Deprecated>>  mOnBoardTextureLists;
  THashList<std::unique_ptr<CDyMaterialResource_Deprecated>> mOnBoardMaterialLists;
  THashList<std::unique_ptr<CDyModelResource_Deprecated>>    mOnBoardModelLists;
  THashList<std::unique_ptr<CDySoundResource_Deprecated>>    mOnBoardSoundLists;

  std::mutex mTemporalIOInsertDeleteGetMutex;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HANDLE_MANAGER_H