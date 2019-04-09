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
#include <Dy/Helper/System/Assertion.h>
#include <Dy/Helper/GlobalType.h>

namespace dy
{

enum class EGLPixelFormatType : char
{
  Float,
  Int,
  NoneError,
};

/// @brief Get internal GLenum value from `EGLPixelFormatType`.
TGlEnum DyGetGLTypeFrom(EGLPixelFormatType type);

struct PDyVaoAttributeFormat final
{
  EGLPixelFormatType mType = EGLPixelFormatType::NoneError;
  bool  mIsMustNormalized = false;
  TU32  mElementCount     = MDY_INITIALIZE_DEFUINT;
  TU32  mOffsetByteSize   = MDY_INITIALIZE_DEFUINT;

  PDyVaoAttributeFormat(
    EGLPixelFormatType type, 
    bool isMustNormalized, 
    TU32 elementCount, 
    TU32 offsetByteSize) 
    : mType{ type }, 
      mIsMustNormalized{ isMustNormalized }, 
      mElementCount{ elementCount }, 
      mOffsetByteSize{ offsetByteSize }
  {
    MDY_ASSERT_MSG(
      this->mType != EGLPixelFormatType::NoneError, 
      "Attribute format type must not be ::NullError.");
    MDY_ASSERT_MSG(
      this->mElementCount != 0, 
      "Attribute element count must be positive.");
  }
};

/// @struct DGlVaoBindInformation
/// @brief use this.
struct DGlVaoBindInformation final
{
  TU32 mOffsetByteSize  = 0;
  TU32 mStrideByteSize  = 0;
  bool mIsUsingDefaultDyAttributeModel = true;

  /// If `mIsUsingDefaultDyAttributeModel` is true, `mAttributeFormatList` will be neglected.
  std::vector<PDyVaoAttributeFormat> mAttributeFormatList = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_MESH_DDYGLVAOBINDINFORMATION_H