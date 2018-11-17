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
#include <optional>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Helper/Type/Vector2.h>

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
MDY_NODISCARD std::optional<nlohmann::json> DyGetJsonAtlasFromFile(const std::string& filePath) noexcept;

///
/// @brief  Exceptionable.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  name Header string to find.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TParam1 Json binding type parameter
///
template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyJsonGetValueFrom(_MIN_ const TParam1& jsonAtlas, _MIN_ const std::string_view& name)
{
  return jsonAtlas.at(MSVSTR(name)).template get<TReturnType>();
}

///
/// @brief  Get value from json and bind value to destination automatically.
/// Destination type must implement copy assignment operator or default behavior.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  key Header string to find.
/// @param  destination Destination value.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TJsonAtlas Json binding type paramter.
/// @return
///
template <typename TReturnType, typename TJsonAtlas>
void DyJsonGetValueFromTo(_MIN_ const TJsonAtlas& jsonAtlas,
                          _MIN_ const std::string_view& key,
                          _MINOUT_ TReturnType& destination)
{
  if constexpr (std::is_move_assignable_v<TReturnType> == true)
  {
    destination = std::move(jsonAtlas.at(MSVSTR(key)).template get<TReturnType>());
  }
  else
  {
    destination = jsonAtlas.at(MSVSTR(key)).template get<TReturnType>();
  }
}

///
/// @brief  Find "Header" String is exist on given json atlas.
/// @param  atlas Valid immutable json atlas instance.
/// @param  string Header string to verify.
/// @return If found, return DY_SUCCESS or DY_FAILURE.
///
inline MDY_NODISCARD EDySuccess
DyCheckHeaderIsExist(_MIN_ const nlohmann::json& atlas, _MIN_ const std::string_view& string) noexcept
{
  if (atlas.find(MSVSTR(string)) == atlas.end())  { return DY_FAILURE; }
  else                                            { return DY_SUCCESS; }
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_JSON_HELPER_H