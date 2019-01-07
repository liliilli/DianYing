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

GLenum DyGetAttachmentTypeValue(_MIN_ const EDyGlAttachmentType attachment) noexcept
{ // Integrity test
  if (attachment == EDyGlAttachmentType::NoneError) { return GL_NONE; }
  if (attachment == EDyGlAttachmentType::Depth)     { return GL_DEPTH_ATTACHMENT; }
  return GL_COLOR_ATTACHMENT0 + static_cast<TI32>(attachment);
}

GLenum DyGetParameterNameValue(const EDyGlParameterName attachment) noexcept
{
  switch (attachment)
  {
  case EDyGlParameterName::TextureMinFilter: return GL_TEXTURE_MIN_FILTER;
  case EDyGlParameterName::TextureMagFilter: return GL_TEXTURE_MAG_FILTER;
  case EDyGlParameterName::TextureWrappingS: return GL_TEXTURE_WRAP_S;
  case EDyGlParameterName::TextureWrappingT: return GL_TEXTURE_WRAP_T;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
  return GL_NONE;
}

GLenum DyGetParameterValueValue(const EDyGlParameterValue value) noexcept
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
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
  return GL_NONE;
}

} /// ::dy namespace