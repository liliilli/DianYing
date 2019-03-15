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
#include <Dy/Element/Canvas/FDyBasicGaugeBar.h>

namespace dy
{

EDySuccess FDyBasicGaugeBar::Initialize(const PDyMetaWidgetBarDescriptor& objectMetaDesc)
{
  // Set properties.
  this->pSetObjectName(objectMetaDesc.mUiObjectSpecifierName);

  this->mForegroundColor        = objectMetaDesc.mForegroundColor;
  this->mBackgroundColor        = objectMetaDesc.mBackgroundColor;
  this->mIsUsingBackgroundColor = objectMetaDesc.mIsUsingBackground;
  this->mPadding                = objectMetaDesc.mPadding;
  this->mValues.mMin            = objectMetaDesc.mMin;
  this->mValues.mMax            = objectMetaDesc.mMax;

  this->SetRelativePosition(objectMetaDesc.mInitialPosition);
  this->SetFrameSize(objectMetaDesc.mWidgetSize);
  this->SetOrigin(objectMetaDesc.mOrigin);

  // Initialize FontRenderer.
  PCtorBasicGaugeBarRendererInformation desc = {};
  desc.mPtrUiObject = this;
  MDY_CALL_ASSERT_SUCCESS(this->mRenderer.Initialize(desc));

  return DY_SUCCESS;
}

void FDyBasicGaugeBar::TryActivateInstance()
{

}

void FDyBasicGaugeBar::TryDeactivateInstance()
{

}

void FDyBasicGaugeBar::Release()
{
  this->mRenderer.Release();
}

TF32 FDyBasicGaugeBar::GetPresentValue() const noexcept
{
  return this->mValues.mPresent;
}

TF32 FDyBasicGaugeBar::GetMinimumValue() const noexcept
{
  return this->mValues.mMin;
}

TF32 FDyBasicGaugeBar::GetMaximumValue() const noexcept
{
  return this->mValues.mMax;
}

TF32 FDyBasicGaugeBar::GetPercentage() const noexcept
{
  return this->mValues.mPercentage;
}

const DColorRGBA& FDyBasicGaugeBar::GetForegroundColor() const noexcept
{
  return this->mForegroundColor;
}

const DColorRGBA& FDyBasicGaugeBar::GetBackgroundColor() const noexcept
{
  return this->mBackgroundColor;
}

void FDyBasicGaugeBar::SetPresentValue(_MIN_ const TF32 value) noexcept
{
  this->mValues.mPresent = value;
  this->pUpdatePercentage();
}

void FDyBasicGaugeBar::SetForegroundColor(_MIN_ const DColorRGBA& color)
{
  this->mForegroundColor = color;
}

void FDyBasicGaugeBar::SetBackgroundColor(_MIN_ const DColorRGBA& color)
{
  this->mBackgroundColor = color;
}

void FDyBasicGaugeBar::Render()
{
  this->mRenderer.Render();
}

const DVector2& FDyBasicGaugeBar::GetRenderPosition() const noexcept
{
  return this->mCentralFinalPosition;
}

const TI32 FDyBasicGaugeBar::GetPadding() const noexcept
{
  return this->mPadding;
}

std::string FDyBasicGaugeBar::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

void FDyBasicGaugeBar::pUpdatePercentage()
{
  this->mValues.mPercentage =
      (this->mValues.mPresent - this->mValues.mMin)
    / (this->mValues.mMax - this->mValues.mMin);
}

} /// ::dy namespace