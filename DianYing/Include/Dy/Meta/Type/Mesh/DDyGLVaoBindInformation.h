#ifndef GUARD_DY_META_TYPE_MESH_DDYGLVAOBINDINFORMATION_H
#define GUARD_DY_META_TYPE_MESH_DDYGLVAOBINDINFORMATION_H
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

#include <Dy/Helper/System/Macroes.h>
#include <Dy/Helper/GlobalType.h>

namespace dy
{

enum class EDyGLPixelFormatType : char
{
  Float,
  Int,
  NoneError,
};

///
/// @brief Get internal GLenum value from `EDyGLPixelFormatType`.
///
inline static MDY_NODISCARD GLenum DyGetGLTypeFrom(_MIN_ EDyGLPixelFormatType type) noexcept
{
  switch(type)
  {
  case EDyGLPixelFormatType::Float: return GL_FLOAT;
  case EDyGLPixelFormatType::Int:   return GL_INT;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

struct PDyVaoAttributeFormat final
{
  EDyGLPixelFormatType mType     = EDyGLPixelFormatType::NoneError;
  bool  mIsMustNormalized = false;
  TU32  mElementCount     = MDY_INITIALIZE_DEFUINT;
  TU32  mOffsetByteSize   = MDY_INITIALIZE_DEFUINT;

  PDyVaoAttributeFormat(_MIN_ EDyGLPixelFormatType type, _MIN_ bool isMustNormalized, _MIN_ TU32 elementCount, _MIN_ TU32 offsetByteSize) :
      mType{ type }, 
      mIsMustNormalized{ isMustNormalized }, 
      mElementCount{ elementCount }, 
      mOffsetByteSize{ offsetByteSize }
  {
    MDY_ASSERT_MSG(mType != EDyGLPixelFormatType::NoneError, "Attribute format type must not be ::NullError.");
    MDY_ASSERT_MSG(mElementCount != 0, "Attribute element count must be positive.");
  }
};

///
/// @struct DDyGLVaoBindInformation
/// @brief use this.
///
struct DDyGLVaoBindInformation final
{
  TU32 mOffsetByteSize  = MDY_INITIALIZE_DEFUINT;
  TU32 mStrideByteSize  = MDY_INITIALIZE_DEFUINT;
  bool mIsUsingDefaultDyAttributeModel = true;

  /// If `mIsUsingDefaultDyAttributeModel` is true, `mAttributeFormatList` will be neglected.
  std::vector<PDyVaoAttributeFormat> mAttributeFormatList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_MESH_DDYGLVAOBINDINFORMATION_H