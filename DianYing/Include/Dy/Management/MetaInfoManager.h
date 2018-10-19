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
#include <Dy/Meta/Descriptor/ScriptDescriptor.h>

namespace dy
{

///
/// @class MDyMetaInfo
/// @brief Resource path and information (not populaized) container type
///
class MDyMetaInfo : public ISingleton<MDyMetaInfo>
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
  FORCEINLINE MDY_NODISCARD bool IsScriptMetaInformationExist(_MIN_ const std::string& specifierName)
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

private:
  template <typename TType>
  using THashMap = std::unordered_map<std::string, TType>;

  ///
  /// @brief
  /// @param  metaFilePath
  ///
  MDY_NODISCARD EDySuccess pReadScriptResourceMetaInformation(_MIN_ const std::string& metaFilePath);

  /// Level meta information map.
  THashMap<PDyLevelConstructDescriptor> mLevelInfoMap   = {};
  /// Script meta information map.
  THashMap<PDyMetaScriptInformation>    mScriptMetaInfo = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_EXTERNALRESOURCEINFOMANAGER_H