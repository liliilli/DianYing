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
#include <Dy/Core/Resource/Internal/TextureEnums.h>

namespace dy
{

GLenum DyGLGetLowTextureType(_MIN_ EDyTextureStyleType iType) noexcept
{
  switch (iType)
  {
  case EDyTextureStyleType::D1: return GL_TEXTURE_1D;
  case EDyTextureStyleType::D2: return GL_TEXTURE_2D; 
  case EDyTextureStyleType::D2Rectangle: return GL_TEXTURE_RECTANGLE;
  case EDyTextureStyleType::D2Shadow: return GL_TEXTURE_2D;
  case EDyTextureStyleType::D1Array: return GL_TEXTURE_1D_ARRAY;
  case EDyTextureStyleType::D2Array: return GL_TEXTURE_2D_ARRAY;
  case EDyTextureStyleType::D2ShadowArray: return GL_TEXTURE_2D_ARRAY;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

GLenum DyGlGetLowDataFormatType(_MIN_ EDyGlBufferDataInternalFormat iValue) noexcept
{
  switch (iValue)
  {
  case EDyGlBufferDataInternalFormat::RED8:   return GL_R8;
  case EDyGlBufferDataInternalFormat::RG8:    return GL_RG8;
  case EDyGlBufferDataInternalFormat::RGB8:   return GL_RGB8;
  case EDyGlBufferDataInternalFormat::RGBA8:  return GL_RGBA8;
  case EDyGlBufferDataInternalFormat::RGBA16Float: return GL_RGBA16F;
  case EDyGlBufferDataInternalFormat::RGB32Float: return GL_RGB32F;
  case EDyGlBufferDataInternalFormat::RGBA32Float:return GL_RGBA32F;
  case EDyGlBufferDataInternalFormat::DEPTH32:return GL_DEPTH_COMPONENT32;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

} /// ::dy namespace