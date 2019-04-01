#ifndef GUARD_DY_HELPER_STRINGSWITCH_H
#define GUARD_DY_HELPER_STRINGSWITCH_H
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

#include <Dy/Helper/System/HashCompileCrc32.h>

namespace dy
{

/// @brief Get runtime string to crc-32 hash value for using switch-case statements.
inline TU32 SwitchStrInput(const std::string& iInput)
{
  return hash::ToHashCrc32(iInput.c_str());
}

/// @brief Do compile time or runtime conversion of literal string to hash value.
constexpr TU32 CaseStr(const char* iLiteral)
{
  return hash::ToHashCrc32(iLiteral);
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_STRINGSWITCH_H