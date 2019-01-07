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
#include <Dy/Element/Canvas/FDyImage.h>
#include <Dy/Component/Ctor/PDyImageRenderer.h>

namespace dy
{

EDySuccess FDyImage::Initialize(const PDyMetaWidgetImageDescriptor& objectMetaDesc)
{
  // Set properties.
  this->pSetObjectName(objectMetaDesc.mUiObjectSpecifierName);
  this->SetRelativePosition(objectMetaDesc.mInitialPosition);
  this->SetFrameSize(objectMetaDesc.mWidgetSize);
  this->mOrigin = objectMetaDesc.mOrigin;

  this->mIsSizeToContent  = objectMetaDesc.mIsSizeToContent;
  this->SetTintColor(objectMetaDesc.mTintColor);
  this->SetImageName(objectMetaDesc.mImageSpecifierName);
  return DY_SUCCESS;
}

void FDyImage::Release() { }

std::string FDyImage::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void FDyImage::Render()
{
  this->mRenderer.Render();
}

void FDyImage::SetImageName(const std::string& iName) noexcept
{
  this->mImageName = iName;
  this->mRenderer.UpdateImageBinding(this->mImageName);
}

const std::string& FDyImage::GetImageName() const noexcept
{
  return this->mImageName;
}

void FDyImage::SetTintColor(const DDyColorRGBA& iTintColor) noexcept
{
  this->mTintColor = iTintColor;
}

const DDyColorRGBA& FDyImage::GetTintColor() const noexcept
{
  return this->mTintColor;
}

} /// ::dy namespace