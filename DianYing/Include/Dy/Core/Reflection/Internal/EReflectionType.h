#ifndef GUARD_DY_CORE_REFLECTION_INTERNAL_EREFLECTIONTYPE_H
#define GUARD_DY_CORE_REFLECTION_INTERNAL_EREFLECTIONTYPE_H
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

#include <string>
#include <Dy/Core/Resource/Internal/XShaderTypes.h>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DVector4.h>
#include <Dy/Helper/Type/DMatrix2x2.h>
#include <Dy/Helper/Type/DMatrix3x3.h>

namespace dy
{
class DMatrix4x4;
struct DColorRGB;
} /// ::dy namespace

namespace dy::math
{
//struct DMatrix3<TReal>;
//struct DMatrix4<TReal>;
}

namespace dy::reflect
{

enum class EReflectScopeType 
{
  Int, 
  UInt,
  Float, 
  Double,
  String, 
  ComplexFloat, 
  Vector2,
  Vector3,
  Vector4,
  VectorInt2,
  VectorInt3,
  VectorInt4,
  Matrix2,
  Matrix3,
  Matrix4,
  ColorRGB,
  __Error
};

/// @brief Convert given reflect type to uniform variable type.
[[nodiscard]] EUniformVariableType ToUniformVariableType(EReflectScopeType iReflectType);

template <EReflectScopeType TValue> struct ToActualType;
template <typename TType> struct ToVariableType;

#define REFLECTION_REGISTER_TYPE(__MAEnum__, __MAType__) \
  template <> struct ToActualType<EReflectScopeType::__MAEnum__> { using Type = __MAType__; }; \
  template <> struct ToVariableType<__MAType__> { static constexpr auto Value = EReflectScopeType::__MAEnum__; };

REFLECTION_REGISTER_TYPE(Int,     int)
REFLECTION_REGISTER_TYPE(UInt,    unsigned int)
REFLECTION_REGISTER_TYPE(Float,   float)
REFLECTION_REGISTER_TYPE(Double,  double)
REFLECTION_REGISTER_TYPE(String,  std::string)
REFLECTION_REGISTER_TYPE(Vector2, DVec2)
REFLECTION_REGISTER_TYPE(Vector3, DVec3)
REFLECTION_REGISTER_TYPE(Vector4, DVec4)
REFLECTION_REGISTER_TYPE(Matrix2, DMat2)
REFLECTION_REGISTER_TYPE(Matrix3, DMat3)
REFLECTION_REGISTER_TYPE(Matrix4, DMatrix4x4)
REFLECTION_REGISTER_TYPE(VectorInt2, DIVec2)
REFLECTION_REGISTER_TYPE(VectorInt3, DIVec3)
REFLECTION_REGISTER_TYPE(VectorInt4, DIVec4)
REFLECTION_REGISTER_TYPE(ColorRGB, DColorRGB)

} /// ::dy::reflect namespace

#endif /// GUARD_DY_CORE_REFLECTION_INTERNAL_EREFLECTIONTYPE_H
