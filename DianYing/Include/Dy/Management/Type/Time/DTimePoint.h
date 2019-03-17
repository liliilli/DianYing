#ifndef GUARD_DY_MANAGEMENT_TYPE_TIME_DDYTIME_H
#define GUARD_DY_MANAGEMENT_TYPE_TIME_DDYTIME_H
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

#include <Dy/Helper/GlobalType.h>
#include <Dy/Helper/System/Macroes.h>
#include <Dy/Management/Type/Time/ETimeDay.h>
#include <Dy/Management/Type/Time/ETimeDST.h>

namespace dy
{

/// @struct DTimePoint
/// @brief Time value sturcture type for retrieving time-point.
class DTimePoint final
{
public:
  DTimePoint(_MIN_ tm input);

  /// @brief Get year.
  MDY_NODISCARD auto Year() const noexcept { return this->mYear; }
  /// @brief Get month.
  MDY_NODISCARD auto Month() const noexcept { return this->mMonth; }
  /// @brief Get day.
  MDY_NODISCARD auto Day() const noexcept { return this->mDay; }
  /// @brief Get day of the week as type.
  MDY_NODISCARD auto DayOfWeek() const noexcept { return this->mDayType; }
  /// @brief Get day of this year as value.
  MDY_NODISCARD auto DayOfYear() const noexcept { return this->mDaysFrom; }
  /// @brief Get whether local region is using day light saving time.
  MDY_NODISCARD auto IsUsingDaylightSavingTime() const noexcept { return this->mIsDst; }

  /// @brief Get hour as 24-hours type.
  MDY_NODISCARD auto Hour() const noexcept { return this->mHour; }
  /// @brief Get minutes
  MDY_NODISCARD auto Minute() const noexcept { return this->mMinute; }
  /// @brief Get second
  MDY_NODISCARD auto Second() const noexcept { return this->mSecond; }

private:
  TU16 mYear          = MDY_INITIALIZE_DEFUINT;
  TU16 mMonth         = MDY_INITIALIZE_DEFUINT;
  TU16 mDay           = MDY_INITIALIZE_DEFUINT;
  TU16 mHour          = MDY_INITIALIZE_DEFUINT;
  TU16 mMinute        = MDY_INITIALIZE_DEFUINT;
  TU16 mSecond        = MDY_INITIALIZE_DEFUINT;
  TU16 mDaysFrom      = MDY_INITIALIZE_DEFUINT;
  ETimeDay mDayType = ETimeDay::Sunday;
  ETimeDST mIsDst   = ETimeDST::NotAvailable;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_TIME_DDYTIME_H