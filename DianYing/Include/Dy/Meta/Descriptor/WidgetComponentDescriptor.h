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
#include <Dy/Helper/Type/Color.h>
#include <Dy/Meta/Descriptor/WidgetCommonDescriptor.h>

namespace dy
{

///
/// @struct PDyMetaWidgetTextDescriptor
/// @brief Text component of widget descriptor
///
struct PDyMetaWidgetTextDescriptor final : public PDyMetaWidgetCommonBaseDesc
{
  /// UCS-2 string
  DDyString     mInitialString      = MDY_INITILAIZE_EMPTYSTR;
  /// Initial position of canvas.
  DDyVectorInt2 mInitialPosition    = {};
  /// InitialColor(rgb) + InitialAlpha(a)
  DDyColor      mInitialColor       = DDyColor::White;
  /// FontSize
  TU32          mFontSize           = MDY_INITIALIZE_DEFUINT;
  ///
  TU32          mEdgeSize           = MDY_INITIALIZE_DEFUINT;
  ///
  std::string   mFontSpecifierName  = MDY_INITILAIZE_EMPTYSTR;
  ///
  bool          mIsBold             = false;
  ///
  bool          mIsItalic           = false;
  ///
  bool          mIsUsingEdge        = false;
  ///
  bool          mIsUsingDefaultFont = true;

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