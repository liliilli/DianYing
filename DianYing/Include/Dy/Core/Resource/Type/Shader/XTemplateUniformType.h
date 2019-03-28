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
#include <Dy/Core/Resource/Internal/XShaderTypes.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Helper/Type/DVector2.h>

namespace dy
{

template <EUniformVariableType TType>
struct MDY_PRIVATE(UniformBinder);

template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Matrix4Array> final
{ using ValueType = std::vector<DMatrix4x4>; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Vector3Array> final  
{ using ValueType = std::vector<DVec3>; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::FloatArray> final
{ using ValueType = std::vector<TF32>; };

template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Matrix4> final  { using ValueType = DMatrix4x4; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Matrix3> final  { using ValueType = DMat3; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Vector4> final  { using ValueType = DVec4; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Vector3> final  { using ValueType = DVec3; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Vector2> final  { using ValueType = DVec2; };
//template <> struct MDY_PRIVATE(UniformBinder)<EAttributeVariableType::IVec4> final  { using ValueType = DIVec4; };
//template <> struct MDY_PRIVATE(UniformBinder)<EAttributeVariableType::IVec3> final  { using ValueType = DVec4; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::IVec2> final    { using ValueType = DIVec2; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Integer> final  { using ValueType = TI32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Unsigned> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Float> final    { using ValueType = TF32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Bool> final     { using ValueType = bool; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Texture1D>      final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Texture2D>      final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Texture2DArray> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Texture2DRectangle> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Texture2DShadowArray> final { using ValueType = TU32; };
template <> struct MDY_PRIVATE(UniformBinder)<EUniformVariableType::Texture2DCubemap> final { using ValueType = TU32; };

template <typename TType, std::size_t Amount>
MDY_NODISCARD std::vector<TType> 
DyConvertToVector(_MIN_ const std::array<TType, Amount>& iInput)
{
  return std::vector<TType>{iInput.begin(), iInput.end()};
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_TYPE_TEMPLATEUNIFORMTYPE_H