#include <precompiled.h>
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

/// Header file
#include <Dy/Meta/Type/EDyComponentTypes.h>
#include <nlohmann/json.hpp>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>

//!
//! Forward declaration & local translation unit.
//!

namespace
{

///
/// @brief  Get additional type for constructing properties of object.
/// @param  typeString Dependency type string from json.
/// @return Dependency component properties type
///
MDY_NODISCARD dy::EDyComponentMetaType
DyGetComponentTypeFrom(_MIN_ const std::string& typeString) noexcept
{
  static MDY_SET_IMMUTABLE_STRING(sDirectionalLight,  "DirectionalLight");
  static MDY_SET_IMMUTABLE_STRING(sScript,            "Script");
  static MDY_SET_IMMUTABLE_STRING(sTransform,         "Transform");
  static MDY_SET_IMMUTABLE_STRING(sModelFilter,       "ModelFilter");
  static MDY_SET_IMMUTABLE_STRING(sModelRenderer,     "ModelRenderer");
  static MDY_SET_IMMUTABLE_STRING(sCamera,            "Camera");

  if (typeString == sScript)           { return dy::EDyComponentMetaType::Script; }
  if (typeString == sDirectionalLight) { return dy::EDyComponentMetaType::DirectionalLight; }
  if (typeString == sTransform)        { return dy::EDyComponentMetaType::Transform; }
  if (typeString == sModelFilter)      { return dy::EDyComponentMetaType::ModelFilter; }
  if (typeString == sModelRenderer)    { return dy::EDyComponentMetaType::ModelRenderer; }
  if (typeString == sCamera)           { return dy::EDyComponentMetaType::Camera; }
  else                                 { MDY_UNEXPECTED_BRANCH(); }
  return dy::EDyComponentMetaType::NoneError;
}

///
/// @brief  Get EDyShadowType from valid std::string.
/// @param  value
/// @return Shadow type, if not found just cause assertion.
///
MDY_NODISCARD dy::EDyShadowType DyGetShadowTypeFromString(_MIN_ const std::string& value)
{
  using dy::EDyShadowType;
  static MDY_SET_IMMUTABLE_STRING(sHardShadow, "Hard");
  static MDY_SET_IMMUTABLE_STRING(sSoftShadow, "Soft");

  if (value == sHardShadow)       { return EDyShadowType::Hard; }
  else if (value == sSoftShadow)  { return EDyShadowType::Soft; }

  MDY_UNEXPECTED_BRANCH();
  return EDyShadowType::NoneError;
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