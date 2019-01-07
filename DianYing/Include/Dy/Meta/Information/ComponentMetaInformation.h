#ifndef GUARD_DY_META_INFORMATION_COMPONENTMETAINFORMATION_H
#define GUARD_DY_META_INFORMATION_COMPONENTMETAINFORMATION_H
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

#include <any>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Meta/Type/EDyComponentTypes.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include "Dy/Helper/Type/Area2D.h"

namespace dy
{

///
/// @brief Component meta information type.
/// Used at \n
/// `PDyPrefabInstanceMetaInfo` \n
/// `PDyObjectMetaInfo`.
///
using TComponentMetaList  = std::vector<std::pair<EDyComponentMetaType, std::any>>;
void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const TComponentMetaList& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ TComponentMetaList& p);

///
/// @struct IDyMetaInformation
/// @brief Dependency information
///
struct IDyMetaInformation
{
  /// This mType must be EDyCDyComponentType::Script.
  EDyComponentMetaType mType = EDyComponentMetaType::NoneError;
};

///
/// @struct PDyTransformComponentMetaInfo
/// @brief  World transform information.
///
struct PDyTransformComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Local position
    DDyVector3        mLocalPosition = {};
    /// Local rotation
    DDyVector3        mLocalRotation = {};
    /// Local scale
    DDyVector3        mLocalScale = DDyVector3{ 1, 1, 1 };
    /// World position
    DDyVector3        mWorldPosition = {};
    /// World rotation
    DDyVector3        mWorldRotation = {};
    /// World scale
    DDyVector3        mWorldScale = DDyVector3{ 1, 1, 1 };
  };

  /// Details
  DDetails mDetails = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyTransformComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTransformComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyTransformComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyTransformComponentMetaInfo::DDetails& p);

///
/// @struct PDyScriptComponentMetaInfo
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyPawn.
///
struct PDyScriptComponentMetaInfo final : public IDyMetaInformation
{
  using DDetails = PDyScriptReferenceMetaInfo;

  /// Details
  DDetails        mDetails            = {};
  /// Component is initially activated or not.
  bool            mInitiallyActivated = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyScriptComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyScriptComponentMetaInfo& p);

///
/// @struct PDyModelFilterComponentMetaInfo
/// @brief
///
struct PDyModelFilterComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    std::string mModelSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  };

  /// Details
  DDetails mDetails = {};
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelFilterComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelFilterComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelFilterComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelFilterComponentMetaInfo::DDetails& p);

///
/// @struct PDyModelRendererComponentMetaInfo
/// @brief  Dependency information to CDYModelRenderer component.
/// (EDyComponentMetaType::ModelRenderer)
///
struct PDyModelRendererComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Create shadow or not.
    bool                      mIsEnabledCreateShadow  = false;
    /// Material resource name list to apply
    std::vector<std::string>  mMaterialName           = {};
  };

  /// Details
  DDetails mDetails = {};
  /// Component is initially activated or not.
  bool     mInitiallyActivated     = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelRendererComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyModelRendererComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyModelRendererComponentMetaInfo::DDetails& p);

///
/// @struct PDyCameraComponentMetaInfo
/// @brief  Dependency information to CDyCamera (NEW! since 0.0.0~) component.
///
struct PDyCameraComponentMetaInfo final : public IDyMetaInformation
{
private:
  inline static TF32 sInitialNear    = 0.3f;
  inline static TF32 sInitialFar     = 1000.f;
public:
  struct DDetails final
  {
    /// Fov must be range from 0.1f to 180.f
    TF32       mInitialFieldOfView     = 70.f;
    ///
    EDyCameraProjectionType mProjectionType = EDyCameraProjectionType::NoneError;
    /// Near value of camera culling.
    TF32       mNear                   = PDyCameraComponentMetaInfo::sInitialNear;
    /// Far value of camera culling.
    TF32       mFar                    = PDyCameraComponentMetaInfo::sInitialFar;
    /// Viewport area
    /// Custom viewport size, but mIsCustomViewport must be enabled.
    DDyArea2D   mViewportSize           = {};
    /// Camera will be focused instantly.
    bool        mIsFocusInstantly       = false;
    /// Clipped camera will be unclipped.
    bool        mIsEnableMeshUnClipped  = false;
  };
  /// Details
  DDetails    mDetails                = {};
  /// Component is initially activated or not.
  bool        mInitiallyActivated     = false;
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyCameraComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const PDyCameraComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyCameraComponentMetaInfo::DDetails& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_COMPONENTMETAINFORMATION_H