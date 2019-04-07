#ifndef GUARD_DY_CORE_THREAD_IO_DDYIOTASKDEFERRED_H
#define GUARD_DY_CORE_THREAD_IO_DDYIOTASKDEFERRED_H
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

#include <Dy/Core/Thread/IO/DRescIOTask.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

struct DRescIODeferredTask final
{
  struct DConditionItem final
  {
    std::string       mSpecifier  = MDY_INITIALIZE_EMPTYSTR;
    EResourceType   mType       = EResourceType::NoneError;
    EResourceStyle  mStyle      = EResourceStyle::NoneError;

    friend MDY_NODISCARD bool operator==(_MIN_ const DConditionItem& lhs, _MIN_ const DConditionItem& rhs) noexcept
    {
      return std::tie(lhs.mSpecifier, lhs.mType, lhs.mStyle) == std::tie(rhs.mSpecifier, rhs.mType, rhs.mStyle);
    }

    DConditionItem(_MIN_ const std::string& iSpecifier, _MIN_ EResourceType iType, _MIN_ EResourceStyle iStyle) :
        mSpecifier{iSpecifier}, mType{iType}, mStyle{iStyle} {};
  };
  using TConditionList = std::vector<DRescIODeferredTask::DConditionItem>;

  DRescIOTask mTask;
  std::vector<DConditionItem> mCondition;

  DRescIODeferredTask(_MIN_ const DRescIOTask& iTask, _MIN_ const std::vector<DConditionItem>& iCondition) :
      mTask{iTask}, mCondition{iCondition} {};

  /// @brief
  MDY_NODISCARD EDySuccess TryRemoveDependentItem(
      _MIN_ const std::string& iSpecifier, 
      _MIN_ EResourceType& iType, 
      _MIN_ EResourceStyle& iStyle) noexcept;

  /// @brief
  MDY_NODISCARD bool IsSatisfiedReinsertCondition() const noexcept { return mCondition.empty(); }
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_DDYIOTASKDEFERRED_H