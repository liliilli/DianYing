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
#include <Dy/Meta/Type/EDyComponentTypes.h>
#include <nlohmann/json.hpp>

#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>
#include <Dy/Helper/Internal/XStringSwitch.h>
#include <Dy/Helper/System/Assertion.h>

//!
//! Forward declaration & local translation unit.
//!

namespace
{

/// @brief  Get additional type for constructing properties of object.
/// @param  typeString Dependency type string from json.
/// @return Dependency component properties type
MDY_NODISCARD dy::EDyComponentMetaType
DyGetComponentTypeFrom(const std::string& typeString) noexcept
{
  switch (dy::string::Input(typeString))
  {
  case dy::string::Case("DirectionalLight"): return dy::EDyComponentMetaType::DirectionalLight; 
  case dy::string::Case("PointLight"):       return dy::EDyComponentMetaType::PointLight;
  case dy::string::Case("SpotLight"):        return dy::EDyComponentMetaType::SpotLight;
  case dy::string::Case("Script"):           return dy::EDyComponentMetaType::Script; 
  case dy::string::Case("Transform"):        return dy::EDyComponentMetaType::Transform; 
  case dy::string::Case("ModelFilter"):      return dy::EDyComponentMetaType::ModelFilter;
  case dy::string::Case("ModelRenderer"):    return dy::EDyComponentMetaType::ModelRenderer; 
  case dy::string::Case("ModelAnimator"):    return dy::EDyComponentMetaType::ModelAnimator;
  case dy::string::Case("Camera"):           return dy::EDyComponentMetaType::Camera;
  case dy::string::Case("SoundSource"):      return dy::EDyComponentMetaType::SoundSource;
  case dy::string::Case("PhysicsRigidbody"): return dy::EDyComponentMetaType::Rigidbody;
  case dy::string::Case("PhysicsCollider"):  return dy::EDyComponentMetaType::Collider;
  case dy::string::Case("Skybox"):           return dy::EDyComponentMetaType::Skybox;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(dy::EDyComponentMetaType::NoneError);
  }
}

/// @brief  Get EDyShadowType from valid std::string.
/// @return Shadow type, if not found just cause assertion.
MDY_NODISCARD dy::EDyShadowType DyGetShadowTypeFromString(const std::string& value)
{
  using dy::EDyShadowType;
  static MDY_SET_IMMUTABLE_STRING(sHardShadow, "Hard");
  static MDY_SET_IMMUTABLE_STRING(sSoftShadow, "Soft");

  if (value == sHardShadow)       { return EDyShadowType::Hard; }
  else if (value == sSoftShadow)  { return EDyShadowType::Soft; }

  MDY_UNEXPECTED_BRANCH();
  return EDyShadowType::__Error;
}

}

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyComponentMetaType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyComponentMetaType& p)
{
  p = DyGetComponentTypeFrom(j.get<std::string>());
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyShadowType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyShadowType& p)
{
  p = DyGetShadowTypeFromString(j.get<std::string>());
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyCameraProjectionType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyCameraProjectionType& p)
{
  const auto cameraModeString = j.get<std::string>();

  if      (cameraModeString == sValuePerspective)   { p = EDyCameraProjectionType::Perspecitve; }
  else if (cameraModeString == sValueOrthographic)  { p = EDyCameraProjectionType::Orthographic; }
  else
  {
    MDY_UNEXPECTED_BRANCH();
    p = EDyCameraProjectionType::NoneError;
  }
}

} /// ::dy namespace