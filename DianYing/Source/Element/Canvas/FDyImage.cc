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

  this->MDY_PRIVATE(SetUpdateRendererFlag)(true);
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

void FDyImage::SetImageName(_MIN_ const std::string& iName) noexcept
{
  if (this->mImageName != iName)
  {
    this->mImageName = iName;
    if (this->mIsUpdateRenderer == true) { this->mRenderer.UpdateMaterial(); }
  }
}

void FDyImage::SetShaderSpecifier(_MIN_ const std::string& iSpecifier) noexcept
{
  if (this->mShaderName != iSpecifier)
  {
    this->mShaderName = iSpecifier;
    if (this->mIsUpdateRenderer == true) { this->mRenderer.UpdateMaterial(); }
  }
}

const std::string& FDyImage::GetShaderSpecifier() const noexcept
{
  return this->mShaderName;
}

const std::string& FDyImage::GetImageName() const noexcept
{
  return this->mImageName;
}

void FDyImage::SetTintColor(_MIN_ const DDyColorRGBA& iTintColor) noexcept
{
  this->mTintColor = iTintColor;
}

const DDyColorRGBA& FDyImage::GetTintColor() const noexcept
{
  return this->mTintColor;
}

void FDyImage::MDY_PRIVATE(SetUpdateRendererFlag)(_MIN_ bool iIsActivated) noexcept
{
  if (this->mIsUpdateRenderer != iIsActivated)
  {
    this->mIsUpdateRenderer = iIsActivated;
    if (this->mIsUpdateRenderer == true) { this->mRenderer.UpdateMaterial(); }
  }
}

} /// ::dy namespace