#ifndef GUARD_DY_META_DESCRIPTOR_WIDGETIMAGEMETAINFORMATION_H
#define GUARD_DY_META_DESCRIPTOR_WIDGETIMAGEMETAINFORMATION_H
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
#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Helper/Type/DColorRGBA.h>

namespace dy
{

///
/// @struct PDyMetaWidgetImageDescriptor
/// @brief image(`FWidgetImage`) component of widget descriptor
///
struct PDyMetaWidgetImageDescriptor final : public PDyMetaWidgetCommonBaseDesc
{
  /// Size (px) of text widget (leaf widget).
  DIVec2 mWidgetSize = {};
  /// Initial position of canvas.
  DIVec2 mInitialPosition = {};
  /// Originable to parent frame (except for list layout)
  EDyOrigin     mOrigin = EDyOrigin::Center_Center;

  /// Image specifier name from texture.
  std::string   mImageSpecifierName = {};
  /// Region to display on widget size. \n
  /// If `mIsSizeToContent` is true, widget will be realigned following partial region.
  DArea2D     mImageRegion        = {};
  /// ImageTintColor(rgb) + ImageAlpha(a)
  DColorRGBA  mTintColor          = DColorRGBA::White;
  /// Check use automatically align size to image(texture) size.
  bool          mIsSizeToContent    = false;

  ///
  /// @brief Factory function for PDyMetaWidgetImageDescriptor.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetImageDescriptor.
  ///
  static std::unique_ptr<PDyMetaWidgetImageDescriptor> 
  CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_META_DESCRIPTOR_WIDGETIMAGEMETAINFORMATION_H