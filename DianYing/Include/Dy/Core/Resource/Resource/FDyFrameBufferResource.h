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

#include <Dy/Core/Resource/Type/TInformationBinder.h>
#include <Dy/Core/Resource/Type/TResourceBinder.h>
#include <Dy/Management/Type/AttachmentInformation.h>

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

/// @class FDyFrameBufferResource
/// @brief Frame buffer resource type (abstract)
class FDyFrameBufferResource 
{
public:
  FDyFrameBufferResource() = default;
  virtual ~FDyFrameBufferResource() = 0;

  /// @brief Get specifier name of framebuffer resource.
  MDY_NODISCARD const std::string& GetSpecifierName();
  /// @brief Get framebuffer id.
  MDY_NODISCARD virtual TU32 GetTargetFrameBufferId() const noexcept = 0;
  /// @brief Get framebuffer size.
  MDY_NODISCARD const DVectorInt2& GetFrameBufferSize();

  /// @brief Bind Framebuffer.
  EDySuccess virtual BindFrameBuffer() const noexcept = 0;
  /// @brief Unbind framebuffer and revert to default framebuffer.
  void UnbindFrameBuffer() const noexcept; 

  /// @brief Push framebuffer's internal gl states.
  void PushGlobalStates();
  /// @brief Pop framebuffer's internal gl states.
  void PopGlobalStates();

  /// @brief Swap.
  void virtual Swap() = 0;

protected:
  std::string   mSpecifierName;
  DVectorInt2 mFrameBufferSize  = {};

  using TAttachmentBinder = std::unique_ptr<TDyResourceBinderAttachment>;
  using TAttachmentBinderList = std::vector<TAttachmentBinder>;

  TDyInformationBinderFrameBuffer mBinderFrameBuffer;
  TAttachmentBinderList           mBinderAttachmentList   = {};
  TBlendingEquationList           mAttachmentBlendingList = {};
  TDyResourceBinderAttachment     mBinderDepthBuffer      = {};
};

inline FDyFrameBufferResource::~FDyFrameBufferResource() = default;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYFRAMEBUFFERRESOURCE_H
