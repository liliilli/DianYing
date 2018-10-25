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
MDY_NODISCARD std::optional<nlohmann::json> DyGetJsonAtlas(const std::string& filePath) noexcept;

///
/// @brief  Exceptionable.
/// @param  jsonAtlas Immutable valid json atlas like-a types.
/// @param  name Header string to find.
/// @tparam TReturnType Type to retrieve from json atlas instance.
/// @tparam TParam1 Json binding type parameter
/// @return
/// @TODO SCRIPT THIS.
///
template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyGetValue(_MIN_ const TParam1& jsonAtlas, _MIN_ const std::string_view& name)
{
  return jsonAtlas.at(MSVSTR(name)).template get<TReturnType>();
}

///
/// @brief  Exceptionable.
/// @param  jsonAtlas
/// @return
/// @TODO SCRIPT THIS.
///
inline MDY_NODISCARD dy::DDyVector3 DyGetDDyVector3FromJson(_MIN_ const nlohmann::json& jsonAtlas)
{
  dy::DDyVector3 vector;
  vector.X = jsonAtlas.at("X").get<TF32>();
  vector.Y = jsonAtlas.at("Y").get<TF32>();
  vector.Z = jsonAtlas.at("Z").get<TF32>();
  return vector;
}

///
/// @brief
/// @param  jsonAtlas
/// @return
/// @TODO SCRIPT THIS
///
inline MDY_NODISCARD dy::DDyVector2 DyGetDDyVector2FromJson(_MIN_ const nlohmann::json& jsonAtlas)
{
  dy::DDyVector2 vector = {};
  vector.X = jsonAtlas.at("X").get<TF32>();
  vector.Y = jsonAtlas.at("Y").get<TF32>();
  return vector;
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_JSON_HELPER_H