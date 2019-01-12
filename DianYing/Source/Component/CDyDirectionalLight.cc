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

EDySuccess CDyDirectionalLight::pTryUpdateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  this->mIsNeededUpdateValueToGpu = false;
  return DY_SUCCESS;
}

} /// ::dy namespace