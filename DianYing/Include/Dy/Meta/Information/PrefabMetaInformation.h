#ifndef GUARD_DY_ELEMENT_DESCRIPTOR_PREFABMETAINFORMATION_H
#define GUARD_DY_ELEMENT_DESCRIPTOR_PREFABMETAINFORMATION_H
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

#include <any>
#include <vector>
#include <utility>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Element/Descriptor/GlobalEnums.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Meta/Type/EDyComponentTypes.h>

namespace dy
{

///
/// @struct PDyPrefabInstanceMetaInfo
/// @brief  Prefab meta information
///
struct PDyPrefabInstanceMetaInfo final
{
  using TChildNameList      = std::vector<std::string>;
  using TChildMetaInfoList  = std::vector<std::unique_ptr<PDyPrefabInstanceMetaInfo>>;

  struct DCommonProperties final
  {
    std::string mParentSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
    std::string mParentUUID           = MDY_INITIALIZE_EMPTYSTR;
    /// Flag is initially activated.
    bool        mIsInitiallyActivated = false;
  };

  /// Meta prefab's name.
  std::string         mSpecifierName          = MDY_INITIALIZE_EMPTYSTR;
  /// Meta UUID
  std::string         mUUID                   = MDY_INITIALIZE_EMPTYSTR;
  /// The type (light, pawn, pp block etc...) of object
  EDyMetaObjectType   mPrefabType             = EDyMetaObjectType::NoneError;
  ///
  DCommonProperties   mCommonProperties       = {};
  /// Dependency information which are vary along with mType.
  /// If mType is "EDyFDyObjectType::FdyPawn", must be converted to "PDyScriptComponentMetaInformation".
  /// If mType is ... not implemented yet!
  TComponentMetaList  mMetaComponentInfo      = {};
  /// Children list
  TChildMetaInfoList  mChildrenList           = {};

  ///
  /// @brief Factory function for PDyPrefabInstanceMetaInfo.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyPrefabInstanceMetaInfo.
  ///
  static MDY_NODISCARD std::unique_ptr<PDyPrefabInstanceMetaInfo>
  CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyPrefabInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyPrefabInstanceMetaInfo& p);

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyPrefabInstanceMetaInfo::DCommonProperties& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyPrefabInstanceMetaInfo::DCommonProperties& p);

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_PREFABMETAINFORMATION_H