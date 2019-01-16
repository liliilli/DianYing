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
#include <Dy/Helper/Internal/ImageBinaryBuffer.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#define STBI_FAILURE_USERMSG
#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4100 4505)
#endif
#include <stb_image.h>
#include "Dy/Management/LoggingManager.h"
#if defined(_WIN32)
#pragma warning(pop)
#endif

namespace
{

///
/// @brief Return color format
/// @param[in] channelsValue Color channels value for being used to get GL_COLOR channels.
///
dy::EDyImageColorFormatStyle GetColorFormat(const int32_t channelsValue) noexcept {
  switch (channelsValue) {
  /// Red (one channel)
  case 1:   return dy::EDyImageColorFormatStyle::R;
  /// Red and Green only.
  case 2:   return dy::EDyImageColorFormatStyle::RG;
  /// RGB without alpha
  case 3:   return dy::EDyImageColorFormatStyle::RGB;
  /// RGB with alpha
  case 4:   return dy::EDyImageColorFormatStyle::RGBA;
  /// else, return Error type
  default:  return dy::EDyImageColorFormatStyle::NoneError;
  }
}

} /// unnamed namespace

namespace dy
{

std::optional<GLenum> DyGLGetImageFormatFrom(_MIN_ EDyImageColorFormatStyle style) noexcept
{
  switch (style)
  {
  case EDyImageColorFormatStyle::R:         return GL_RED;
  case EDyImageColorFormatStyle::RG:        return GL_RG;
  case EDyImageColorFormatStyle::RGB:       return GL_RGB;
  case EDyImageColorFormatStyle::RGBA:      return GL_RGBA;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }
}

GLenum DyGlGetImagePixelTypeFrom(_MIN_ EDyGlImagePixelReadType iType) noexcept
{
  switch (iType)
  {
  case EDyGlImagePixelReadType::UnsignedByte: return GL_UNSIGNED_BYTE;
  case EDyGlImagePixelReadType::Float:        return GL_FLOAT;
  default: return GL_NONE;
  }
}

DDyImageBinaryDataBuffer::DDyImageBinaryDataBuffer(const std::string& imagePath)
{
  stbi_set_flip_vertically_on_load(true);

  this->mBufferStartPoint = stbi_load(imagePath.c_str(), &this->mWidth, &this->mHeight, &this->mImageChannel, 0);
  this->mImageFormat      = GetColorFormat(this->mImageChannel);
  if (this->mImageFormat == EDyImageColorFormatStyle::NoneError)
  {
    stbi_image_free(this->mBufferStartPoint);
    MDY_LOG_ERROR("{}", stbi_failure_reason());
    this->mIsBufferCreatedProperly = false;
  }
  else if (this->mBufferStartPoint == nullptr)
  {
    this->mIsBufferCreatedProperly = false;
  }
  else
  {
    this->mIsBufferCreatedProperly = true;
  }
}

DDyImageBinaryDataBuffer::~DDyImageBinaryDataBuffer()
{
  if (mIsBufferCreatedProperly)
  {
    stbi_image_free(mBufferStartPoint);
  }
}

} /// ::dy namespace