#ifndef GUARD_DY_MANAGEMENT_TYPE_ATTACHMENTINFORMATION_H
#define GUARD_DY_MANAGEMENT_TYPE_ATTACHMENTINFORMATION_H
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

#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>

namespace dy
{

/// @enum   EDyGlAttachmentType
/// @brief  Attachment type for binding textures to framebuffer.
enum class EDyGlAttachmentType : TI32
{
  Color0 = 0, Color1, Color2, Color3,
  Color4, Color5, Color6, Color7,
  Color8, Color9, Color10, Color11,
  Color12, Color13, Color14, Color15,
  Depth,
  NoneError = 0xFF
};

/// @brief  Get attachment type value from EDyGlAttachmentType.
/// @param  iAttType
/// @return GL_COLOR_ATTACHMENT_X
MDY_NODISCARD GLenum DyGetAttachmentTypeValue(EDyGlAttachmentType iAttType) noexcept;

/// @struct PBlendingEquation
/// @brief Describes blending equation [src, mode, dst] 
/// when setting blending option to attachment.
struct PBlendingEquation final
{
  enum class EMode
  {
    SrcAddDst,  // Operator will be Src + Dst.
    SrcSubDst,  // Operator will be Src - Dst.
    DstSubSrc,  // Operator will be Dst - Src.
    CompareMin, // Operator will be min(Src, Dst) to each element.
    CompareMax, // Operator will be max(Src, Dst) to each element.

    __Error
  };

  enum class EFunc
  {
    Zero,               // Factor will be (0, 0, 0, 0)
    One,                // Factor will be (1, 1, 1, 1)
    SrcColor,           // Factor will be (Sr, Sg, Sb, Sa)
    OneMinusSrcColor,   // Factor will be 1 - (Sr, Sg, Sb, Sa)
    DstColor,           // Factor will be (Dr, Dg, Db, Da)
    OneMinusDstColor,   // Factor will be 1 - (Dr, Dg, Db, Da)
    SrcAlpha,           // Factor will be vec4(Sa)
    OneMinusSrcAlpha,   // Factor will be 1 - vec4(Sa)
    DstAlpha,           // Factor will be vec4(Da)
    OneMinusDstAlpha,   // Factor will be 1 - vec4(Da)
    ConstColor,         // Factor will be mConstColor.
    OneMinusConstColor, // Factor will be 1 - mConstColor.

    __Error
  };

  /// @brief Result color will be R = S*(mSrcFunc) mBlendMode D*(mDstFunc).
  /// D is destination, commonly canvas color of attachment.
  /// S is source, commonly blending color from shader.
  EMode mBlendMode = EMode::SrcAddDst;
  EFunc mSrcFunc   = EFunc::SrcAlpha;
  EFunc mDstFunc   = EFunc::OneMinusSrcAlpha;
  /// @brief This variable will be used 
  /// when using EFunc::ConstColor, EFunc::OneMinusConstColor
  DDyColorRGBA mConstantColor = DDyColorRGBA{};

  PBlendingEquation() = default;
  PBlendingEquation(
    EFunc iSrc, EMode iMode, EFunc iDst, 
    const DDyColorRGBA& iConstantColor = DDyColorRGBA{});

  MDY_NODISCARD static GLenum ToGLenum(EMode iMode);
  MDY_NODISCARD static EMode  ToMode(GLenum iGlMode);

  MDY_NODISCARD static GLint ToGLenum(EFunc iFunc);
  MDY_NODISCARD static EFunc ToFunc(GLenum iGlFunc);
};
using TBlendingEquationList = std::vector<PBlendingEquation>;

/// @struct PDyGlAttachmentBinderInformation
/// @brief Helper binder type for creating FBO with arbitary attachments.
struct PDyGlAttachmentBinderInformation final
{
  std::string         mAttachmentName = MDY_INITIALIZE_EMPTYSTR;
  EDyGlAttachmentType mAttachmentType = EDyGlAttachmentType::NoneError;

  PDyGlAttachmentBinderInformation() = default;
  PDyGlAttachmentBinderInformation(const std::string& iSpecifier, EDyGlAttachmentType iType) 
    : mAttachmentName{iSpecifier},
      mAttachmentType{iType}
  {};
};
using TAttachmentBinderList = std::vector<PDyGlAttachmentBinderInformation>;

/// @struct PDyGlAttachmentInformation
/// @brief
struct PDyGlAttachmentInformation final
{
  std::string                               mAttachmentName = MDY_INITIALIZE_EMPTYSTR;
  std::vector<PDyGlTexParameterInformation> mParameterList  = {};
  DDyVectorInt2                             mAttachmentSize = {};
  DDyColorRGBA                              mBorderColor    = DDyColorRGBA::Black;

  ///
  /// @brief  Get attachment id. If attachment is not initialized yet, just return 0.
  /// @return Attachment texture id.
  ///
  FORCEINLINE MDY_NODISCARD TU32 GetAttachmentId() const noexcept
  {
    return this->mAttachmentId;
  }

private:
  ///
  TU32 mAttachmentId = MDY_INITIALIZE_DEFUINT;

  friend class MDyFramebuffer;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_ATTACHMENTINFORMATION_H