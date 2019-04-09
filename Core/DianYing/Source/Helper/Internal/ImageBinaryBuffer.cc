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
#include <Dy/Helper/Internal/DImageBinaryBuffer.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#define STBI_FAILURE_USERMSG

#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4100 4505)
#endif

#include <stb_image.h>
#include <nlohmann/json.hpp>
#include <Dy/Include/GlInclude.h>
#include <Dy/Management/MLog.h>
#include <Dy/Helper/System/Assertion.h>

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

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyImageColorFormatStyle& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyImageColorFormatStyle& p)
{
  const auto& deserializedStringValue = j.get_ref<const std::string&>();
  if (deserializedStringValue == "DEFAULT") 
  { 
    p = EDyImageColorFormatStyle::RGBA; 
    return;
  }

  if (deserializedStringValue == "R")         { p = EDyImageColorFormatStyle::R; }
  else if (deserializedStringValue == "RG")   { p = EDyImageColorFormatStyle::RG; }
  else if (deserializedStringValue == "RGB")  { p = EDyImageColorFormatStyle::RGB; }
  else if (deserializedStringValue == "RGBA") { p = EDyImageColorFormatStyle::RGBA; }
  else { MDY_UNEXPECTED_BRANCH(); }
}

std::optional<TGlEnum> GlGetImageFormatFrom(EDyImageColorFormatStyle style) noexcept
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

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlImagePixelReadType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlImagePixelReadType& p)
{
  const auto& deserializedStringValue = j.get_ref<const std::string&>();
  if (deserializedStringValue == "DEFAULT") 
  { 
    p = EDyGlImagePixelReadType::UnsignedByte; 
    return;
  }

  if (deserializedStringValue == "UnsignedByte") { p = EDyGlImagePixelReadType::UnsignedByte; }
  else if (deserializedStringValue == "Float")   { p = EDyGlImagePixelReadType::Float; }
  else { MDY_UNEXPECTED_BRANCH(); }
}

TGlEnum GlGetImagePixelTypeFrom(EDyGlImagePixelReadType iType) noexcept
{
  switch (iType)
  {
  case EDyGlImagePixelReadType::UnsignedByte: return GL_UNSIGNED_BYTE;
  case EDyGlImagePixelReadType::Float:        return GL_FLOAT;
  default: return GL_NONE;
  }
}

DImageBinaryBuffer::DImageBinaryBuffer(const std::string& imagePath)
{
  stbi_set_flip_vertically_on_load(true);

  this->mBufferStartPoint = stbi_load(
    imagePath.c_str(), 
    &this->mWidth, &this->mHeight, &this->mImageChannel, 0);

  this->mImageFormat = GetColorFormat(this->mImageChannel);
  if (this->mImageFormat == EDyImageColorFormatStyle::NoneError)
  {
    stbi_image_free(this->mBufferStartPoint);
    DyPushLogError("{}", stbi_failure_reason());
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

DImageBinaryBuffer::DImageBinaryBuffer(std::size_t size, void* ptrEntry)
{
  stbi_set_flip_vertically_on_load(true);

  this->mBufferStartPoint = stbi_load_from_memory(
    reinterpret_cast<stbi_uc*>(ptrEntry), size, 
    &this->mWidth, &this->mHeight, &this->mImageChannel, 0);

  this->mImageFormat = GetColorFormat(this->mImageChannel);
  if (this->mImageFormat == EDyImageColorFormatStyle::NoneError)
  {
    stbi_image_free(this->mBufferStartPoint);
    DyPushLogError("{}", stbi_failure_reason());
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

DImageBinaryBuffer::~DImageBinaryBuffer()
{
  if (mIsBufferCreatedProperly)
  {
    stbi_image_free(mBufferStartPoint);
  }
}

} /// ::dy namespace