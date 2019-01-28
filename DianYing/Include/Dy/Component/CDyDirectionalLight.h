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

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Meta/Information/ComponentLightMetaInfo.h>
#include <Dy/Builtin/Constant/CSM.h>

#define MDY_BYTEPADDING(__Type__) MDY_NOTUSED __Type__ MDY_TOKENPASTE2(____padding, __LINE__)

namespace dy
{
  class CDyCamera;

  ///
/// @struct DDyUboDirectionalLight
/// @brief  This structure must be aligned by 16 bytes. (for std140 GL standard block layout)
/// @TODO SCRIPT THIS
///
struct alignas(16) DDyUboDirectionalLight final
{
  /// World space light direction
  DDyVector3  mDirection  = {};
  MDY_BYTEPADDING(TI32);
  /// Light tint color
  DDyColorRGBA    mDiffuse    = DDyColorRGBA::White;
  DDyColorRGBA    mSpecular   = DDyColorRGBA::White;
  DDyColorRGBA    mAmbient    = DDyColorRGBA::White;
  /// Light intensity for this light component.
  float       mIntensity  = MDY_INITIALIZE_DEFINT;
};

///
/// @struct DDyUboDirShadow
/// @brief  This structure must be aligned by 16 bytes.
/// @TODO SCRIPT THIS
///
struct alignas(16) DDyUboDirShadow final
{
  DDyMatrix4x4          mProjViewMatrix = {};
  DDyClamp<float, 0, 2> mBias           = 0.02f;
  DDyClamp<float, 0, 1> mStrength       = 0.5f;
};

///
/// @struct CDyDirectionalLight
/// @brief
/// @TODO SCRIPT THIS
///
class CDyDirectionalLight final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyDirLightComponentMetaInfo>
{
public:
  CDyDirectionalLight(FDyActor& actorReference);
  virtual ~CDyDirectionalLight() = default;

  ///
  /// @brief  Initialize component
  /// @param  metaInfo
  /// @return If successful just return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyDirLightComponentMetaInfo& metaInfo) override final;

  ///
  /// @brief  Release component.
  ///
  void Release() override final;

  CDyDirectionalLight(const CDyDirectionalLight&)                                 = delete;
  CDyDirectionalLight& operator=(const CDyDirectionalLight&)                      = delete;
  CDyDirectionalLight(CDyDirectionalLight&& instance)                   noexcept  = default;
  CDyDirectionalLight& operator=(CDyDirectionalLight&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyDirectionalLight);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyDirectionalLight)

  /// @brief Do nothing.
  void Update(float dt) override final;

  /// @brief
  /// @return
  MDY_NODISCARD std::string ToString() override final;

  /// @brief
  /// @return Casting light
  MDY_NODISCARD bool IsCastingLight() const noexcept { return this->mIsCastingLight; }

  /// @brief
  /// @return
  MDY_NODISCARD bool IsCastingShadow() const noexcept { return this->mIsCastingShadow; }

  /// @brief
  /// @param flag
  void SetCastingLightFlag(_MIN_ const bool flag) noexcept;

  /// @brief
  /// @param flag
  void SetCastingShadowFlag(_MIN_ const bool flag) noexcept;

  /// @brief
  /// @return
  MDY_NODISCARD const DDyVector3& GetLightDirection() const noexcept
  {
    return this->mData.mDirection;
  }

  /// @brief
  /// @return
  MDY_NODISCARD float GetLightIntensity() const noexcept
  {
    return this->mData.mIntensity;
  }

  /// @brief
  /// @return
  MDY_NODISCARD const DDyColorRGBA& GetLightDiffuseColor() const noexcept
  {
    return this->mData.mDiffuse;
  }

  /// @brief
  /// @param direction
  FORCEINLINE void SetLightDirection(_MIN_ const DDyVector3& direction) noexcept
  {
    this->mData.mDirection = direction.Normalize();
    this->mIsNeededUpdateValueToGpu = true;
  }

  /// @brief Update view matrix agian.
  void UpdateLightViewMatrix();
  /// @brief Get view matrix of light.
  MDY_NODISCARD const DDyMatrix4x4& GetLightViewMatrix() const noexcept;

