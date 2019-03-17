#ifndef GUARD_DY_COMPONENT_CDYLIGHTPOINT_H
#define GUARD_DY_COMPONENT_CDYLIGHTPOINT_H
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
#include <Dy/Component/Internal/Lights/DUboPointLight.h>
#include <Dy/Meta/Components/PCompPointLightMetaInfo.h>

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

/// @struct CLightPoint
/// @brief Point light component.
/// @TODO IMPLEMENT POINT SHADOW.
class CLightPoint final : 
  public AGeneralBaseComponent, 
  public IInitializeHelper<PCompPointLightMetaInfo>
{
public:
  explicit CLightPoint(FActor& actorReference);
  virtual ~CLightPoint() = default;

  /// @brief  Initialize component
  /// @return If successful just return DY_SUCCESS or DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PCompPointLightMetaInfo& metaInfo) override final;
  /// @brief  Release component.
  void Release() override final;

  CLightPoint(const CLightPoint&)            = delete;
  CLightPoint& operator=(const CLightPoint&) = delete;
  CLightPoint(CLightPoint&& instance)            noexcept = default;
  CLightPoint& operator=(CLightPoint&& instance) noexcept = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CLightPoint);
  MDY_SET_CRC32_HASH_WITH_TYPE(CLightPoint)

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

  /// @brief Get uniform-buffer-object information of this light.
  MDY_NODISCARD const DUboPointLight& GetUboLightInfo() noexcept;

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
  DUboPointLight mData = {};
  /// Flag for casting light (binding to lighting system)
  bool mIsCastingLight = false;

  ///
  bool mIsBindedToRenderingManagerAsLighting = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYLIGHTPOINT_H