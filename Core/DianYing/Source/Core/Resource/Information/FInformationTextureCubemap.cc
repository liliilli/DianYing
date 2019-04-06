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
#include <Dy/Core/Resource/Information/FInformationTextureCubemap.h>

namespace dy
{

FInformationTextureCubemap::FInformationTextureCubemap(_MIN_ const PDyTextureInstanceMetaInfo& metaInfo) :
  FInformationTexture(metaInfo)
{
  MDY_ASSERT_MSG_FORCE(metaInfo.mSourceType != EDyResourceSource::Builtin, "Cubemap can not support builtin type textures.");

  // But external, read buffer from file, get width, height and image format and convert raw buffer to buffer type.
  // Width and Height would be saved into mTextureInformation.
  const auto& refExternalFilePath = std::get<PDyTextureInstanceMetaInfo::DCubemapFilePath>(metaInfo.mExternalFilePath);

  for (TU32 cubemapId = 0; cubemapId < 6; ++cubemapId)
  {
    std::unique_ptr<DImageBinaryBuffer> ptrBufferData = nullptr;
    DIVec2*     ptrSize            = nullptr;
    std::vector<TU8>* ptrInstanceBuffer  = nullptr;

    const auto type = static_cast<EDyCubemapFragment>(cubemapId);
    switch (type)
    {
    case EDyCubemapFragment::Top: 
    { ptrBufferData = std::make_unique<DImageBinaryBuffer>(refExternalFilePath.mTopPath);
      this->mImageActualPixelFormat = ptrBufferData->GetImageFormat();
      ptrSize = &this->mTopTextureSize;
      ptrInstanceBuffer = &this->mTopTextureImageBuffer;
    } break;
    case EDyCubemapFragment::Bottom: 
    { ptrBufferData = std::make_unique<DImageBinaryBuffer>(refExternalFilePath.mBottomPath);
      ptrSize = &this->mBottomTextureSize;
      ptrInstanceBuffer = &this->mBottomTextureImageBuffer;
    } break;
    case EDyCubemapFragment::Left:
    { ptrBufferData = std::make_unique<DImageBinaryBuffer>(refExternalFilePath.mLeftPath);
      ptrSize = &this->mLeftTextureSize;
      ptrInstanceBuffer = &this->mLeftTextureImageBuffer;
    } break;
    case EDyCubemapFragment::Right:
    { ptrBufferData = std::make_unique<DImageBinaryBuffer>(refExternalFilePath.mRightPath);
      ptrSize = &this->mRightTextureSize;
      ptrInstanceBuffer = &this->mRightTextureImageBuffer;
    } break;
    case EDyCubemapFragment::Front:
    { ptrBufferData = std::make_unique<DImageBinaryBuffer>(refExternalFilePath.mFrontPath);
      ptrSize = &this->mFrontTextureSize;
      ptrInstanceBuffer = &this->mFrontTextureImageBuffer;
    } break;
    case EDyCubemapFragment::Back:
    { ptrBufferData = std::make_unique<DImageBinaryBuffer>(refExternalFilePath.mBackPath);
      ptrSize = &this->mBackTextureSize;
      ptrInstanceBuffer = &this->mBackTextureImageBuffer;
    } break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }

    MDY_ASSERT_MSG_FORCE(ptrBufferData->IsBufferCreatedProperly() == true, "Texture buffer can not be created properly.");
    ptrSize->X = ptrBufferData->GetImageWidth(); ptrSize->Y = ptrBufferData->GetImageHeight();

    TI32 pixelSize = 0;
    switch (this->mImageActualPixelFormat)
    {
    case EDyImageColorFormatStyle::R:    pixelSize = 1; break;
    case EDyImageColorFormatStyle::RG:   pixelSize = 2; break;
    case EDyImageColorFormatStyle::RGB:  pixelSize = 3; break;
    case EDyImageColorFormatStyle::RGBA: pixelSize = 4; break;
    default: MDY_UNEXPECTED_BRANCH();
    }

    const TU64 byteSize = ptrSize->X * ptrSize->Y * pixelSize;
    ptrInstanceBuffer->resize(byteSize);
    memcpy(ptrInstanceBuffer->data(), ptrBufferData->GetBufferStartPoint(), byteSize);
  }
}

const std::vector<TU8>& FInformationTextureCubemap::GetBufferOf(_MIN_ EDyCubemapFragment iValue) const noexcept
{
  switch (iValue)
  {
  case EDyCubemapFragment::Top: return this->mTopTextureImageBuffer;
  case EDyCubemapFragment::Bottom: return this->mBottomTextureImageBuffer;
  case EDyCubemapFragment::Left: return this->mLeftTextureImageBuffer;
  case EDyCubemapFragment::Right: return this->mRightTextureImageBuffer;
  case EDyCubemapFragment::Front: return this->mFrontTextureImageBuffer;
  case EDyCubemapFragment::Back: return this->mBackTextureImageBuffer;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(this->mBackTextureImageBuffer);
  }
}

const DIVec2& FInformationTextureCubemap::GetSizeOf(_MIN_ EDyCubemapFragment iValue) const noexcept
{
  switch (iValue)
  {
  case EDyCubemapFragment::Top: return this->mTopTextureSize; 
  case EDyCubemapFragment::Bottom: return this->mBottomTextureSize; 
  case EDyCubemapFragment::Left: return this->mLeftTextureSize; 
  case EDyCubemapFragment::Right: return this->mRightTextureSize; 
  case EDyCubemapFragment::Front: return this->mFrontTextureSize; 
  case EDyCubemapFragment::Back: return this->mBackTextureSize; 
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(this->mBackTextureSize);
  }
}

} /// ::dy namesapce