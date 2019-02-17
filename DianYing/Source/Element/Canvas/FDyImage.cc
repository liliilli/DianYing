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
#include <Dy/Management/IO/MetaInfoManager.h>

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
  this->SetRenderableImageName(objectMetaDesc.mImageSpecifierName);

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

void FDyImage::SetRenderableImageName(_MIN_ const std::string& iName, _MIN_ bool iIsMaterial) noexcept
{
  // Check vailidty.
  if (this->mImageName == iName && this->mIsMaterial != iIsMaterial) { return; }

  auto& refMetaInfo = MDyMetaInfo::GetInstance();
  if (iIsMaterial == true)
  {
    // Check
    if (refMetaInfo.IsMaterialMetaInfoExist(iName) == false)
    {
      MDY_LOG_ERROR("Failed to set image name, name : {}, material : {}", iName, iIsMaterial);
      return;
    }

    this->mImageName = iName; this->mIsMaterial = true;
    MDY_NOT_IMPLEMENTED_ASSERT();
  }
  else
  {
    // Check
    if (refMetaInfo.IsTextureMetaInfoExist(iName) == false)
    {
      MDY_LOG_ERROR("Failed to set image name, name : {}, material : {}", iName, iIsMaterial);
      return;
    }

    this->mImageName = iName; this->mIsMaterial = false;
    if (this->mIsUpdateRenderer == true) 
    { 
      this->mRenderer.UpdateRenderableTarget(); 
    }
  }
}

std::pair<const std::string&, bool> FDyImage::GetRenderableImageName() const noexcept
{
  return {this->mImageName, this->mIsMaterial};
}

void FDyImage::SetTintColor(_MIN_ const DDyColorRGBA& iTintColor) noexcept
{
  this->mTintColor = iTintColor;
}

const DDyColorRGBA& FDyImage::GetTintColor() const noexcept
{
  return this->mTintColor;
}

bool FDyImage::IsUsingMaterial() const noexcept
{
  return this->mIsMaterial;
}

void FDyImage::MDY_PRIVATE(SetUpdateRendererFlag)(_MIN_ bool iIsActivated) noexcept
{
  if (this->mIsUpdateRenderer != iIsActivated)
  {
    this->mIsUpdateRenderer = iIsActivated;
    if (this->mIsUpdateRenderer == true) { this->mRenderer.UpdateRenderableTarget(); }
  }
}

} /// ::dy namespace