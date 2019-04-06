#ifndef GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H
#define GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
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
#include <nlohmann/json_fwd.hpp>

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Meta/Type/DDyResourceName.h>
#include <Dy/Meta/Information/MetaInfoFont.h>

//!
//! Forward declaration
//!

namespace dy::reflect
{
class RBuiltinResources;
} /// ::dy::reflect namespace

namespace dy
{
struct PDyRenderItemInstanceMetaInfo;
struct PDyRenderPipelineInstanceMetaInfo;
struct PDyGLShaderInstanceMetaInfo;
struct PDyMeshInstanceMetaInfo;
struct PDyModelInstanceMetaInfo;
struct PDyModelSkelInstanceMetaInfo;
struct PDyModelAnimInstanceMetaInfo;
struct PDyTextureInstanceMetaInfo;
struct PDyMaterialInstanceMetaInfo;
struct PDyGlFrameBufferInstanceMetaInfo;
struct PDyGlAttachmentInstanceMetaInfo;
struct PDySoundInstanceMetaInfo;
struct PDyMetaWidgetRootDescriptor;
struct PDyMetaFontInformation;
struct PDyPrefabInstanceMetaInfo;
struct PDyLevelConstructMetaInfo;
struct PDyScriptInstanceMetaInfo;
struct PLevelInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class MIOMeta
/// @brief Resource path and information (not populaized) container type
class MIOMeta final : public ISingleton<MIOMeta>
{
  MDY_SINGLETON_PROPERTIES(MIOMeta);
  MDY_SINGLETON_DERIVED(MIOMeta);
public:
  template <typename TType>
  using THashMap = std::unordered_map<std::string, TType>;
  using TResourceSpecifierList = std::vector<DDyResourceName>;

  /// @brief Get level construction descriptor meta information from levelName 
  /// which is same to actual level instance to be created.
  /// @return Return value is nullable if not found.
  const PLevelInstanceMetaInfo* 
  GetLevelMetaInformation(const std::string& levelName) const noexcept;

  MDY_NODISCARD std::vector<std::pair<std::string, const PDyScriptInstanceMetaInfo*>> 
  GetGlobalScriptMetaInfos() const noexcept;

  /// @brief  Get valid script meta information instance.
  /// @return valid general script meta information instance.
  MDY_NODISCARD const PDyScriptInstanceMetaInfo&    
  GetScriptMetaInformation(const std::string& specifierName) const;
  /// @brief  Get prefab meta information instance.
  /// @return valid prefab meta information instance.
  MDY_NODISCARD const PDyPrefabInstanceMetaInfo&    
  GetPrefabMetaInformation(const std::string& specifierName) const;
  /// @brief
  MDY_NODISCARD const PDyMetaFontInformation&       
  GetFontMetaInformation(const std::string& specifierName) const;
  /// @brief
  MDY_NODISCARD const PDyMetaWidgetRootDescriptor&  
  GetWidgetMetaInformation(const std::string& specifierName) const;
  /// @brief  Get shader instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyGLShaderInstanceMetaInfo&  
  GetGLShaderMetaInformation(const std::string& specifier) const;
  /// @brief  Get builtin mesh instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyMeshInstanceMetaInfo&     
  GetBtMeshMetaInformation(const std::string& specifier) const;
  /// @brief  Get model instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyModelInstanceMetaInfo&     
  GetModelMetaInformation(const std::string& specifier) const;
  /// @brief  Get model instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyModelSkelInstanceMetaInfo&
  GetModelSkeletonMetaInformation(const std::string& specifier) const;
  /// @brief  Get model animation scrap meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyModelAnimInstanceMetaInfo& 
  GetModelAnimScrapMetaInformation(const std::string& specifier) const;
  /// @brief  Get texture instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyTextureInstanceMetaInfo&   
  GetTextureMetaInformation(const std::string& specifier) const;
  /// @brief  Get material instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyMaterialInstanceMetaInfo&  
  GetMaterialMetaInformation(const std::string& specifier) const;
  /// @brief  Get framebuffer instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyGlFrameBufferInstanceMetaInfo&
  GetGlFrameBufferMetaInformation(const std::string& specifier) const;
  /// @brief  Get attachment instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDyGlAttachmentInstanceMetaInfo&  
  GetGLAttachmentMetaInformation(const std::string& specifier) const;
  /// @brief  Get sound instance meta information from container.
  /// @return Return value is undefined if not found.
  MDY_NODISCARD const PDySoundInstanceMetaInfo&
  GetSoundMetaInformation(const std::string& specifier) const;
  /// @brief  Get render pipeline meta information.
  MDY_NODISCARD const PDyRenderPipelineInstanceMetaInfo&
  GetRenderPipeline(const std::string& iRenderPipelineSpecifier) const;
  /// @brief  Get render item meta information.
  MDY_NODISCARD const PDyRenderItemInstanceMetaInfo&
  GetRenderItem(const std::string& iRenderItemSpecifier) const;

  /// @brief Get loading widget pointer if exist, or return nullptr (nothing) value
  MDY_NODISCARD const PDyMetaWidgetRootDescriptor* 
  MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)() const noexcept;

