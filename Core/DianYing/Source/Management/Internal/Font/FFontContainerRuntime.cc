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
#include <Dy/Management/Internal/Font/FFontContainerRuntime.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>

namespace dy
{
  
FFontContainerRuntime::FFontContainerRuntime(const PDyMetaFontInformation::DRuntime& details)
  : AFontContainer{
    PDyMetaFontInformation::ELoadingType::Runtime,
    PDyMetaFontInformation::EFontType::Plain}
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

FFontContainerRuntime::~FFontContainerRuntime()
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  XGLWrapper::DeleteTexture(this->mTexImageResId);
}

bool FFontContainerRuntime::IsCharacterGlyphExist(TChr16 fontCode)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return this->mCharContainer.find(fontCode) != this->mCharContainer.end();
}

const DDyFontCharacterInfo& FFontContainerRuntime::GetGlyphCharacter(TChr16 fontCode)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  if (this->IsCharacterGlyphExist(fontCode) == true)  { return this->mCharContainer[fontCode]; }
  else                                                { return sDefaultFontCharacterInfo; }
}

TI32 FFontContainerRuntime::GetLinefeedHeight(TI32 fontSize) const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return 0;
}

const DDyFontCharacterInfo& FFontContainerRuntime::operator[](TChr16 fontCode)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return this->mCharContainer[fontCode];
}

TI32 FFontContainerRuntime::GetFontTextureArrayId() const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return this->mTexImageResId;
}

} /// ::dy namespace