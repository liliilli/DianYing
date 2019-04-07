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
#include <Dy/Component/CLightPoint.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Element/FActor.h>
#include <Dy/Component/CTransform.h>

namespace dy
{

CLightPoint::CLightPoint(FActor& actorReference) 
  : AGeneralBaseComponent(actorReference)
{ }

EDySuccess CLightPoint::Initialize(const PCompPointLightMetaInfo& iMetaInfo)
{
  this->mData.mColor      = iMetaInfo.mDetails.mColor;
  this->mData.mIntensity  = iMetaInfo.mDetails.mIntensity;
  this->mData.mRange      = iMetaInfo.mDetails.mRange;

  this->mIsCastingLight   = iMetaInfo.mDetails.mIsCastingLight;

  if (iMetaInfo.mInitiallyActivated == true) { this->Activate(); }
  return EDySuccess::DY_SUCCESS;
}

void CLightPoint::Release()
{
  if (this->IsActivated() == true) 
  {
    this->Deactivate();
  }
}

std::string CLightPoint::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CLightPoint::ActivateLighting(bool iFlag) noexcept
{
  this->mIsCastingLight = iFlag;
}

bool CLightPoint::IsActiveLighting() const noexcept
{ 
  return this->mIsCastingLight; 
}

void CLightPoint::SetIntensity(TF32 iIntensity) noexcept
{
  this->mData.mIntensity = std::max(iIntensity, 0.0f);
}

float CLightPoint::GetIntensity() const noexcept
{
  return this->mData.mIntensity;
}

void CLightPoint::SetColor(const DColorRGB& iColor) noexcept
{
  this->mData.mColor = iColor;
}

const DColorRGB& CLightPoint::GetColor() const noexcept
{
  return this->mData.mColor;
}

const DUboPointLight& CLightPoint::GetUboLightInfo() noexcept
{
  this->mData.mPosition = this->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
  return this->mData;
}

void CLightPoint::TryActivateInstance()
{
  MRendering::GetInstance().__BindPointLight(*this);
}

void CLightPoint::TryDeactivateInstance()
{
  MDY_CALL_ASSERT_SUCCESS(
    MRendering::GetInstance().__UnbindPointLight(*this)
  );
}

} /// ::dy namespace