#ifndef GUARD_DY_COMPONENT_CDySkybox_H
#define GUARD_DY_COMPONENT_CDySkybox_H
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

#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

//!
//! Forward declaration
//!

namespace dy
{
class FResourceMaterial;
class CModelFilter;
class FResourceMesh;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CSkybox
/// @brief Skybox component. If activated, skybox will be rendered after opaque mesh rendering.
/// @reference https://docs.unity3d.com/kr/530/Manual/class-Skybox.html
class CSkybox final : public AGeneralBaseComponent, public IInitializeHelper<PDySkyboxComponentMetaInfo>
{
public:
  CSkybox(FActor& actorReference) : AGeneralBaseComponent{actorReference} {};
  virtual ~CSkybox() = default;

  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDySkyboxComponentMetaInfo& descriptor) override final;

  /// Release component.
  void Release() override final;

  /// @brief Update sequence etc.
  void Update(_MIN_ TF32 dt) override final {};

  /// @brief
  MDY_NODISCARD std::string ToString() override final;

  /// @brief Private function. Get reference of texture binder.
  MDY_NODISCARD TDyResourceBinderTexture& MDY_PRIVATE(GetTextureBinderReference)() noexcept;

  /// @brief Set exposure value. If value is out of range from 0 to 10, input value will be aligned.
  void SetExposure(_MIN_ TF32 iValue) noexcept;
  /// @brief Get exposure value. 
  MDY_NODISCARD TF32 GetExposure() const noexcept;

  /// @brief Set rotation degrees of y-axis.
  void SetRotationDegree(_MIN_ TF32 iDegree) noexcept;
  /// @brief Get rotation degrees of y-axis.
  MDY_NODISCARD TF32 GetRotationDegree() const noexcept;

  /// @brief
  void SetTintColor(_MIN_ const DColorRGB& iTintColor) noexcept;
  /// @brief
  MDY_NODISCARD const DColorRGB& GetTintColor() const noexcept;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  TDyResourceBinderTexture mBinderTexture;

  std::string           mTextureSpecifier = MDY_INITIALIZE_EMPTYSTR;
  DClamp<TF32, 0, 10> mExposure         = 0.0f;
  DColorRGB           mTintColor        = {};
  TF32                  mRotationDegree   = 0.0f;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CSkybox);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDySkybox);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDySkybox_H