  /// @brief  Check script meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsGLShaderMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check mesh meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsMeshMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check scene meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsLevelMetaInformation(const std::string& specifier) const noexcept;
  /// @brief  Check model meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsModelMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check model skeleton meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsModelSkeletonMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check model animation scrap meta information is exist.
  /// @return Return value is falue if not found.
  MDY_NODISCARD bool IsModelAnimScrapMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check texture meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsTextureMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check material meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsMaterialMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check widget meta information is exist.
  /// @return Return value is false if not found.
  MDY_NODISCARD bool IsWidgetMetaInfoExist(const std::string& specifier) const noexcept;
  /// @brief  Check script meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsScriptMetaInformationExist(const std::string& specifierName) const noexcept;
  /// @brief  Check prefab meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsPrefabMetaInformationExist(const std::string& specifierName) const noexcept;
  /// @brief  Check font meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsFontMetaInformationExist(const std::string& specifierName) const noexcept;
  /// @brief  Check attachment meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsAttachmentMetaInfoExist(const std::string& specifierName) const noexcept;
  /// @brief  Check framebuffer meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsFrameBufferMetaInfoExist(const std::string& speicfierName) const noexcept;
  /// @brief  Check sound meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsSoundMetaInfoExist(const std::string& specifierName) const noexcept;
  /// @brief  Check render pipeline meta information is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsRenderPipelineExist(const std::string& iRenderPipelineName) const noexcept;
  /// @brief  Check render item is exist.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsRenderItemExist(const std::string& iRenderItemName) const noexcept;

  /// @brief Check loading widget is exist and ready.
  /// @return If found, return true or false.
  MDY_NODISCARD bool IsLoadingWidgetMetaInfoExist() const noexcept;

  /// @brief Get booting loading resource specifier list. \n
  /// This function must not be called twice.
  void MDY_PRIVATE(PopulateBootResourceSpecifierList)() const noexcept;

  /// @brief Get global loading resource specifier list. \n
  /// THis function must not be called twice. 
  void MDY_PRIVATE(PopulateGlobalResourceSpecifierList)() const noexcept;

  void MDY_PRIVATE(InitiateMetaInformation)();
  void MDY_PRIVATE(InitiateMetaInformationComp)(const nlohmann::json& iJson);

private:
  class Impl; Impl* mInternal = nullptr;

  MDY_NODISCARD EDySuccess pReadScriptResourceMetaInformation(const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadPrefabResourceMetaInformation(const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadWidgetResourceMetaInformation(const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadLevelResourceMetaInformation (const std::string& metaFilepath);

  MDY_NODISCARD EDySuccess pReadFontResourceMetaInformation     (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadModelResourceMetaInformation    (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadModelMeshResourceMetaInformation(const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadModelSkeletonMetaInformation    (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadModelAnimationMetaInformation   (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadTextureResourceMetaInformation  (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadShaderResourceMetaInformation   (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadMaterialResourceMetaInformation (const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadSoundResourceMetaInformation    (const std::string& metaFilePath);

  MDY_NODISCARD EDySuccess pReadScriptMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadPrefabMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadWidgetMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadFontMetaAtlas  (const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadLevelMetaAtlas (const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadShaderMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadSoundMetaAtlas    (const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadTextureMetaAtlas  (const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadMaterialMetaAtlas (const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadModelAnimMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadModelMeshMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadModelSkelMetaAtlas(const nlohmann::json& iJson);
  MDY_NODISCARD EDySuccess pReadModelMetaAtlas    (const nlohmann::json& iJson);

  MDY_NODISCARD EDySuccess pfAddPrefabMetaInfo        (const std::string_view& iMetaInfo);
  MDY_NODISCARD EDySuccess pfAddFontMetaInfo          (const PBuiltinMetaFontInfo& iMetaInfo);
  MDY_NODISCARD EDySuccess pfAddWidgetMetaInformation (const std::string& metaInformationString);
  MDY_NODISCARD EDySuccess pfAddScriptMetaInformation (const PDyScriptInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddGLShaderMetaInfo      (const PDyGLShaderInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddBuiltinMeshMetaInfo   (const PDyMeshInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddModelMetaInfo         (const PDyModelInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddTextureMetaInfo       (const PDyTextureInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddMaterialMetaInfo      (const PDyMaterialInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddGLAttachmentMetaInfo  (const PDyGlAttachmentInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddGLFrameBufferMetaInfo (const PDyGlFrameBufferInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddRenderPipelineMetaInfo(const PDyRenderPipelineInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddRenderIteMIOMeta    (const PDyRenderItemInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess MDY_PRIVATE(AddLoadingWidgetMetaInformation)(const std::string& widgetMetaInfo);

  MDY_NODISCARD EDySuccess MDY_PRIVATE(AddBootResourceSpecifierList)(const TResourceSpecifierList& list);
  MDY_NODISCARD EDySuccess MDY_PRIVATE(AddGlobalResourceSpecifierList)(const TResourceSpecifierList& list);

  friend class ::dy::reflect::RBuiltinResources;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H