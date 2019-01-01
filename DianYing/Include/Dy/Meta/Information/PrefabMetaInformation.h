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

#include <vector>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Element/Descriptor/GlobalEnums.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @struct PDyPrefabInstanceMetaInfo
/// @brief  Prefab meta information
///
struct PDyPrefabInstanceMetaInfo final
{
  using TChildNameList      = std::vector<std::string>;

  struct DCommonProperties final
  {
    /// @brief Parent full specifier name like a A.B.C
    std::string mParentSpecifierName = MDY_INITIALIZE_EMPTYSTR;
    /// @brief Prefab object tag.
    std::string mTagSpecifier = MDY_INITIALIZE_EMPTYSTR;
  };

  /// Meta prefab's name.
  std::string         mSpecifierName          = MDY_INITIALIZE_EMPTYSTR;
  /// The type (light, pawn, pp block etc...) of object
  EDyMetaObjectType   mPrefabType             = EDyMetaObjectType::NoneError;
  ///
  DCommonProperties   mCommonProperties       = {};
  /// Dependency information which are vary along with mType.
  TComponentMetaList  mMetaComponentInfo      = {};
  /// Prefab children list
  std::vector<std::unique_ptr<PDyPrefabInstanceMetaInfo>> mChildrenList = {};

  ///
  /// @brief Factory function for PDyPrefabInstanceMetaInfo.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyPrefabInstanceMetaInfo.
  ///
  static MDY_NODISCARD std::unique_ptr<PDyPrefabInstanceMetaInfo>
  CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

using TPrefabMetaInfoList = std::vector<std::unique_ptr<PDyPrefabInstanceMetaInfo>>;

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyPrefabInstanceMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyPrefabInstanceMetaInfo& p);

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyPrefabInstanceMetaInfo::DCommonProperties& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyPrefabInstanceMetaInfo::DCommonProperties& p);

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_PREFABMETAINFORMATION_H