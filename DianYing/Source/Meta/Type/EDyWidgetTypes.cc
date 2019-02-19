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
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <nlohmann/json.hpp>

#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Helper/StringSwitch.h>

//!
//! Forward declaration & local translation unit data
//!

namespace
{

/// EDyHorizontalAlignment

MDY_SET_IMMUTABLE_STRING(sValue_Hori_Left,    "Left");
MDY_SET_IMMUTABLE_STRING(sValue_Hori_Center,  "Center");
MDY_SET_IMMUTABLE_STRING(sValue_Hori_Right,   "Right");

/// EDyVerticalAlignment

MDY_SET_IMMUTABLE_STRING(sValue_Vert_Bottom,  "Bottom");
MDY_SET_IMMUTABLE_STRING(sValue_Vert_Center,  "Center");
MDY_SET_IMMUTABLE_STRING(sValue_Vert_Top,     "Top");

/// EDyOrigin

MDY_SET_IMMUTABLE_STRING(sValue_Orig_Lef_Bot,  "Left_Bottom");
MDY_SET_IMMUTABLE_STRING(sValue_Orig_Lef_Cen,  "Left_Center");
MDY_SET_IMMUTABLE_STRING(sValue_Orig_Lef_Top,   "Left_Top");

MDY_SET_IMMUTABLE_STRING(sValue_Orig_Cen_Bot,  "Center_Bottom");
MDY_SET_IMMUTABLE_STRING(sValue_Orig_Cen_Cen,  "Center_Center");
MDY_SET_IMMUTABLE_STRING(sValue_Orig_Cen_Top,  "Center_Top");

MDY_SET_IMMUTABLE_STRING(sValue_Orig_Rig_Bot,  "Right_Bottom");
MDY_SET_IMMUTABLE_STRING(sValue_Orig_Rig_Cen,  "Right_Center");
MDY_SET_IMMUTABLE_STRING(sValue_Orig_Rig_Top,  "Right_Top");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyHorizontalAlignment& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyHorizontalAlignment& p)
{
  switch (DyStrSwitchInput(j.get<std::string>()))
  {
  case DyStrCase("Left"): { p = EDyHorizontalAlignment::Left; } break;
  case DyStrCase("Center"): { p = EDyHorizontalAlignment::Center; } break;
  case DyStrCase("Right"): { p = EDyHorizontalAlignment::Right; } break;
  default: MDY_UNEXPECTED_BRANCH();
  }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyVerticalAlignment& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyVerticalAlignment& p)
{
  const auto valueString = j.get<std::string>();

  if      (valueString == sValue_Vert_Bottom) { p = EDyVerticalAlignment::Bottom; }
  else if (valueString == sValue_Vert_Center) { p = EDyVerticalAlignment::Center; }
  else if (valueString == sValue_Vert_Top)    { p = EDyVerticalAlignment::Top; }
  else                                        { MDY_UNEXPECTED_BRANCH(); }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyOrigin& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyOrigin& p)
{
  const auto valueString = j.get<std::string>();

  if      (valueString == sValue_Orig_Lef_Bot)  { p = EDyOrigin::Left_Bottom; }
  else if (valueString == sValue_Orig_Lef_Cen)  { p = EDyOrigin::Left_Center; }
  else if (valueString == sValue_Orig_Lef_Top)  { p = EDyOrigin::Left_Top; }

  else if (valueString == sValue_Orig_Cen_Bot)  { p = EDyOrigin::Center_Bottom; }
  else if (valueString == sValue_Orig_Cen_Cen)  { p = EDyOrigin::Center_Center; }
  else if (valueString == sValue_Orig_Cen_Top)  { p = EDyOrigin::Center_Top; }

  else if (valueString == sValue_Orig_Rig_Bot)  { p = EDyOrigin::Right_Bottom; }
  else if (valueString == sValue_Orig_Rig_Cen)  { p = EDyOrigin::Right_Center; }
  else if (valueString == sValue_Orig_Rig_Top)  { p = EDyOrigin::Right_Top; }

  else                                          { MDY_UNEXPECTED_BRANCH(); }
}

DDyVector2 DyGetPositionWithOrigin(const DDyVector2& position, const DDyVectorInt2& frameSize, EDyOrigin origin)
{
  switch (origin)
  {
  case EDyOrigin::Left_Bottom:  return position - (frameSize / 2);
  case EDyOrigin::Left_Center:  return position - DDyVectorInt2{ frameSize.X >> 1, 0 };
  case EDyOrigin::Left_Top:     return position + DDyVectorInt2{ -(frameSize.X >> 1), frameSize.Y >> 1 };
  case EDyOrigin::Center_Bottom:return position - DDyVectorInt2{ 0, frameSize.Y >> 1 };
  case EDyOrigin::Center_Center:return position;
  case EDyOrigin::Center_Top:   return position + DDyVectorInt2{ 0, frameSize.Y >> 1 };
  case EDyOrigin::Right_Bottom: return position + DDyVectorInt2{ frameSize.X >> 1, -(frameSize.Y >> 1) };
  case EDyOrigin::Right_Center: return position + DDyVectorInt2{ frameSize.X >> 1, 0 };
  case EDyOrigin::Right_Top:    return position + (frameSize / 2);
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_UNEXPECTED_BRANCH();
  return {};
}

} /// ::dy namespace