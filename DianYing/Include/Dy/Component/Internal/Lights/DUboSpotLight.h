#ifndef GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOSPOTLIGHT_H
#define GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOSPOTLIGHT_H
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

#include <Dy/Helper/Type/ColorRGB.h>
#include <Dy/Core/Reflection/RReflection.h>

#define MDY_BYTEPADDING(__Type__) MDY_NOTUSED __Type__ MDY_TOKENPASTE2(____padding, __LINE__){};

namespace dy
{

/// @struct DDyUboSpotLight
/// @brief This structure is for spot light information binding.
/// Does not need to be specified ubo140.
struct DDyUboSpotLight final
{
  REGISTER_UNIFORM_STRUCT(DDyUboPointLight)

  /// @brief Light's position.
  mutable DDyVector3 mPosition = DDyVector3{};
  /// @brief Light direction.
  DDyVector3  mDirection = DDyVector3{};
  /// @brief Light color
  DDyColorRGB mColor = DDyColorRGB::Black;
  /// @brief Light intensity for spot light component.
  TF32 mIntensity = 0.0f;
  /// @brief Light range for spot light component.
  TF32 mRange = 0.0f;
  /// @brief Light range angle for spot light component.
  TF32 mRangeAngle = 0.0f;
};

MDyReflectionStart
{
  REGISTER_TYPE(DDyUboSpotLight, uDySpotLight)
    .REGISTER_VARIABLE(DDyUboSpotLight, mPosition, mWorldPosition)
    .REGISTER_VARIABLE(DDyUboSpotLight, mDirection, mDirection)
    .REGISTER_VARIABLE(DDyUboSpotLight, mColor, mColor)
    .REGISTER_VARIABLE(DDyUboSpotLight, mIntensity, mIntensity)
    .REGISTER_VARIABLE(DDyUboSpotLight, mRange, mRange)
    .REGISTER_VARIABLE(DDyUboSpotLight, mRangeAngle, mRangeAngle)
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOSPOTLIGHT_H