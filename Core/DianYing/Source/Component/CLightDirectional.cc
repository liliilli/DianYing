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
#include <Dy/Component/CLightDirectional.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Math/Utility/XMath.h>
#include <Dy/Builtin/Constant/GeneralLevel.h>
#include <Dy/Component/CCamera.h>
#include <Dy/Element/FActor.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Component/CTransform.h>
#include <Math/Utility/XGraphicsMath.h>

namespace dy
{

CLightDirectional::CLightDirectional(FActor& actorReference)
  : AGeneralBaseComponent(actorReference),
    mFarPlanes()
{ }

EDySuccess CLightDirectional::Initialize(const PDirLightComponentMetaInfo& metaInfo)
{
  this->mData.mDirection  = metaInfo.mDetails.mDirection.Normalize();
  this->mData.mIntensity  = metaInfo.mDetails.mIntensity;
  this->mData.mDiffuse    = metaInfo.mDetails.mDiffuse;

  this->mIsCastingLight   = metaInfo.mDetails.mIsCastingLight;
  this->mIsCastingShadow  = metaInfo.mDetails.mIsCastingShadow;

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
    auto& settingManager = MSetting::GetInstance();
    const auto& i = settingManager.GetGlobalDefaultShadowMapResolution();
    this->mShadowResolution = DVec2{static_cast<TF32>(i.X), static_cast<TF32>(i.Y)};
  }

  // Set first time flag to false to use second time flag logics.
  if (metaInfo.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CLightDirectional::Release()
{
  MDY_NOUSE_RTVAL_EXPR(pTryDeactivateDirectionalLight());
  MDY_NOUSE_RTVAL_EXPR(pTryDeactivateCastingShadow());
}

void CLightDirectional::Update(float dt)
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

MDY_NODISCARD std::string CLightDirectional::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void CLightDirectional::SetCastingLightFlag(bool flag) noexcept
{
  if (this->mIsCastingLight == flag) { return; }
  //
  this->mIsCastingLight = flag;
  if (flag == true) { MDY_NOTUSED auto _ = pTryActivateDirectionalLight(); }
  else              { MDY_NOTUSED auto _ = pTryDeactivateDirectionalLight(); }
}

void CLightDirectional::SetCastingShadowFlag(bool flag) noexcept
{
  if (this->mIsCastingShadow == flag) { return; }
  //
  this->mIsCastingShadow = flag;
  if (flag == true) { MDY_NOTUSED auto _ = pTryActivateCastingShadow(); }
  else              { MDY_NOTUSED auto _ = pTryDeactivateCastingShadow(); }
}

const DVec3& CLightDirectional::GetLightDirection() const noexcept
{
  return this->mData.mDirection;
}

float CLightDirectional::GetIntensity() const noexcept
{
  return this->mData.mIntensity;
}

void CLightDirectional::SetDiffuseColor(const DColorRGBA& iColor) noexcept
{
  this->mData.mDiffuse = iColor;
  this->mIsNeededUpdateValueToGpu = true;
}

const DColorRGBA& CLightDirectional::GetDiffuseColor() const noexcept
{
  return this->mData.mDiffuse;
}

void CLightDirectional::SetLightDirection(const DVec3& direction) noexcept
{
  this->mData.mDirection = direction.Normalize();
  this->mIsNeededUpdateValueToGpu = true;
}

void CLightDirectional::UpdateLightViewMatrix()
{
  auto fwd = this->GetLightDirection();
  if (fwd == kLevelUpDir) { fwd += DVec3{0.01f, 0.01f, 0.01f}; }
  fwd *= -1.0f;

  const auto& pos = this->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
  const auto eye = pos + fwd;
  this->mLightViewMatrix = math::LookAt(pos, eye, kLevelUpDir);
}

const DMat4& CLightDirectional::GetLightViewMatrix() const noexcept
{
  return this->mLightViewMatrix;
}

void CLightDirectional::UpdateCSMFrustum(const CCamera& iRefCamera)
{
  this->FrustumBoundingBoxLightViewSpace(
      iRefCamera.GetNear(), iRefCamera.GetFar(), 
      iRefCamera, 
      this->minFrustum, this->maxFrustum);
}

void CLightDirectional::UpdateProjectionMatrix()
{
  using namespace math;
  this->mLightProjMatrix = ProjectionMatrix<TReal>(
    EGraphics::OpenGL, EProjection::Orthogonal,
    this->minFrustum.X, this->maxFrustum.X, 
    this->minFrustum.Y, this->maxFrustum.Y, 
    -this->maxFrustum.Z, -this->minFrustum.Z
  );
}

const DMat4& CLightDirectional::GetProjectionMatrix() const noexcept
{
  return this->mLightProjMatrix;
}

void CLightDirectional::UpdateSegmentFarPlanes(_MIN_ const CCamera& iPtrCamera)
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

    const auto projectedDepth = this->mOldProjectionMatrix * DVec4{0, 0, -viewDepth, 1};
    this->mFarPlanes[i - 1] = viewDepth;
    this->mDataShadow.mNormalizedFarPlanes[i - 1] = (projectedDepth.Z / projectedDepth.W) * 0.5f + 0.5f;
  }
}

