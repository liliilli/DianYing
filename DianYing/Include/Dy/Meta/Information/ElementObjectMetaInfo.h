#ifndef GUARD_DY_META_INFORMATION_ELEMENTOBJECTMETAINFO_H
#define GUARD_DY_META_INFORMATION_ELEMENTOBJECTMETAINFO_H
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
#include <nlohmann/json_fwd.hpp>
#include <Dy/Element/Descriptor/GlobalEnums.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @struct PDyObjectMetaInfo
/// @brief object meta information container
///
struct PDyObjectMetaInfo final
{
  PDyObjectMetaInfo() = default;
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(PDyObjectMetaInfo);

  struct DCommonProperties final
  {
    ///
    std::string mParentSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
    /// FDyObject is initially activated or not.
    bool        mInitialActivated     = false;
    /// FDyObject is using prefab.
    bool        mIsUsingPrefab        = false;
    /// Prefab specifier name.
    std::string mPrefabSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  };

  /// Object specifier name
  std::string       mSpecifierName    = MDY_INITIALIZE_EMPTYSTR;
  /// Object properties.
  DCommonProperties mProperties       = {};
  /// The type (light, pawn, pp block etc...) of object
  EDyMetaObjectType mObjectType       = EDyMetaObjectType::NoneError;

  /// Dependency information which are varied along with mType.
  TComponentMetaList mMetaComponentInfo;
  /// `Children object` Meta informations.
  std::vector<std::unique_ptr<PDyObjectMetaInfo>> mChildrenList = {};
};

using TObjectMetaInfoList = std::vector<std::unique_ptr<PDyObjectMetaInfo>>;

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyObjectMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyObjectMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyObjectMetaInfo::DCommonProperties& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyObjectMetaInfo::DCommonProperties& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_ELEMENTOBJECTMETAINFO_H