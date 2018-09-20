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
/// @enum EDyAttributeVariableType
/// @brief Shader internal variable type for attributes.
///
enum class EDyAttributeVariableType
{
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
  Float,
  NoneError,
};

///
/// @enum EDyUniformVariableType
/// @brief Shader internal variable type for uniform and textures.
///
enum class EDyUniformVariableType
{
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
  Texture1D,
  Texture2D,
  NoneError
};

///
/// @enum EDyShaderFragmentType
/// @brief Shader fragment type. each fragment values are binded to each shader code prior to linking.
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
/// @struct DDyAttributeVariableInformation
/// @brief Store attribute variable information.
/// @todo NOT USED IN PRODUCTION CODE YET.
///
struct DDyAttributeVariableInformation final
{
  std::string               mVariableName     = "";
  int32_t                   mVariableSlotSize = 0;
  EDyAttributeVariableType  mVariableType     = EDyAttributeVariableType::NoneError;
  int32_t                   mVariableLocation = 0;
};

///
/// @struct DDyUniformVariableInformation
/// @brief Store uniform variable information.
/// @todo NOT USED IN PRODUCTION CODE YET.
///
struct DDyUniformVariableInformation final
{
  std::string               mVariableName     = "";
  int32_t                   mVariableSlotSize = 0;
  EDyUniformVariableType    mVariableType     = EDyUniformVariableType::NoneError;
  int32_t                   mVariableLocation = 0;
};

///
/// @struct PDyShaderFragmentInformation
/// @brief Shader fragment information type for constructing shader program, resource informations.
///
struct PDyShaderFragmentInformation final
{
  EDyShaderFragmentType mShaderType;
  std::string           mShaderPath = "";
};

///
/// @struct PDyShaderConstructionDescriptor
/// @brief Actual shader construction descriptor type.
///
struct PDyShaderConstructionDescriptor final
{
  std::string                               mShaderName       = "";
  std::vector<PDyShaderFragmentInformation> mShaderFragments  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H