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
#include <Dy/Core/Resource/Information/FDyTextureGeneralInformation.h>

namespace dy
{

FDyTextureGeneralInformation::FDyTextureGeneralInformation(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo) :
    FDyTextureInformation(metaInfo)
{
  if (metaInfo.mSourceType == EDyResourceSource::Builtin)
  { // If builtin, just copy buffer to information.
    this->mTextureImageBuffer     = *metaInfo.mPtrBuiltinBuffer;
    this->mTextureSize            = metaInfo.mBuiltinBufferSize;
  }
  else
  { // But external, read buffer from file, get width, height and image format and convert raw buffer to buffer type.
    // Width and Height would be saved into mTextureInformation.
    const auto dataBuffer = std::make_unique<DDyImageBinaryDataBuffer>(std::get<std::string>(metaInfo.mExternalFilePath));
    MDY_ASSERT(dataBuffer->IsBufferCreatedProperly() == true, "Texture buffer can not be created properly.");

    TI32 pixelSize = 0;
    this->mImageActualPixelFormat = dataBuffer->GetImageFormat();
    switch (this->mImageActualPixelFormat)
    {
    case EDyImageColorFormatStyle::R:    pixelSize = 1; break;
    case EDyImageColorFormatStyle::RG:   pixelSize = 2; break;
    case EDyImageColorFormatStyle::RGB:  pixelSize = 3; break;
    case EDyImageColorFormatStyle::RGBA: pixelSize = 4; break;
    default: MDY_UNEXPECTED_BRANCH();
    }

    this->mTextureSize.X = dataBuffer->GetImageWidth();
    this->mTextureSize.Y = dataBuffer->GetImageHeight();

    const TU64 byteSize = this->mTextureSize.X * this->mTextureSize.Y * pixelSize;
    this->mTextureImageBuffer.resize(byteSize);

    const auto* ptrBuffer = dataBuffer->GetBufferStartPoint();
    memcpy(this->mTextureImageBuffer.data(), ptrBuffer, byteSize);
  }
}

const std::vector<TU08>& FDyTextureGeneralInformation::GetBuffer() const noexcept
{
  return this->mTextureImageBuffer;
}

const DDyVectorInt2& FDyTextureGeneralInformation::GetSize() const noexcept
{
  return this->mTextureSize;
}

} /// ::dy namespace
