#ifndef GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATEUNIFORMTYPE_H
#define GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATEUNIFORMTYPE_H
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

#include <vector>
#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Helper/Type/VectorInt2.h>

namespace dy
{

template <EDyUniformVariableType TType>
struct MDY_PRIVATE_SPECIFIER(UniformBinder);

template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Matrix4Array> final
{ using ValueType = std::vector<DDyMatrix4x4>; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Vector3Array> final  
{ using ValueType = std::vector<DDyVector3>; };

template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Matrix4> final  { using ValueType = DDyMatrix4x4; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Matrix3> final  { using ValueType = DDyMatrix3x3; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Vector4> final  { using ValueType = DDyVector4; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Vector3> final  { using ValueType = DDyVector3; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Vector2> final  { using ValueType = DDyVector2; };
//template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyAttributeVariableType::IVec4> final  { using ValueType = DDyVectorInt4; };
//template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyAttributeVariableType::IVec3> final  { using ValueType = DDyVector4; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::IVec2> final    { using ValueType = DDyVectorInt2; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Integer> final  { using ValueType = TI32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Float> final    { using ValueType = TF32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Bool> final     { using ValueType = bool; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Texture1D>      final { using ValueType = TU32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Texture2D>      final { using ValueType = TU32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Texture2DArray> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Texture2DRectangle> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Texture2DShadowArray> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE_SPECIFIER(UniformBinder)<EDyUniformVariableType::Texture2DCubemap> final { using ValueType = TU32; };

template <typename TType, std::size_t Amount>
MDY_NODISCARD std::vector<TType> 
DyConvertToVector(_MIN_ const std::array<TType, Amount>& iInput)
{
  return std::vector<TType>{iInput.begin(), iInput.end()};
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATEUNIFORMTYPE_H