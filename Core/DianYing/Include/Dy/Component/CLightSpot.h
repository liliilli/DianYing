#ifndef GUARD_DY_COMPONENT_CDYLIGHTSPOT_H
#define GUARD_DY_COMPONENT_CDYLIGHTSPOT_H
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

#include <Dy/Builtin/Constant/CSM.h>
#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Component/Internal/Lights/DUboSpotLight.h>
#include <Dy/Meta/Components/PCompSpotLightMetaInfo.h>

//!
//! Forward declaration
//!

namespace dy
{
class CCamera;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct CLightSpot
/// @brief Spot light component that has range-angle, distance-range, direction.
class CLightSpot final : public AGeneralBaseComponent, public IInitializeHelper<PCompSpotLightMetaInfo>
{
public:
  explicit CLightSpot(FActor& actorReference);
  virtual ~CLightSpot() = default;

  /// @brief  Initialize component
  /// @return If successful just return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PCompSpotLightMetaInfo& metaInfo) override final;
  /// @brief  Release component.
  void Release() override final;

  CLightSpot(const CLightSpot&)            = delete;
  CLightSpot& operator=(const CLightSpot&) = delete;
  CLightSpot(CLightSpot&& instance)            noexcept = default;
  CLightSpot& operator=(CLightSpot&& instance) noexcept = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CLightSpot);
  MDY_SET_CRC32_HASH_WITH_TYPE(CLightSpot)

  /// @brief Do nothing.
  void Update(TF32 dt) override final {};
  /// @brief Get information of component.
  MDY_NODISCARD std::string ToString() override final;

  /// @brief
  void ActivateLighting(bool iFlag) noexcept;
  /// @brief
  MDY_NODISCARD bool IsActiveLighting() const noexcept;

  /// @brief Set intensity of light.
  void SetIntensity(TF32 iIntensity) noexcept;
  /// @brief Get light's intensity.
  MDY_NODISCARD float GetIntensity() const noexcept;
  
  /// @brief Set light's diffuse color.
  void SetColor(const DColorRGB& iColor) noexcept;
  /// @brief Get light's diffuse color.
  MDY_NODISCARD const DColorRGB& GetColor() const noexcept;

  /// @brief Set angle range of spot light.
  void SetAngle(TF32 iDegree) noexcept;
  /// @brief Get angle value of spot light.
  MDY_NODISCARD TF32 GetAngle() const noexcept;

  /// @brief Set direction of spot light. input direction will be normalized automatically.
  void SetDirection(const DVec3& iDirection);
  /// @brief Get direction of spot light.
  MDY_NODISCARD const DVec3& GetDirection() const noexcept;

  /// @brief Get uniform-buffer-object information of this light.
  MDY_NODISCARD const DUboSpotLight& GetUboLightInfo() noexcept;

private:
  /// @brief This function must be implemented. 
  /// This function will be called when `Activate()` or `SetupFlagAsParent` called, 
  /// and final state boolean value is true from false.
  void TryActivateInstance() override final;
  
  /// @brief This function must be implemented.
  /// This function will be called when `Deactivate()` or `SetupFlagAsParent(false)` called,
  /// and final state boolean value is false from true.
  void TryDeactivateInstance() override final;

  /// Data for uniform buffer object. <Diffuse, Intensity>
  DUboSpotLight mData = {};
  /// Flag for casting light (binding to lighting system)
  bool mIsCastingLight = false;

  ///
  bool mIsBindedToRenderingManagerAsLighting = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYLIGHTSPOT_H