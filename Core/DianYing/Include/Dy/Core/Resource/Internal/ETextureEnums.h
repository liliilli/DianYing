#ifndef GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H
#define GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H
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

/// @enum ETextureMapType
/// @brief Texture mapping type
enum class ETextureMapType : unsigned char
{
  Diffuse,
  Specular,
  Ambient,
  Emissive,
  Height,
  Normal,
  Shininess,
  Opacity,
  Displacement,
  LightMap,
  Reflection,
  Unknown,      // Unknown is used for other mapping type and customized type.
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const ETextureMapType& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ ETextureMapType& p);

/// @enum ETextureStyleType
/// @brief Texture loading type
enum class ETextureStyleType : unsigned char
{
  NoneError,
  D1,
  D2,
  D2Rectangle, // There is no power-of-two restriction but can not generate mipmaps. UV must be size-scaled float, not texture coordinate.
  D1Array,
  D2Array,
  D2Shadow,
  D2ShadowArray,
  D2Cubemap   // D2Cubemap
};

/// @brief Serialization function.
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const ETextureStyleType& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ ETextureStyleType& p);

/// @brief Get converted low-level gl texture type from wrapping type.
TGlEnum GlGetLowTextureType(ETextureStyleType iType) noexcept;

/// @enum  EGlBufferDataInternalFormat
/// @brief RenderBuffer internal format.
enum class EGlBufferDataInternalFormat : unsigned char
{
  RED8,
  RED32Float,
  RED32UnsignedInt,
  RG8,
  RGB8,
  RGBA8,
  RGBA16Float,
  RGB32Float,
  RGBA32Float,
  DEPTH32,
  NoneError = 0xFF
};

TGlEnum GlGetLowDataFormatType(EGlBufferDataInternalFormat iValue) noexcept;

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H