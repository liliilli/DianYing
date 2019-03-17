#ifndef GUARD_DY_META_INFORMATION_PCOMPSPOTLIGHTMETAINFO_H
#define GUARD_DY_META_INFORMATION_PCOMPSPOTLIGHTMETAINFO_H
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

namespace dy
{

/// @struct PCompSpotLightMetaInfo
/// @brief Dependency information to DDyObjectInformation::mMetaComponentInfo 
/// when mType is FDySpotLight.
struct PCompSpotLightMetaInfo final : public IDyMetaInformation
{
  struct DDetails final
  {
    /// Tinting color
    DColorRGB mColor = DColorRGB::White;
    /// Direction
    DVector3  mDirection;
    /// Intensity of light must be set up to 0~. (Total energy of light)
    TF32 mIntensity = 1.0f;
    /// Range (radius) of light.
    TF32 mRange = 1.0f;
    /// Range of angle.
    TF32 mRangeAngle = 45.f;
    /// Flag for enabling light casting initially.
    bool mIsCastingLight = false;
  };

  /// Details information
  DDetails mDetails = {};
  /// Component is initially activated or not.
  bool     mInitiallyActivated = false;
};

void to_json(nlohmann::json& oJson, const PCompSpotLightMetaInfo& iMeta);
void from_json(const nlohmann::json& iJson, PCompSpotLightMetaInfo& oMeta);

void to_json(nlohmann::json& oJson, const PCompSpotLightMetaInfo::DDetails& iMeta);
void from_json(const nlohmann::json& iJson, PCompSpotLightMetaInfo::DDetails& oMeta);

} /// ::dy namespace

#endif /// GUARD_DY_META_INFORMATION_PCOMPSPOTLIGHTMETAINFO_H
