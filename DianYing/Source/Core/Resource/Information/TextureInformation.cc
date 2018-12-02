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
/// @TODO IMPLEMEMNT PARAMETER (MIN_ MAG_ MIPMAP_) CUSTOM OPTION MECHANISM.
///

/// Header file
#include <Dy/Core/Resource/Information/TextureInformation.h>
#include <Dy/Core/Resource/Resource/TextureResource.h>
#include <Dy/Management/LoggingManager.h>

namespace
{

MDY_SET_IMMUTABLE_STRING(kTextureInformationTemplate,   "{} | Texture information {} : {}");
MDY_SET_IMMUTABLE_STRING(kTextureInformationBindTo,     "{} | Texture information binds to {}");
MDY_SET_IMMUTABLE_STRING(kTextureInformation,           "DDyTextureInformation");

#if defined(NDEBUG) == false
void DyDebugPrintTextureInformation(_MIN_ const dy::PDyTextureInstanceMetaInfo& textMetaInfo)
{
  // Set properties and output log for properties.
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "name", textMetaInfo.mSpecifierName);
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "local path", textMetaInfo.mExternalFilePath);

  switch (textMetaInfo.mTextureType)
  {
  case dy::EDyTextureStyleType::D1: MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "texture type", "Texture1D"); break;
  case dy::EDyTextureStyleType::D2: MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "texture type", "Texture2D"); break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "mipmap creation flag", textMetaInfo.mIsUsingDefaultMipmapGeneration ? "ON" : "OFF");
  MDY_LOG_INFO_D(kTextureInformationTemplate, kTextureInformation, "custom parameter options", textMetaInfo.mIsEnabledCustomedTextureParameter ? "ON" : "OFF");
}
#endif

} /// ::unnamed namespace

namespace dy
{

DDyTextureInformation::DDyTextureInformation(_MIN_ const PDyTextureInstanceMetaInfo& textureConstructionDescriptor) :
    mTextureInformation{textureConstructionDescriptor}
{
#if defined(NDEBUG) == false
  // Print texture debug information.
  DyDebugPrintTextureInformation(textureConstructionDescriptor);
#endif

  if (this->mTextureInformation.mSourceType == EDyResourceSource::Builtin)
  { // If builtin, just copy buffer to information.
    this->mTextureImageBuffer = *this->mTextureInformation.mPtrBuiltinBuffer;
    this->mImageActualPixelFormat = this->mTextureInformation.mTextureColorType;
  }
  else
  { // But external, read buffer from file, get width, height and image format and convert raw buffer to buffer type.
    // Width and Height would be saved into mTextureInformation.
    const auto dataBuffer = std::make_unique<DDyImageBinaryDataBuffer>(this->mTextureInformation.mExternalFilePath);
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

    this->mTextureInformation.mBuiltinBufferSize.X = dataBuffer->GetImageWidth();
    this->mTextureInformation.mBuiltinBufferSize.Y = dataBuffer->GetImageHeight();
    const auto* ptrBuffer = dataBuffer->GetBufferStartPoint();
    const TU64  byteSize  =
        static_cast<TU64>(this->mTextureInformation.mBuiltinBufferSize.X)
      * this->mTextureInformation.mBuiltinBufferSize.Y
      * pixelSize;
    this->mTextureImageBuffer.reserve(byteSize);
    for (TU64 byteCount = 0; byteCount < byteSize; ++byteCount)
    {
      this->mTextureImageBuffer.emplace_back(*ptrBuffer);
      ptrBuffer++;
    }
  }
}

DDyTextureInformation::~DDyTextureInformation()
{
  MDY_LOG_INFO_D(kTextureInformationTemplate, "~CDyShaderInformation", "name", this->mTextureInformation.mSpecifierName);
  if (this->__mLinkedTextureResourcePtr) { this->__mLinkedTextureResourcePtr->__pfResetTextureInformationLink(); }
}

void DDyTextureInformation::__pfLinkTextureResource(CDyTextureResource* ptr) const noexcept
{
  MDY_LOG_DEBUG_D(kTextureInformationBindTo, "__pfLinkTextureResource", reinterpret_cast<std::ptrdiff_t>(ptr));
  this->__mLinkedTextureResourcePtr = ptr;
}
} /// ::dy namespace