#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H
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
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Meta/Type/EDyResourceTypes.h>

namespace dy
{

/// @enum EAttributeVariableType
/// @brief Shader internal variable type for attributes.
enum class EAttributeVariableType
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

/// @enum EUniformVariableType
/// @brief Shader internal variable type for uniform and textures.
enum class EUniformVariableType
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
  Unsigned,
  Float,
  FloatArray,
  Bool,
  Texture1D,
  Texture2D,
  NoneError,
  Texture2DArray,
  Texture2DShadow,
  Texture2DShadowArray,

  Matrix4Array,
  Vector3Array,
  Texture2DRectangle, // Could not create mipmap from this, and texel retriving uv is texuture's width and height.
  Texture2DCubemap,   // GL_SAMPLER_CUBE (samplerCube)
};

/// @brief Convert uniform array type to uniform item type.
/// If given argument is already item type, do not but just return iType.
MDY_NODISCARD EUniformVariableType ToUniformItemType(EUniformVariableType iType);

void to_json(nlohmann::json& oJson, const EUniformVariableType& iUniformType);
void from_json(const nlohmann::json& iJson, EUniformVariableType& iUniformType);

/// @struct DAttributeVariableInformation
/// @brief Store attribute variable information.
struct DAttributeVariableInformation final
{
  std::string            mVariableName;
  TU32                   mVariableSlotSize = 0;
  EAttributeVariableType mVariableType     = EAttributeVariableType::NoneError;
  TU32                   mVariableLocation = 0;

  DAttributeVariableInformation(
    const std::string& iSpecifier,
    const TU32 iSlotSize,
    const EAttributeVariableType iType,
    const TU32 iLocation) 
    : mVariableName {iSpecifier},
      mVariableSlotSize { iSlotSize },
      mVariableType {iType},
      mVariableLocation {iLocation} 
  { };
};

/// @struct DUniformVariableInformation
/// @brief Store uniform variable information.
struct DUniformVariableInformation final
{
  std::string          mVariableName;
  TU32                 mVariableSlotSize = 0;
  EUniformVariableType mVariableType     = EUniformVariableType::NoneError;
  TU32                 mVariableLocation = 0;

  DUniformVariableInformation(
    const std::string& iSpecifier,
    const TU32 iSlotSize,
    const EUniformVariableType iType,
    const TU32 iLocation) 
    : mVariableName {iSpecifier},
      mVariableSlotSize { iSlotSize },
      mVariableType {iType},
      mVariableLocation {iLocation} {};
};

struct DUniformStructVarInformation final
{
  std::vector<DUniformVariableInformation> mMemberValues;
};

/// @struct DUniformBufferObjectInformation
/// @brief 
struct DUniformBufferObjectInformation final
{
  std::string mUboSpecifierName = MDY_INITIALIZE_EMPTYSTR;

  DUniformBufferObjectInformation(const std::string& iName) 
    : mUboSpecifierName { iName } {};
};

/// @struct PShaderFragmentInformation
/// @brief Shader fragment information type for constructing shader program, resource informations.
struct PShaderFragmentInformation final
{
  EDyShaderFragmentType mShaderType = EDyShaderFragmentType::Vertex;
  std::string           mShaderFragmentCode;

  /// @deprecated If enabled, loader will load mShaderFragmentCode, but has nullptr throw assert.
  bool                  mIsEnabledRawLoadShaderCode_Deprecated = false;
  /// @deprecated I/O Shader fragment file path when mIsEnabledRawLoadShaderCode_Deprecated false (by default)
  std::string           mShaderPath_Deprecated;
};

/// @struct PyShaderConstructionDescriptor
/// @brief Actual shader construction descriptor type.
struct PyShaderConstructionDescriptor final
{
  std::string                             mShaderName;
  std::vector<PShaderFragmentInformation> mShaderFragments = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_SHADER_TYPE_H