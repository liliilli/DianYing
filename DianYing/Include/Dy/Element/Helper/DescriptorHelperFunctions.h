#ifndef GUARD_DY_ELEMENT_HELPER_DESCRIPTORJSONHELPERFUNCTIONS_H
#define GUARD_DY_ELEMENT_HELPER_DESCRIPTORJSONHELPERFUNCTIONS_H
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

#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>
#include <Dy/Helper/Type/Color32.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @brief  Get additional type for constructing properties of object.
/// @param  typeString Dependency type string from json.
/// @return Dependency component properties type
///
MDY_NODISCARD EDyComponentMetaType DyGetComponentTypeFrom(_MIN_ const std::string& typeString) noexcept;

///
/// @brief  Get viewport rectangle size from proper jsonAtlas, save it to metaInfo as input value.
/// @param  jsonAtlas
/// @param  metaInfo
/// @TODO SCRIPT THIS
///
void DyGetViewportRectFromJson(_MIN_ const nlohmann::json& jsonAtlas, _MOUT_ DDyCameraMetaInformation& metaInfo);

///
/// @brief Get DDyColor32 RGB color from TU32 24 bit value.
/// @param bit24Value 24bit value, [23...16] R [15...8] G [7...0] B
/// @return RGB Color with alpha 1.0 (always alpha 1.0)
///
MDY_NODISCARD dy::DDyColor32 DyGetRGBColorFromTU32(_MIN_ const TU32 bit24Value) noexcept;

///
/// @brief
/// @param    componentMetaInfo
/// @return
/// @TODO SCRIPT THIS
///
MDY_NODISCARD DDyTransformMetaInformation
CreateTransformMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo);

///
/// @brief
/// @param    componentMetaInfo
/// @return
/// @TODO SCRIPT THIS
///
MDY_NODISCARD PDyScriptComponentMetaInformation
CreateScriptMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo);

///
/// @brief
/// @param    componentMetaInfo
/// @return
/// @TODO SCRIPT THIS
///
MDY_NODISCARD DDyDirectionalLightMetaInformation
CreateDirectionalLightMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo);

///
/// @brief
/// @param
/// @return
/// @TODO SCRIPT THIS
///
MDY_NODISCARD DDyModelFilterMetaInformation
CreateModelFilterMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo);

///
/// @brief
/// @param  componentMetaInfo
/// @return
/// @TODO SCRIPT THIS
///
MDY_NODISCARD DDyModelRendererMetaInformation
CreateModelRendererMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo);

///
/// @brief
/// @param  componentMetaInfo
/// @return
/// @TODO SCRIPT THIS
///
MDY_NODISCARD DDyCameraMetaInformation
CreateCameraMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo);

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_HELPER_DESCRIPTORJSONHELPERFUNCTIONS_H