  /// @brief
  void UpdateCSMFrustum(_MIN_ const CDyCamera& iRefCamera);
  /// @brief Update projection matrix again.
  void UpdateProjectionMatrix();
  /// @brief Get projection matrix of light.
  MDY_NODISCARD const DDyMatrix4x4& GetProjectionMatrix() const noexcept;

  /// @brief Update segment far planes.
  void UpdateSegmentFarPlanes(_MIN_ const CDyCamera& iPtrCamera);
  /// @brief Get CSM far planes.
  MDY_NODISCARD const std::array<TF32, kCSMSegment>& GetCSMFarPlanes() const noexcept;
  /// @brief Get CSM normalized far planes.
  MDY_NODISCARD const std::array<TF32, kCSMSegment>& GetCSMNormalizedFarPlanes() const noexcept;

  /// @brief Update light projection and viewports of CSM.
  void UpdateLightProjectionAndViewports(
      _MIN_ const CDyCamera& iRefCamera, 
      _MIN_ const std::array<TF32, kCSMSegment>& iFarPlanes,
      _MIN_ const std::array<TF32, kCSMSegment>& iNormalizedFarPlanes);
  
  /// @brief Get Cascaded-indexed viewports for shadow map array.
  MDY_NODISCARD const std::array<DDyArea2D, kCSMSegment>& GetCSMIndexedViewports() const noexcept;
  /// @brief Get Calculated Cascaded Shadow Mapping VPSB Matrix list.
  MDY_NODISCARD const std::array<DDyMatrix4x4, kCSMSegment>& GetCSMLightSegmentVPSBMatrix() const noexcept;

  /// @brief
  /// @param intensity
  void SetIntensity(_MIN_ const float intensity) noexcept
  {
    if (intensity < 0.f)  { this->mData.mIntensity = 0.001f; }
    else                  { this->mData.mIntensity = intensity; }
    this->mIsNeededUpdateValueToGpu = true;
  }

  /// @brief
  /// @param color
  void SetLightDiffuseColor(_MIN_ const DDyColorRGBA& color) noexcept
  {
    this->mData.mDiffuse = color;
    this->mIsNeededUpdateValueToGpu = true;
  }

  /// @brief Get uniform-buffer-object information of this light.
  MDY_NODISCARD DDyUboDirectionalLight GetUboLightInfo() const noexcept;

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
      _MIN_ TF32 iNear,
      _MIN_ TF32 iFar,
      _MIN_ const CDyCamera& iRefCamera, 
      _MOUT_ DDyVector4& iMin, 
      _MOUT_ DDyVector4& iMax) const;

  /// Data for uniform buffer object. <Direction, Diffuse, Specular, Ambient, Intensity>
  DDyUboDirectionalLight    mData             = {};
  /// Data for shadow buffer object.  <PV, Bias, Strength>
  DDyUboDirShadow           mDataShadow       = {};
  /// Shadow type.
  EDyShadowType             mShadowType       = EDyShadowType::NoneError;
  /// Shadow map resolution for shadow.
  DDyVector2                mShadowResolution = {};
  /// Shadow culling layer.
  std::vector<std::string>  mShadowCullingLayerList = {};
 
  /// Find a bounding box of whole camera frustum in light view space.
  DDyVector4 minFrustum {NumericalMax<TF32>};
  DDyVector4 maxFrustum {NumericalMin<TF32>};
  std::array<DDyArea2D, kCSMSegment>    mLightViewports;
  std::array<DDyMatrix4x4, kCSMSegment> mLightSegmentVPSBMatrices;

  std::array<TF32, kCSMSegment> mFarPlanes;
  std::array<TF32, kCSMSegment> mNormalizedFarPlanes;
  DDyMatrix4x4                  mOldProjectionMatrix; 
    
  DDyMatrix4x4 mLightViewMatrix;
  DDyMatrix4x4 mLightProjMatrix;

  /// Flag for casting light (binding to lighting system)
  MDY_NOTUSED bool          mIsCastingLight   = false;
  /// Flag for casting shadow (binding to shadow system)
  MDY_NOTUSED bool          mIsCastingShadow  = false;

  ///
  bool        mIsBindedToRenderingManagerAsLighting = false;
  ///
  bool        mIsBindedToRenderingManagerAsShadow = false;
  /// Flag for updating new value to gpu memory buffer.
  bool        mIsNeededUpdateValueToGpu = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYDIRECTIONALLIGHT_H