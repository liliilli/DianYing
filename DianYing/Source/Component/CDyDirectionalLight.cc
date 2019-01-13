#include <precompiled.h>
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

/// Header file
#include <Dy/Component/CDyDirectionalLight.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Dy/Builtin/Constant/GeneralLevel.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/SettingManager.h>

namespace dy
{

CDyDirectionalLight::CDyDirectionalLight(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyDirectionalLight::Initialize(const PDyDirLightComponentMetaInfo& metaInfo)
{
  this->mData.mDirection  = metaInfo.mDetails.mDirection.Normalize();
  this->mData.mIntensity  = metaInfo.mDetails.mIntensity;
  this->mIsCastingLight   = metaInfo.mDetails.mIsCastingLight;
  this->mIsCastingShadow  = metaInfo.mDetails.mIsCastingShadow;

  this->mData.mDiffuse    = metaInfo.mDetails.mDiffuse;
  this->mData.mSpecular   = metaInfo.mDetails.mSpecular;
  this->mData.mAmbient    = metaInfo.mDetails.mAmbient;

  this->mShadowType             = metaInfo.mDetails.mShadowType;
  this->mShadowCullingLayerList = metaInfo.mDetails.mShadowCullingMaskLayer;
  this->mDataShadow.mBias       = metaInfo.mDetails.mShadowBias;
  this->mDataShadow.mStrength   = metaInfo.mDetails.mShadowStrength;

  if (metaInfo.mDetails.mIsUsingGlobalShadowResolution == false)
  { // Shadow resolution
    this->mShadowResolution = metaInfo.mDetails.mShadowResolution;
  }
  else
  {
    auto& settingManager = MDySetting::GetInstance();
    const auto& i = settingManager.GetGlobalDefaultShadowMapResolution();
    this->mShadowResolution = DDyVector2{static_cast<TF32>(i.X), static_cast<TF32>(i.Y)};
  }

  // Set first time flag to false to use second time flag logics.
  if (metaInfo.mInitiallyActivated) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyDirectionalLight::Release()
{
  MDY_NOUSE_RTVAL_EXPR(pTryDeactivateDirectionalLight());
  MDY_NOUSE_RTVAL_EXPR(pTryDeactivateCastingShadow());
}

void CDyDirectionalLight::Update(float dt)
{
  if (this->mIsCastingLight == true && this->mIsNeededUpdateValueToGpu == true)
  {
    MDY_CALL_ASSERT_SUCCESS(this->pTryUpdateDirectionalLight());
  }

  if (this->mIsCastingShadow == true)
  {
    /// @TODO NOT IMPLEMENTED YET
  }
}

void CDyDirectionalLight::Activate() noexcept
{
  ADyBaseComponent::Activate();
  // Customized body ∨

  if (this->IsCastingLight() == true)
  {
    { MDY_NOTUSED auto _ = pTryActivateDirectionalLight(); }
    { MDY_NOTUSED auto _ = pTryDeactivateDirectionalLight(); }
  }
  if (this->IsCastingShadow() == true)
  {
    { MDY_NOTUSED auto _ = pTryActivateCastingShadow(); }
    { MDY_NOTUSED auto _ = pTryDeactivateCastingShadow(); }
  }
}

void CDyDirectionalLight::Deactivate() noexcept
{
  ADyBaseComponent::Deactivate();
  // Customized body ∨

  if (this->mIsBindedToRenderingManagerAsLighting == true)
  {
    MDY_CALL_ASSERT_SUCCESS(this->pTryDeactivateDirectionalLight());
    this->mIsBindedToRenderingManagerAsLighting = false;
  }
  if (this->mIsBindedToRenderingManagerAsShadow == true)
  {
    MDY_CALL_ASSERT_SUCCESS(this->pTryDeactivateCastingShadow());
    this->mIsBindedToRenderingManagerAsShadow = false;
  }
}

void CDyDirectionalLight::pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept
{
  ADyBaseComponent::pPropagateParentActorActivation(actorBool);
  // Customized body ∨
  { MDY_NOTUSED auto _ = pTryActivateDirectionalLight();    }
  { MDY_NOTUSED auto _ = pTryDeactivateDirectionalLight();  }
  { MDY_NOTUSED auto _ = pTryActivateCastingShadow();       }
  { MDY_NOTUSED auto _ = pTryDeactivateCastingShadow();     }
}

MDY_NODISCARD std::string CDyDirectionalLight::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void CDyDirectionalLight::SetCastingLightFlag(const bool flag) noexcept
{
  if (this->mIsCastingLight == flag) { return; }
  //
  this->mIsCastingLight = flag;
  if (flag == true) { MDY_NOTUSED auto _ = pTryActivateDirectionalLight(); }
  else              { MDY_NOTUSED auto _ = pTryDeactivateDirectionalLight(); }
}

void CDyDirectionalLight::SetCastingShadowFlag(const bool flag) noexcept
{
  if (this->mIsCastingShadow == flag) { return; }
  //
  this->mIsCastingShadow = flag;
  if (flag == true) { MDY_NOTUSED auto _ = pTryActivateCastingShadow(); }
  else              { MDY_NOTUSED auto _ = pTryDeactivateCastingShadow(); }
}

void CDyDirectionalLight::UpdateLightViewMatrix()
{
  auto fwd = this->GetLightDirection();
  if (fwd == kLevelUpDir) { fwd += DDyVector3{0.01f, 0.01f, 0.01f}; }
  fwd *= -1.0f;

  const auto& pos = this->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
  const auto eye = pos + fwd;
  this->mLightViewMatrix = glm::lookAt(
    static_cast<glm::vec3>(pos), 
    static_cast<glm::vec3>(eye), 
    static_cast<glm::vec3>(kLevelUpDir)); 
}

const DDyMatrix4x4& CDyDirectionalLight::GetLightViewMatrix() const noexcept
{
  return this->mLightViewMatrix;
}

void CDyDirectionalLight::UpdateCSMFrustum(const CDyCamera& iRefCamera)
{
  this->FrustumBoundingBoxLightViewSpace(
      iRefCamera.GetNear(), iRefCamera.GetFar(), 
      iRefCamera, 
      this->minFrustum, this->maxFrustum);
}

void CDyDirectionalLight::UpdateProjectionMatrix()
{
  this->mLightProjMatrix = glm::ortho(
      this->minFrustum.X, this->maxFrustum.X, 
      this->minFrustum.Y, this->maxFrustum.Y, 
      -this->maxFrustum.Z, -this->minFrustum.Z);
      //0.0f, this->minFrustum.Z);
}

const DDyMatrix4x4& CDyDirectionalLight::GetProjectionMatrix() const noexcept
{
  return this->mLightProjMatrix;
}

void CDyDirectionalLight::UpdateSegmentFarPlanes(_MIN_ const CDyCamera& iPtrCamera)
{
  if (const auto& proj = iPtrCamera.GetProjectionMatrix(); 
      this->mOldProjectionMatrix == proj) 
  { return; }
  else
  { this->mOldProjectionMatrix = proj; }

  const auto nearPlane  = iPtrCamera.GetNear();
  const auto farPlane   = iPtrCamera.GetFar();
  const auto diffPlane  = farPlane - nearPlane;
  
  static constexpr TF32 frustumSplitCorrection = 0.1f;

  for (TU32 i = 1; i <= kCSMSegment; ++i)
  {
    const TF32 distFactor = static_cast<TF32>(i) / kCSMSegment;
    const TF32 stdTerm    = nearPlane * pow(farPlane / nearPlane, distFactor);
    const TF32 corrTerm   = nearPlane + distFactor * diffPlane;
    const TF32 viewDepth  = frustumSplitCorrection * stdTerm + (1.0f - frustumSplitCorrection) * corrTerm;

    const auto projectedDepth = this->mOldProjectionMatrix.MultiplyVector({0, 0, -viewDepth, 1});
    this->mFarPlanes[i - 1] = viewDepth;
    this->mNormalizedFarPlanes[i - 1] = (projectedDepth.Z / projectedDepth.W) * 0.5f + 0.5f;
  }
}

const std::array<TF32, kCSMSegment>& CDyDirectionalLight::GetCSMFarPlanes() const noexcept
{
  return this->mFarPlanes;
}

const std::array<TF32, kCSMSegment>& CDyDirectionalLight::GetCSMNormalizedFarPlanes() const noexcept
{
  return this->mNormalizedFarPlanes;
}

void CDyDirectionalLight::UpdateLightProjectionAndViewports(
    _MIN_ const CDyCamera& iRefCamera, 
    _MIN_ const std::array<TF32, kCSMSegment>& iFarPlanes,
    _MIN_ const std::array<TF32, kCSMSegment>& iNormalizedFarPlanes)
{
   // Find a bounding box of segment in light view space.
  TF32 nearSegmentPlane = 0.0f;
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    DDyVector4 minSegment {NumericalMax<TF32>};
    DDyVector4 maxSegment {NumericalMin<TF32>};
    this->FrustumBoundingBoxLightViewSpace(nearSegmentPlane, iFarPlanes[i], iRefCamera, minSegment, maxSegment);

    // Update viewports.
    const DDyVector2 frustumSize {maxFrustum.X - minFrustum.X, maxFrustum.Y - minFrustum.Y};
    const TF32 segmentSizeX = maxSegment.X - minSegment.X;
    const TF32 segmentSizeY = maxSegment.Y - minSegment.Y;
    const TF32 segmentSize  = std::max(segmentSizeX, segmentSizeY);

    const DDyVector2 offsetBottomLeft         {minSegment.X - minFrustum.X, minSegment.Y - minFrustum.Y};
    const DDyVector2 offsetSegmentSizeRatio   {offsetBottomLeft.X / segmentSize, offsetBottomLeft.Y / segmentSize};
    const DDyVector2 frustumSegmentSizeRatio  {frustumSize.X / segmentSize, frustumSize.Y / segmentSize};

    DDyVector2 pixelOffsetTopLeft = offsetSegmentSizeRatio * kCSMAttachmentTextureSize;
    DDyVector2 pixelFrustumSize   = frustumSegmentSizeRatio * kCSMAttachmentTextureSize;

    // Scale factor that helps if frustum size is supposed to be bigger than maximum viewport size.
    const DDyVector2 scaleFactor{
        32768 < pixelFrustumSize.X ? 32768 / pixelFrustumSize.X : 1.0f,
        32768 < pixelFrustumSize.Y ? 32768 / pixelFrustumSize.Y : 1.0f
    };
    pixelOffsetTopLeft *= scaleFactor;
    pixelFrustumSize   *= scaleFactor;

    mLightViewports[i].mLeftDown  = pixelOffsetTopLeft * -1;
    mLightViewports[i].mRightUp   = pixelFrustumSize;

    // Update light view-projection matrices per segments.
    DDyMatrix4x4 lightProjMatrix = glm::ortho(minSegment.X, minSegment.X + segmentSize, minSegment.Y, minSegment.Y + segmentSize, -maxFrustum.Z, -minFrustum.Z);
    //DDyMatrix4x4 lightProjMatrix = glm::ortho(minSegment.X, minSegment.X + segmentSize, minSegment.Y, minSegment.Y + segmentSize, 0.0f, minFrustum.Z);
    DDyMatrix4x4 lightScale = DDyMatrix4x4::CreateWithScale(DDyVector3{0.5f * scaleFactor.X, 0.5f * scaleFactor.Y, 0.5f});
    DDyMatrix4x4 lightBias  = DDyMatrix4x4::CreateWithTranslation(DDyVector3{0.5f * scaleFactor.X, 0.5f * scaleFactor.Y, 0.5f});

    this->mLightSegmentVPSBMatrices[i] = lightBias.Multiply(lightScale).Multiply(lightProjMatrix).Multiply(this->mLightViewMatrix);
    nearSegmentPlane = iNormalizedFarPlanes[i];
  }
}

const std::array<DDyArea2D, kCSMSegment>& CDyDirectionalLight::GetCSMIndexedViewports() const noexcept
{
  return this->mLightViewports;
}

const std::array<DDyMatrix4x4, kCSMSegment>& 
CDyDirectionalLight::GetCSMLightSegmentVPSBMatrix() const noexcept
{
  return this->mLightSegmentVPSBMatrices;
}

void CDyDirectionalLight::FrustumBoundingBoxLightViewSpace(
    _MIN_ TF32 iNear, _MIN_ TF32 iFar, 
    _MIN_ const CDyCamera& iRefCamera,
    _MIN_ DDyVector4& iMin, DDyVector4& iMax) const
{
   DDyVector4 minResult {NumericalMax<TF32>};
  DDyVector4 maxResult {NumericalMin<TF32>};

  const TF32 fov  = math::DegToRadVal<TF32> * iRefCamera.GetFieldOfView();
  const auto xywh = iRefCamera.GetPixelizedViewportRectangle();
  const auto pos  = iRefCamera.GetPosition();
  const TF32 nearHeight = 2.0f * tan(fov) * iNear;
  const TF32 nearWidth  = nearHeight * xywh[2] / xywh[3];
  const TF32 farHeight  = 2.0f * tan(fov) * iFar;
  const TF32 farWidth   = farHeight * xywh[2] / xywh[3];
  
  const auto& camViewMatrix = iRefCamera.GetViewMatrix();
  const auto rightDir = DDyVector3{camViewMatrix[0]};
  const auto upDir    = DDyVector3{camViewMatrix[1]};
  const auto forDir   = DDyVector3{camViewMatrix[2]};

  const auto nc = pos + forDir * iNear;
  const auto fc = pos + forDir * iFar;

  // Vertices in a world space.
  std::array<DDyVector4, 8> boundingBoxVertices =
  { //         z    y                             x
    DDyVector4{nc - (upDir * nearHeight * 0.5f) - (rightDir * nearWidth * 0.5f), 1.0f}, // NBL
    DDyVector4{nc - (upDir * nearHeight * 0.5f) + (rightDir * nearWidth * 0.5f), 1.0f}, // NBR
    DDyVector4{nc + (upDir * nearHeight * 0.5f) + (rightDir * nearWidth * 0.5f), 1.0f}, // NTR
    DDyVector4{nc + (upDir * nearHeight * 0.5f) - (rightDir * nearWidth * 0.5f), 1.0f}, // NTL

    DDyVector4{fc - (upDir * farHeight * 0.5f) - (rightDir * farWidth * 0.5f), 1.0f}, // FBL
    DDyVector4{fc - (upDir * farHeight * 0.5f) + (rightDir * farWidth * 0.5f), 1.0f}, // FBR
    DDyVector4{fc + (upDir * farHeight * 0.5f) + (rightDir * farWidth * 0.5f), 1.0f}, // FTR
    DDyVector4{fc + (upDir * farHeight * 0.5f) - (rightDir * farWidth * 0.5f), 1.0f}, // FTL
  };

  for (TU32 vertId = 0; vertId < 8; ++vertId)
  { // Light view space.
    boundingBoxVertices[vertId] = this->mLightViewMatrix.MultiplyVector(boundingBoxVertices[vertId]);
    // Update bounding box. (at least small point and at most biggest point)
    for (TU32 i = 0; i < 4; ++i)
    {
      minResult[i] = std::min(minResult[i], boundingBoxVertices[vertId][i]);
      maxResult[i] = std::max(maxResult[i], boundingBoxVertices[vertId][i]);
    }
  }

  iMin = minResult;
  iMax = maxResult;
}

EDySuccess CDyDirectionalLight::pTryUpdateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  this->mIsNeededUpdateValueToGpu = false;
  return DY_SUCCESS;
}
  
DDyUboDirectionalLight CDyDirectionalLight::GetUboLightInfo() const noexcept
{
  return this->mData;
}

EDySuccess CDyDirectionalLight::pTryActivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == true)  { return DY_FAILURE; }
  if (this->mIsCastingLight == false)                       { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsLighting = true;

  // Bind and get a index of UBO array.
  MDyRendering::GetInstance().MDY_PRIVATE_SPECIFIER(BindMainDirectionalLight)(*this);
  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryDeactivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  if (this->mIsCastingLight == true)                        { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsLighting = false;

  // Try Unbind from lighting system.
  MDyRendering::GetInstance().MDY_PRIVATE_SPECIFIER(UnbindMainDirectionalLight)(*this);
  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryActivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == true)    { return DY_FAILURE; }
  if (this->mIsCastingShadow == false)                      { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsShadow = true;

  // Try bind to lighting system.
  MDyRendering::GetInstance().MDY_PRIVATE_SPECIFIER(BindMainDirectionalShadow)(*this);
  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryDeactivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == false)   { return DY_FAILURE; }
  if (this->mIsCastingShadow == true)                       { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsShadow = false;

  // Unbind from shadow system.
  MDyRendering::GetInstance().MDY_PRIVATE_SPECIFIER(UnbindMainDirectionalShadow)(*this);
  return DY_SUCCESS;
}


} /// ::dy namespace