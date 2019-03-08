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
#include <Dy/Management/Type/AttachmentInformation.h>
#include <array>

//!
//! Local translation unit functions
//!

//!
//! Implementation
//!

namespace dy
{

GLenum DyGetAttachmentTypeValue(EDyGlAttachmentType iAttType) noexcept
{ // Integrity test
  if (iAttType == EDyGlAttachmentType::NoneError) { return GL_NONE; }
  if (iAttType == EDyGlAttachmentType::Depth)     { return GL_DEPTH_ATTACHMENT; }
  return GL_COLOR_ATTACHMENT0 + static_cast<TI32>(iAttType);
}

GLenum DyGetTexParameterNameValue(_MIN_ const EDyGlParameterName attachment) noexcept
{
  switch (attachment)
  {
  case EDyGlParameterName::TextureMinFilter:    return GL_TEXTURE_MIN_FILTER;
  case EDyGlParameterName::TextureMagFilter:    return GL_TEXTURE_MAG_FILTER;
  case EDyGlParameterName::TextureWrappingS:    return GL_TEXTURE_WRAP_S;
  case EDyGlParameterName::TextureWrappingT:    return GL_TEXTURE_WRAP_T;
  case EDyGlParameterName::TextureCompareMode:  return GL_TEXTURE_COMPARE_MODE;
  case EDyGlParameterName::TextureCompareFunc:  return GL_TEXTURE_COMPARE_FUNC;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
  return GL_NONE;
}

GLenum DyGetTexParameterValueValue(_MIN_ const EDyGlParameterValue value) noexcept
{
  switch (value)
  {
  case EDyGlParameterValue::Nearest:              return GL_NEAREST;
  case EDyGlParameterValue::Linear:               return GL_LINEAR;
  case EDyGlParameterValue::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
  case EDyGlParameterValue::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
  case EDyGlParameterValue::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
  case EDyGlParameterValue::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
  case EDyGlParameterValue::ClampToEdge:          return GL_CLAMP_TO_EDGE;
  case EDyGlParameterValue::ClampToBorder:        return GL_CLAMP_TO_BORDER;
  case EDyGlParameterValue::Repeat:               return GL_REPEAT;
  case EDyGlParameterValue::MirroredRepeat:       return GL_MIRRORED_REPEAT;
  // TextureCompareMode
  case EDyGlParameterValue::CompareRefToTexture:  return GL_COMPARE_REF_TO_TEXTURE;
  case EDyGlParameterValue::CompareNone:          return GL_NONE;
  // TextureCompareFunc
  case EDyGlParameterValue::Greater:              return GL_GREATER;
  case EDyGlParameterValue::GreaterEqual:         return GL_GEQUAL;
  case EDyGlParameterValue::Equal:                return GL_EQUAL;
  case EDyGlParameterValue::NotEqual:             return GL_NOTEQUAL;
  case EDyGlParameterValue::LessEqual:            return GL_LEQUAL;
  case EDyGlParameterValue::Less:                 return GL_LESS;
  case EDyGlParameterValue::Always:               return GL_ALWAYS;
  case EDyGlParameterValue::Never:                return GL_NEVER;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
  return GL_NONE;
}

} /// ::dy namespace