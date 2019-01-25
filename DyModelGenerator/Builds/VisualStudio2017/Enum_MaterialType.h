#pragma once
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

#include <nlohmann/json.hpp>
#include "juce_core/system/juce_PlatformDefs.h"
#include <assimp/material.h>

///
/// @enum EDyMaterialBlendMode
/// @brief Specify material's blend mode. The way of rendering will be changed by this value.
///
enum class EDyMaterialBlendMode
{
  // Material will be forward to deferred rendering (pass 2)
  Opaque,
  // Material will be forward to forwarding rendering afterward deferred rendering (pass 3)
  TranslucentOIT,
  // I dont know but will be used later. (pass x)
  Custom,
};

inline void to_json(nlohmann::json& j, const EDyMaterialBlendMode& p)
{
  std::string serializedValue;
  switch (p)
  {
  case EDyMaterialBlendMode::Opaque:          serializedValue = "Opaque"; break;
  case EDyMaterialBlendMode::TranslucentOIT:  serializedValue = "TranslucentOIT"; break;
  case EDyMaterialBlendMode::Custom:          serializedValue = "Custom"; break;
  default: jassert(false); break;
  }
  j = {"BlendMode", serializedValue};
}

inline void from_json(const nlohmann::json& j, EDyMaterialBlendMode& p)
{
  (void)j;
  (void)p;
}

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
  Unknown,
};

[[nodiscard]] inline EDyTextureMapType ConvertAiTextureTypeToDyType(aiTextureType iType)
{
  switch (iType)
  {
  case aiTextureType_DIFFUSE:   return EDyTextureMapType::Diffuse;
  case aiTextureType_SPECULAR:  return EDyTextureMapType::Specular;
  case aiTextureType_AMBIENT:   return EDyTextureMapType::Ambient;
  case aiTextureType_EMISSIVE:  return EDyTextureMapType::Emissive;
  case aiTextureType_HEIGHT:    return EDyTextureMapType::Height;
  case aiTextureType_NORMALS:   return EDyTextureMapType::Normal;
  case aiTextureType_SHININESS: return EDyTextureMapType::Shininess;
  case aiTextureType_OPACITY:   return EDyTextureMapType::Opacity;
  case aiTextureType_DISPLACEMENT:  return EDyTextureMapType::Displacement;
  case aiTextureType_LIGHTMAP:      return EDyTextureMapType::LightMap;
  case aiTextureType_REFLECTION:    return EDyTextureMapType::Reflection;
  case aiTextureType_UNKNOWN:       return EDyTextureMapType::Unknown;
  default: return EDyTextureMapType::Unknown; 
  }
}

inline void to_json(nlohmann::json& j, const EDyTextureMapType& p)
{
  std::string serializedValue;
  switch (p)
  {
  case EDyTextureMapType::Diffuse:  serializedValue = "Diffuse";  break;
  case EDyTextureMapType::Specular: serializedValue = "Specular"; break;
  case EDyTextureMapType::Ambient:  serializedValue = "Ambient";  break;
  case EDyTextureMapType::Emissive: serializedValue = "Emissive"; break;
  case EDyTextureMapType::Height:   serializedValue = "Height";   break;
  case EDyTextureMapType::Normal:   serializedValue = "Normal";   break;
  case EDyTextureMapType::Shininess:  serializedValue = "Shiniess"; break;
  case EDyTextureMapType::Opacity:    serializedValue = "Opacity";  break;
  case EDyTextureMapType::Displacement: serializedValue = "Displacement"; break;
  case EDyTextureMapType::LightMap:     serializedValue = "LightMap";     break;
  case EDyTextureMapType::Reflection:   serializedValue = "Reflection";   break;
  case EDyTextureMapType::Unknown:      serializedValue = "Unknown";      break;
  default: jassert(false); break;
  }
  j = {"DefaultType", serializedValue};
}

inline void from_json(const nlohmann::json& j, EDyTextureMapType& p)
{
  (void)j;
  (void)p;
}
