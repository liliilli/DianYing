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
#include <Dy/Helper/Internal/ImageBinaryBuffer.h>

namespace
{

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

EDySuccess CDyTextureComponent::pInitializeTextureResource(const PDyTextureConstructionDescriptor& textureConstructionDescriptor)
{
  // Make image binary data buffer.
  DDyImageBinaryDataBuffer dataBuffer(textureConstructionDescriptor.mTexturePath);
  if (!dataBuffer.IsBufferCreatedProperly())
  {
    return DY_FAILURE;
  }

  // Forward dataBuffer's retrieved information to data members.
  mTextureWidth   = dataBuffer.GetImageWidth();
  mTextureHeight  = dataBuffer.GetImageHeight();
  int32_t glImageFormat = GL_NO_ERROR;
  switch (dataBuffer.GetImageFormat())
  {
  case EDyImageColorFormatStyle::R:     glImageFormat = GL_R;     break;
  case EDyImageColorFormatStyle::RG:    glImageFormat = GL_RG;    break;
  case EDyImageColorFormatStyle::RGB:   glImageFormat = GL_RGB;   break;
  case EDyImageColorFormatStyle::RGBA:  glImageFormat = GL_RGBA;  break;
  default: // Unexpected branch (Error)
    glImageFormat = GL_NO_ERROR;
    return DY_FAILURE;
  }

  glGenTextures(1, &mTextureResourceId);
  glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
               mTextureWidth, mTextureHeight, 0,
               glImageFormat, GL_UNSIGNED_BYTE,
               dataBuffer.GetBufferStartPoint());

  if (!textureConstructionDescriptor.mIsEnabledCustomedTextureParameter)
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
