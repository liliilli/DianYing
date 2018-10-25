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
#include <Dy/Management/Type/AttachmentInformation.h>
#include <array>

//!
//! Local translation unit functions
//!

namespace
{

template <TI32 TCount>
using TTargetList = std::array<dy::EDyGlParameterValue, TCount>;

///
/// @brief
/// @param value
/// @param targetList
/// @return
///
template <TI32 TValue>
MDY_NODISCARD bool DyIsHaveValueIn(
    _MIN_ const dy::EDyGlParameterValue value,
    _MIN_ const std::array<dy::EDyGlParameterValue, TValue>& targetList)
{
  for (const auto& target : targetList)
  {
    if (value == target) { return true; }
  }
  return false;
}

} /// ::unnamed namespace

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

EDySuccess DyCheckTextureParameter(_MIN_ const PDyGlTexParameterInformation& parameter)
{
  static constexpr TTargetList<6> sTextureMinFilter = {
    EDyGlParameterValue::Linear, EDyGlParameterValue::Nearest,
    EDyGlParameterValue::LinearMipmapLinear, EDyGlParameterValue::LinearMipmapNearest,
    EDyGlParameterValue::LinearMipmapNearest, EDyGlParameterValue::NearestMipmapNearest
  };

  static constexpr TTargetList<2> sTextureMagFilter = {
    EDyGlParameterValue::Linear, EDyGlParameterValue::Nearest,
  };

  static constexpr TTargetList<4> sTextureWrappingSTR = {
    EDyGlParameterValue::ClampToBorder, EDyGlParameterValue::ClampToEdge,
    EDyGlParameterValue::Repeat, EDyGlParameterValue::MirroredRepeat
  };

  switch (parameter.mParameterOption)
  {
  case EDyGlParameterName::TextureMinFilter:
    return DyIsHaveValueIn(parameter.mParameterValue, sTextureMinFilter) ? DY_SUCCESS : DY_FAILURE;
  case EDyGlParameterName::TextureMagFilter:
    return DyIsHaveValueIn(parameter.mParameterValue, sTextureMagFilter) ? DY_SUCCESS : DY_FAILURE;
  case EDyGlParameterName::TextureWrappingS:
  case EDyGlParameterName::TextureWrappingT:
    return DyIsHaveValueIn(parameter.mParameterValue, sTextureWrappingSTR) ? DY_SUCCESS : DY_FAILURE;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
  return DY_FAILURE;
}

EDySuccess DyCheckTextureParameterList(_MIN_ const std::vector<PDyGlTexParameterInformation>& parameterList)
{
  for (const auto& parameterItem : parameterList)
  {
    if (DyCheckTextureParameter(parameterItem) == DY_FAILURE) { return DY_FAILURE; }
  }
  return DY_SUCCESS;
}

} /// ::dy namespace