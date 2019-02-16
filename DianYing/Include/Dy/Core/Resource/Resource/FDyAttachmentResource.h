#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTRESOURCE_H
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

#include <Dy/Helper/Type/VectorInt2.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyAttachmentInformation;
enum class EDyGlBufferDataInternalFormat : unsigned char;
enum class EDyTextureStyleType : unsigned char;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyAttachmentResource final
{
public:
  FDyAttachmentResource(_MIN_ const FDyAttachmentInformation& information);
  ~FDyAttachmentResource();

  /// @brief Get specifier name of attachment information.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

  /// @brief Get binded attachment resource id.
  MDY_NODISCARD FORCEINLINE TU32 GetAttachmentId() const noexcept { return this->mAttachmentId; };

  /// @brief Check this attachment resource is render buffer.
  MDY_NODISCARD bool IsRenderBuffer() const noexcept { return this->mIsRenderBuffer; }
  
  /// @brief Get buffer type for intenal pixel.
  MDY_NODISCARD const EDyGlBufferDataInternalFormat& GetBufferType() const noexcept;

  /// @brief Get attachment type for this attachment.
  MDY_NODISCARD const EDyTextureStyleType& GetAttachmentType() const noexcept;

  /// @brief Get specified mipmap generation level for this attachment.
  MDY_NODISCARD TU32 GetMipmapLevel() const noexcept;

  /// @brief Get the number of depth of this attahchment.
  MDY_NODISCARD TU32 GetDepthNumber() const noexcept;

private:
  std::string mSpecifierName   = MDY_INITIALIZE_EMPTYSTR;
  TU32        mAttachmentId    = MDY_INITIALIZE_DEFUINT;
  bool        mIsRenderBuffer  = false;
  TDyInformationBinderAttachment mInformationBinder;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTRESOURCE_H
