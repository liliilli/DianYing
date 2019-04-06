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
#include <Dy/Element/Widget/FWidgetText.h>
#include <Dy/Meta/Descriptor/WidgetTextMetaInformation.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Management/MFont.h>

namespace dy
{

std::string FWidgetText::ToString()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return "";
}

EDySuccess FWidgetText::Initialize(const PDyMetaWidgetTextDescriptor& objectMetaDesc)
{
  /// @brief Bind font container resource instance pointer to this.
  /// @TODO IMPLEMENT CHECKING DEFAULT FONT CASE.
  static auto GetFontResource = [](const std::string& fontSpecifierName) -> AFontContainer*
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
  this->SetRelativePosition(static_cast<DVec2>(objectMetaDesc.mInitialPosition));
  this->SetFrameSize(objectMetaDesc.mWidgetSize);
  this->SetOrigin(objectMetaDesc.mOrigin);

  this->mTextString             = objectMetaDesc.mTextString;
  this->mForegroundColor        = objectMetaDesc.mTextColor;
  this->mBackgroundColor        = objectMetaDesc.mBackgroundColor;
  this->mEdgeColor              = objectMetaDesc.mEdgeColor;
  this->mIsUsingEdgeRendering   = objectMetaDesc.mIsUsingEdge;
  this->mIsUsingBackgroundColor = objectMetaDesc.mIsUsingBackground;
  this->mFontSize               = objectMetaDesc.mFontSize;
  this->mAlignment              = objectMetaDesc.mAlignment;

  if (objectMetaDesc.mFontName.empty() == true)
  {
    this->mPtrFontContainer = GetFontResource("dyBtFtDefaultGothic");
  }
  else
  {
    this->mPtrFontContainer = GetFontResource(objectMetaDesc.mFontName);
  }

  // Activate when flag is true.
  if (objectMetaDesc.mIsActivated == true)
  {
    this->Activate();
  }

  return DY_SUCCESS;
}

void FWidgetText::Release() { }

//!
//! Getter
//!

const DString& FWidgetText::GetText() const noexcept
{
  return this->mTextString;
}

TU32 FWidgetText::GetFontSize() const noexcept
{
  return this->mFontSize;
}

const DColorRGBA& FWidgetText::GetBackgroundColor() const noexcept
{
  return this->mBackgroundColor;
}

const DColorRGBA& FWidgetText::GetEdgeColor() const noexcept
{
  return this->mEdgeColor;
}

const DColorRGBA& FWidgetText::GetForegroundColor() const noexcept
{
  return this->mForegroundColor;
}

const DVec2& FWidgetText::GetRenderPosition() const noexcept
{
  return this->mCentralFinalPosition;
}

//!
//! Setter
//!

void FWidgetText::SetAlignment(_MIN_ EDyHorizontalAlignment iAlignment) noexcept
{
  this->mAlignment = iAlignment;
}

EDyHorizontalAlignment FWidgetText::GetAlignment() const noexcept
{
  return this->mAlignment;
}

void FWidgetText::SetText(_MIN_ const std::string& newU8Text) noexcept
{
  this->mTextString = DString{newU8Text};
}

void FWidgetText::SetFontSize(_MIN_ TU32 fontSize) noexcept
{
  this->mFontSize = fontSize;
}

EDySuccess FWidgetText::SetFontName(_MIN_ const std::string& fontName)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

void FWidgetText::SetColor(_MIN_ const DColorRGBA& color)
{
  this->mForegroundColor = color;
}

void FWidgetText::Render()
{
  this->mRenderer.Render();
}

void FWidgetText::TryActivateInstance()
{

}

void FWidgetText::TryDeactivateInstance()
{

}

} /// ::dy namespace