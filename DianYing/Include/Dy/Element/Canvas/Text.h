#ifndef GUARD_DY_ELEMENT_CANVAS_H
#define GUARD_DY_ELEMENT_CANVAS_H
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

#include <Dy/Element/Object.h>
#include <Dy/Element/Descriptor/CanvasDescriptor.h>
#include <Dy/Helper/Type/DyString.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Management/Type/FontContainer.h>
#include "Dy/Component/Internal/CDyFontRenderer.h"

//!
//! Forward declaration
//!

namespace dy
{
class CFont2DRenderer;
} /// ::opgs16::component namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyText
/// @brief This class display text on position aligned with FontManager.
/// @log
/// 2018-04-17 Move ::canvas::FDyText to ::opgs16::element::canvas::FDyText.
/// 2018-05-28 Remove pointer to implementation idiom.
///
class FDyText final : public FDyObject, public IDyInitializeHelper<PDyMetaWidgetTextDescriptor>
{
  MDY_SET_CRC32_HASH_WITH_TYPE(FDyText);
  MDY_SET_TYPEMATCH_FUNCTION(FDyObject, FDyText);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyText);
public:
  ///
  /// @brief Text component (stand-alone) constructor.
  /// In initialization time of this class, initial text and position and color set.
  /// position parameter was based on screen or parent's size where component
  /// is in hierarchy structrue of parent.
  ///
	FDyText();
  virtual ~FDyText() = default;

  ///
  /// @brief
  /// @return
  /// @TODO NOT IMPLEMENTED YET
  ///
  MDY_NODISCARD std::string ToString() override;

  ///
  /// @brief Initialize FDyActor.
  /// @param objectMetaDesc Meta descriptor information instance for FDyActor.
  /// @return Success / Failure flag.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyMetaWidgetTextDescriptor& objectMetaDesc) override;

  ///
  /// @brief Release function (virtual) because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  ///
  MDY_NODISCARD void Release() override;

  ///
  /// @brief Get text string to display.
  /// @return Text string which this instance has.
  ///
  MDY_NODISCARD const DDyString& GetText() const noexcept;

  ///
  /// @brief  Return reference of valid font container.
  /// @return Valid font container reference.
  ///
  MDY_NODISCARD FDyFontContainer& GetFontContainer() const noexcept
  {
    return *this->mFontContainer;
  }

  ///
  /// @brief Get
  /// @TODO THIS FUNCTION IS NOT IMPLEMENED YET.
  ///
  MDY_NODISCARD std::string GetUtf8Text() const noexcept;

  ///
  /// @brief Get font size which this instance has.
  /// @return unsigned font size value.
  ///
  MDY_NODISCARD TU32 GetFontSize() const noexcept;

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD const DDyColor& GetColor() const noexcept;

  ///
  /// @brief Set text string to display.
  /// @param[in] new_text New text string.
  ///
	void SetText(_MIN_ const std::string& new_text) noexcept;

	///
	/// @brief Set font size of string.
  /// This method has side-effect for accessing FontManager.
	/// @param fontSize
	///
	void SetFontSize(_MIN_ TU32 fontSize) noexcept;

	///
	/// @brief Set this object font to render with.
	/// If FontManager doesn't find font, return false to inform method was failed.
	/// @param[in] fontName Font name tag.
	/// @return Success flag.
	///
	EDySuccess SetFontName(_MIN_ const std::string& fontName);

  ///
  /// @brief Set string color, single color with composited of {r, g, b}.
  ///
  /// @param[in] color New color value with consists of {r, g, b} glm::vec3 vector.
  /// each value must be in range of [0, 1], otherwise clamped to 0 or 1.
  ///
  void SetColor(_MIN_ const DDyColor& color);

private:
  /// Text to display on screen. String must be following UTF-8 encoding.
  DDyString         mTextString = {""};
  /// Text container instance to display font.
  FDyFontContainer* mFontContainer = nullptr;
  /// Font color
	DDyColor          mFontColor  = DDyColor::White;
  /// Font size
  TU32              mFontSize   = 16;

  CDyFontRenderer   mRenderer = {};

  bool mIsTextDirty  = true;
  bool mIsColorDirty = true;
};

} /// ::dy namespace.

#endif /// GUARD_DY_ELEMENT_CANVAS_TEXT_H
