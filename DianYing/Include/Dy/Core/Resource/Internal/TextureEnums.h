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

/// @enum EDyTextureMapType
/// @brief Texture mapping type
enum class EDyTextureMapType : unsigned char
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
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyTextureMapType& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyTextureMapType& p);

/// @enum EDyTextureStyleType
/// @brief Texture loading type
enum class EDyTextureStyleType : unsigned char
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
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyTextureStyleType& p);
/// @brief Deerialization function.
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyTextureStyleType& p);

/// @brief Get converted low-level gl texture type from wrapping type.
MDY_NODISCARD GLenum DyGLGetLowTextureType(_MIN_ EDyTextureStyleType iType) noexcept;

/// @enum  EDyGlBufferDataInternalFormat
/// @brief RenderBuffer internal format.
enum class EDyGlBufferDataInternalFormat : unsigned char
{
  RED8,
  RG8,
  RGB8,
  RGBA8,
  RGBA16Float,
  RGB32Float,
  RGBA32Float,
  DEPTH32,
  NoneError = 0xFF
};

MDY_NODISCARD GLenum DyGlGetLowDataFormatType(_MIN_ EDyGlBufferDataInternalFormat iValue) noexcept;

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTENRAL_TEXTURE_ENUMS_H