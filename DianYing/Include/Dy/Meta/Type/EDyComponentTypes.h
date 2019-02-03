#ifndef GUARD_DY_META_TYPE_EDYCOMPONENTTYPES_H
#define GUARD_DY_META_TYPE_EDYCOMPONENTTYPES_H
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

///
/// @enum   EDyComponentMetaType
/// @brief  Meta type for component
///
enum class EDyComponentMetaType
{
  Transform, // CDyTransform
  Script, // CDyScript
  ModelFilter, // CDyModelFilter
  ModelRenderer, // CDyModelRenderer
  ModelAnimator, // CDyModelAnimator
  DirectionalLight, // CDyDirectionalLight
  Camera, // CDyCamera
  SoundSource, // CDySoundSource

  // ERROR!
  NoneError
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const EDyComponentMetaType& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyComponentMetaType& p);

///
/// @enum   EDyShadowType
/// @brief  EDyShadowType
///
enum class EDyShadowType
{
  Hard,
  Soft,
  NoneError
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const EDyShadowType& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyShadowType& p);

///
/// @enum   EDyCameraProjectionType
/// @brief  projection type of camera.
///
enum class EDyCameraProjectionType
{
  Orthographic,
  Perspecitve,
  NoneError
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const EDyCameraProjectionType& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyCameraProjectionType& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_EDYCOMPONENTTYPES_H