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
#include <Dy/Element/Widget/FWidgetBasicGaugeBar.h>

namespace dy
{

EDySuccess FWidgetBasicGaugeBar::Initialize(const PDyMetaWidgetBarDescriptor& objectMetaDesc)
{
  // Set properties.
  this->pSetObjectName(objectMetaDesc.mUiObjectSpecifierName);

  this->mForegroundColor        = objectMetaDesc.mForegroundColor;
  this->mBackgroundColor        = objectMetaDesc.mBackgroundColor;
  this->mIsUsingBackgroundColor = objectMetaDesc.mIsUsingBackground;
  this->mPadding                = objectMetaDesc.mPadding;
  this->mValues.mMin            = objectMetaDesc.mMin;
  this->mValues.mMax            = objectMetaDesc.mMax;

  this->SetRelativePosition(static_cast<DVec2>(objectMetaDesc.mInitialPosition));
  this->SetFrameSize(objectMetaDesc.mWidgetSize);
  this->SetOrigin(objectMetaDesc.mOrigin);

  // Initialize FontRenderer.
  PCtorBasicGaugeBarRendererInformation desc = {};
  desc.mPtrUiObject = this;
  MDY_CALL_ASSERT_SUCCESS(this->mRenderer.Initialize(desc));

  // Activate when flag is true.
  if (objectMetaDesc.mIsActivated == true)
  {
    this->Activate();
  }

  return DY_SUCCESS;
}

void FWidgetBasicGaugeBar::TryActivateInstance()
{

}

void FWidgetBasicGaugeBar::TryDeactivateInstance()
{

}

void FWidgetBasicGaugeBar::Release()
{
  this->mRenderer.Release();
}

TF32 FWidgetBasicGaugeBar::GetPresentValue() const noexcept
{
  return this->mValues.mPresent;
}

TF32 FWidgetBasicGaugeBar::GetMinimumValue() const noexcept
{
  return this->mValues.mMin;
}

TF32 FWidgetBasicGaugeBar::GetMaximumValue() const noexcept
{
  return this->mValues.mMax;
}

TF32 FWidgetBasicGaugeBar::GetPercentage() const noexcept
{
  return this->mValues.mPercentage;
}

const DColorRGBA& FWidgetBasicGaugeBar::GetForegroundColor() const noexcept
{
  return this->mForegroundColor;
}

const DColorRGBA& FWidgetBasicGaugeBar::GetBackgroundColor() const noexcept
{
  return this->mBackgroundColor;
}

void FWidgetBasicGaugeBar::SetPresentValue(_MIN_ const TF32 value) noexcept
{
  this->mValues.mPresent = value;
  this->pUpdatePercentage();
}

void FWidgetBasicGaugeBar::SetForegroundColor(_MIN_ const DColorRGBA& color)
{
  this->mForegroundColor = color;
}

void FWidgetBasicGaugeBar::SetBackgroundColor(_MIN_ const DColorRGBA& color)
{
  this->mBackgroundColor = color;
}

void FWidgetBasicGaugeBar::Render()
{
  this->mRenderer.Render();
}

const DVec2& FWidgetBasicGaugeBar::GetRenderPosition() const noexcept
{
  return this->mCentralFinalPosition;
}

const TI32 FWidgetBasicGaugeBar::GetPadding() const noexcept
{
  return this->mPadding;
}

std::string FWidgetBasicGaugeBar::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void FWidgetBasicGaugeBar::pUpdatePercentage()
{
  this->mValues.mPercentage =
      (this->mValues.mPresent - this->mValues.mMin)
    / (this->mValues.mMax - this->mValues.mMin);
}

} /// ::dy namespace