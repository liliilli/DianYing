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

#include <Dy/Core/Resource/Internal/ETextureEnums.h>
#include <Dy/Meta/Type/EDyTextureParameter.h>
#include <Dy/Meta/Type/EDyResourceTypes.h>
#include <Dy/Helper/Type/DVectorInt2.h>
#include <Dy/Helper/Type/DColorRGBA.h>
#include <Dy/Helper/Type/DClamp.h>

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
  FDyAttachmentInformation(const PDyGlAttachmentInstanceMetaInfo& metaInfo);
  ~FDyAttachmentInformation() = default;

  /// @brief Get attachment's specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

   /// @brief Get attachment size.
  MDY_NODISCARD const DVectorInt2& GetBufferSize() const noexcept { return this->mAttachmentSize; }

  /// @brief Get attachments' parameter list.
  MDY_NODISCARD const auto& GetParameterList() const noexcept { return this->mParameterList; }

  /// @brief Get Border color of attachment.
  MDY_NODISCARD const DColorRGBA& GetBorderColor() const noexcept { return this->mBorderColor; }

  /// @brief Get buffer type for intenal pixel.
  MDY_NODISCARD const auto& GetBufferType() const noexcept { return this->mBufferType; }

  /// @brief Get attachment type for this attachment.
  MDY_NODISCARD const auto& GetAttachmentType() const noexcept { return this->mAttachmentType; }

  /// @brief Get specified mipmap generation level for this attachment.
  MDY_NODISCARD TU32 GetMipmapLevel() const noexcept { return this->mMipmapLevels; }

  /// @brief Get the number of depth of this attahchment.
  MDY_NODISCARD TU32 GetDepthNumber() const noexcept { return this->mDepthNumber; }

  /// @brief Check information is ping-pong enabled.
  MDY_NODISCARD bool IsPingPong() const noexcept;

private:
  std::string                   mSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  TTextureParameterList         mParameterList  = {};
  DVectorInt2                 mAttachmentSize = {};
  DColorRGBA                  mBorderColor    = DColorRGBA::Black;
  EGlBufferDataInternalFormat mBufferType     = EGlBufferDataInternalFormat::NoneError;
  ETextureStyleType           mAttachmentType = ETextureStyleType::D2;
  DClamp<TU32, 1, 16>         mMipmapLevels   = 1;
  /// @brief This variable is only enabled if only mAttachmentType is `Array` type.
  TU32                          mDepthNumber    = 0;
  /// @brief When enabled, attachment will be created as ping-pong (two-attachment) attachment.
  bool                          mIsPingpong     = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTINFORMATION_H