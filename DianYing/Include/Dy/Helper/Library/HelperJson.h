#ifndef GUARD_DY_HELPER_JSON_HELPER_H
#define GUARD_DY_HELPER_JSON_HELPER_H
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

#include <nlohmann/json.hpp>
#include <optional>

//!
//! Implementation
//!

namespace dy
{

/// @brief Check key is exist in present json instance.
bool DyIsJsonKeyExist(const nlohmann::json& json, const std::string& key) noexcept;

/// @brief Read json file and return json container. If any error has happened just return nullopt.
MDY_NODISCARD std::optional<nlohmann::json> DyGetJsonAtlasFromFile(const std::string& filePath) noexcept;

/// @brief  Exceptionable.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  name Header string to find.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TParam1 Json binding type parameter
template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyJsonGetValueFrom(_MIN_ const TParam1& jsonAtlas, _MIN_ const char* name);

template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyJsonGetValueFrom(_MIN_ const TParam1& jsonAtlas, _MIN_ const std::string_view& name);

/// @brief  Get value from json and bind value to destination automatically.
/// Destination type must implement copy assignment operator or default behavior.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  iKey Header string to find.
/// @param  destination Destination value.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TJsonAtlas Json binding type paramter.
template <typename TReturnType, typename TJsonAtlas>
void DyJsonGetValueFromTo(
    _MIN_ const TJsonAtlas& jsonAtlas, 
    _MIN_ const std::string& iKey, 
    _MINOUT_ TReturnType& destination);

/// @brief  Find "Header" String is exist on given json atlas.
/// @param  atlas Valid immutable json atlas instance.
/// @param  iString Header string to verify.
/// @return If found, return DY_SUCCESS or DY_FAILURE.
MDY_NODISCARD inline EDySuccess
DyCheckHeaderIsExist(
    _MIN_ const nlohmann::json& atlas, 
    _MIN_ const std::string& iString) noexcept;

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_JSON_HELPER_H
#include <Dy/Helper/Library/HelperJson.inl>