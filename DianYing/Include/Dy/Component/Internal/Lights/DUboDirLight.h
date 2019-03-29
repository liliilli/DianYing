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

#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Helper/Type/DClamp.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Builtin/Constant/CSM.h>

#define MDY_BYTEPADDING(__Type__) MDY_NOTUSED __Type__ MDY_TOKENPASTE2(____padding, __LINE__){};

namespace dy
{

/// @struct DUboDirectionalLight
/// @brief  This structure must be aligned by 16 bytes. (for std140 GL standard block layout)
struct alignas(16) DUboDirectionalLight final
{
  /// World space light direction
  DVec3 mDirection = {};
  MDY_BYTEPADDING(TI32)
  /// Light tint color
  DColorRGBA mDiffuse = DColorRGBA::White;
  /// Light intensity for this light component.
  float mIntensity = MDY_INITIALIZE_DEFINT;
};

/// @struct DDyUboDirShadow
/// @brief  This structure must be aligned by 16 bytes.
struct alignas(16) DDyUboDirShadow final
{
  /// @brief Calculated Cascaded Shadow Mapping VPSB Matrix list.
  std::array<DMat4, 4>   mLightVPSBMatrix = {};
  /// @brief CSM normalized far planes.
  std::array<TF32, kCSMSegment> mNormalizedFarPlanes = {};
  DClamp<float, 0, 2>         mBias     = 0.02f;
  DClamp<float, 0, 1>         mStrength = 0.8f;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DLIGHTUBOS_H