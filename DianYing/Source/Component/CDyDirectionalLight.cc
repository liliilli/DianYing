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

namespace dy
{

CDyDirectionalLight::CDyDirectionalLight(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyDirectionalLight::Initialize(const DDyDirectionalLightMetaInformation& metaInfo)
{
  this->mData.mDirection  = metaInfo.mDirection.Normalize();
  this->mData.mIntensity  = metaInfo.mIntensity;
  this->mIsCastingLight   = metaInfo.mIsCastingLight;
  this->mIsCastingShadow  = metaInfo.mIsCastingShadow;

  this->mData.mDiffuse    = metaInfo.mDiffuse;
  this->mData.mSpecular   = metaInfo.mSpecular;
  this->mData.mAmbient    = metaInfo.mAmbient;

  // Set first time flag to false to use second time flag logics.
  if (metaInfo.mInitiallyActivated) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyDirectionalLight::Release()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
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
  return MDY_INITILAIZE_EMPTYSTR;
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
  MDY_LOG_CRITICAL("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  this->mIsBindedToRenderingManagerAsLighting = true;
  return DY_FAILURE;
}

EDySuccess CDyDirectionalLight::pTryActivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == true)    { return DY_FAILURE; }
  if (this->mIsCastingShadow == false)                      { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  MDY_NOT_IMPLEMENTED_ASSERT();

  this->mIsBindedToRenderingManagerAsShadow = true;
  return DY_FAILURE;
}

EDySuccess CDyDirectionalLight::pTryDeactivateDirectionalLight()
{
  if (this->mIsBindedToRenderingManagerAsLighting == false) { return DY_FAILURE; }
  if (this->mIsCastingLight == true)                        { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }

  MDY_LOG_CRITICAL("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  this->mIsBindedToRenderingManagerAsLighting = false;
  return DY_FAILURE;
}

EDySuccess CDyDirectionalLight::pTryDeactivateCastingShadow()
{
  if (this->mIsBindedToRenderingManagerAsShadow == false)   { return DY_FAILURE; }
  if (this->mIsCastingShadow == true)                       { return DY_FAILURE; }
  if (this->mActivateFlag.IsOutputValueChanged() == false)  { return DY_FAILURE; }
  MDY_NOT_IMPLEMENTED_ASSERT();

  this->mIsBindedToRenderingManagerAsShadow = false;
  return DY_FAILURE;
}

} /// ::dy namespace