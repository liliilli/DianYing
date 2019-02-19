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

#include <Dy/Element/Canvas/UiObject.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Meta/Descriptor/WidgetImageMetaInformation.h>
#include <Dy/Component/Internal/CDyImageRenderer.h>

namespace dy
{

///
/// @class FDyImage
/// @brief This class display image on given position.
///
class FDyImage final : public FDyUiObject, public IDyInitializeHelper<PDyMetaWidgetImageDescriptor>
{
  MDY_SET_CRC32_HASH_WITH_TYPE(FDyImage);
  MDY_SET_TYPEMATCH_FUNCTION(FDyUiObject, FDyImage);
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyImage);
public:
  FDyImage() : mRenderer{*this} {};
  virtual ~FDyImage() = default;

  ///
  /// @brief Initialize FDyActor.
  /// @param objectMetaDesc Meta descriptor information instance for FDyImage.
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
  void SetTintColor(_MIN_ const DDyColorRGBA& iTintColor) noexcept;
  /// @brief Get tint color.
  MDY_NODISCARD const DDyColorRGBA& GetTintColor() const noexcept;


  /// @brief Set update renderer flag from inside renderer to this.
  void MDY_PRIVATE(SetUpdateRendererFlag)(_MIN_ bool iIsActivated) noexcept;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  /// Tint color for image rendering (with alpha)
  DDyColorRGBA      mTintColor = DDyColorRGBA::White;
  /// Size to content.
  bool              mIsSizeToContent = false;
  /// Image name list
  std::string       mImageName  = MDY_INITIALIZE_EMPTYSTR;
  bool              mIsMaterial = false;
  /// Renderer for this object.
  CDyImageRenderer  mRenderer;
  bool              mIsUpdateRenderer = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_CANVAS_FDYIMAGE_H