const std::array<TF32, kCSMSegment>& CLightDirectional::GetCSMFarPlanes() const noexcept
{
  return this->mFarPlanes;
}

void CLightDirectional::UpdateLightProjectionAndViewports(
  const CCamera& iRefCamera, 
  const std::array<TF32, kCSMSegment>& iFarPlanes,
  const std::array<TF32, kCSMSegment>& iNormalizedFarPlanes)
{
   // Find a bounding box of segment in light view space.
  TF32 nearSegmentPlane = 0.0f;
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    DVec4 minSegment {kMaxValueOf<TF32>};
    DVec4 maxSegment {kMinValueOf<TF32>};
    this->FrustumBoundingBoxLightViewSpace(nearSegmentPlane, iFarPlanes[i], iRefCamera, minSegment, maxSegment);

    // Update viewports.
    const DVec2 frustumSize {maxFrustum.X - minFrustum.X, maxFrustum.Y - minFrustum.Y};
    const TF32 segmentSizeX = maxSegment.X - minSegment.X;
    const TF32 segmentSizeY = maxSegment.Y - minSegment.Y;
    const TF32 segmentSize  = std::max(segmentSizeX, segmentSizeY);

    const DVec2 offsetBottomLeft         {minSegment.X - minFrustum.X, minSegment.Y - minFrustum.Y};
    const DVec2 offsetSegmentSizeRatio   {offsetBottomLeft.X / segmentSize, offsetBottomLeft.Y / segmentSize};
    const DVec2 frustumSegmentSizeRatio  {frustumSize.X / segmentSize, frustumSize.Y / segmentSize};

    DVec2 pixelOffsetTopLeft = offsetSegmentSizeRatio * kCSMAttachmentTextureSize;
    DVec2 pixelFrustumSize   = frustumSegmentSizeRatio * kCSMAttachmentTextureSize;

    // Scale factor that helps if frustum size is supposed to be bigger than maximum viewport size.
    const DVec2 scaleFactor{
        32768 < pixelFrustumSize.X ? 32768 / pixelFrustumSize.X : 1.0f,
        32768 < pixelFrustumSize.Y ? 32768 / pixelFrustumSize.Y : 1.0f
    };
    pixelOffsetTopLeft *= scaleFactor;
    pixelFrustumSize   *= scaleFactor;

    mLightViewports[i].SetStartPoint(pixelOffsetTopLeft * -1);
    mLightViewports[i].SetWh(pixelFrustumSize);

    // Update light view-projection matrices per segments.
    using namespace math;
    DMat4 lightProjMatrix = ProjectionMatrix<TReal>(
      EGraphics::OpenGL, EProjection::Orthogonal,
      minSegment.X, minSegment.X + segmentSize, 
      minSegment.Y, minSegment.Y + segmentSize, 
      -this->maxFrustum.Z, -this->minFrustum.Z);
    DMat4 lightScale = FMat4::CreateWithScale(0.5f * scaleFactor.X, 0.5f * scaleFactor.Y, 0.5f);
    DMat4 lightBias  = FMat4::CreateWithTranslation(0.5f * scaleFactor.X, 0.5f * scaleFactor.Y, 0.5f);

    this->mDataShadow.mLightVPSBMatrix[i] = 
      ((lightBias * lightScale) * lightProjMatrix) * this->mLightViewMatrix;
    nearSegmentPlane = iNormalizedFarPlanes[i];
  }
}

const std::array<DArea2D, kCSMSegment>& CLightDirectional::GetCSMIndexedViewports() const noexcept
{
  return this->mLightViewports;
}

void CLightDirectional::SetIntensity(TF32 iIntensity) noexcept
{
  if (iIntensity < 0.f) { this->mData.mIntensity = 0.001f; }
  else                  { this->mData.mIntensity = iIntensity; }
  this->mIsNeededUpdateValueToGpu = true;
}

