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
#include <Dy/Helper/Library/HelperFilesystem.h>

#include <filesystem>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>

namespace dy
{

void to_json(nlohmann::json& oJson, const DFilePath& iFont)
{
  oJson = iFont->string();
}

void from_json(const nlohmann::json& iJson, DFilePath& oFont)
{
  oFont = json::GetValue<std::string>(iJson);
  MDY_ASSERT(IsFileExist(oFont) == true);
}

bool IsFileExist(const std::string& iFilePath)
{
  namespace fs = std::filesystem;
  return fs::exists(iFilePath);
}

bool IsFileExist(const TFilePath& iFilePath)
{
  namespace fs = std::filesystem;
  return exists(iFilePath);
}

MDY_NODISCARD bool IsFileExist(const DFilePath& iFilePath)
{
  namespace fs = std::filesystem;
  return fs::exists(iFilePath);
}

} /// ::dy namespace