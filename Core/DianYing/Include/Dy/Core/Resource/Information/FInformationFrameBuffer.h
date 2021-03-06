#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationFrameBuffer_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationFrameBuffer_H
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
#include <Dy/Management/Type/AttachmentInformation.h>
#include <Dy/Core/Resource/Type/TInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGlFrameBufferInstanceMetaInfo;
class FInformationAttachment;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FInformationFrameBuffer
/// @brief FrameBuffer information resource.
class FInformationFrameBuffer final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FInformationFrameBuffer);

  /// @brief Construct framebuffer information.
  FInformationFrameBuffer(_MIN_ const PDyGlFrameBufferInstanceMetaInfo& metaInfo);
  ~FInformationFrameBuffer() = default;

  /// @brief Get framebuffer specifier name.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

  /// @brief Get frame buffer size.
  MDY_NODISCARD const DIVec2& GetFrameBufferSize() const noexcept { return this->mFrameBufferSize; }

  /// @brief Get attachment info list.
  MDY_NODISCARD const auto& GetAttachmentInformationBinderList() const noexcept { return this->mAttachmentInfoList; }

  /// @brief Get attachment binding mode list.
  MDY_NODISCARD const TBlendingEquationList& GetAttachmentBlendings() const noexcept; 

  /// @brief Check populated frame buffer is using default depth buffer.
  MDY_NODISCARD bool IsUsingDepthBuffer() const noexcept { return this->mIsUsingDepthBuffer; }

  /// @brief Get depth information binder instance as reference which is immutable.
  MDY_NODISCARD const auto& GetDepthBufferBinder() const noexcept { return this->mDepthAttachment; }

  /// @brief Check populated frame buffer is using pixel shader.
  MDY_NODISCARD bool IsUsingPixelShader() const noexcept { return this->mIsNotUsingPixelShader == false; }

  /// @brief Check framebuffer will be created as ping-pong framebuffer.
  MDY_NODISCARD bool IsPingPong() const noexcept;

private:
  using TAttachmentInformation = std::pair<
    PDyGlAttachmentBinderInformation, std::unique_ptr<TDyInformationBinderAttachment>
  >;
  using TAttachmentInfoBinderList = std::vector<TAttachmentInformation>;

  std::string               mSpecifierName;
  TAttachmentInfoBinderList mAttachmentInfoList = {};
  TBlendingEquationList     mAttachmentBlendings = {};
  DIVec2             mFrameBufferSize    = {};

  TAttachmentInformation    mDepthAttachment    = {};
  bool                      mIsUsingDepthBuffer = false;
  bool                      mIsNotUsingPixelShader = false;
  /// @brief When enabled, attachment will be created as ping-pong (two-attachment) attachment.
  bool                      mIsPingpong     = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FInformationFrameBuffer_H
