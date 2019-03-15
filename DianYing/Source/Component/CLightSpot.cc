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
#include <Dy/Component/CLightSpot.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Element/Actor.h>
#include <Dy/Component/CDyTransform.h>

namespace dy
{

CLightSpot::CLightSpot(FDyActor& actorReference) 
  : AGeneralBaseComponent(actorReference)
{ }

EDySuccess CLightSpot::Initialize(const PCompSpotLightMetaInfo& iMetaInfo)
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

void CLightSpot::Release()
{
  if (this->IsActivated() == true) 
  {
    this->Deactivate();
  }
}

std::string CLightSpot::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CLightSpot::ActivateLighting(bool iFlag) noexcept
{
  this->mIsCastingLight = iFlag;
}

bool CLightSpot::IsActiveLighting() const noexcept
{ 
  return this->mIsCastingLight; 
}

void CLightSpot::SetIntensity(TF32 iIntensity) noexcept
{
  this->mData.mIntensity = std::max(iIntensity, 0.0f);
}

float CLightSpot::GetIntensity() const noexcept
{
  return this->mData.mIntensity;
}

void CLightSpot::SetColor(const DColorRGB& iColor) noexcept
{
  this->mData.mColor = iColor;
}

const DColorRGB& CLightSpot::GetColor() const noexcept
{
  return this->mData.mColor;
}

void CLightSpot::SetAngle(TF32 iDegree) noexcept
{
  this->mData.mRangeAngle = std::clamp(iDegree, 0.0f, 180.f);
}

TF32 CLightSpot::GetAngle() const noexcept
{
  return this->mData.mRangeAngle;
}

void CLightSpot::SetDirection(const DVector3& iDirection)
{
  this->mData.mDirection = iDirection.Normalize();
}

const DVector3& CLightSpot::GetDirection() const noexcept
{
  return this->mData.mDirection;
}

const DUboSpotLight& CLightSpot::GetUboLightInfo() noexcept
{
  this->mData.mPosition = this->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
  return this->mData;
}

void CLightSpot::TryActivateInstance()
{
  MRendering::GetInstance().__BindSpotLight(*this);
}

void CLightSpot::TryDeactivateInstance()
{
  MDY_CALL_ASSERT_SUCCESS(
    MRendering::GetInstance().__UnbindSpotLight(*this)
  );
}

} /// ::dy namespace