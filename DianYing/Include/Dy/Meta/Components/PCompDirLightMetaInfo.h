#ifndef GUARD_DY_META_INFORMATION_COMPONENTLIGHTMETAINFO_H
#define GUARD_DY_META_INFORMATION_COMPONENTLIGHTMETAINFO_H
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

#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Meta/Type/EDyComponentTypes.h>
#include <Dy/Helper/Type/DClamp.h>
#include <Dy/Helper/Type/DColorRGBA.h>

namespace dy
{

/// @struct PDirLightComponentMetaInfo
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo when mType is FDyDirectionalLight.
struct PDirLightComponentMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Shadow type. (@TODO IMPLEMENT THIS)
    EDyShadowType mShadowType = EDyShadowType::__Error;
    /// Shadow strength. 
    DClamp<float, 0, 1> mShadowStrength = MDY_INITIALIZE_DEFINT;
    /// Shadow bias.
    DClamp<float, 0, 2> mShadowBias = 0.02;
    /// NOT USED
    MDY_NOTUSED DVector2    mShadowResolution = {};
    /// Normalized light direction vector.
    DVector3    mDirection  = {};
    /// Tinting color
    DColorRGBA  mDiffuse    = DColorRGBA::White;
    /// Intensity of light must be set up to 0~.
    TF32          mIntensity  = MDY_INITIALIZE_DEFINT;
    /// 
    MDY_NOTUSED std::vector<std::string> mShadowCullingMaskLayer = {};
    ///
    MDY_NOTUSED bool mIsUsingGlobalShadowResolution = false;
    ///
    bool mIsCastingLight = false;
    ///
    bool mIsCastingShadow = false;
  };

  /// Details information
  DDetails mDetails = {};
  /// Component is initially activated or not.
  bool     mInitiallyActivated = false;

};

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDirLightComponentMetaInfo& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDirLightComponentMetaInfo& p);

void to_json  (_MINOUT_ nlohmann::json& j, _MIN_ const PDirLightComponentMetaInfo::DDetails& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDirLightComponentMetaInfo::DDetails& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_COMPONENTLIGHTMETAINFO_H