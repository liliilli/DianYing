#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTINFORMATION_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTINFORMATION_H
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

#include <Dy/Core/Resource/Internal/TextureEnums.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <Dy/Meta/Type/EDyResourceTypes.h>
#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Helper/Type/ColorRGBA.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGlAttachmentInstanceMetaInfo;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyAttachmentInformation
/// @brief Attachment information resource. Attachment does not support RenderBuffer yet.
///
class FDyAttachmentInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyAttachmentInformation);

  /// @brief Construct attachment information.
  FDyAttachmentInformation(_MIN_ const PDyGlAttachmentInstanceMetaInfo& metaInfo);
  ~FDyAttachmentInformation() = default;

  /// @brief Get attachment's specifier name.
  FORCEINLINE MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

   /// @brief Get attachment size.
  FORCEINLINE MDY_NODISCARD const DDyVectorInt2& GetBufferSize() const noexcept { return this->mAttachmentSize; }

  /// @brief Get attachments' parameter list.
  MDY_NODISCARD const auto& GetParameterList() const noexcept { return this->mParameterList; }

  /// @brief Get Border color of attachment.
  MDY_NODISCARD const DDyColorRGBA& GetBorderColor() const noexcept { return this->mBorderColor; }

  /// @brief Get buffer type for intenal pixel.
  MDY_NODISCARD const auto& GetBufferType() const noexcept { return this->mBufferType; }

  /// @brief Get attachment type for this attachment.
  MDY_NODISCARD const auto& GetAttachmentType() const noexcept { return this->mAttachmentType; }

private:
  std::string                   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  TTextureParameterList         mParameterList  = {};
  DDyVectorInt2                 mAttachmentSize = {};
  DDyColorRGBA                  mBorderColor    = DDyColorRGBA::Black;
  EDyRenderBufferInternalFormat mBufferType     = EDyRenderBufferInternalFormat::NoneError;
  EDyTextureStyleType           mAttachmentType = EDyTextureStyleType::D2;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTINFORMATION_H