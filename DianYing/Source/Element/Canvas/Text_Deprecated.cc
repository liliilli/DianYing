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
#include <Dy/Element/Canvas/Text_Deprecated.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Management/FontManager.h>

namespace dy
{
FDyText_Deprecated::FDyText_Deprecated() : mFontContainer(MDyFont::GetInstance().GetDefaultFontContainer())
{ }

std::string FDyText_Deprecated::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

EDySuccess FDyText_Deprecated::Initialize(_MIN_ const PDyMetaWidgetTextDescriptor& objectMetaDesc)
{
  this->mTextString             = objectMetaDesc.mInitialString;
  this->mForegroundColor        = objectMetaDesc.mInitialColor;
  this->mBackgroundColor        = objectMetaDesc.mBackgroundColor;
  this->mEdgeColor              = objectMetaDesc.mEdgeColor;
  this->mIsUsingEdgeRendering   = objectMetaDesc.mIsUsingEdge;
  this->mIsUsingBackgroundColor = objectMetaDesc.mIsUsingBackground;
  this->mFontSize               = objectMetaDesc.mFontSize;
  this->mPosition               = objectMetaDesc.mInitialPosition;

  PDyFontRendererCtorInformation_Deprecated desc = {};
  desc.mFontComponentPtr = this;
  MDY_CALL_ASSERT_SUCCESS(this->mRenderer.Initialize(desc));

  // @TODO IMPLEMENT OTHER PROPERTIES

  return DY_SUCCESS;
}

void FDyText_Deprecated::Release()
{
  this->mRenderer.Release();
}

//!
//! Getter
//!

const DDyString& FDyText_Deprecated::GetText() const noexcept
{
  return this->mTextString;
}

std::string FDyText_Deprecated::GetUtf8Text() const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_EMPTYSTR;
}

TU32 FDyText_Deprecated::GetFontSize() const noexcept
{
  return this->mFontSize;
}

const DDyColorRGBA& FDyText_Deprecated::GetBackgroundColor() const noexcept
{
  return this->mBackgroundColor;
}

const DDyColorRGBA& FDyText_Deprecated::GetEdgeColor() const noexcept
{
  return this->mEdgeColor;
}

const DDyColorRGBA& FDyText_Deprecated::GetForegroundColor() const noexcept
{
  return this->mForegroundColor;
}

const DDyVectorInt2& FDyText_Deprecated::GetRenderPosition() const noexcept
{
  return this->mPosition;
}

//!
//! Setter
//!

void FDyText_Deprecated::SetText(const std::string& newU8Text) noexcept
{
  this->mTextString = DDyString{newU8Text};
}

void FDyText_Deprecated::SetFontSize(TU32 fontSize) noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mFontSize = fontSize;
}

EDySuccess FDyText_Deprecated::SetFontName(const std::string& fontName)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

void FDyText_Deprecated::SetColor(const DDyColorRGBA& color)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mForegroundColor = color;
}

void FDyText_Deprecated::Render()
{
  this->mRenderer.Render();
}

} /// ::dy namespace