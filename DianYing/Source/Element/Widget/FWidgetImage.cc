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
#include <Dy/Element/Widget/FWidgetImage.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MLog.h>

namespace dy
{

EDySuccess FWidgetImage::Initialize(const PDyMetaWidgetImageDescriptor& objectMetaDesc)
{
  // Set properties.
  this->pSetObjectName(objectMetaDesc.mUiObjectSpecifierName);
  this->SetRelativePosition(static_cast<DVec2>(objectMetaDesc.mInitialPosition));
  this->SetFrameSize(objectMetaDesc.mWidgetSize);
  this->mOrigin = objectMetaDesc.mOrigin;

  this->mIsSizeToContent  = objectMetaDesc.mIsSizeToContent;
  this->SetTintColor(objectMetaDesc.mTintColor);
  this->SetRenderableImageName(objectMetaDesc.mImageSpecifierName);

  this->MDY_PRIVATE(SetUpdateRendererFlag)(true);

  // Activate when flag is true.
  if (objectMetaDesc.mIsActivated == true)
  {
    this->Activate();
  }

  return DY_SUCCESS;
}

void FWidgetImage::Release() { }

std::string FWidgetImage::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void FWidgetImage::Render()
{
  this->mRenderer.Render();
}

void FWidgetImage::SetRenderableImageName(_MIN_ const std::string& iName, _MIN_ bool iIsMaterial) noexcept
{
  // Check vailidty.
  if (this->mImageName == iName && this->mIsMaterial != iIsMaterial) { return; }

  auto& refMetaInfo = MIOMeta::GetInstance();
  if (iIsMaterial == true)
  {
    // Check
    if (refMetaInfo.IsMaterialMetaInfoExist(iName) == false)
    {
      DyPushLogError("Failed to set image name, name : {}, material : {}", iName, iIsMaterial);
      return;
    }

    this->mImageName = iName; this->mIsMaterial = true;
    if (this->mIsUpdateRenderer == true)
    {
      this->mRenderer.UpdateRenderableTarget(); 
    }
  }
  else
  {
    // Check
    if (refMetaInfo.IsTextureMetaInfoExist(iName) == false)
    {
      DyPushLogError("Failed to set image name, name : {}, material : {}", iName, iIsMaterial);
      return;
    }

    this->mImageName = iName; this->mIsMaterial = false;
    if (this->mIsUpdateRenderer == true) 
    { 
      this->mRenderer.UpdateRenderableTarget(); 
    }
  }
}

std::pair<const std::string&, bool> FWidgetImage::GetRenderableImageName() const noexcept
{
  return {this->mImageName, this->mIsMaterial};
}

void FWidgetImage::SetTintColor(_MIN_ const DColorRGBA& iTintColor) noexcept
{
  this->mTintColor = iTintColor;
}

const DColorRGBA& FWidgetImage::GetTintColor() const noexcept
{
  return this->mTintColor;
}

bool FWidgetImage::IsUsingMaterial() const noexcept
{
  return this->mIsMaterial;
}

TDyResourceBinderMaterial* FWidgetImage::GetUsingMaterial()
{
  // Check
  if (this->IsUsingMaterial() == false) { return nullptr; }

  // Get from internal object.
  return this->mRenderer.GetUsingMaterial();
}

void FWidgetImage::MDY_PRIVATE(SetUpdateRendererFlag)(_MIN_ bool iIsActivated) noexcept
{
  if (this->mIsUpdateRenderer != iIsActivated)
  {
    this->mIsUpdateRenderer = iIsActivated;
    if (this->mIsUpdateRenderer == true) { this->mRenderer.UpdateRenderableTarget(); }
  }
}

void FWidgetImage::TryActivateInstance()
{

}

void FWidgetImage::TryDeactivateInstance()
{

}

} /// ::dy namespace