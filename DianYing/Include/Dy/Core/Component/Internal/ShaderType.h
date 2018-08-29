#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <vector>

namespace dy
{

///
/// @enum EDyUniformVariableType
/// @brief Shader internal variable type for uniform and textures.
///
enum class EDyUniformVariableType {
  Matrix4 = 0,
  Matrix3,
  Matrix2,
  Vector4,
  Vector3,
  Vector2,
  IVec4,
  IVec3,
  IVec2,
  Integer,
  IntegerPointer,
  Float,
  FloatPointer,
  Texture2D
};

///
/// @enum EDyShaderFragmentType
/// @brief
///
enum class EDyShaderFragmentType
{
  Vertex,
  TesselationControl,
  TesselationEvaluation,
  Geometry,
  Pixel
};

///
/// @brief
///
struct PDyShaderFragmentInformation final
{
  EDyShaderFragmentType mShaderType;
  std::string           mShaderPath = "";
};

///
/// @struct PDyShaderConstructionDescriptor
/// @brief
///
struct PDyShaderConstructionDescriptor final
{
  std::vector<PDyShaderFragmentInformation> mShaderFragments;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H