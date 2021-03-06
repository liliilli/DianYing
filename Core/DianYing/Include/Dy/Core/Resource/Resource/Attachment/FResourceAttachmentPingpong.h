#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceAttachmentPingpong_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceAttachmentPingpong_H
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
#include <Dy/Core/Resource/Resource/AResourceAttachmentBase.h>

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

class FResourceAttachmentPingpong final : public AResourceAttachmentBase
{
public:
  FResourceAttachmentPingpong(const FInformationAttachment& information);
  virtual ~FResourceAttachmentPingpong();

  /// @brief Get binded attachment resource id.
  TU32 GetSourceAttachmentId() const noexcept override final; 

  /// @brief Get targeted attachment resource id.
  TU32 GetTargetAttachmentId() const noexcept override final;

  /// @brief Swap target and texture binding indexes.
  void Swap() override final;

private:
  std::array<TU32, 2> mAttachmentIds = {};
  bool mIsLeftTarget = true;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceAttachmentPingpong_H
