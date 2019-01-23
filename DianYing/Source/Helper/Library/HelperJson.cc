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

  std::ifstream stream { filePath, std::ios_base::in };
  if (stream.bad() == true || stream.fail() == true) {
    MDY_LOG_ERROR("DyGetJsonAtlasFromFile | Unexpected error occurred in reading serializaition file. | Path : {}", filePath);
    stream.close();
    return std::nullopt;
  }

  nlohmann::json jsonAtlas;
  try
  {
    // parsing input with a syntax error
    stream >> jsonAtlas;
  }
  catch (nlohmann::json::parse_error& e)
  {
    // output exception information
    MDY_LOG_ERROR("message: {}\nException id: {}\nByte position of error: {}", e.what(), e.id, e.byte);
    MDY_UNEXPECTED_BRANCH();
  }
  stream.close();

  return jsonAtlas;
}

} /// ::dy namespace