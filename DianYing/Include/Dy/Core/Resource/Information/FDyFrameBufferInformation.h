#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERINFORMATION_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERINFORMATION_H
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
#include <Dy/Management/Type/AttachmentInformation.h>
#include <Dy/Core/Resource/Type/TDyInformationBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGlFrameBufferInstanceMetaInfo;
class FDyAttachmentInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyFrameBufferInformation
/// @brief FrameBuffer information resource.
///
class FDyFrameBufferInformation final
{
public:
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(FDyFrameBufferInformation);

  /// @brief Construct framebuffer information.
  FDyFrameBufferInformation(_MIN_ const PDyGlFrameBufferInstanceMetaInfo& metaInfo);
  ~FDyFrameBufferInformation() = default;

  /// @brief Get framebuffer specifier name.
  FORCEINLINE MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }

  /// @brief Get frame buffer size.
  FORCEINLINE MDY_NODISCARD const auto& GetFrameBufferSize() const noexcept { return this->mFrameBufferSize; }

  /// @brief Get attachment info list.
  MDY_NODISCARD const auto& GetAttachmentInformationBinderList() const noexcept { return this->mAttachmentInfoList; }

  /// @brief Check populated frame buffer is using default depth buffer.
  MDY_NODISCARD bool IsUsingDepthBuffer() const noexcept { return this->mIsUsingDepthBuffer; }

  /// @brief Get depth information binder instance as reference which is immutable.
  MDY_NODISCARD const auto& GetDepthBufferBinder() const noexcept { return this->mDepthAttachment; }

  /// @brief Check populated frame buffer is using pixel shader.
  MDY_NODISCARD bool IsUsingPixelShader() const noexcept { return this->mIsNotUsingPixelShader == false; }

private:
  using TAttachmentInformation = std::pair<PDyGlAttachmentBinderInformation, std::unique_ptr<TDyIInformationBinderAttachment>>;
  using TAttachmentInfoBinderList = std::vector<TAttachmentInformation>;

  std::string               mSpecifierName      = MDY_INITIALIZE_EMPTYSTR;
  TAttachmentInfoBinderList mAttachmentInfoList = {};
  DDyVectorInt2             mFrameBufferSize    = {};

  // @TODO NOT USED YET BUT IMPLEMENT FOR COLOR ATTACHMENT BINDING FLAW SO REPLACE EXISTING BOOL FLAG BELOW.
  TAttachmentInformation    mDepthAttachment        = {};
  bool                      mIsUsingDepthBuffer     = false;
  bool                      mIsNotUsingPixelShader  = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERINFORMATION_H
