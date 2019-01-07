#ifndef GUARD_DY_HELPER_IO_HELPER_H
#define GUARD_DY_HELPER_IO_HELPER_H
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
/// @brief Read file as binary mode and return chunk of file buffer.
///
[[nodiscard]] std::optional<std::vector<char>> DyReadBinaryFileAll(const std::string& filePath);

///
/// @brief Get file name from path without file speicification (.exe, like)
///
[[nodiscard]] std::string DyGetFileNameFromPath(const std::string& path) noexcept;

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_IO_HELPER_H