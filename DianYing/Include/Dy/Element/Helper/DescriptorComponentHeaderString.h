#ifndef GUARD_DY_ELEMENT_HELPER_DESCRIPTORCOMPONENTHEADERSTRING_H
#define GUARD_DY_ELEMENT_HELPER_DESCRIPTORCOMPONENTHEADERSTRING_H
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

#include <string_view>

namespace dy
{

MDY_SET_IMMUTABLE_STRING(sHeaderActivated,      "Activated");

//!
//! CTransform
//!

MDY_SET_IMMUTABLE_STRING(sHeaderLocalPosition, "LPosition");
MDY_SET_IMMUTABLE_STRING(sHeaderLocalAngle,   "LRotation");
MDY_SET_IMMUTABLE_STRING(sHeaderLocalScale,   "LScale");
MDY_SET_IMMUTABLE_STRING(sHeaderWorldPosition,"WPosition");
MDY_SET_IMMUTABLE_STRING(sHeaderWorldAngle,   "WRotation");
MDY_SET_IMMUTABLE_STRING(sHeaderWorldScale,   "WScale");

//!
//! CDyScript
//!

MDY_SET_IMMUTABLE_STRING(sHeaderName,   "Name");

//!
//! Light component common headers
//!


//!
//! CLightDirectional
//!

MDY_SET_IMMUTABLE_STRING(sHeaderLightDirection,   "LightDirection");
MDY_SET_IMMUTABLE_STRING(sHeaderLightIntensity,   "LightIntensity");
MDY_SET_IMMUTABLE_STRING(sHeaderLightDiffuse,     "LightDiffuse");
MDY_SET_IMMUTABLE_STRING(sHeaderLightSpecular,    "LightSpecular");
MDY_SET_IMMUTABLE_STRING(sHeaderLightAmbient,     "LightAmbient");
MDY_SET_IMMUTABLE_STRING(sHeaderIsCastingLight,   "IsCastingLight");
MDY_SET_IMMUTABLE_STRING(sHeaderIsCastingShadow,  "IsCastingShadow");

MDY_SET_IMMUTABLE_STRING(sHeaderShadowType,       "ShadowType");
MDY_SET_IMMUTABLE_STRING(sHeaderShadowStrength,   "ShadowStrength");
MDY_SET_IMMUTABLE_STRING(sHeaderShadowResolution, "ShadowResolution");
MDY_SET_IMMUTABLE_STRING(sHeaderIsUsingGlobalShadowResolution, "IsUsingGlobalShadowResolution");
MDY_SET_IMMUTABLE_STRING(sHeaderShadowBias,       "ShadowBias");
MDY_SET_IMMUTABLE_STRING(sHeaderShadowCullingMaskLayer, "ShadowCullingMaskLayer");

//!
//! CModelFilter
//!

MDY_SET_IMMUTABLE_STRING(sHeaderModelName,      "ModelName");

//!
//! CModelRenderer
//!

MDY_SET_IMMUTABLE_STRING(sHeaderShadow,         "Shadow");
MDY_SET_IMMUTABLE_STRING(sHeaderMaterials,      "Materials");

//!
//! CCamera
//!

MDY_SET_IMMUTABLE_STRING(sHeaderFieldOfView,      "FieldOfView");
MDY_SET_IMMUTABLE_STRING(sHeaderProjection,       "Projection");
MDY_SET_IMMUTABLE_STRING(sValuePerspective,       "Perspective");
MDY_SET_IMMUTABLE_STRING(sValueOrthographic,        "Orthographic");

MDY_SET_IMMUTABLE_STRING(sHeaderClippingNear,     "ClippingNear");
MDY_SET_IMMUTABLE_STRING(sHeaderClippingFar,      "ClippingFar");
MDY_SET_IMMUTABLE_STRING(sHeaderViewportRect,     "ViewportRect");
MDY_SET_IMMUTABLE_STRING(sHeaderIsFocusInstantly, "IsFocusInstantly");
MDY_SET_IMMUTABLE_STRING(sHeaderIsMeshUnclipped,  "IsMeshUnclipped");

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_HELPER_DESCRIPTORCOMPONENTHEADERSTRING_H