#ifndef GUARD_DY_HELPER_LIBRARY_HELPERSTRING_H
#define GUARD_DY_HELPER_LIBRARY_HELPERSTRING_H
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

#include <string>

namespace dy
{

/// @brief  Make UTF-8 string with arbitary number of arguments.
/// If iFormatString is "{} World{1}", and following args are "Hello", and "!",
/// Returned string will be "Hello World!".
///
/// @return UTF-8 string.
template <typename... TArgs>
MDY_NODISCARD std::string MakeStringU8(const std::string& iFormatString, TArgs&&... iArgs);

/// @brief Convert given string to lower string.
/// This function does not detect UTF-8, but only ANSI, so need to be checked prior to calling function.
void ToLower(std::string& ioString);

/// @brief Convert given string to lower string and return as new string.
/// This function does not detect UTF-8, but only ANSI, so need to be checked prior to calling function.
MDY_NODISCARD std::string ToLowerCopy(const std::string& iString);

/// @brief Convert given string to upper string.
/// This function does not detect UTF-8, but only ANSI, so need to be checked prior to calling function.
void ToUpper(std::string& ioString);

/// @brief Convert given string to upper string and return as new string.
/// This function does not detect UTF-8, but only ANSI, so need to be checked prior to calling function.
MDY_NODISCARD std::string ToUpperCopy(const std::string& iString);

/// @brief Get random string wich length count.
MDY_NODISCARD std::string CreateRandomString(TU32 iLength);

/// @brief Check given iString has postfix, `iPostfix`.
MDY_NODISCARD bool HasPostfix(const std::string& iString, const std::string& iPostfix);

/// @brief Try remove postfix, if postfix is exist in given string.
MDY_NODISCARD std::string TryRemovePostfix(const std::string& iString, const std::string& iPostfix);

} /// ::dy namesapce

#endif /// GUARD_DY_HELPER_LIBRARY_HELPERSTRING_H
#include <Dy/Helper/Library/HelperString.inl>

