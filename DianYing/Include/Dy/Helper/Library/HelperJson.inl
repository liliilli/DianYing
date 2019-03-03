#ifndef GUARD_DY_HELPER_LIBRARY_HelperJson_INL
#define GUARD_DY_HELPER_LIBRARY_HelperJson_INL
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

#include <Dy/Helper/Library/HelperJson.h>

namespace dy
{

bool DyIsJsonKeyExist(const nlohmann::json& json, const std::string& key) noexcept;

MDY_NODISCARD std::optional<nlohmann::json> DyGetJsonAtlasFromFile(const std::string& filePath) noexcept;

template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyJsonGetValueFrom(_MIN_ const TParam1& jsonAtlas, _MIN_ const char* name)
{
  return jsonAtlas.at(name).template get<TReturnType>();
}

template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyJsonGetValueFrom(_MIN_ const TParam1& jsonAtlas, _MIN_ const std::string_view& name)
{
  return DyJsonGetValueFrom<TParam1>(jsonAtlas, (name));
}

template <typename TReturnType, typename TJsonAtlas>
void DyJsonGetValueFromTo(
    _MIN_ const TJsonAtlas& jsonAtlas, 
    _MIN_ const std::string& iKey, 
    _MINOUT_ TReturnType& destination)
{
  if constexpr (std::is_move_assignable_v<TReturnType> == true)
  {
    destination = std::move(jsonAtlas.at(iKey).template get<TReturnType>());
  }
  else
  {
    destination = jsonAtlas.at(iKey).template get<TReturnType>();
  }
}

MDY_NODISCARD inline EDySuccess
DyCheckHeaderIsExist(
    _MIN_ const nlohmann::json& atlas, 
    _MIN_ const std::string& iString) noexcept
{
  if (atlas.find(iString) == atlas.end()) { return DY_FAILURE; }
  else                                    { return DY_SUCCESS; }
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_LIBRARY_HelperJson_INL