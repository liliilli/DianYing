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
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Helper/Type/DVectorInt2.h>

namespace dy
{

template <EDyUniformVariableType TType>
struct MDY_PRIVATE(UniformBinder);

template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Matrix4Array> final
{ using ValueType = std::vector<DMatrix4x4>; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Vector3Array> final  
{ using ValueType = std::vector<DVector3>; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::FloatArray> final
{ using ValueType = std::vector<TF32>; };

template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Matrix4> final  { using ValueType = DMatrix4x4; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Matrix3> final  { using ValueType = DMatrix3x3; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Vector4> final  { using ValueType = DVector4; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Vector3> final  { using ValueType = DVector3; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Vector2> final  { using ValueType = DVector2; };
//template <> struct MDY_PRIVATE(UniformBinder)<EDyAttributeVariableType::IVec4> final  { using ValueType = DVectorInt4; };
//template <> struct MDY_PRIVATE(UniformBinder)<EDyAttributeVariableType::IVec3> final  { using ValueType = DVector4; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::IVec2> final    { using ValueType = DVectorInt2; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Integer> final  { using ValueType = TI32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Unsigned> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Float> final    { using ValueType = TF32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Bool> final     { using ValueType = bool; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Texture1D>      final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Texture2D>      final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Texture2DArray> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Texture2DRectangle> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Texture2DShadowArray> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EDyUniformVariableType::Texture2DCubemap> final { using ValueType = TU32; };

template <typename TType, std::size_t Amount>
MDY_NODISCARD std::vector<TType> 
DyConvertToVector(_MIN_ const std::array<TType, Amount>& iInput)
{
  return std::vector<TType>{iInput.begin(), iInput.end()};
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATEUNIFORMTYPE_H