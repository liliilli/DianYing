#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Resource/Internal/ShaderType.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/StringSwitch.h>
#include <Dy/Helper/Library/HelperString.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(sMatrix4, "matrix4");
MDY_SET_IMMUTABLE_STRING(sMatrix3, "matrix3");
MDY_SET_IMMUTABLE_STRING(sMatrix2, "matrix2");
MDY_SET_IMMUTABLE_STRING(sVector4, "vector4");
MDY_SET_IMMUTABLE_STRING(sVector3, "vector3");
MDY_SET_IMMUTABLE_STRING(sVector2, "vector2");
MDY_SET_IMMUTABLE_STRING(sVectorInt4, "vectorint4");
MDY_SET_IMMUTABLE_STRING(sVectorInt3, "vectorint3");
MDY_SET_IMMUTABLE_STRING(sVectorInt2, "vectorint2");
MDY_SET_IMMUTABLE_STRING(sInteger, "integer");
MDY_SET_IMMUTABLE_STRING(sUnsigned, "unsigned");
MDY_SET_IMMUTABLE_STRING(sFloat, "float");
MDY_SET_IMMUTABLE_STRING(sArrayFloat, "arrayfloat");
MDY_SET_IMMUTABLE_STRING(sBool, "bool");
MDY_SET_IMMUTABLE_STRING(sArrayMatrix4, "arraymatrix4");
MDY_SET_IMMUTABLE_STRING(sArrayVector3, "arrayvector3");

} /// ::unnamed namespace

namespace dy
{

EDyUniformVariableType ToUniformItemType(EDyUniformVariableType iType)
{
  using EUniform = EDyUniformVariableType;
  switch (iType) 
  { 
  case EUniform::IntegerPointer:  return EUniform::Integer;
  case EUniform::FloatArray:      return EUniform::Float;
  case EUniform::Texture2DArray:  return EUniform::Texture2D;
  case EUniform::Texture2DShadowArray: return EUniform::Texture2DShadow;
  case EUniform::Matrix4Array:    return EUniform::Matrix4;
  case EUniform::Vector3Array:    return EUniform::Vector3;
  default: return iType;
  }
}

void to_json(nlohmann::json& oJson, const EDyUniformVariableType& iUniformType)
{
  std::string typeString = "";
  switch (iUniformType)
  {
  case EDyUniformVariableType::Matrix4: typeString = sMatrix4; break;
  case EDyUniformVariableType::Matrix3: typeString = sMatrix3; break;
  case EDyUniformVariableType::Matrix2: typeString = sMatrix2; break;
  case EDyUniformVariableType::Vector4: typeString = sVector4; break;
  case EDyUniformVariableType::Vector3: typeString = sVector3; break;
  case EDyUniformVariableType::Vector2: typeString = sVector2; break;
  case EDyUniformVariableType::IVec4: typeString = sVectorInt4; break;
  case EDyUniformVariableType::IVec3: typeString = sVectorInt3; break;
  case EDyUniformVariableType::IVec2: typeString = sVectorInt2; break;
  case EDyUniformVariableType::Integer: typeString = sInteger; break;
  case EDyUniformVariableType::Unsigned: typeString = sUnsigned; break;
  case EDyUniformVariableType::Float: typeString = sFloat; break;
  case EDyUniformVariableType::Bool: typeString = sBool; break;
  case EDyUniformVariableType::Matrix4Array: typeString = sArrayMatrix4; break;
  case EDyUniformVariableType::Vector3Array: typeString = sArrayVector3; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  oJson = typeString;
}

void from_json(const nlohmann::json& iJson, EDyUniformVariableType& iUniformType)
{
  const auto typeString = iJson.get<std::string>();
  switch (DyStrSwitchInput(ToLowerCopy(typeString)))
  {
  case DyStrCase(sMatrix4): iUniformType = EDyUniformVariableType::Matrix4; break;
  case DyStrCase(sMatrix3): iUniformType = EDyUniformVariableType::Matrix3; break;
  case DyStrCase(sMatrix2): iUniformType = EDyUniformVariableType::Matrix2; break;
  case DyStrCase(sVector4): iUniformType = EDyUniformVariableType::Vector4; break;
  case DyStrCase(sVector3): iUniformType = EDyUniformVariableType::Vector3; break;
  case DyStrCase(sVector2): iUniformType = EDyUniformVariableType::Vector2; break;
  case DyStrCase(sVectorInt4): iUniformType = EDyUniformVariableType::IVec4; break;
  case DyStrCase(sVectorInt3): iUniformType = EDyUniformVariableType::IVec3; break;
  case DyStrCase(sVectorInt2): iUniformType = EDyUniformVariableType::IVec2; break;
  case DyStrCase(sInteger): iUniformType = EDyUniformVariableType::Integer; break;
  case DyStrCase(sUnsigned): iUniformType = EDyUniformVariableType::Unsigned; break;
  case DyStrCase(sFloat): iUniformType = EDyUniformVariableType::Float; break;
  case DyStrCase(sBool): iUniformType = EDyUniformVariableType::Bool; break;
  case DyStrCase(sArrayMatrix4): iUniformType = EDyUniformVariableType::Matrix4Array; break;
  case DyStrCase(sArrayVector3): iUniformType = EDyUniformVariableType::Vector3Array; break;
  case DyStrCase(sArrayFloat): iUniformType = EDyUniformVariableType::FloatArray; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace