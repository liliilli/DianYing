#ifndef GUARD_DY_EDITOR_HELPER_WIN32HELPER_H
#define GUARD_DY_EDITOR_HELPER_WIN32HELPER_H
#if defined(MDY_FLAG_IN_EDITOR) && defined(MDY_PLATFORM_FLAG_WINDOWS)
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

namespace dy::editor
{

///
/// @brief Get unicode (w_char) string of absolute directory path.
/// If not selected anything, just return empty string.
///
std::wstring DyWinGetDirectoryPathFromDialog() noexcept;

///
/// @brief Get unicode (w_char) string of absolute file path.
/// If not selected anything, just return empty string.
///
std::wstring DyWinGetFilePathFromDialog() noexcept;

} /// ::dy::editor namespace

#endif /// MDY_FLAG_IN_EDITOR && MDY_PLATFORM_FLAG_WINDOWS
#endif /// GUARD_DY_EDITOR_HELPER_WIN32HELPER_H