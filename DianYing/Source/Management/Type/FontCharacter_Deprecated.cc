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
#include <Dy/Management/Type/FontCharacter_Deprecated.h>

//!
//! Forward declaration
//!

namespace
{

}

//!
//! Implementation
//!

namespace dy
{

DDyFontCharacter_Deprecated::DDyFontCharacter_Deprecated(DDyFontCharacter_Deprecated&& rhs) noexcept :
    mTextureId{ rhs.mTextureId },
    mSize { rhs.mSize },
    mBearing { rhs.mBearing },
    mAdvance{ rhs.mAdvance },
    mGlyphScale( rhs.mGlyphScale ),
    mTranslate( rhs.mTranslate )
{
  rhs.mTextureId  = MDY_INITIALIZE_DEFUINT;
  rhs.mSize       = DDyVectorInt2{};
  rhs.mBearing    = DDyVectorInt2{};
  rhs.mAdvance    = MDY_INITIALIZE_DEFUINT;
  rhs.mGlyphScale = DDyVector2{};
  rhs.mTranslate  = DDyVector2{};
}

DDyFontCharacter_Deprecated& DDyFontCharacter_Deprecated::operator=(DDyFontCharacter_Deprecated&& rhs) noexcept
{
  if (this == &rhs) { return *this; }
  mTextureId  = rhs.mTextureId;
  mSize       = rhs.mSize;
  mBearing    = rhs.mBearing;
  mAdvance    = rhs.mAdvance;
  mGlyphScale = rhs.mGlyphScale;
  mTranslate  = rhs.mTranslate;

  rhs.mTextureId = 0;
  rhs.mSize       = DDyVectorInt2{};
  rhs.mBearing    = DDyVectorInt2{};
  rhs.mAdvance    = MDY_INITIALIZE_DEFUINT;
  rhs.mGlyphScale = DDyVector2{};
  rhs.mTranslate  = DDyVector2{};
  return *this;
}

} /// ::dy namespace