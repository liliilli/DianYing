#ifndef GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTGENERALRESOURCE_H
#define GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTGENERALRESOURCE_H
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
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>

namespace dy
{

class FDyAttachmentGeneralResource final : public FDyAttachmentResource
{
public:
  explicit FDyAttachmentGeneralResource(const FDyAttachmentInformation& information);
  virtual ~FDyAttachmentGeneralResource();

  /// @brief Get binded attachment resource id.
  MDY_NODISCARD TU32 GetSourceAttachmentId() const noexcept override final;

  /// @brief Get targeted attachment resource id.
  /// But in general attachment, only one attachment is bound, 
  /// so return same id with GetSourceAttachmentId.
  MDY_NODISCARD TU32 GetTargetAttachmentId() const noexcept override final;

  /// @brief Swap target and texture binding indexes.
  /// But in general attachment, only have one attachment in instance so do nothing.
  void Swap() override final { /* Do nothing */ }

private:
  TU32 mAttachmentId = 0;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RESOURCE_INFORMATION_FDYATTACHMENTGENERALRESOURCE_H
