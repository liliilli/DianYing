#ifndef GUARD_DY_COMPONENT_DESCRIPTOR_COMPONENTMETADESCRIPTOR_H
#define GUARD_DY_COMPONENT_DESCRIPTOR_COMPONENTMETADESCRIPTOR_H
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

#include <string>
#include <vector>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/Color.h>

namespace dy
{

///
/// @enum   EDyComponentMetaType
/// @brief  Meta type for component
///
enum class EDyComponentMetaType
{
  // CDyTransform
  Transform,
  // CDyScript
  Script,
  // CDyModelFilter
  ModelFilter,
  // CDyModelRenderer
  ModelRenderer,
  // CDyDirectionalLight
  DirectionalLight,
  // CDyCamera
  Camera,
  // ERROR!
  NoneError
};

///
/// @struct IDyMetaInformation
/// @brief Dependency information
///
struct IDyMetaInformation
{
  /// This mType must be EDyCDyComponentType::Script.
  EDyComponentMetaType mType       = EDyComponentMetaType::NoneError;
  /// hashTo must be same to hash string value of DDyObjectInformation instance to be binded.
  std::string          mBindHashTo = MDY_INITILAIZE_EMPTYSTR;
};

///
/// @struct DDyCameraMetaInformation
/// @brief  Dependency information to CDyCamera (NEW! since 0.0.0~) component.
///
struct DDyCameraMetaInformation final : public IDyMetaInformation
{
  /// Fov must be range from 0.1f to 180.f
  float       mInitialFieldOfView     = 70.f;
  /// Near value of camera culling.
  float       mNear                   = DDyCameraMetaInformation::sInitialNear;
  /// Far value of camera culling.
  float       mFar                    = DDyCameraMetaInformation::sInitialFar;
  /// Camera will project objects as orthographic without vanishing point.
  bool        mIsOrthographic         = false;
  /// Clipped camera will be unclipped.
  bool        mIsEnableMeshUnClipped  = false;
  /// Camera will be focused instantly.
  bool        mIsFocusInstantly       = false;
  /// Component is initially activated or not.
  bool        mInitiallyActivated     = false;
  /// Custom viewport size, but mIsCustomViewport must be enabled.
  DDyVector2  mViewportSizeXY         = {0, 0};
  /// Custom viewport size
  DDyVector2  mViewportSizeWH         = {1, 1};
private:
  inline static float sInitialNear    = 0.3f;
  inline static float sInitialFar     = 1000.f;
};

///
/// @struct DDyModelFilterMetaInformation
/// @brief  Dependency information to CDyModelFilter component.
/// (EDyComponentMetaType::ModelFilter)
///
struct DDyModelFilterMetaInformation final : public IDyMetaInformation
{
  /// Model resource name to filter
  std::string     mModelName          = MDY_INITILAIZE_EMPTYSTR;
  /// Component is initially activated or not.
  bool            mInitiallyActivated = false;
};

///
/// @struct DDyModelRendererMetaInformation
/// @brief  Dependency information to CDYModelRenderer component.
/// (EDyComponentMetaType::ModelRenderer)
///
struct DDyModelRendererMetaInformation final : public IDyMetaInformation
{
  /// Create shadow or not.
  bool                      mIsEnabledCreateShadow  = false;
  /// Material resource name list to apply
  std::vector<std::string>  mMaterialName           = {};
  /// Component is initially activated or not.
  bool                      mInitiallyActivated     = false;
};

///
/// @struct PDyScriptComponentMetaInformation
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyPawn.
///
struct PDyScriptComponentMetaInformation final : public IDyMetaInformation
{
  /// Script name for specifying
  std::string     mScriptSpecifierName = MDY_INITILAIZE_EMPTYSTR;
  /// Component is initially activated or not.
  bool            mInitiallyActivated = false;
};

///
/// @struct DDyDirectionalLightMetaInformation
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyDirectionalLight.
///
struct DDyDirectionalLightMetaInformation final : public IDyMetaInformation
{
  /// Normalized light direction vector.
  DDyVector3          mDirection  = {};
  /// Tinting color
  DDyColor            mDiffuse    = DDyColor::White;
  DDyColor            mSpecular   = DDyColor::White;
  DDyColor            mAmbient    = DDyColor::White;
  /// Intensity of light must be set up to 0~.
  TF32                mIntensity  = MDY_INITIALIZE_DEFINT;
  /// Component is initially activated or not.
  bool                mInitiallyActivated = false;
  ///
  bool                mIsCastingLight = false;
  ///
  bool                mIsCastingShadow = false;
};

///
/// @struct DDyTransformMetaInformation
/// @brief  World transform information.
///
struct DDyTransformMetaInformation final : public IDyMetaInformation
{
  /// Local position
  DDyVector3        mLocalPosition  = {};
  /// Local rotation
  DDyVector3        mLocalRotation  = {};
  /// Local scale
  DDyVector3        mLocalScale     = DDyVector3{1, 1, 1};
  /// World position
  DDyVector3        mWorldPosition  = {};
  /// World rotation
  DDyVector3        mWorldRotation  = {};
  /// World scale
  DDyVector3        mWorldScale     = DDyVector3{1, 1, 1};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_DESCRIPTOR_COMPONENTMETADESCRIPTOR_H