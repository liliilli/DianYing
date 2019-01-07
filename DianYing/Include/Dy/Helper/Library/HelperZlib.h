#ifndef GUARD_DY_HELPER_HELPERZLIB_H
#define GUARD_DY_HELPER_HELPERZLIB_H
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

namespace dy::zlib
{

///
/// @brief  Compress uncompressed plain string to compressed string.
/// @param  uncompressedString
/// @return compressed string.
///
MDY_NODISCARD std::string DyCompressString(const std::string& uncompressedString);

///
/// @brief  Decompress compressed string to plain string.
/// @param  compressedString
/// @return plain string which have been decompressed.
///
MDY_NODISCARD std::string DyDecompressString(const std::string& compressedString);

} /// ::dy::zlib namespace

#endif /// GUARD_DY_HELPER_HELPERZLIB_H