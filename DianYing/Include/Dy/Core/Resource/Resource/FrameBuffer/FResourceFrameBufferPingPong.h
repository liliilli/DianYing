#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceFrameBufferPingPong_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceFrameBufferPingPong_H
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
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>

namespace dy
{

/// @class FResourceFrameBufferPingPong
/// @brief Framebuffer resource that supports color attachments 
/// that is one-attachment & two-attachment.
class FResourceFrameBufferPingPong final : public AResourceFrameBufferBase
{
public:
  FResourceFrameBufferPingPong(const FInformationFrameBuffer& iInformation);
  virtual ~FResourceFrameBufferPingPong();

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

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceFrameBufferPingPong_H
