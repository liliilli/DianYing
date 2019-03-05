#ifndef GUARD_DY_ELEMENT_DESCRIPTOR_GLOBALENUMS_H
#define GUARD_DY_ELEMENT_DESCRIPTOR_GLOBALENUMS_H
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

#include <nlohmann/json_fwd.hpp>

namespace dy
{

/// @enum   EDyMetaObjectType
/// @brief  Meta object type for using DDyObjectInformation and PDyPrefabInstanceMetaInfo
enum class EDyMetaObjectType
{
  Actor,
  SceneScriptor,
  Object,
  NoneError
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyMetaObjectType& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyMetaObjectType& p);

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_DESCRIPTOR_GLOBALENUMS_H