#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLVAOBINDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLVAOBINDESCRIPTOR_H
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

namespace dy
{

///
/// @struct PDyGLVaoBindDescriptor
/// @brief Descriptor instance for binding attribute properties for valid Vertex Array Object id.
///
struct PDyGLVaoBindDescriptor final
{
  enum class EFormatType : TU08
  {
    Float,
    Int,
    NoneError,
  };

  struct PAttributeFormat final
  {
    const EFormatType mType       = EFormatType::NoneError;
    const bool  mIsMustNormalized = false;
    const TU32  mElementCount     = MDY_INITIALIZE_DEFUINT;
    const TU32  mOffsetByteSize   = MDY_INITIALIZE_DEFUINT;

    PAttributeFormat(_MIN_ EFormatType type, _MIN_ bool isMustNormalized, _MIN_ TU32 elementCount, _MIN_ TU32 offsetByteSize) :
      mType{type}, mIsMustNormalized{isMustNormalized}, mElementCount{elementCount}, mOffsetByteSize{offsetByteSize}
    {
      MDY_ASSERT(mType != EFormatType::NoneError, "Attribute format type must not be ::NullError.");
      MDY_ASSERT(mElementCount != 0, "Attribute element count must be positive.");
    }
  };

  TU32 mVaoId           = MDY_INITIALIZE_DEFUINT;
  TU32 mBoundVboId      = MDY_INITIALIZE_DEFUINT;
  TU32 mBoundEboId      = MDY_INITIALIZE_DEFUINT;
  TU32 mOffsetByteSize  = MDY_INITIALIZE_DEFUINT;
  TU32 mStrideByteSize  = MDY_INITIALIZE_DEFUINT;
  bool mIsUsingDefaultDyAttributeModel = false;

  /// If `mIsUsingDefaultDyAttributeModel` is true, `mAttributeFormatList` will be neglected.
  std::vector<PAttributeFormat> mAttributeFormatList = {};
};

/// @brief Get internal GLenum value from `EDyGLPixelFormatType`.
inline static MDY_NODISCARD GLenum DyGetGLTypeFrom(_MIN_ PDyGLVaoBindDescriptor::EFormatType type) noexcept
{
  switch(type)
  {
  case PDyGLVaoBindDescriptor::EFormatType::Float:  return GL_FLOAT;
  case PDyGLVaoBindDescriptor::EFormatType::Int:    return GL_INT;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLVAOBINDESCRIPTOR_H