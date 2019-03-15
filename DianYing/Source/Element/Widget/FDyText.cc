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
#include <Dy/Element/Canvas/Text.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Management/MFont.h>

namespace dy
{

std::string FDyText::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

EDySuccess FDyText::Initialize(const PDyMetaWidgetTextDescriptor& objectMetaDesc)
{
  /// @brief Bind font container resource instance pointer to this.
  /// @TODO IMPLEMENT CHECKING DEFAULT FONT CASE.
  static auto GetFontResource = [](const std::string& fontSpecifierName) -> IFontContainer*
  {
    auto& fontManager = MFont::GetInstance();
    if (fontManager.IsFontResourceContainerExist(fontSpecifierName) == false)
    {
      MDY_CALL_ASSERT_SUCCESS(fontManager.CreateFontResourceContainer(fontSpecifierName));
    }

    return fontManager.GetFontResourceContainer(fontSpecifierName);
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Set Properties.
  this->pSetObjectName(objectMetaDesc.mUiObjectSpecifierName);
  this->SetRelativePosition(objectMetaDesc.mInitialPosition);
  this->SetFrameSize(objectMetaDesc.mWidgetSize);
  this->SetOrigin(objectMetaDesc.mOrigin);

  this->mTextString             = objectMetaDesc.mInitialString;
  this->mForegroundColor        = objectMetaDesc.mInitialColor;
  this->mBackgroundColor        = objectMetaDesc.mBackgroundColor;
  this->mEdgeColor              = objectMetaDesc.mEdgeColor;
  this->mIsUsingEdgeRendering   = objectMetaDesc.mIsUsingEdge;
  this->mIsUsingBackgroundColor = objectMetaDesc.mIsUsingBackground;
  this->mFontSize               = objectMetaDesc.mFontSize;
  this->mAlignment              = objectMetaDesc.mAlignment;
  this->mPtrFontContainer       = GetFontResource(objectMetaDesc.mFontSpecifierName);

  return DY_SUCCESS;
}

void FDyText::Release() { }

//!
//! Getter
//!

const DString& FDyText::GetText() const noexcept
{
  return this->mTextString;
}

TU32 FDyText::GetFontSize() const noexcept
{
  return this->mFontSize;
}

const DColorRGBA& FDyText::GetBackgroundColor() const noexcept
{
  return this->mBackgroundColor;
}

const DColorRGBA& FDyText::GetEdgeColor() const noexcept
{
  return this->mEdgeColor;
}

const DColorRGBA& FDyText::GetForegroundColor() const noexcept
{
  return this->mForegroundColor;
}

const DVector2& FDyText::GetRenderPosition() const noexcept
{
  return this->mCentralFinalPosition;
}

//!
//! Setter
//!

void FDyText::SetAlignment(_MIN_ EDyHorizontalAlignment iAlignment) noexcept
{
  this->mAlignment = iAlignment;
}

EDyHorizontalAlignment FDyText::GetAlignment() const noexcept
{
  return this->mAlignment;
}

void FDyText::SetText(_MIN_ const std::string& newU8Text) noexcept
{
  this->mTextString = DString{newU8Text};
}

void FDyText::SetFontSize(_MIN_ TU32 fontSize) noexcept
{
  this->mFontSize = fontSize;
}

EDySuccess FDyText::SetFontName(_MIN_ const std::string& fontName)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

void FDyText::SetColor(_MIN_ const DColorRGBA& color)
{
  this->mForegroundColor = color;
}

void FDyText::Render()
{
  this->mRenderer.Render();
}

void FDyText::TryActivateInstance()
{

}

void FDyText::TryDeactivateInstance()
{

}

} /// ::dy namespace