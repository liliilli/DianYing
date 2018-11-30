#ifndef GUARD_DY_MANAGEMENT_TYPE_ATTACHMENTINFORMATION_H
#define GUARD_DY_MANAGEMENT_TYPE_ATTACHMENTINFORMATION_H
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

#include <Dy/Helper/Type/ColorRGBA.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>

namespace dy
{

///
/// @enum   EDyGlAttachmentType
/// @brief  Attachment type for binding textures to framebuffer.
///
enum class EDyGlAttachmentType : TI32
{
  Color0 = 0, Color1, Color2, Color3,
  Color4, Color5, Color6, Color7,
  Color8, Color9, Color10, Color11,
  Color12, Color13, Color14, Color15,
  Depth,
  NoneError = 0xFF
};

///
/// @brief  Get attachment type value from EDyGlAttachmentType.
/// @param  attachment
/// @return GL_COLOR_ATTACHMENT_X
///
MDY_NODISCARD GLenum DyGetAttachmentTypeValue(_MIN_ const EDyGlAttachmentType attachment) noexcept;

///
/// @struct PDyGlAttachmentBinderInformation
/// @brief
///
struct PDyGlAttachmentBinderInformation final
{
  std::string         mAttachmentName = MDY_INITIALIZE_EMPTYSTR;
  EDyGlAttachmentType mAttachmentType = EDyGlAttachmentType::NoneError;
};

///
/// @struct PDyGlAttachmentInformation
/// @brief
///
struct PDyGlAttachmentInformation final
{
  std::string                               mAttachmentName = MDY_INITIALIZE_EMPTYSTR;
  std::vector<PDyGlTexParameterInformation> mParameterList  = {};
  DDyVectorInt2                             mAttachmentSize = {};
  DDyColorRGBA                                  mBorderColor    = DDyColorRGBA::Black;

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