void CLightDirectional::FrustumBoundingBoxLightViewSpace(
    TF32 iNear, TF32 iFar, 
    const CCamera& iRefCamera,
    DVec4& iMin, DVec4& iMax) const
{
   DVec4 minResult {kMaxValueOf<TF32>};
  DVec4 maxResult {kMinValueOf<TF32>};

  const TF32 fov  = math::kToRadian<TF32> * iRefCamera.GetFieldOfView();
  const auto xywh = iRefCamera.GetPixelizedViewportRectangle();
  const auto pos  = iRefCamera.GetPosition();
  const TF32 nearHeight = 2.0f * tan(fov) * iNear;
  const TF32 nearWidth  = nearHeight * xywh[2] / xywh[3];
  const TF32 farHeight  = 2.0f * tan(fov) * iFar;
  const TF32 farWidth   = farHeight * xywh[2] / xywh[3];
  
  const auto& camViewMatrix = iRefCamera.GetViewMatrix();
  const auto rightDir = DVec3{camViewMatrix[0].X, camViewMatrix[0].Y, camViewMatrix[0].Z};
  const auto upDir    = DVec3{camViewMatrix[1].X, camViewMatrix[1].Y, camViewMatrix[1].Z};
  const auto forDir   = DVec3{camViewMatrix[2].X, camViewMatrix[2].Y, camViewMatrix[2].Z};

  const auto nc = pos + forDir * iNear;
  const auto fc = pos + forDir * iFar;

  // Vertices in a world space.
  std::array<DVec4, 8> boundingBoxVertices =
  { //         z    y                             x
    DVec4{nc - (upDir * nearHeight * 0.5f) - (rightDir * nearWidth * 0.5f), 1.0f}, // NBL
    DVec4{nc - (upDir * nearHeight * 0.5f) + (rightDir * nearWidth * 0.5f), 1.0f}, // NBR
    DVec4{nc + (upDir * nearHeight * 0.5f) + (rightDir * nearWidth * 0.5f), 1.0f}, // NTR
    DVec4{nc + (upDir * nearHeight * 0.5f) - (rightDir * nearWidth * 0.5f), 1.0f}, // NTL

    DVec4{fc - (upDir * farHeight * 0.5f) - (rightDir * farWidth * 0.5f), 1.0f}, // FBL
    DVec4{fc - (upDir * farHeight * 0.5f) + (rightDir * farWidth * 0.5f), 1.0f}, // FBR
    DVec4{fc + (upDir * farHeight * 0.5f) + (rightDir * farWidth * 0.5f), 1.0f}, // FTR
    DVec4{fc + (upDir * farHeight * 0.5f) - (rightDir * farWidth * 0.5f), 1.0f}, // FTL
  };

  for (TU32 vertId = 0; vertId < 8; ++vertId)
  { // Light view space.
    boundingBoxVertices[vertId] = this->mLightViewMatrix * boundingBoxVertices[vertId];
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

EDySuccess CLightDirectional::pTryUpdateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  this->mIsNeededUpdateValueToGpu = false;
  return DY_SUCCESS;
}
  
const DUboDirectionalLight& CLightDirectional::GetUboLightInfo() const noexcept
{
  return this->mData;
}

const DDyUboDirShadow& CLightDirectional::GetUboShadowInfo() const noexcept
{
  return this->mDataShadow;
}

void CLightDirectional::TryActivateInstance()
{
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

void CLightDirectional::TryDeactivateInstance()
{
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

EDySuccess CLightDirectional::pTryActivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == true)  { return DY_FAILURE; }
  if (this->mIsCastingLight == false)                       { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsLighting = true;

  // Bind and get a index of UBO array.
  MRendering::GetInstance().MDY_PRIVATE(BindMainDirectionalLight)(*this);
  return DY_SUCCESS;
}

EDySuccess CLightDirectional::pTryDeactivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  if (this->mIsCastingLight == true)                        { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsLighting = false;

  // Try Unbind from lighting system.
  MRendering::GetInstance().MDY_PRIVATE(UnbindMainDirectionalLight)(*this);
  return DY_SUCCESS;
}

EDySuccess CLightDirectional::pTryActivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == true)    { return DY_FAILURE; }
  if (this->mIsCastingShadow == false)                      { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsShadow = true;

  // Try bind to lighting system.
  MRendering::GetInstance().MDY_PRIVATE(BindMainDirectionalShadow)(*this);
  return DY_SUCCESS;
}

EDySuccess CLightDirectional::pTryDeactivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == false)   { return DY_FAILURE; }
  if (this->mIsCastingShadow == true)                       { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsShadow = false;

  // Unbind from shadow system.
  MRendering::GetInstance().MDY_PRIVATE(UnbindMainDirectionalShadow)(*this);
  return DY_SUCCESS;
}

} /// ::dy namespace