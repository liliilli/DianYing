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

#include <Dy/Core/Reflection/Internal/EReflectionType.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy::reflect
{

EUniformVariableType ToUniformVariableType(EReflectScopeType iReflectType)
{
  using EReturnType = EUniformVariableType;

  switch (iReflectType) 
  { 
  case EReflectScopeType::Int:      return EReturnType::Integer;
  case EReflectScopeType::Float:    return EReturnType::Float;
  case EReflectScopeType::Vector2:  return EReturnType::Vector2;
  case EReflectScopeType::Vector3:  return EReturnType::Vector3;
  case EReflectScopeType::Vector4:  return EReturnType::Vector4;
  case EReflectScopeType::VectorInt2: return EReturnType::IVec2;
  case EReflectScopeType::VectorInt4: return EReturnType::IVec4;
  case EReflectScopeType::Matrix3:    return EReturnType::Matrix3;
  case EReflectScopeType::Matrix4:    return EReturnType::Matrix4;
  case EReflectScopeType::ColorRGB:   return EReturnType::Vector3;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(EUniformVariableType::NoneError);
  }
}

} /// ::dy::reflect namespace