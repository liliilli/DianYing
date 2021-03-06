#ifndef GUARD_DY_META_TYPE_EDYWIDGETTYPES_H
#define GUARD_DY_META_TYPE_EDYWIDGETTYPES_H
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

#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/Type/DVector2.h>

//!
//! Forward declaration
//!

namespace dy
{
using DIVec2 = math::DVector2<math::TI32>;
}

//!
//! Implementation
//!

namespace dy
{
///
/// @enum   EDyHorizontalAlignment
/// @brief
/// Usage
/// PDyMetaWidgetTextDescriptor   : Text alignment.
/// PDyMetaWidgetHorizontalLayout : Children(Leaf) widget alignment.
///
enum class EDyHorizontalAlignment
{
  Left,
  Center,
  Right
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyHorizontalAlignment& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyHorizontalAlignment& p);

///
/// @enum   EDyVerticalAlignment
/// @brief
/// Usage
/// PDyMetaWidgetVerticalLayout   : Children(Leaf) widget alignment.
///
enum class EDyVerticalAlignment
{
  Top,
  Center,
  Bottom
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyVerticalAlignment& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyVerticalAlignment& p);

///
/// @enum   EDyOrigin
/// @brief
/// Usage
/// PDyMetaWidgetTextDescriptor   : Layout alignment to `Border`.
///
/// This enum is need for calculating originable instance's final rendering position. \n
/// IOriginable instance has a feature of calculating final poisiton mechanism with UiObject, or MFontManager. \n
///
/// \(7)|(8)/(9) At first, method gets Origin value of child which is left and calculate \n
/// -(4).(5)-(6) with parent_x, y, width, height to be aligned Parent's region without \n
/// /(1)|(2)\(3) applying child's local position. \n
///
/// Afterward, this method sets final rendering position of child and apply changed value to
/// children of child calling SetUiParentPosition() of children of child.
///
enum class EDyOrigin
{
  Left_Bottom,
  Left_Center,
  Left_Top,

  Center_Bottom,
  Center_Center,
  Center_Top,

  Right_Bottom,
  Right_Center,
  Right_Top
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyOrigin& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyOrigin& p);

///
/// @brief  Get calculated position with origin.
/// @param
/// @param
/// @param
/// @return
///
MDY_NODISCARD DVec2 DyGetPositionWithOrigin(const DVec2& position, const DIVec2& frameSize, EDyOrigin origin);

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_EDYWIDGETTYPES_H