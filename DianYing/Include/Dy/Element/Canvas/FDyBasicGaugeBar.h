#ifndef GUARD_DY_ELEMENT_CANVAS_FDYBASICGUAGEBAR_H
#define GUARD_DY_ELEMENT_CANVAS_FDYBASICGUAGEBAR_H
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

#include <Dy/Element/Canvas/UiObject.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Meta/Descriptor/WidgetBarMetaInformation.h>
#include <Dy/Component/Internal/CDyBasicGaugeBarRenderer.h>

namespace dy
{

///
/// @class FDyBasicGaugeBar
/// @brief This class display gauge bar horizontally with plain colors.
///
class FDyBasicGaugeBar : public FDyUiObject, public IDyInitializeHelper<PDyMetaWidgetBarDescriptor>
{
  MDY_SET_CRC32_HASH_WITH_TYPE(FDyBasicGaugeBar);
  MDY_SET_TYPEMATCH_FUNCTION(FDyUiObject, FDyBasicGaugeBar);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyBasicGaugeBar);
public:
  FDyBasicGaugeBar() = default;
  virtual ~FDyBasicGaugeBar() = default;

  ///
  /// @brief Initialize FDyActor.
  /// @param objectMetaDesc Meta descriptor information instance for FDyBasicGaugeBar.
  /// @return Success / Failure flag.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyMetaWidgetBarDescriptor& objectMetaDesc) override;

  ///
  /// @brief Release function (virtual) because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  ///
  MDY_NODISCARD void Release() override;

  ///
  /// @brief  Get present value of gauge.
  /// @return value of gauge on this frame.
  ///
  MDY_NODISCARD TF32 GetPresentValue() const noexcept;

  ///
  /// @brief  Get minimum value of gauge.
  /// @return minimum value of gauge on this frame.
  ///
  MDY_NODISCARD TF32 GetMinimumValue() const noexcept;

  ///
  /// @brief  Get maximum value of gauge.
  /// @return maximum value of gauge on this frame.
  ///
  MDY_NODISCARD TF32 GetMaximumValue() const noexcept;

  ///
  /// @brief  Get percentage value (0 ~ 1) of gauge.
  ///
  MDY_NODISCARD TF32 GetPercentage() const noexcept;

  ///
  /// @brief  Get DDyColor foreground color of component.
  /// @return Return foreground color.
  ///
  MDY_NODISCARD const DDyColorRGBA& GetForegroundColor() const noexcept;

  ///
  /// @brief  Get DDyColor background color of component.
  /// @return Return background color.
  ///
  MDY_NODISCARD const DDyColorRGBA& GetBackgroundColor() const noexcept;

  ///
  /// @brief  Get position to be rendered in UI.
  /// @return Final rendered posiiton.
  ///
  MDY_NODISCARD const DDyVector2& GetRenderPosition() const noexcept;

  ///
  /// @brief  Get padding value of this bar.
  ///
  MDY_NODISCARD const TI32 GetPadding() const noexcept;

  ///
  /// @brief  Check if this component is using background color.
  /// @return If used, return true or false.
  ///
  FORCEINLINE MDY_NODISCARD bool CheckIsUsingBackgroundColor() const noexcept
  {
    return this->mIsUsingBackgroundColor;
  }

  ///
  /// @brief Set present gauge value.
  /// @param value gauge value.
  ///
  void SetPresentValue(_MIN_ const TF32 value) noexcept;

  ///
  /// @brief Set string color, single color with composited of {r, g, b, a}.
  /// @param[in] color New color value with consists of {r, g, b, a} glm::vec4 vector.
  /// each value must be in range of [0, 1], otherwise clamped to 0 or 1.
  ///
  void SetForegroundColor(_MIN_ const DDyColorRGBA& color);

  ///
  /// @brief Set string color, single color with composited of {r, g, b, a}.
  /// @param[in] color New color value with consists of {r, g, b, a} glm::vec4 vector.
  /// each value must be in range of [0, 1], otherwise clamped to 0 or 1.
  ///
  void SetBackgroundColor(_MIN_ const DDyColorRGBA& color);

  ///
  /// @brief Render font (old-way)
  ///
  void Render() override final;

  ///
  /// @brief
  /// @return
  /// @TODO NOT IMPLEMENTED YET
  ///
  MDY_NODISCARD std::string ToString() override;

private:
  /// @brief Set transient variable, `mValues::mPercentage`.
  void pUpdatePercentage();

  struct DValue final
  {
    TF32 mMin     = MDY_INITIALIZE_DEFINT;
    TF32 mMax     = MDY_INITIALIZE_DEFINT;
    TF32 mPresent = MDY_INITIALIZE_DEFINT;

    MDY_TRANSIENT TF32 mPercentage = 0.0f;
  };

  /// Foreground (bar) color
  DDyColorRGBA      mForegroundColor  = DDyColorRGBA::White;
  /// Bar region background color, `mIsUsingBackgroundColor` must be checked true for using.
  DDyColorRGBA      mBackgroundColor  = DDyColorRGBA{ 0, 0, 0, 0 };
  /// Values for describing bar.
  DValue            mValues           = {};
  /// Padding value for rendering foreground region.
  TI32              mPadding          = MDY_INITIALIZE_DEFINT;;
  /// Renderer for this object.
  CDyBasicGaugeBarRenderer mRenderer  = {};

  ///
  bool mIsUsingBackgroundColor = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_FDYBASICGUAGEBAR_H