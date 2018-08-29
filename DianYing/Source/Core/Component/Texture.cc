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
#include <Dy/Core/Component/Texture.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_FAILURE_STRINGS
#define STBI_FAILURE_USERMSG
#if defined(_WIN32)
#pragma warning(push)
#pragma warning(disable:4100 4505)
#endif
#include <stb_image.h>
#if defined(_WIN32)
#pragma warning(pop)
#endif

namespace
{

enum class EDyImageColorFormatStyle
{
  NoneError,
  R,
  RG,
  RGB,
  RGBA
};

///
/// @brief Return color format
/// @param[in] channelsValue Color channels value for being used to get GL_COLOR channels.
///
EDyImageColorFormatStyle GetColorFormat(const int32_t channelsValue) noexcept {
  switch (channelsValue) {
  /// Red (one channel)
  case 1:   return EDyImageColorFormatStyle::R;
  /// Red and Green only.
  case 2:   return EDyImageColorFormatStyle::RG;
  /// RGB without alpha
  case 3:   return EDyImageColorFormatStyle::RGB;
  /// RGB with alpha
  case 4:   return EDyImageColorFormatStyle::RGBA;
  /// else, return Error type
  default:  return EDyImageColorFormatStyle::NoneError;
  }
}

///
/// @brief Set texture default parameter setting.
/// GL_TEXTURE_MIN_FILTER to GL_NEAREST
/// GL_TEXTURE_MAG_FILTER to GL_NEAREST
/// GL_TEXTURE_WRAP_S to GL_REPEAT
/// GL_TEXTURE_WRAP_T to GL_REPEAT
///
void DyGlSetDefaultOptionSetting(uint32_t textureId) {
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

} /// ::unnamed namespace

namespace dy
{

CDyTextureComponent::~CDyTextureComponent()
{
  if (mTextureResourceId)
  {
    glDeleteTextures(1, &mTextureResourceId);
  }
}

EDySuccess CDyTextureComponent::pInitializeTextureResource(
    const PDyTextureConstructionDescriptor& textureConstructionDescriptor)
{
  stbi_set_flip_vertically_on_load(true);

  int32_t imageChannels = 0;
  const auto binaryData = stbi_load(
      textureConstructionDescriptor.mTexturePath.c_str(),
      &mTextureWidth, &mTextureHeight,
      &imageChannels, 0);
  const EDyImageColorFormatStyle format = GetColorFormat(imageChannels);
  if (format == EDyImageColorFormatStyle::NoneError)
  {
    stbi_image_free(binaryData);
#ifdef false
    stbi_failure_reason();
#endif
    return DY_FAILURE;
  }
  if (!binaryData)
  {
    return DY_FAILURE;
  }

  int32_t glImageFormat;
  switch (format)
  {
  case EDyImageColorFormatStyle::R:     glImageFormat = GL_R;     break;
  case EDyImageColorFormatStyle::RG:    glImageFormat = GL_RG;    break;
  case EDyImageColorFormatStyle::RGB:   glImageFormat = GL_RGB;   break;
  case EDyImageColorFormatStyle::RGBA:  glImageFormat = GL_RGBA;  break;
  default: // Unexpected branch (Error)
    glImageFormat = 0;
    return DY_FAILURE;
  }

  glGenTextures(1, &mTextureResourceId);
  glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
  glTexImage2D(GL_TEXTURE_2D,
               0, GL_RGBA,
               mTextureWidth, mTextureHeight, 0,
               glImageFormat, GL_UNSIGNED_BYTE,
               binaryData);
  stbi_image_free(binaryData);

  if (!textureConstructionDescriptor.mIsTextureParameterCustomed)
  {
    DyGlSetDefaultOptionSetting(mTextureResourceId);
  }
  else
  {
#ifdef false
    const auto& textureOptions = textureConstructionDescriptor.mTextureParameterOptions;
    for (const auto& option : textureOptions)
    {
      const auto glOption = DyGlSpecifyOption(option.GetOption());
      if (glOption == GL_NO_ERROR)
      {
        // Output error log to sink...
        return DY_FAILURE;
      }

      const auto glOptionValue = DyGlSpecifyOption(option.GetValue());
      if (glOptionValue == GL_NO_ERROR)
      {
        // Output error log to sink...
        return DY_FAILURE;
      }
    }
#endif
  }

  return DY_SUCCESS;
}

} /// ::dy namespace
