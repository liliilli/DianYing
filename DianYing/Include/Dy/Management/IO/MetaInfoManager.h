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
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Element/Descriptor/PrefabDescriptor.h>
#include <Dy/Meta/Descriptor/ScriptDescriptor.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Information/FontMetaInformation.h>
#include <Dy/Builtin/Helper/BuiltinInformationDeliver.h>

namespace dy
{

///
/// @class MDyMetaInfo
/// @brief Resource path and information (not populaized) container type
///
class MDyMetaInfo : public IDySingleton<MDyMetaInfo>
{
  MDY_SINGLETON_PROPERTIES(MDyMetaInfo);
  MDY_SINGLETON_DERIVED(MDyMetaInfo);
public:
  ///
  /// @brief Get level construction descriptor meta information from levelName which is same to actual level instance to be created.
  /// @return Return value is nullable if not found.
  ///
  const PDyLevelConstructDescriptor* GetLevelMetaInformation(const std::string& levelName) const noexcept;

  ///
  /// @brief
  /// @param  specifierName
  /// @return
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD bool IsScriptMetaInformationExist(_MIN_ const std::string& specifierName) const noexcept
  {
    return this->mScriptMetaInfo.find(specifierName) != this->mScriptMetaInfo.end();
  }

  ///
  /// @brief
  /// @param  specifierName
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD const PDyMetaScriptInformation& GetScriptMetaInformation(_MIN_ const std::string& specifierName) const;

  ///
  /// @brief
  /// @param  specifierName
  /// @return
  /// @TODO SCRIPT THIS
  ///
  FORCEINLINE MDY_NODISCARD bool IsPrefabMetaInformationExist(_MIN_ const std::string& specifierName) const noexcept
  {
    return this->mPrefabMetaInfo.find(specifierName) != this->mPrefabMetaInfo.end();
  }

  ///
  /// @brief
  /// @param  specifierName
  /// @return
  /// @TODO SCRIPT THIS
  ///
  MDY_NODISCARD const PDyPrefabMetaInformation& GetPrefabMetaInformation(_MIN_ const std::string& specifierName) const;

  ///
  /// @brief
  /// @param  specifierName
  /// @return
  ///
  MDY_NODISCARD const PDyMetaFontInformation& GetFontMetaInformation(_MIN_ const std::string& specifierName) const;

  ///
  /// @brief
  /// @param  specifierName
  /// @return
  ///
  FORCEINLINE MDY_NODISCARD bool IsFontMetaInformationExist(_MIN_ const std::string& specifierName) const noexcept
  {
    return this->mFontMetaInfo.find(specifierName) != this->mFontMetaInfo.end();
  }

private:
  template <typename TType>
  using THashMap = std::unordered_map<std::string, TType>;

  ///
  /// @brief
  /// @param  metaFilePath
  ///
  MDY_NODISCARD EDySuccess pReadScriptResourceMetaInformation(_MIN_ const std::string& metaFilePath);

  ///
  /// @brief
  /// @param
  /// @return
  ///
  MDY_NODISCARD EDySuccess pReadPrefabResourceMetaInformation(_MIN_ const std::string& metaFilePath);

  ///
  /// @brief
  /// @param
  /// @return
  ///
  MDY_NODISCARD EDySuccess pReadWidgetResourceMetaInformation(_MIN_ const std::string& metaFilePath);

  ///
  /// @brief
  /// @param
  /// @return
  ///
  MDY_NODISCARD EDySuccess pReadFontResourceMetaInformation(_MIN_ const std::string& metaFilePath);

  ///
  /// @brief
  /// @param  metaInformationString
  /// @return
  ///
  MDY_NODISCARD EDySuccess pfAddWidgetMetaInformation(_MIN_ const std::string& metaInformationString);

  /// Level meta information map.
  THashMap<PDyLevelConstructDescriptor> mLevelInfoMap   = {};
  /// Script meta information map.
  THashMap<PDyMetaScriptInformation>    mScriptMetaInfo = {};
  /// Font meta information map.
  THashMap<PDyMetaFontInformation>      mFontMetaInfo   = {};

  //!
  //! Hierarchial meta information containers.
  //!

  /// Prefab meta information map. (Temporary use std::unique_ptr, for reconstructing children tree)
  THashMap<std::unique_ptr<PDyPrefabMetaInformation>>    mPrefabMetaInfo = {};
  /// Widget meta information map.
  THashMap<std::unique_ptr<PDyMetaWidgetRootDescriptor>> mWidgetMetaInfo = {};

  friend EDySuccess FDyBuiltinInformationDeliver::ForwardWidgetMetaInformation(const std::string_view& metaString);
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H