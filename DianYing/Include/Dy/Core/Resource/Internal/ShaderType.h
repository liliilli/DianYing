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
#include <Dy/Meta/Type/EDyResourceTypes.h>

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
  Bool,
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
  Bool,
  Texture1D,
  Texture2D,
  NoneError,
  Texture2DArray
};

///
/// @struct DDyAttributeVariableInformation
/// @brief Store attribute variable information.
/// @todo NOT USED IN PRODUCTION CODE YET.
///
struct DDyAttributeVariableInformation final
{
  std::string               mVariableName     = MDY_INITIALIZE_EMPTYSTR;
  TI32                      mVariableSlotSize = MDY_INITIALIZE_DEFINT;
  EDyAttributeVariableType  mVariableType     = EDyAttributeVariableType::NoneError;
  TI32                      mVariableLocation = MDY_INITIALIZE_DEFINT;
};

///
/// @struct DDyUniformVariableInformation
/// @brief Store uniform variable information.
/// @todo NOT USED IN PRODUCTION CODE YET.
///
struct DDyUniformVariableInformation final
{
  std::string               mVariableName     = MDY_INITIALIZE_EMPTYSTR;
  TI32                      mVariableSlotSize = MDY_INITIALIZE_DEFINT;
  EDyUniformVariableType    mVariableType     = EDyUniformVariableType::NoneError;
  TI32                      mVariableLocation = MDY_INITIALIZE_DEFINT;
};

///
/// @struct DDyUniformBufferObjectInformation
///
struct DDyUniformBufferObjectInformation final
{
  std::string               mUboSpecifierName = MDY_INITIALIZE_EMPTYSTR;
};

///
/// @struct PDyShaderFragmentInformation
/// @brief Shader fragment information type for constructing shader program, resource informations.
///
struct PDyShaderFragmentInformation final
{
  EDyShaderFragmentType mShaderType         = EDyShaderFragmentType::Vertex;
  std::string           mShaderFragmentCode = MDY_INITIALIZE_EMPTYSTR;

  /// @deprecated If enabled, loader will load mShaderFragmentCode, but has nullptr throw assert.
  bool                  mIsEnabledRawLoadShaderCode_Deprecated = false;
  /// @deprecated I/O Shader fragment file path when mIsEnabledRawLoadShaderCode_Deprecated false (by default)
  std::string           mShaderPath_Deprecated      = "";
};

///
/// @struct PDyShaderConstructionDescriptor
/// @brief Actual shader construction descriptor type.
///
struct PDyShaderConstructionDescriptor final
{
  std::string                               mShaderName       = MDY_INITIALIZE_EMPTYSTR;
  std::vector<PDyShaderFragmentInformation> mShaderFragments  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H