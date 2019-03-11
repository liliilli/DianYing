#ifndef GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DLIGHTUBOS_H
#define GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DLIGHTUBOS_H
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

#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/Clamp.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Builtin/Constant/CSM.h>

#define MDY_BYTEPADDING(__Type__) MDY_NOTUSED __Type__ MDY_TOKENPASTE2(____padding, __LINE__){};

namespace dy
{

/// @struct DDyUboDirectionalLight
/// @brief  This structure must be aligned by 16 bytes. (for std140 GL standard block layout)
struct alignas(16) DDyUboDirectionalLight final
{
  /// World space light direction
  DDyVector3 mDirection = {};
  MDY_BYTEPADDING(TI32)
  /// Light tint color
  DDyColorRGBA mDiffuse = DDyColorRGBA::White;
  /// Light intensity for this light component.
  float mIntensity = MDY_INITIALIZE_DEFINT;
};

/// @struct DDyUboDirShadow
/// @brief  This structure must be aligned by 16 bytes.
struct alignas(16) DDyUboDirShadow final
{
  /// @brief Calculated Cascaded Shadow Mapping VPSB Matrix list.
  std::array<DDyMatrix4x4, 4>   mLightVPSBMatrix = {};
  /// @brief CSM normalized far planes.
  std::array<TF32, kCSMSegment> mNormalizedFarPlanes = {};
  DDyClamp<float, 0, 2>         mBias     = 0.02f;
  DDyClamp<float, 0, 1>         mStrength = 0.8f;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DLIGHTUBOS_H