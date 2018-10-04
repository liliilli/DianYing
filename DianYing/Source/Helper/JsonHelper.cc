#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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
#include <Dy/Helper/JsonHelper.h>

#include <filesystem>
#include <nlohmann/json.hpp>
#include <Dy/Management/LoggingManager.h>

namespace dy
{

bool DyIsJsonKeyExist(const nlohmann::json& json, const std::string& key) noexcept
{
  return json.find(key) != json.end();
}

std::optional<nlohmann::json> DyGetJsonAtlas(const std::string& filePath) noexcept
{
  if (!std::filesystem::exists(filePath))
  {
    MDY_LOG_CRITICAL("DyGetJsonAtlas | File path is not exist so failed to read serialization file. | Path : {}", filePath);
    return std::nullopt;
  }

  std::ifstream stream { filePath, std::ios_base::in };
  if (!stream.good()) {
    MDY_LOG_ERROR("DyGetJsonAtlas | Unexpected error occurred in reading serializaition file. | Path : {}", filePath);
    stream.close();
    return std::nullopt;
  }

  nlohmann::json jsonAtlas;
  stream >> jsonAtlas;
  stream.close();

  return jsonAtlas;
}

} /// ::dy namespace