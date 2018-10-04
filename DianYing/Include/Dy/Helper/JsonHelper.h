#ifndef GUARD_DY_HELPER_JSON_HELPER_H
#define GUARD_DY_HELPER_JSON_HELPER_H
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

#include <nlohmann/json.hpp>

namespace dy
{

///
/// @brief Check key is exist in present json instance.
///
bool DyIsJsonKeyExist(const nlohmann::json& json, const std::string& key) noexcept;

///
/// @brief Read json file and return json container. If any error has happened just return nullopt.
/// @TODO NEED TO WRAPPING VANILLA NLOHMANN::JSON?
///
std::optional<nlohmann::json> DyGetJsonAtlas(const std::string& filePath) noexcept;

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_JSON_HELPER_H