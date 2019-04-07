#ifndef GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H
#define GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H
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
#include <Dy/Component/Internal/Lights/DUboDirLight.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Meta/Components/PCompDirLightMetaInfo.h>

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

/// @struct CLightDirectional
/// @brief Direction light component.
class CLightDirectional final : public AGeneralBaseComponent, public IInitializeHelper<PDirLightComponentMetaInfo>
{
public:
  CLightDirectional(FActor& actorReference);
  virtual ~CLightDirectional() = default;

  /// @brief  Initialize component
  /// @param  metaInfo
  /// @return If successful just return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PDirLightComponentMetaInfo& metaInfo) override final;

  /// @brief  Release component.
  void Release() override final;

  CLightDirectional(const CLightDirectional&)                                 = delete;
  CLightDirectional& operator=(const CLightDirectional&)                      = delete;
  CLightDirectional(CLightDirectional&& instance)                   noexcept  = default;
  CLightDirectional& operator=(CLightDirectional&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::AGeneralBaseComponent, CLightDirectional);
  MDY_SET_CRC32_HASH_WITH_TYPE(CLightDirectional)

  /// @brief Do nothing.
  void Update(float dt) override final;

  /// @brief
  /// @return
  MDY_NODISCARD std::string ToString() override final;

  /// @brief
  void SetCastingLightFlag(bool flag) noexcept;
  /// @brief
  MDY_NODISCARD bool IsCastingLight() const noexcept { return this->mIsCastingLight; }

  /// @brief
  void SetCastingShadowFlag(bool flag) noexcept;
  /// @brief
  MDY_NODISCARD bool IsCastingShadow() const noexcept { return this->mIsCastingShadow; }

  /// @brief Get light's direction. Direction forwards to outside from origin.
  MDY_NODISCARD const DVec3& GetLightDirection() const noexcept;

  /// @brief Set intensity of light.
  void SetIntensity(TF32 iIntensity) noexcept;
  /// @brief Get light's intensity.
  MDY_NODISCARD float GetIntensity() const noexcept;
  
  /// @brief Set light's diffuse color.
  void SetDiffuseColor(const DColorRGBA& iColor) noexcept;
  /// @brief Get light's diffuse color.
  MDY_NODISCARD const DColorRGBA& GetDiffuseColor() const noexcept;

  /// @brief Set light's direction. direction must be forward from origin to outside.
  void SetLightDirection(const DVec3& direction) noexcept;

  /// @brief Update view matrix agian.
  void UpdateLightViewMatrix();
  /// @brief Get view matrix of light.
  MDY_NODISCARD const DMat4& GetLightViewMatrix() const noexcept;

  /// @brief
  void UpdateCSMFrustum(const CCamera& iRefCamera);
  /// @brief Update projection matrix again.
  void UpdateProjectionMatrix();
  /// @brief Get projection matrix of light.
  MDY_NODISCARD const DMat4& GetProjectionMatrix() const noexcept;

  /// @brief Update segment far planes.
  void UpdateSegmentFarPlanes(_MIN_ const CCamera& iPtrCamera);
  /// @brief Get CSM far planes.
  MDY_NODISCARD const std::array<TF32, kCSMSegment>& GetCSMFarPlanes() const noexcept;

  /// @brief Update light projection and viewports of CSM.
  void UpdateLightProjectionAndViewports(
      const CCamera& iRefCamera, 
      const std::array<TF32, kCSMSegment>& iFarPlanes,
      const std::array<TF32, kCSMSegment>& iNormalizedFarPlanes);
  
  /// @brief Get Cascaded-indexed viewports for shadow map array.
  MDY_NODISCARD const std::array<DArea2D, kCSMSegment>& GetCSMIndexedViewports() const noexcept;

  /// @brief Get uniform-buffer-object information of this light.
  MDY_NODISCARD const DUboDirectionalLight& GetUboLightInfo() const noexcept;
  /// @brief Get uniform-buffer-object information of this light shadow.
  MDY_NODISCARD const DDyUboDirShadow& GetUboShadowInfo() const noexcept;

private:
  /// @brief This function must be implemented. 
  /// This function will be called when `Activate()` or `SetupFlagAsParent` called, 
  /// and final state boolean value is true from false.
  void TryActivateInstance() override final;
  
  /// @brief This function must be implemented.
  /// This function will be called when `Deactivate()` or `SetupFlagAsParent(false)` called,
  /// and final state boolean value is false from true.
  void TryDeactivateInstance() override final;

  /// @brief Try activate camera operation. \n
  /// If parent is activate and itself activated, update all properties newly.
  MDY_NODISCARD EDySuccess pTryActivateDirectionalLight();
  /// @brief
  MDY_NODISCARD EDySuccess pTryUpdateDirectionalLight();
  /// @brief Try deactivate camera operation. \n
  /// If either parent or itself is deactivated, disconnect it with all outside system.
  MDY_NODISCARD EDySuccess pTryDeactivateDirectionalLight();

  /// @brief
  MDY_NODISCARD EDySuccess pTryActivateCastingShadow();
  /// @brief
  MDY_NODISCARD EDySuccess pTryDeactivateCastingShadow();

  /// @brief Update iMin iMax as Bounding box of camera & given near & far.
  void FrustumBoundingBoxLightViewSpace(
      TF32 iNear,
      TF32 iFar,
      const CCamera& iRefCamera, 
      DVec4& iMin, 
      DVec4& iMax) const;

  /// Data for uniform buffer object. <Direction, Diffuse, Specular, Ambient, Intensity>
  DUboDirectionalLight    mData       = {};
  /// Data for shadow buffer object.  <VPSB[4], FarPlane, Bias, Strength>
  DDyUboDirShadow           mDataShadow = {};
  /// Shadow type.
  EDyShadowType             mShadowType = EDyShadowType::__Error;
  /// Shadow map resolution for shadow.
  DVec2                mShadowResolution = {};
  /// Shadow culling layer.
  std::vector<std::string>  mShadowCullingLayerList = {};
 
  /// Find a bounding box of whole camera frustum in light view space.
  DVec4 minFrustum {kMaxValueOf<TF32>};
  DVec4 maxFrustum {kMinValueOf<TF32>};
  std::array<DArea2D, kCSMSegment> mLightViewports;

  std::array<TF32, kCSMSegment> mFarPlanes;
  DMat4 mOldProjectionMatrix; 
    
  DMat4 mLightViewMatrix;
  DMat4 mLightProjMatrix;

  /// Flag for casting light (binding to lighting system)
  MDY_NOTUSED bool mIsCastingLight   = false;
  /// Flag for casting shadow (binding to shadow system)
  MDY_NOTUSED bool mIsCastingShadow  = false;

  ///
  bool        mIsBindedToRenderingManagerAsLighting = false;
  ///
  bool        mIsBindedToRenderingManagerAsShadow = false;
  /// Flag for updating new value to gpu memory buffer.
  bool        mIsNeededUpdateValueToGpu = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H