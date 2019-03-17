#ifndef GUARD_DY_META_DESCRIPTOR_WIDGETLAYOUTMETAINFORMATION_H
#define GUARD_DY_META_DESCRIPTOR_WIDGETLAYOUTMETAINFORMATION_H
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

#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Helper/Type/DArea2D.h>

namespace dy
{

///
/// @struct PDyMetaWidgetHorizontalLayout
/// @brief Horizontal-aligned layout component of widget descriptor
///
struct PDyMetaWidgetHorizontalLayout final : public PDyMetaWidgetCommonBaseDesc
{
  DArea2D               mArea       = {};
  EDyHorizontalAlignment  mAlignment  = EDyHorizontalAlignment::Left;

  ///
  /// @brief Factory function for PDyMetaWidgetHorizontalLayout.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetHorizontalLayout.
  ///
  static std::unique_ptr<PDyMetaWidgetHorizontalLayout> CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

///
/// @struct PDyMetaWidgetVerticalLayout
/// @brief Vertical-aligned layout component of widget descriptor
///
struct PDyMetaWidgetVerticalLayout final : public PDyMetaWidgetCommonBaseDesc
{
  DArea2D               mArea       = {};
  EDyVerticalAlignment    mAlignment  = EDyVerticalAlignment::Top;

  ///
  /// @brief Factory function for PDyMetaWidgetVerticalLayout.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetVerticalLayout.
  ///
  static std::unique_ptr<PDyMetaWidgetVerticalLayout> CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_META_DESCRIPTOR_WIDGETLAYOUTMETAINFORMATION_H