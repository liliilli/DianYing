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
#include <Dy/Core/Resource/Information/FDyTextureInformation.h>

namespace dy
{

FDyTextureInformation::FDyTextureInformation(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo)
{
  // Common
  this->mSpecifierName        = metaInfo.mSpecifierName;
  this->mBorderColor          = metaInfo.mBorderColor;
  this->mParameterOptionList  = metaInfo.mParameterList;
  this->mTextureType          = metaInfo.mTextureType;
  this->mIsUsingDefaultMipmap = metaInfo.mIsUsingDefaultMipmapGeneration;
  this->mIsUsingCustomParams  = metaInfo.mIsEnabledCustomedTextureParameter;
  this->mInternalPixelReadType= metaInfo.mPixelReadType;

  if (metaInfo.mSourceType == EDyResourceSource::Builtin)
  {
    this->mImageActualPixelFormat = metaInfo.mTextureColorType;
  }
}

FDyTextureInformation::~FDyTextureInformation() = default;

const std::string& FDyTextureInformation::GetSpecifierName() const noexcept
{
  return this->mSpecifierName;
}

EDyImageColorFormatStyle FDyTextureInformation::GetFormat() const noexcept
{
  return this->mImageActualPixelFormat;
}

EDyGlImagePixelReadType FDyTextureInformation::GetPixelReadType() const noexcept
{
  return this->mInternalPixelReadType;
}

ETextureStyleType FDyTextureInformation::GetType() const noexcept
{
  return this->mTextureType;
}

const DColorRGBA& FDyTextureInformation::GetBorderColor() const noexcept
{
  return this->mBorderColor;
}

bool FDyTextureInformation::IsUsingCustomizedParamater() const noexcept
{
  return this->mIsUsingCustomParams;
}

bool FDyTextureInformation::IsUsingDefaultMipmap() const noexcept
{
  return this->mIsUsingDefaultMipmap;
}

const TTextureParameterList& FDyTextureInformation::GetParameterList() const noexcept
{
  return this->mParameterOptionList;
}

} /// ::dy namespace