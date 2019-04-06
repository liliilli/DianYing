#ifndef GUARD_DY_META_DESCRIPTOR_WIDGETCOMPONENTDESCRIPTOR_H
#define GUARD_DY_META_DESCRIPTOR_WIDGETCOMPONENTDESCRIPTOR_H
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

#include <Dy/Helper/Type/DString.h>
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>

namespace dy
{

/// @struct PDyMetaWidgetTextDescriptor
/// @brief Text component of widget descriptor
struct PDyMetaWidgetTextDescriptor final : public PDyMetaWidgetCommonBaseDesc
{
  /// Size (px) of text widget (leaf widget).
  DIVec2 mWidgetSize         = {};
  /// Initial position of canvas.
  DIVec2 mInitialPosition    = {};
  /// Originable to parent frame (except for list layout)
  EDyOrigin   mOrigin             = EDyOrigin::Center_Center;

  /// UCS-2 string
  DString     mTextString;
  /// InitialColor(rgb) + InitialAlpha(a)
  DColorRGBA  mTextColor = DColorRGBA::White;
  /// Edge color of text. `mIsUsingEdge` must be enabled to use this.
  DColorRGBA  mEdgeColor = DColorRGBA{0, 0, 0, 0};
  /// Background color of text. `mIsUsingBackground` must be enabled to use this.
  DColorRGBA  mBackgroundColor = DColorRGBA{0, 0, 0, 0};
  /// Font Size (px?)
  TU32        mFontSize = 10;
  /// If empty, just use default font.
  std::string mFontName;
  /// Text alignment.
  EDyHorizontalAlignment mAlignment = EDyHorizontalAlignment::Left;
  /// Check rendering edge to text.
  bool        mIsUsingEdge        = false;
  /// Check use background outside of font region.
  bool        mIsUsingBackground  = false;

  ///
  /// @brief Factory function for PDyMetaWidgetTextDescriptor.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetTextDescriptor.
  ///
  static std::unique_ptr<PDyMetaWidgetTextDescriptor>
  CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_META_DESCRIPTOR_WIDGETCOMPONENTDESCRIPTOR_H