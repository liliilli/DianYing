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
#include <Dy/Management/Internal/Font/FFontContainerCompressed.h>

namespace dy
{
  
FFontContainerCompressed::FFontContainerCompressed(
  const PDyMetaFontInformation::DExternalCompressed& details)
  : AFontContainer{
    PDyMetaFontInformation::ELoadingType::ExternalCompressed,
    details.mFontType}
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

FFontContainerCompressed::~FFontContainerCompressed()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  glDeleteTextures(1, &this->mTexImageResId);
}

bool FFontContainerCompressed::IsCharacterGlyphExist(TChr16 fontCode)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return this->mCharContainer.find(fontCode) != this->mCharContainer.end();
}

const DDyFontCharacterInfo& FFontContainerCompressed::GetGlyphCharacter(TChr16 fontCode)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  if (this->IsCharacterGlyphExist(fontCode) == true)  { return this->mCharContainer[fontCode]; }
  else                                                { return sDefaultFontCharacterInfo; }
}

TI32 FFontContainerCompressed::GetLinefeedHeight(TI32 fontSize) const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return 0;
}

const DDyFontCharacterInfo& FFontContainerCompressed::operator[](TChr16 fontCode)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return this->mCharContainer[fontCode];
}

TI32 FFontContainerCompressed::GetFontTextureArrayId() const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return this->mTexImageResId;
}

} /// ::dy namespace