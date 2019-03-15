#ifndef GUARD_DY_META_DESCRIPTOR_WIDGETBARMETAINFORMATION_H
#define GUARD_DY_META_DESCRIPTOR_WIDGETBARMETAINFORMATION_H
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
#include <Dy/Helper/Type/DVectorInt2.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Helper/Type/DColorRGBA.h>

namespace dy
{

///
/// @struct PDyMetaWidgetBarDescriptor
/// @brief basic guage bar(`FWidgetBasicGaugeBar`) component of widget descriptor
///
struct PDyMetaWidgetBarDescriptor final : public PDyMetaWidgetCommonBaseDesc
{
  /// Size (px) of text widget (leaf widget).
  DVectorInt2 mWidgetSize       = {};
  /// Initial position of canvas.
  DVectorInt2 mInitialPosition  = {};
  /// Originable to parent frame (except for list layout)
  EDyOrigin     mOrigin           = EDyOrigin::Center_Center;

  /// ForegroundColor(rgb) + ForegroundAlpha(a)
  DColorRGBA  mForegroundColor  = DColorRGBA::White;
  /// Background color of text. `mIsUsingBackground` must be enabled to use this.
  DColorRGBA  mBackgroundColor  = DColorRGBA{ 0, 0, 0, 0 };
  /// Check use background outside of foreground region.
  bool          mIsUsingBackground= false;
  /// Min of bar gauge.
  TF32          mMin              = MDY_INITIALIZE_DEFINT;
  /// Max of bar gauge.
  TF32          mMax              = MDY_INITIALIZE_DEFINT;
  /// Padding from `mFrameSize` for displaying foreground region.
  TI32          mPadding          = MDY_INITIALIZE_DEFINT;

  ///
  /// @brief Factory function for PDyMetaWidgetBarDescriptor.
  /// @param itemAtlas valid json atlas instance.
  /// @return If succeeded, return heap instance of PDyMetaWidgetBarDescriptor.
  ///
  static std::unique_ptr<PDyMetaWidgetBarDescriptor>
      CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas);
};

} /// ::dy namespace

#endif /// GUARD_DY_META_DESCRIPTOR_WIDGETBARMETAINFORMATION_H