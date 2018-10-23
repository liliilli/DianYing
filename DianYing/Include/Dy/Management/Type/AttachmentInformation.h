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

#include <Dy/Helper/Type/Color.h>
#include <Dy/Helper/Type/VectorInt2.h>

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
  NoneError = 0xFF
};

///
/// @brief  Get attachment type value from EDyGlAttachmentType.
/// @param  attachment
/// @return GL_COLOR_ATTACHMENT_X
///
MDY_NODISCARD GLenum DyGetAttachmentTypeValue(_MIN_ const EDyGlAttachmentType attachment) noexcept;

///
/// @enum   EDyGlParameterName
/// @brief  Texture parameter option name.
///
enum class EDyGlParameterName
{
  TextureMinFilter,
  TextureMagFilter,
  TextureWrappingS,
  TextureWrappingT,

  NoneError
};

///
/// @brief  Get internal plain parameter value from EDyGlParameterName.
/// @param  name EDyGlParameterName
/// @return OpenGL C-plain parameter value.
///
MDY_NODISCARD GLenum DyGetParameterNameValue(_MIN_ const EDyGlParameterName name) noexcept;

///
/// @enum   EDyGlParameterValue
/// @brief  Texture parameter value name.
///
enum class EDyGlParameterValue
{
  Nearest,
  Linear,
  NearestMipmapNearest,
  LinearMipmapNearest,
  NearestMipmapLinear,
  LinearMipmapLinear,

  ClampToEdge,
  ClampToBorder,
  Repeat,
  MirroredRepeat,

  NoneError
};

///
/// @brief  Get internal plain parameter value from EDyGlParameterValue.
/// @param  value EDyGlParameterValue
/// @return OpenGL C-plain parameter value.
///
MDY_NODISCARD GLenum DyGetParameterValueValue(_MIN_ const EDyGlParameterValue value) noexcept;

///
/// @struct PDyGlTexParameterInformation
/// @brief
///
struct PDyGlTexParameterInformation final
{
  EDyGlParameterName  mParameterOption = EDyGlParameterName::NoneError;
  EDyGlParameterValue mParameterValue  = EDyGlParameterValue::NoneError;

  PDyGlTexParameterInformation(_MIN_ EDyGlParameterName name, _MIN_ EDyGlParameterValue value) :
      mParameterOption(name),
      mParameterValue(value)
  { }
};

///
/// @brief
/// @param  parameter
/// @return
///
MDY_NODISCARD EDySuccess DyCheckTextureParameter(_MIN_ const PDyGlTexParameterInformation& parameter);

///
/// @brief
/// @param  parameterList
/// @return
///
MDY_NODISCARD EDySuccess DyCheckTextureParameterList(_MIN_ const std::vector<PDyGlTexParameterInformation>& parameterList);

///
/// @struct PDyGlAttachmentBinderInformation
/// @brief
///
struct PDyGlAttachmentBinderInformation final
{
  std::string         mAttachmentName = MDY_INITILAIZE_EMPTYSTR;
  EDyGlAttachmentType mAttachmentType = EDyGlAttachmentType::NoneError;
};

///
/// @struct PDyGlAttachmentInformation
/// @brief
///
struct PDyGlAttachmentInformation final
{
  std::string                               mAttachmentName = MDY_INITILAIZE_EMPTYSTR;
  std::vector<PDyGlTexParameterInformation> mParameterList  = {};
  DDyVectorInt2                             mAttachmentSize = {};
  DDyColor                                  mBorderColor    = DDyColor::Black;

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