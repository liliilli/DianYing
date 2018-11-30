#ifndef GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H
#define GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H
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
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>
#include <Dy/Meta/Information/FontMetaInformation.h>
#include <Dy/Meta/Information/PrefabMetaInformation.h>
#include <Dy/Helper/ContainerHelper.h>
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <Dy/Meta/Information/ModelMetaInformation.h>

namespace dy
{

///
/// @class MDyMetaInfo
/// @brief Resource path and information (not populaized) container type
///
class MDyMetaInfo final : public IDySingleton<MDyMetaInfo>
{
  MDY_SINGLETON_PROPERTIES(MDyMetaInfo);
  MDY_SINGLETON_DERIVED(MDyMetaInfo);
public:
  ///
  /// @brief Get level construction descriptor meta information from levelName which is same to actual level instance to be created.
  /// @return Return value is nullable if not found.
  ///
  const PDyLevelConstructMetaInfo* GetLevelMetaInformation(const std::string& levelName) const noexcept;

  ///
  /// @brief  Get valid script meta information instance.
  /// @return valid script meta information instance.
  ///
  MDY_NODISCARD const PDyScriptInstanceMetaInfo& GetScriptMetaInformation(_MIN_ const std::string& specifierName) const;

  /// @brief
  MDY_NODISCARD const PDyPrefabInstanceMetaInfo& GetPrefabMetaInformation(_MIN_ const std::string& specifierName) const;

  /// @brief
  MDY_NODISCARD const PDyMetaFontInformation& GetFontMetaInformation(_MIN_ const std::string& specifierName) const;

  /// @brief
  MDY_NODISCARD const PDyMetaWidgetRootDescriptor& GetWidgetMetaInformation(_MIN_ const std::string& specifierName) const;

  ///
  /// @brief  Get shader instance meta information from container.
  /// @return Return value is undefined if not found.
  ///
  MDY_NODISCARD const PDyGLShaderInstanceMetaInfo& GetGLShaderMetaInformation(_MIN_ const std::string& specifier) const;

  ///
  /// @brief  Get model instance meta information from container.
  /// @return Return value is undefined if not found.
  ///
  MDY_NODISCARD const PDyModelInstanceMetaInfo& GetModelMetaInformation(_MIN_ const std::string& specifier) const;

  ///
  /// @brief  Check script meta information is exist.
  /// @return Return value is false if not found.
  ///
  FORCEINLINE MDY_NODISCARD bool IsGLShaderMetaInfoExist(_MIN_ const std::string& specifier) const noexcept
  {
    return DyIsMapContains(this->mShaderMetaInfo, specifier);
  }

  ///
  /// @brief  Check model meta information is exist.
  /// @return Return value is false if not found.
  ///
  FORCEINLINE MDY_NODISCARD bool IsModelMetaInfoExist(_MIN_ const std::string& specifier) const noexcept
  {
    return DyIsMapContains(this->mModelMetaInfo, specifier);
  }

  ///
  /// @brief  Check script meta information is exist.
  /// @return If found, return true or false.
  ///
  FORCEINLINE MDY_NODISCARD bool IsScriptMetaInformationExist(_MIN_ const std::string& specifierName) const noexcept
  {
    return DyIsMapContains(this->mScriptMetaInfo, specifierName);
  }

  ///
  /// @brief  Check prefab meta information is exist.
  /// @return If found, return true or false.
  ///
  FORCEINLINE MDY_NODISCARD bool IsPrefabMetaInformationExist(_MIN_ const std::string& specifierName) const noexcept
  {
    return DyIsMapContains(this->mPrefabMetaInfo, specifierName);
  }

  ///
  /// @brief  Check font meta information is exist.
  /// @return If found, return true or false.
  ///
  FORCEINLINE MDY_NODISCARD bool IsFontMetaInformationExist(_MIN_ const std::string& specifierName) const noexcept
  {
    return DyIsMapContains(this->mFontMetaInfo, specifierName);
  }

  template <typename TType>
  using THashMap = std::unordered_map<std::string, TType>;
private:
  MDY_NODISCARD EDySuccess pReadScriptResourceMetaInformation(_MIN_ const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadPrefabResourceMetaInformation(_MIN_ const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadWidgetResourceMetaInformation(_MIN_ const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadFontResourceMetaInformation(_MIN_ const std::string& metaFilePath);
  MDY_NODISCARD EDySuccess pReadSceneResourceMetaInformation(_MIN_ const std::string& metaFilepath);

  MDY_NODISCARD EDySuccess pfAddWidgetMetaInformation (_MIN_ const std::string& metaInformationString);
  MDY_NODISCARD EDySuccess pfAddScriptMetaInformation (_MIN_ const PDyScriptInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddGLShaderMetaInfo      (_MIN_ const PDyGLShaderInstanceMetaInfo& metaInfo);
  MDY_NODISCARD EDySuccess pfAddModelMetaInfo         (_MIN_ const PDyModelInstanceMetaInfo& metaInfo);

  /// Level meta information map.
  THashMap<PDyLevelConstructMetaInfo>   mLevelInfoMap   = {};
  /// Script meta information map.
  THashMap<PDyScriptInstanceMetaInfo>   mScriptMetaInfo = {};
  /// Font meta information map.
  THashMap<PDyMetaFontInformation>      mFontMetaInfo   = {};
  /// GL shader meta information map.
  THashMap<PDyGLShaderInstanceMetaInfo> mShaderMetaInfo = {};
  /// Model meta information map.
  THashMap<PDyModelInstanceMetaInfo>    mModelMetaInfo  = {};

  //!
  //! Hierarchial meta information containers.
  //!

  /// Prefab meta information map. (Temporary use std::unique_ptr, for reconstructing children tree)
  THashMap<std::unique_ptr<PDyPrefabInstanceMetaInfo>>   mPrefabMetaInfo = {};
  /// Widget meta information map.
  THashMap<std::unique_ptr<PDyMetaWidgetRootDescriptor>> mWidgetMetaInfo = {};

  friend void ::dy::reflect::RDyBuiltinResource::BindBuiltinResourcesToMetaManager();
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H