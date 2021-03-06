#ifndef GUARD_DYMETA_TYPE_TEXTUREPARAMETER_H
#define GUARD_DYMETA_TYPE_TEXTUREPARAMETER_H
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

/// @enum   EDyGlParameterName
/// @brief  Texture parameter option name.
enum class EDyGlParameterName
{
  TextureMinFilter,
  TextureMagFilter,
  TextureWrappingS,
  TextureWrappingT,
  // Specifies the texture comparison mode for currently bound depth textures. 
  // That is, a texture whose internal format is GL_DEPTH_COMPONENT_*
  TextureCompareMode,
  // Specifies the comparison operator used 
  // when GL_TEXTURE_COMPARE_MODE is set to GL_COMPARE_R(REF)_TO_TEXTURE.  
  TextureCompareFunc,

  NoneError
};

/// @brief Serialization function
void to_json(nlohmann::json& j, const EDyGlParameterName& p);
/// @brief Deserialization function
void from_json(const nlohmann::json& j, EDyGlParameterName& p);
/// @brief Convert plain string value to proper `EDyGlParameterName`. \n
/// If no value is matched, just return NoneError.
MDY_NODISCARD EDyGlParameterName 
DyConvertStringToEDyGlParameterName(const std::string& iString) noexcept;

/// @brief  Get internal plain parameter value from EDyGlParameterName.
/// @param  name EDyGlParameterName
/// @return OpenGL C-plain parameter value.
TGlEnum DyGetTexParameterNameValue(EDyGlParameterName name) noexcept;

/// @enum   EDyGlParameterValue
/// @brief  Texture parameter value name.
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

  CompareRefToTexture,  // TextureCompareMode
  CompareNone,          // TextureCompareMode

  Greater,              // TextureCompareFunc
  GreaterEqual,         // TextureCompareFunc
  Equal,                // TextureCompareFunc
  NotEqual,             // TextureCompareFunc
  LessEqual,            // TextureCompareFunc
  Less,                 // TextureCompareFunc
  Always,               // TextureCompareFunc
  Never,                // TextureCompareFunc

  NoneError
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyGlParameterValue& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyGlParameterValue& p);

/// @brief Convert plain string value to proper `EDyGlParameterValue`. \n
/// If no value is matched, just return NoneError.
MDY_NODISCARD EDyGlParameterValue 
DyConvertStringToEDyGlParameterValue(_MIN_ const std::string& iString) noexcept;
/// @brief  Get internal plain parameter value from EDyGlParameterValue.
/// @param  value EDyGlParameterValue
/// @return OpenGL C-plain parameter value.
TGlEnum DyGetTexParameterValueValue(EDyGlParameterValue value) noexcept;

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
  PDyGlTexParameterInformation(EDyGlParameterName name, EDyGlParameterValue value) :
      mParameterOption(name),
      mParameterValue(value)
  { }
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGlTexParameterInformation& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGlTexParameterInformation& p);

/// @brief Parameter list.
using TTextureParameterList = std::vector<PDyGlTexParameterInformation>;
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const TTextureParameterList& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ TTextureParameterList& p);

/// @brief Check texture parameter item is specified correctly.
/// @return If succeeded, return EDySuccess::DY_SUCCESS otherwise EDySuccess::DY_FAILURE.
MDY_NODISCARD EDySuccess DyCheckTextureParameter(_MIN_ const PDyGlTexParameterInformation& parameter);

/// @brief Check Texture parameter list is specified correctly.
/// @return If succeeded, return EDySuccess::DY_SUCCESS otherwise EDySuccess::DY_FAILURE.
MDY_NODISCARD EDySuccess DyCheckTextureParameterList(_MIN_ const std::vector<PDyGlTexParameterInformation>& parameterList);

} /// ::dy namespace

#endif /// GUARD_DYMETA_TYPE_TEXTUREPARAMETER_H