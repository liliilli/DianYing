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
#include <Dy/Core/Resource/Internal/XShaderTypes.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Internal/XStringSwitch.h>
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

EUniformVariableType ToUniformItemType(EUniformVariableType iType)
{
  using EUniform = EUniformVariableType;
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

void to_json(nlohmann::json& oJson, const EUniformVariableType& iUniformType)
{
  std::string typeString = "";
  switch (iUniformType)
  {
  case EUniformVariableType::Matrix4: typeString = sMatrix4; break;
  case EUniformVariableType::Matrix3: typeString = sMatrix3; break;
  case EUniformVariableType::Matrix2: typeString = sMatrix2; break;
  case EUniformVariableType::Vector4: typeString = sVector4; break;
  case EUniformVariableType::Vector3: typeString = sVector3; break;
  case EUniformVariableType::Vector2: typeString = sVector2; break;
  case EUniformVariableType::IVec4: typeString = sVectorInt4; break;
  case EUniformVariableType::IVec3: typeString = sVectorInt3; break;
  case EUniformVariableType::IVec2: typeString = sVectorInt2; break;
  case EUniformVariableType::Integer: typeString = sInteger; break;
  case EUniformVariableType::Unsigned: typeString = sUnsigned; break;
  case EUniformVariableType::Float: typeString = sFloat; break;
  case EUniformVariableType::Bool: typeString = sBool; break;
  case EUniformVariableType::Matrix4Array: typeString = sArrayMatrix4; break;
  case EUniformVariableType::Vector3Array: typeString = sArrayVector3; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  oJson = typeString;
}

void from_json(const nlohmann::json& iJson, EUniformVariableType& iUniformType)
{
  const auto typeString = iJson.get<std::string>();
  switch (SwitchStrInput(ToLowerCopy(typeString)))
  {
  case CaseStr(sMatrix4): iUniformType = EUniformVariableType::Matrix4; break;
  case CaseStr(sMatrix3): iUniformType = EUniformVariableType::Matrix3; break;
  case CaseStr(sMatrix2): iUniformType = EUniformVariableType::Matrix2; break;
  case CaseStr(sVector4): iUniformType = EUniformVariableType::Vector4; break;
  case CaseStr(sVector3): iUniformType = EUniformVariableType::Vector3; break;
  case CaseStr(sVector2): iUniformType = EUniformVariableType::Vector2; break;
  case CaseStr(sVectorInt4): iUniformType = EUniformVariableType::IVec4; break;
  case CaseStr(sVectorInt3): iUniformType = EUniformVariableType::IVec3; break;
  case CaseStr(sVectorInt2): iUniformType = EUniformVariableType::IVec2; break;
  case CaseStr(sInteger): iUniformType = EUniformVariableType::Integer; break;
  case CaseStr(sUnsigned): iUniformType = EUniformVariableType::Unsigned; break;
  case CaseStr(sFloat): iUniformType = EUniformVariableType::Float; break;
  case CaseStr(sBool): iUniformType = EUniformVariableType::Bool; break;
  case CaseStr(sArrayMatrix4): iUniformType = EUniformVariableType::Matrix4Array; break;
  case CaseStr(sArrayVector3): iUniformType = EUniformVariableType::Vector3Array; break;
  case CaseStr(sArrayFloat): iUniformType = EUniformVariableType::FloatArray; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

} /// ::dy namespace