#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
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

#include <Dy/Core/Resource/Type/TDyInformationBinder.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyFrameBufferInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{
class FDyFrameBufferResource final
{
public:
  FDyFrameBufferResource(_MIN_ const FDyFrameBufferInformation& iInformation);
  ~FDyFrameBufferResource();

  /// @brief Get specifier name of framebuffer resource.
  MDY_NODISCARD const std::string& GetSpecifierName() const noexcept { return this->mSpecifierName; }
  /// @brief Get framebuffer id.
  MDY_NODISCARD TU32 GetFrameBufferId() const noexcept { return this->mFrameBufferId; }
  /// @brief Get framebuffer size.
  MDY_NODISCARD const DDyVectorInt2& GetFrameBufferSize() const noexcept { return this->mFrameBufferSize; } 

  /// @brid Framebuffer.
  EDySuccess BindFrameBuffer()   const noexcept;
  void       UnbindFrameBuffer() const noexcept; 

private:
  std::string   mSpecifierName = MDY_INITIALIZE_EMPTYSTR;
  TU32          mFrameBufferId = MDY_INITIALIZE_DEFUINT;
  DDyVectorInt2 mFrameBufferSize = {};

  using TAttachmentBinder = std::unique_ptr<TDyResourceBinderAttachment>;
  using TAttachmentBinderList = std::vector<TAttachmentBinder>;
  TDyInformationBinderFrameBuffer  mBinderFrameBuffer;
  TAttachmentBinderList             mBinderAttachmentList = {};
  TDyResourceBinderAttachment      mBinderDepthBuffer    = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
