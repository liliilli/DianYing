#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDyFrameBufferPingPongResource_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDyFrameBufferPingPongResource_H
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
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>

namespace dy
{

/// @class FDyFrameBufferPingPongResource
/// @brief Framebuffer resource that supports color attachments 
/// that is one-attachment & two-attachment.
class FDyFrameBufferPingPongResource final : public FDyFrameBufferResource
{
public:
  FDyFrameBufferPingPongResource(const FDyFrameBufferInformation& iInformation);
  virtual ~FDyFrameBufferPingPongResource();

  /// @brief Get Framebuffer id on target.
  MDY_NODISCARD TU32 GetTargetFrameBufferId() const noexcept override final;

  /// @brief Bind framebuffer to system.
  EDySuccess BindFrameBuffer() const noexcept override final;

  /// @brief Do nothing.
  void Swap() override final;

private:
  std::array<TU32, 2> mFrameBufferIds = {};
  bool mIsLeftTarget = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDyFrameBufferPingPongResource_H
