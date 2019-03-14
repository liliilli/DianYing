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
    DyPushLogCritical("File path is not exist, so failed to read serialization file. {}", filePath);
    return std::nullopt;
  }

  std::FILE* fd = std::fopen(filePath.c_str(), "r");
  std::fseek(fd, 0, SEEK_END);
  const auto size = ftell(fd);
  std::fseek(fd, 0, SEEK_SET);

  std::vector<char> buffer(size + 1);
  std::fread(buffer.data(), sizeof(char), size, fd);
  std::fclose(fd);

  nlohmann::json json;
  try 
  {
    json = nlohmann::json::parse(buffer);
  }
  catch (nlohmann::json::parse_error& e)
  {
    MDY_ASSERT_MSG_FORCE(false, e.what());
  }
  return json;
}

std::optional<nlohmann::json> 
DyGetJsonAtlasFromFile(const std::filesystem::path& iFilePath) noexcept
{
  std::FILE* fd = std::fopen(iFilePath.string().c_str(), "r");
  std::fseek(fd, 0, SEEK_END);
  const auto size = ftell(fd);
  std::fseek(fd, 0, SEEK_SET); 

  std::vector<char> buffer(size + 1);
  std::fread(buffer.data(), sizeof(char), size, fd);
  std::fclose(fd);

  try 
  {
    return nlohmann::json::parse(buffer);
  }
  catch (nlohmann::json::parse_error& e)
  {
    MDY_ASSERT_MSG_FORCE(false, e.what());
    return std::nullopt;
  }
}

std::optional<nlohmann::json> 
GetJsonAtlasFromString(const std::string& iSerializedString) noexcept
{
  try
  {
    auto result = nlohmann::json::parse(iSerializedString);
    return result;
  }
  catch (nlohmann::json::parse_error& e)
  {
    DyPushLogCritical(e.what());
    return std::nullopt;
  }
}

} /// ::dy namespace