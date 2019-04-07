#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Type/Time/DTimePoint.h>

namespace dy
{

DTimePoint::DTimePoint(_MIN_ tm input) :
  mYear{static_cast<TU16>(input.tm_year + 1900)},
  mMonth{static_cast<TU16>(input.tm_mon + 1)},
  mDay{static_cast<TU16>(input.tm_mday)},
  mHour{static_cast<TU16>(input.tm_hour)},
  mMinute{static_cast<TU16>(input.tm_min)},
  mSecond{static_cast<TU16>(input.tm_sec)},
  mDaysFrom{static_cast<TU16>(input.tm_yday)},
  mDayType{MDY_PRIVATE(DyConverTmDayToDyType)(input.tm_wday)},
  mIsDst{MDY_PRIVATE(DyConverTmDstToDyType)(input.tm_isdst)}
{ }

} /// ::dy namespace