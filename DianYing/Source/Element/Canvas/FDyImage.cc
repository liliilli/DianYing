#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

  this->mTintColor        = objectMetaDesc.mTintColor;
  this->mIsSizeToContent  = objectMetaDesc.mIsSizeToContent;

  PDyImageRendererCtorInformation desc = {};
  desc.mPtrUiObject = this;
  MDY_CALL_ASSERT_SUCCESS(this->mRenderer.Initialize(desc));

  return DY_SUCCESS;
}

void FDyImage::Release()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mRenderer.Release();
}

std::string FDyImage::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

void FDyImage::Render()
{
  this->mRenderer.Render();
}

} /// ::dy namespace