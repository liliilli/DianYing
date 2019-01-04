#ifndef GUARD_DY_META_DESCRIPTOR_WIDGETCOMPONENTDESCRIPTOR_H
#define GUARD_DY_META_DESCRIPTOR_WIDGETCOMPONENTDESCRIPTOR_H
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

#include <Dy/Helper/Type/DyString.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>

namespace dy
{

///
/// @struct PDyMetaWidgetTextDescriptor
/// @brief Text component of widget descriptor
///
struct PDyMetaWidgetTextDescriptor final : public PDyMetaWidgetCommonBaseDesc
{
  /// Size (px) of text widget (leaf widget).
  DDyVectorInt2 mWidgetSize         = {};
  /// Initial position of canvas.
  DDyVectorInt2 mInitialPosition    = {};
  /// Originable to parent frame (except for list layout)
  EDyOrigin     mOrigin             = EDyOrigin::Center_Center;

  /// UCS-2 string
  DDyString     mInitialString      = MDY_INITIALIZE_EMPTYSTR;
  /// InitialColor(rgb) + InitialAlpha(a)
  DDyColorRGBA  mInitialColor       = DDyColorRGBA::White;
  /// Edge color of text. `mIsUsingEdge` must be enabled to use this.
  DDyColorRGBA  mEdgeColor          = DDyColorRGBA{0, 0, 0, 0};
  /// Background color of text. `mIsUsingBackground` must be enabled to use this.
  DDyColorRGBA  mBackgroundColor    = DDyColorRGBA{0, 0, 0, 0};
  /// Font Size (px?)
  TU32          mFontSize           = MDY_INITIALIZE_DEFUINT;
  /// If empty, just use default font.
  std::string   mFontSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  /// Check rendering edge to text.
  bool          mIsUsingEdge        = false;
  /// Check use background outside of font region.
  bool          mIsUsingBackground  = false;

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