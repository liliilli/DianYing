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

PBlendingEquation::PBlendingEquation(
  EFunc iSrc, EMode iMode, EFunc iDst, 
  const DColorRGBA& iConstantColor) 
  : mBlendMode{iMode},
    mSrcFunc{iSrc},
    mDstFunc{iDst},
    mConstantColor{iConstantColor}
{ }

GLenum PBlendingEquation::ToGLenum(EMode iMode)
{
  switch (iMode)
  {
  case EMode::SrcAddDst:  return GL_FUNC_ADD;
  case EMode::SrcSubDst:  return GL_FUNC_SUBTRACT;
  case EMode::DstSubSrc:  return GL_FUNC_REVERSE_SUBTRACT;
  case EMode::CompareMin: return GL_MIN;
  case EMode::CompareMax: return GL_MAX;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

PBlendingEquation::EMode PBlendingEquation::ToMode(GLenum iGlMode)
{
  switch (iGlMode)
  {
  case GL_FUNC_ADD:       return EMode::SrcAddDst;
  case GL_FUNC_SUBTRACT:  return EMode::SrcSubDst;
  case GL_MIN:            return EMode::CompareMin;
  case GL_MAX:            return EMode::CompareMax;
  case GL_FUNC_REVERSE_SUBTRACT: return EMode::DstSubSrc;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(EMode::__Error);
  }
}

GLint PBlendingEquation::ToGLenum(EFunc iFunc)
{
  switch (iFunc)
  {
  case EFunc::Zero:             return GL_ZERO;
  case EFunc::One:              return GL_ONE;
  case EFunc::SrcColor:         return GL_SRC_COLOR;
  case EFunc::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
  case EFunc::DstColor:         return GL_DST_COLOR;
  case EFunc::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
  case EFunc::SrcAlpha:         return GL_SRC_ALPHA;
  case EFunc::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
  case EFunc::DstAlpha:         return GL_DST_ALPHA;
  case EFunc::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
  case EFunc::ConstColor:       return GL_CONSTANT_COLOR;
  case EFunc::OneMinusConstColor: return GL_ONE_MINUS_CONSTANT_COLOR;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

PBlendingEquation::EFunc PBlendingEquation::ToFunc(GLenum iGlFunc)
{
  switch (iGlFunc)
  {
  case GL_ZERO:       return EFunc::Zero;
  case GL_ONE:        return EFunc::One;
  case GL_SRC_COLOR:  return EFunc::SrcColor;
  case GL_ONE_MINUS_SRC_COLOR:  return EFunc::OneMinusSrcColor;
  case GL_DST_COLOR:            return EFunc::DstColor;
  case GL_ONE_MINUS_DST_COLOR:  return EFunc::OneMinusDstColor;
  case GL_SRC_ALPHA:            return EFunc::SrcAlpha;
  case GL_ONE_MINUS_SRC_ALPHA:  return EFunc::OneMinusSrcAlpha;
  case GL_DST_ALPHA:            return EFunc::DstAlpha;
  case GL_ONE_MINUS_DST_ALPHA:  return EFunc::OneMinusDstAlpha;
  case GL_CONSTANT_COLOR:       return EFunc::ConstColor;
  case GL_ONE_MINUS_CONSTANT_COLOR: return EFunc::OneMinusConstColor;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(EFunc::__Error);
  }
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