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
#include <Dy/Component/CDyLightPoint.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Element/Actor.h>

namespace dy
{

CDyLightPoint::CDyLightPoint(FDyActor& actorReference) 
  : ADyGeneralBaseComponent(actorReference)
{ }

EDySuccess CDyLightPoint::Initialize(const PDyCompPointLightMetaInfo& iMetaInfo)
{
  this->mData.mColor      = iMetaInfo.mDetails.mColor;
  this->mData.mIntensity  = iMetaInfo.mDetails.mIntensity;
  this->mData.mRange      = iMetaInfo.mDetails.mRange;

  this->mIsCastingLight   = iMetaInfo.mDetails.mIsCastingLight;

  if (iMetaInfo.mInitiallyActivated == true) { this->Activate(); }
  return DY_SUCCESS;
}

void CDyLightPoint::Release()
{
  if (this->IsActivated() == true) 
  {
    this->Deactivate();
  }
}

std::string CDyLightPoint::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void CDyLightPoint::ActivateLighting(bool iFlag) noexcept
{
  this->mIsCastingLight = iFlag;
}

bool CDyLightPoint::IsActiveLighting() const noexcept
{ 
  return this->mIsCastingLight; 
}

const DDyUboPointLight& CDyLightPoint::GetUboLightInfo() noexcept
{
  this->mData.mPosition = this->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
  return this->mData;
}

void CDyLightPoint::TryActivateInstance()
{
  MDyRendering::GetInstance().__BindPointLight(*this);
}

void CDyLightPoint::TryDeactivateInstance()
{
  MDY_CALL_ASSERT_SUCCESS(
    MDyRendering::GetInstance().__UnbindPointLight(*this)
  );
}

} /// ::dy namespace