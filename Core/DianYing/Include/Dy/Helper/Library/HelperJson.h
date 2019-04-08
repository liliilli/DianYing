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
#include <filesystem>

//!
//! Implementation
//!

namespace dy::json
{

/// @brief Check key is exist in present json instance.
[[nodiscard]] bool HasJsonKey(const nlohmann::json& json, const std::string& key) noexcept;

/// @brief Read json file and return json container. 
/// If any error has happended, just return nullptr.
MDY_NODISCARD std::optional<nlohmann::json> 
GetAtlasFromFile(const std::string& iFilePath) noexcept;

MDY_NODISCARD std::optional<nlohmann::json> 
GetAtlasFromFile(const std::filesystem::path& iFilePath) noexcept;

MDY_NODISCARD std::optional<nlohmann::json> 
GetAtlasFromString(const std::string& iSerializedString) noexcept;

/// @brief  Exceptionable.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  name Header string to find.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TParam1 Json binding type parameter
template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType GetValueFrom(const TParam1& jsonAtlas, const char* name);

template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType GetValueFrom(const TParam1& jsonAtlas, const std::string_view& name);

template <typename TReturnType, typename TJsonParam>
MDY_NODISCARD TReturnType GetValue(const TJsonParam& jsonAtlas);

/// @brief  Get value from json and bind value to oDestination automatically.
/// Destination type must implement copy assignment operator or default behavior.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  iKey Header string to find.
/// @param  oDestination Destination value.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TJsonAtlas Json binding type paramter.
template <typename TReturnType, typename TJsonAtlas>
void GetValueFromTo(
    const TJsonAtlas& jsonAtlas, 
    const std::string& iKey, 
    TReturnType& oDestination);

} /// ::dy::json namespace

#endif /// GUARD_DY_HELPER_JSON_HELPER_H
#include <Dy/Helper/Library/HelperJson.inl>