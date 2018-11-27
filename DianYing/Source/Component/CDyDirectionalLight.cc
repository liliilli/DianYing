#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

EDySuccess CDyDirectionalLight::pTryActivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == true)  { return DY_FAILURE; }
  if (this->mIsCastingLight == false)                       { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsLighting = true;

  // Bind and get a index of UBO array.
  auto& renderingManager  = MDyRendering::GetInstance();
  const auto opIndex      = renderingManager.pGetAvailableDirectionalLightIndex(*this);
  if (opIndex.has_value() == false)
  {
    MDY_LOG_WARNING("CDyDirectionalLight::pTryActivateCastingShadow | Failed to issue available directional light index.");
    return DY_FAILURE;
  }
  this->mCastingLightUboIndex = opIndex.value();

  // Update values
  MDY_NOUSE_RTVAL_EXPR(this->pTryUpdateDirectionalLight());

  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryActivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == true)    { return DY_FAILURE; }
  if (this->mIsCastingShadow == false)                      { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsShadow = true;

  // Try bind to shadow system.
  auto& renderingManager = MDyRendering::GetInstance();
  if (renderingManager.pfIsAvailableDirectionalLightShadow(*this) == false) { return DY_FAILURE; }

  // Update values
  MDY_CALL_ASSERT_SUCCESS(renderingManager.pfUpdateDirectionalLightShadowToGpu(*this));
  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryDeactivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  if (this->mIsCastingLight == true)                        { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsLighting = false;

  // Unbind from lighting system.
  auto& renderingManager = MDyRendering::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(renderingManager.pUnbindDirectionalLight(*this));
  this->mCastingLightUboIndex = MDY_INITIALIZE_DEFINT;

  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryDeactivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == false)   { return DY_FAILURE; }
  if (this->mIsCastingShadow == true)                       { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  this->mIsBindedToRenderingManagerAsShadow = false;

  // Unbind from shadow system.
  auto& renderingManager = MDyRendering::GetInstance();
  if (renderingManager.pfIsAvailableDirectionalLightShadow(*this) == true) { return DY_FAILURE; }

  MDY_CALL_ASSERT_SUCCESS(renderingManager.pfUnbindDirectionalLightShadowToGpu(*this));
  return DY_SUCCESS;
}

EDySuccess CDyDirectionalLight::pTryUpdateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  if (this->mCastingLightUboIndex == MDY_INITIALIZE_DEFINT) { return DY_FAILURE; }

  auto& renderingManager  = MDyRendering::GetInstance();
  MDY_CALL_ASSERT_SUCCESS( \
      renderingManager.pUpdateDirectionalLightValueToGpu(this->mCastingLightUboIndex, this->mData));

  this->mIsNeededUpdateValueToGpu = false;
  return DY_SUCCESS;
}

} /// ::dy namespace