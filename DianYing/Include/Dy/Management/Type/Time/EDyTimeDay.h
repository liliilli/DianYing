#ifndef GUARD_DY_MANAGEMENT_TYPE_TIME_EDYTIMEDAY_H
#define GUARD_DY_MANAGEMENT_TYPE_TIME_EDYTIMEDAY_H
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

namespace dy
{

///
/// @enum EDyTimeDay
/// @brief Time day (sunday ~ saturday) type 
///
enum class EDyTimeDay : char
{
  Sunday    = 0,
  Monday    = 1,
  Tuesday   = 2,
  Wednesday = 3,
  Thursday  = 4,
  Friday    = 5,
  Saturday  = 6
};

inline EDyTimeDay MDY_PRIVATE_SPECIFIER(DyConverTmDayToDyType)(_MIN_ TI32 iInput) noexcept
{
  MDY_ASSERT(iInput >= 0 && iInput <= 6, "iInput is not valid value.");
  return static_cast<EDyTimeDay>(iInput);
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_TIME_EDYTIMEDAY_H