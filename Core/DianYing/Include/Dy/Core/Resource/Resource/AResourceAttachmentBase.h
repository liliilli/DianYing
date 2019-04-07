#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_AResourceAttachmentBase_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_AResourceAttachmentBase_H
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

#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FInformationAttachment;
enum class EGlBufferDataInternalFormat : unsigned char;
enum class ETextureStyleType : unsigned char;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class AResourceAttachmentBase
{
public:
  AResourceAttachmentBase() = default;
  virtual ~AResourceAttachmentBase() = 0;

  /// @brief Get specifier name of attachment information.
  const std::string& GetSpecifierName() const noexcept; 

  /// @brief Get binded source attachment resource id.
  virtual TU32 GetSourceAttachmentId() const noexcept = 0;

  /// @brief Get target (destination) attachment resource id.
  virtual TU32 GetTargetAttachmentId() const noexcept = 0;

  /// @brief Get buffer type for intenal pixel.
  const EGlBufferDataInternalFormat& GetBufferType() const noexcept;

  /// @brief Get attachment type for this attachment.
  const ETextureStyleType& GetAttachmentType() const noexcept;

  /// @brief Get specified mipmap generation level for this attachment.
  TU32 GetMipmapLevel() const noexcept;

  /// @brief Get the number of depth of this attahchment.
  TU32 GetDepthNumber() const noexcept;

  /// @brief Check this attachment resource is render buffer.
  MDY_NODISCARD bool IsRenderBuffer() const noexcept { return this->mIsRenderBuffer; }
  
  /// @brief Check this resource is ping-pong resource.
  MDY_NODISCARD bool IsPingPong() const noexcept;

  /// @brief Swap target and texture binding indexes.
  virtual void Swap() = 0;

protected:
  TDyInformationBinderAttachment mInformationBinder;

  std::string mSpecifierName;
  bool        mIsRenderBuffer = false;
  bool        mIsPingpong     = false;
};

inline AResourceAttachmentBase::~AResourceAttachmentBase() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_AResourceAttachmentBase_H
