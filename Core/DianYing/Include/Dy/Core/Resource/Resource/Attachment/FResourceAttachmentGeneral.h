#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceAttachmentGeneral_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceAttachmentGeneral_H
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

namespace dy
{

class FResourceAttachmentGeneral final : public AResourceAttachmentBase
{
public:
  explicit FResourceAttachmentGeneral(const FInformationAttachment& information);
  virtual ~FResourceAttachmentGeneral();

  /// @brief Get binded attachment resource id.
  TU32 GetSourceAttachmentId() const noexcept override final;

  /// @brief Get targeted attachment resource id.
  /// But in general attachment, only one attachment is bound, 
  /// so return same id with GetSourceAttachmentId.
  TU32 GetTargetAttachmentId() const noexcept override final;

  /// @brief Swap target and texture binding indexes.
  /// But in general attachment, only have one attachment in instance so do nothing.
  void Swap() override final { /* Do nothing */ }

private:
  TU32 mAttachmentId = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FResourceAttachmentGeneral_H
