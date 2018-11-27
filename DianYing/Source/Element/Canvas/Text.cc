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
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Component/Ctor/PDyFontRenderer.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Management/FontManager.h>

namespace dy
{

std::string FDyText::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

EDySuccess FDyText::Initialize(_MIN_ const PDyMetaWidgetTextDescriptor& objectMetaDesc)
{ // Set properties.
  this->pSetObjectName(objectMetaDesc.mUiObjectSpecifierName);

  this->mTextString             = objectMetaDesc.mInitialString;
  this->mForegroundColor        = objectMetaDesc.mInitialColor;
  this->mBackgroundColor        = objectMetaDesc.mBackgroundColor;
  this->mEdgeColor              = objectMetaDesc.mEdgeColor;
  this->mIsUsingEdgeRendering   = objectMetaDesc.mIsUsingEdge;
  this->mIsUsingBackgroundColor = objectMetaDesc.mIsUsingBackground;
  this->mFontSize               = objectMetaDesc.mFontSize;
  this->mPosition               = objectMetaDesc.mInitialPosition;

  // Bind font container resource instance pointer to this.
  auto& fontManager = MDyFont::GetInstance();
  if (fontManager.IsFontResourceContainerExist(objectMetaDesc.mFontSpecifierName) == false)
  {
    MDY_CALL_ASSERT_SUCCESS(fontManager.CreateFontResourceContainer(objectMetaDesc.mFontSpecifierName));
  }
  this->mFontContainer = fontManager.GetFontResourceContainer(objectMetaDesc.mFontSpecifierName);

  // Initialize FontRenderer.
  PDyFontRendererCtorInformation desc = {};
  desc.mFontComponentPtr = this;
  MDY_CALL_ASSERT_SUCCESS(this->mRenderer.Initialize(desc));

  return DY_SUCCESS;
}

void FDyText::Release()
{
  this->mRenderer.Release();
}

//!
//! Getter
//!

const DDyString& FDyText::GetText() const noexcept
{
  return this->mTextString;
}

TU32 FDyText::GetFontSize() const noexcept
{
  return this->mFontSize;
}

const DDyColorRGBA& FDyText::GetBackgroundColor() const noexcept
{
  return this->mBackgroundColor;
}

const DDyColorRGBA& FDyText::GetEdgeColor() const noexcept
{
  return this->mEdgeColor;
}

const DDyColorRGBA& FDyText::GetForegroundColor() const noexcept
{
  return this->mForegroundColor;
}

const DDyVectorInt2& FDyText::GetRenderPosition() const noexcept
{
  return this->mPosition;
}

//!
//! Setter
//!

void FDyText::SetText(const std::string& newU8Text) noexcept
{
  this->mTextString = DDyString{newU8Text};
}

void FDyText::SetFontSize(TU32 fontSize) noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mFontSize = fontSize;
}

EDySuccess FDyText::SetFontName(const std::string& fontName)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

void FDyText::SetColor(const DDyColorRGBA& color)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mForegroundColor = color;
}

void FDyText::Render()
{
  this->mRenderer.Render();
}

} /// ::dy namespace