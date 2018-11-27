#ifndef GUARD_DY_META_INFORMATION_COMPONENTLIGHTMETAINFO_H
#define GUARD_DY_META_INFORMATION_COMPONENTLIGHTMETAINFO_H
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

#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Meta/Type/EDyComponentTypes.h>
#include <Dy/Helper/Type/Clamp.h>
#include <Dy/Helper/Type/ColorRGBA.h>

namespace dy
{

///
/// @struct PDyDirLightComponentMetaInfo
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyDirectionalLight.
///
struct PDyDirLightComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    ///
    EDyShadowType         mShadowType = EDyShadowType::NoneError;
    ///
    DDyClamp<float, 0, 1> mShadowStrength = MDY_INITIALIZE_DEFINT;
    ///
    DDyClamp<float, 0, 2> mShadowBias = 0.02;
    ///
    DDyVector2            mShadowResolution = {};
    /// Normalized light direction vector.
    DDyVector3            mDirection  = {};
    /// Tinting color
    DDyColorRGBA              mDiffuse    = DDyColorRGBA::White;
    DDyColorRGBA              mSpecular   = DDyColorRGBA::White;
    DDyColorRGBA              mAmbient    = DDyColorRGBA::White;
    /// Intensity of light must be set up to 0~.
    TF32                  mIntensity  = MDY_INITIALIZE_DEFINT;
    ///
    std::vector<std::string> mShadowCullingMaskLayer = {};
    ///
    bool                  mIsUsingGlobalShadowResolution = false;
    ///
    bool                  mIsCastingLight = false;
    ///
    bool                  mIsCastingShadow = false;
  };

  /// Details information
  DDetails            mDetails = {};
  /// Component is initially activated or not.
  bool                mInitiallyActivated = false;

};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyDirLightComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyDirLightComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDyDirLightComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyDirLightComponentMetaInfo::DDetails& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_COMPONENTLIGHTMETAINFO_H