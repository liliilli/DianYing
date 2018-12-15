#ifndef GUARD_DY_CORE_THREAD_IO_DDYIOTASKDEFERRED_H
#define GUARD_DY_CORE_THREAD_IO_DDYIOTASKDEFERRED_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <Dy/Core/Thread/IO/DDyIOTask.h>
#include <Dy/Helper/System/Idioms.h>

namespace dy
{

struct DDyIOTaskDeferred final
{
  struct DConditionItem final
  {
    std::string       mSpecifier  = MDY_INITIALIZE_EMPTYSTR;
    EDyResourceType   mType       = EDyResourceType::NoneError;
    EDyResourceStyle  mStyle      = EDyResourceStyle::NoneError;

    friend MDY_NODISCARD bool operator==(_MIN_ const DConditionItem& lhs, _MIN_ const DConditionItem& rhs) noexcept
    {
      return std::tie(lhs.mSpecifier, lhs.mType, lhs.mStyle) == std::tie(rhs.mSpecifier, rhs.mType, rhs.mStyle);
    }

    DConditionItem(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType iType, _MIN_ EDyResourceStyle iStyle) :
        mSpecifier{iSpecifier}, mType{iType}, mStyle{iStyle} {};
  };
  using TConditionList = std::vector<DDyIOTaskDeferred::DConditionItem>;

  DDyIOTask mTask;
  std::vector<DConditionItem> mCondition;

  DDyIOTaskDeferred(_MIN_ const DDyIOTask& iTask, _MIN_ const std::vector<DConditionItem>& iCondition) :
      mTask{iTask}, mCondition{iCondition} {};

  /// @brief
  MDY_NODISCARD EDySuccess TryRemoveDependenciesItem(_MIN_ const std::string& iSpecifier, _MIN_ EDyResourceType& iType, _MIN_ EDyResourceStyle& iStyle) noexcept
  {
    const DConditionItem item{iSpecifier, iType, iStyle};

    for (auto it = this->mCondition.begin(); it != this->mCondition.end(); ++it)
    {
      if (*it == item) {
        const auto index = std::distance(this->mCondition.begin(), it);
        DyFastErase(this->mCondition, index);
        return DY_SUCCESS;
      }
    }

    return DY_FAILURE;
  }

  /// @brief
  MDY_NODISCARD bool IsSatisfiedReinsertCondition() const noexcept { return mCondition.empty(); }
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_THREAD_IO_DDYIOTASKDEFERRED_H