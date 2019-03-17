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

#include <Dy/Helper/Type/DColorRGB.h>
#include <Dy/Core/Reflection/RReflection.h>

#define MDY_BYTEPADDING(__Type__) MDY_NOTUSED __Type__ MDY_TOKENPASTE2(____padding, __LINE__){};

namespace dy
{

/// @struct DUboSpotLight
/// @brief This structure is for spot light information binding.
/// Does not need to be specified ubo140.
struct DUboSpotLight final
{
  REGISTER_UNIFORM_STRUCT(DUboSpotLight)

  /// @brief Light's position.
  mutable DVector3 mPosition = DVector3{};
  /// @brief Light direction.
  DVector3  mDirection = DVector3{};
  /// @brief Light color
  DColorRGB mColor = DColorRGB::Black;
  /// @brief Light intensity for spot light component.
  TF32 mIntensity = 0.0f;
  /// @brief Light range for spot light component.
  TF32 mRange = 0.0f;
  /// @brief Light range angle for spot light component.
  TF32 mRangeAngle = 0.0f;
};

MDyReflectionStart
{
  REGISTER_TYPE(DUboSpotLight, uDySpotLight)
    .REGISTER_VARIABLE(DUboSpotLight, mPosition, mWorldPosition)
    .REGISTER_VARIABLE(DUboSpotLight, mDirection, mDirection)
    .REGISTER_VARIABLE(DUboSpotLight, mColor, mColor)
    .REGISTER_VARIABLE(DUboSpotLight, mIntensity, mIntensity)
    .REGISTER_VARIABLE(DUboSpotLight, mRange, mRange)
    .REGISTER_VARIABLE(DUboSpotLight, mRangeAngle, mRangeAngle)
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOSPOTLIGHT_H