#ifndef GUARD_DY_ELEMENT_CANVAS_FDYIMAGE_H
#define GUARD_DY_ELEMENT_CANVAS_FDYIMAGE_H
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

#include <Dy/Element/Canvas/AWidgetObject.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>
#include <Dy/Component/Internal/WidgetRenderer/CRendererImage.h>

namespace dy
{

/// @class FWidgetImage
/// @brief This class display image on given position.
class FWidgetImage final : public AWidgetObject, public IInitializeHelper<PDyMetaWidgetImageDescriptor>
{
  MDY_SET_CRC32_HASH_WITH_TYPE(FWidgetImage);
  MDY_SET_TYPEMATCH_FUNCTION(AWidgetObject, FWidgetImage);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FWidgetImage);
public:
  FWidgetImage() : mRenderer{*this} {};
  virtual ~FWidgetImage() = default;

  ///
  /// @brief Initialize FActor.
  /// @param objectMetaDesc Meta descriptor information instance for FWidgetImage.
  /// @return Success / Failure flag.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyMetaWidgetImageDescriptor& objectMetaDesc) override;

  ///
  /// @brief Release function (virtual) because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  ///
  MDY_NODISCARD void Release() override;

  std::string ToString() override final;
  void Render() override final;

  /// @brief Set image name. ( texture or materal for UI using )
  void SetRenderableImageName(_MIN_ const std::string& iName, _MIN_ bool iIsMaterial = false) noexcept;
  /// @brief Get image name with material using flag.
  MDY_NODISCARD std::pair<const std::string&, bool> GetRenderableImageName() const noexcept;
  /// @brief Check this image using material.
  MDY_NODISCARD bool IsUsingMaterial() const noexcept;
  /// @brief Try get using material. If image use texture, just return nullptr.
  MDY_NODISCARD TDyResourceBinderMaterial* GetUsingMaterial();

  /// @brief Set tint color.
  void SetTintColor(_MIN_ const DColorRGBA& iTintColor) noexcept;
  /// @brief Get tint color.
  MDY_NODISCARD const DColorRGBA& GetTintColor() const noexcept;


  /// @brief Set update renderer flag from inside renderer to this.
  void MDY_PRIVATE(SetUpdateRendererFlag)(_MIN_ bool iIsActivated) noexcept;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  /// Tint color for image rendering (with alpha)
  DColorRGBA      mTintColor = DColorRGBA::White;
  /// Size to content.
  bool              mIsSizeToContent = false;
  /// Image name list
  std::string       mImageName  = MDY_INITIALIZE_EMPTYSTR;
  bool              mIsMaterial = false;
  /// Renderer for this object.
  CRendererImage  mRenderer;
  bool              mIsUpdateRenderer = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_FDYIMAGE_H
