#ifndef GUARD_DY_MANAGEMENT_TYPE_TIME_EDYTIMETYPE_H
#define GUARD_DY_MANAGEMENT_TYPE_TIME_EDYTIMETYPE_H
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

namespace dy
{

///
/// @enum EDyTimeDST
/// @brief Checking whether time-point is using either daylight, not or not available.
///
enum class EDyTimeDST : char
{
  Using,
  NotUsing,
  NotAvailable,
};

inline EDyTimeDST MDY_PRIVATE_SPECIFIER(DyConverTmDstToDyType)(_MIN_ TI32 iInput) noexcept
{
  if (iInput == 0)      { return EDyTimeDST::NotUsing; }
  else if (iInput > 0)  { return EDyTimeDST::Using; }
  else                  { return EDyTimeDST::NotAvailable; }
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_TIME_EDYTIMETYPE_H