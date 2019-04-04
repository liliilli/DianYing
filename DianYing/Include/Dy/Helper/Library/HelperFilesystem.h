#ifndef GUARD_DY_HELPER_SYSTEM_FILESYSTEM_H
#define GUARD_DY_HELPER_SYSTEM_FILESYSTEM_H
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

#include <filesystem>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

using TFilePath = std::filesystem::path;
void to_json(nlohmann::json& oJson, const TFilePath& iFont);
void from_json(const nlohmann::json& iJson, TFilePath& oFont);

/// @brief Check file path is valid and file is exist on present filesystem. \n
/// This function supports relative path.
MDY_NODISCARD bool IsFileExist(const std::string& iFilePath);

/// @brief Check file path is valid and file is exist on present filesystem. \n
/// This function is just wrapping function of std::filesysstem::exists(iFilePath).
MDY_NODISCARD bool IsFileExist(const TFilePath& iFilePath);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_SYSTEM_FILESYSTEM_H