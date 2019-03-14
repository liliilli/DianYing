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
#include <Dy/Component/CDyLightSpot.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Element/Actor.h>

namespace dy
{

CDyLightSpot::CDyLightSpot(FDyActor& actorReference) 
  : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyLightSpot::Initialize(const PDyCompSpotLightMetaInfo& iMetaInfo)
{
  this->mData.mColor      = iMetaInfo.mDetails.mColor;
  this->mData.mIntensity  = iMetaInfo.mDetails.mIntensity;
  this->mData.mDirection  = iMetaInfo.mDetails.mDirection;
  this->mData.mRange      = iMetaInfo.mDetails.mRange;
  this->mData.mRangeAngle = iMetaInfo.mDetails.mRangeAngle;

  this->mIsCastingLight   = iMetaInfo.mDetails.mIsCastingLight;

  if (iMetaInfo.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyLightSpot::Release()
{
  if (this->IsActivated() == true) 
  {
    this->Deactivate();
  }
}

std::string CDyLightSpot::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CDyLightSpot::ActivateLighting(bool iFlag) noexcept
{
  this->mIsCastingLight = iFlag;
}

bool CDyLightSpot::IsActiveLighting() const noexcept
{ 
  return this->mIsCastingLight; 
}

void CDyLightSpot::SetIntensity(TF32 iIntensity) noexcept
{
  this->mData.mIntensity = std::max(iIntensity, 0.0f);
}

float CDyLightSpot::GetIntensity() const noexcept
{
  return this->mData.mIntensity;
}

void CDyLightSpot::SetColor(const DDyColorRGB& iColor) noexcept
{
  this->mData.mColor = iColor;
}

const DDyColorRGB& CDyLightSpot::GetColor() const noexcept
{
  return this->mData.mColor;
}

void CDyLightSpot::SetAngle(TF32 iDegree) noexcept
{
  this->mData.mRangeAngle = std::clamp(iDegree, 0.0f, 180.f);
}

TF32 CDyLightSpot::GetAngle() const noexcept
{
  return this->mData.mRangeAngle;
}

void CDyLightSpot::SetDirection(const DDyVector3& iDirection)
{
  this->mData.mDirection = iDirection.Normalize();
}

const DDyVector3& CDyLightSpot::GetDirection() const noexcept
{
  return this->mData.mDirection;
}

const DDyUboSpotLight& CDyLightSpot::GetUboLightInfo() noexcept
{
  this->mData.mPosition = this->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
  return this->mData;
}

void CDyLightSpot::TryActivateInstance()
{
  MDyRendering::GetInstance().__BindSpotLight(*this);
}

void CDyLightSpot::TryDeactivateInstance()
{
  MDY_CALL_ASSERT_SUCCESS(
    MDyRendering::GetInstance().__UnbindSpotLight(*this)
  );
}

} /// ::dy namespace