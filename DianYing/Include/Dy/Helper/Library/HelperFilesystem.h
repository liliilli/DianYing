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

/// @struct DFilePath
/// @brief File path wrapper type.
struct DFilePath final
{
  using TFilePath = std::filesystem::path;
  using TStringType = TFilePath::string_type;

  DFilePath() = default;

  DFilePath(const TFilePath& filepath) : mFilePath{ filepath } {};
  DFilePath& operator=(const TFilePath& p)
  {
    this->mFilePath = p;
    return *this;
  }

  DFilePath(TFilePath&& filepath) noexcept : mFilePath{ std::move(filepath) } {};
  DFilePath& operator=(TFilePath&& p)
  {
    this->mFilePath = std::move(p);
    return *this;
  }

  DFilePath(const TStringType& filepath) : mFilePath{ filepath } {};
  DFilePath& operator=(const TStringType& filepath)
  {
    this->mFilePath = filepath;
    return *this;
  }

  TFilePath* operator->() noexcept { return &this->mFilePath; }
  const TFilePath* operator->() const noexcept { return &this->mFilePath; }

  TFilePath& operator*() noexcept { return this->mFilePath; }
  const TFilePath& operator*() const noexcept { return this->mFilePath; }

  operator TFilePath() const noexcept { return this->mFilePath; }

private:
  TFilePath mFilePath;
};

using TFilePath = std::filesystem::path;
void to_json(nlohmann::json& oJson, const DFilePath& iPath);
void from_json(const nlohmann::json& iJson, DFilePath& oPath);

/// @brief Check file path is valid and file is exist on present filesystem. \n
/// This function supports relative path.
MDY_NODISCARD bool IsFileExist(const std::string& iFilePath);

/// @brief Check file path is valid and file is exist on present filesystem. \n
/// This function is just wrapping function of std::filesysstem::exists(iFilePath).
MDY_NODISCARD bool IsFileExist(const TFilePath& iFilePath);

/// @brief Check file path is valid and file is exist on present filesystem. \n
/// This function is just wrapping function of std::filesysstem::exists(iFilePath).
MDY_NODISCARD bool IsFileExist(const DFilePath& iFilePath);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_SYSTEM_FILESYSTEM_H