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
#include <Dy/Helper/Library/HelperJson.h>

#include <filesystem>
#include <nlohmann/json.hpp>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

bool DyIsJsonKeyExist(const nlohmann::json& json, const std::string& key) noexcept
{
  return json.find(key) != json.end();
}

std::optional<nlohmann::json> DyGetJsonAtlasFromFile(const std::string& filePath) noexcept
{
  if (std::filesystem::exists(filePath) == false)
  {
    MDY_LOG_CRITICAL("DyGetJsonAtlasFromFile | File path is not exist so failed to read serialization file. | Path : {}", filePath);
    return std::nullopt;
  }

  std::FILE* fd = fopen(filePath.c_str(), "r");
  std::fseek(fd, 0, SEEK_END);
  const auto size = ftell(fd);
  std::fseek(fd, 0, SEEK_SET);

  std::vector<char> buffer(size);
  std::fread(buffer.data(), sizeof(char), size, fd);
  std::fclose(fd);

  return nlohmann::json::parse(buffer);
}

} /// ::dy namespace