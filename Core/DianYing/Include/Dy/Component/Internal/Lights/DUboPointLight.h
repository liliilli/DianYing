#ifndef GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOPOINTLIGHT_H
#define GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOPOINTLIGHT_H
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

/// @struct DUboPointLight
/// @brief This structure must be aligned by 16 bytes. (for std140 GL standard block layout)
struct alignas(16) DUboPointLight final
{
  REGISTER_UNIFORM_STRUCT(DUboPointLight)

  /// @brief Light's position.
  alignas(16) mutable DVec3 mPosition = DVec3{};
  /// @brief Light color
  alignas(16) DColorRGB mColor = DColorRGB::Black;
  /// @brief Light intensity for point light component.
              TF32 mIntensity = 0.0f;
  /// @brief Light range for point light component.
  alignas(16) TF32 mRange = 0.0f;
};

MDyReflectionStart
{
  REGISTER_TYPE(DUboPointLight, uDyLightPoint)
    .REGISTER_VARIABLE(DUboPointLight, mPosition, mWorldPosition)
    .REGISTER_VARIABLE(DUboPointLight, mColor, mColor)
    .REGISTER_VARIABLE(DUboPointLight, mIntensity, mIntensity)
    .REGISTER_VARIABLE(DUboPointLight, mRange, mRange)
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_LIGHTS_DUBOPOINTLIGHT_H