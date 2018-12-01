#ifndef GUARD_DYMETA_TYPE_TEXTUREPARAMETER_H
#define GUARD_DYMETA_TYPE_TEXTUREPARAMETER_H
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

#include <nlohmann/json_fwd.hpp>

namespace dy
{

///
/// @enum   EDyGlParameterName
/// @brief  Texture parameter option name.
///
enum class EDyGlParameterName
{
  TextureMinFilter,
  TextureMagFilter,
  TextureWrappingS,
  TextureWrappingT,

  NoneError
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlParameterName& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlParameterName& p);

///
/// @brief  Get internal plain parameter value from EDyGlParameterName.
/// @param  name EDyGlParameterName
/// @return OpenGL C-plain parameter value.
///
MDY_NODISCARD GLenum DyGetParameterNameValue(_MIN_ const EDyGlParameterName name) noexcept;

///
/// @enum   EDyGlParameterValue
/// @brief  Texture parameter value name.
///
enum class EDyGlParameterValue
{
  Nearest,
  Linear,
  NearestMipmapNearest,
  LinearMipmapNearest,
  NearestMipmapLinear,
  LinearMipmapLinear,

  ClampToEdge,
  ClampToBorder,
  Repeat,
  MirroredRepeat,

  NoneError
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlParameterValue& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlParameterValue& p);

///
/// @brief  Get internal plain parameter value from EDyGlParameterValue.
/// @param  value EDyGlParameterValue
/// @return OpenGL C-plain parameter value.
///
MDY_NODISCARD GLenum DyGetParameterValueValue(_MIN_ const EDyGlParameterValue value) noexcept;

///
/// @struct PDyGlTexParameterInformation
/// @brief  OpenGL texture paramater option information container. \n
/// This type must be default constructible, so I let it have unnecessary default ctor.
///
struct PDyGlTexParameterInformation final
{
  EDyGlParameterName  mParameterOption = EDyGlParameterName::NoneError;
  EDyGlParameterValue mParameterValue  = EDyGlParameterValue::NoneError;

  PDyGlTexParameterInformation() = default;
  PDyGlTexParameterInformation(_MIN_ EDyGlParameterName name, _MIN_ EDyGlParameterValue value) :
      mParameterOption(name),
      mParameterValue(value)
  { }
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGlTexParameterInformation& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGlTexParameterInformation& p);

///
/// @brief
/// @param  parameter
/// @return
///
MDY_NODISCARD EDySuccess DyCheckTextureParameter(_MIN_ const PDyGlTexParameterInformation& parameter);

///
/// @brief
/// @param  parameterList
/// @return
///
MDY_NODISCARD EDySuccess DyCheckTextureParameterList(_MIN_ const std::vector<PDyGlTexParameterInformation>& parameterList);

} /// ::dy namespace

#endif /// GUARD_DYMETA_TYPE_TEXTUREPARAMETER_H