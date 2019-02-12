#include <precompiled.h>
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

/// Header file
#include <Dy/Core/Resource/Internal/TextureEnums.h>
#include <nlohmann/json.hpp>
#include "Dy/Helper/StringSwitch.h"

namespace dy
{
  
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyTextureMapType& p)
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
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  j = {"DefaultType", serializedValue};
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyTextureMapType& p)
{
  const auto serializedString = j.get<std::string>();
  switch (DyStrSwitchInput(serializedString))
  {
  case DyStrCase("Diffuse"):  p = EDyTextureMapType::Diffuse; break;
  case DyStrCase("Specular"): p = EDyTextureMapType::Specular; break;
  case DyStrCase("Ambient"):  p = EDyTextureMapType::Ambient; break;
  case DyStrCase("Emissive"): p = EDyTextureMapType::Emissive; break;
  case DyStrCase("Height"):   p = EDyTextureMapType::Height; break;
  case DyStrCase("Normal"):   p = EDyTextureMapType::Normal; break;
  case DyStrCase("Shininess"):p = EDyTextureMapType::Shininess; break;
  case DyStrCase("Opacity"):  p = EDyTextureMapType::Opacity; break;
  case DyStrCase("Displacement"): p = EDyTextureMapType::Displacement; break;
  case DyStrCase("LightMap"):   p = EDyTextureMapType::LightMap; break;
  case DyStrCase("Reflection"): p = EDyTextureMapType::Reflection; break;
  case DyStrCase("Unknown"):    p = EDyTextureMapType::Unknown; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyTextureStyleType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyTextureStyleType& p)
{
  const auto& deserializedStringValue = j.get_ref<const std::string&>();
  if (deserializedStringValue == "D1")                  { p = EDyTextureStyleType::D1; }
  else if (deserializedStringValue == "D2")             { p = EDyTextureStyleType::D2; }
  else if (deserializedStringValue == "D2Rectangle")    { p = EDyTextureStyleType::D2Rectangle; }
  else if (deserializedStringValue == "D1Array")        { p = EDyTextureStyleType::D1Array; }
  else if (deserializedStringValue == "D2Array")        { p = EDyTextureStyleType::D2Array; }
  else if (deserializedStringValue == "D2Shadow")       { p = EDyTextureStyleType::D2Shadow; }
  else if (deserializedStringValue == "D2ShadowArray")  { p = EDyTextureStyleType::D2ShadowArray; }
  else if (deserializedStringValue == "D2Cubemap")   { p = EDyTextureStyleType::D2Cubemap; }
  else { MDY_UNEXPECTED_BRANCH(); }
}

GLenum DyGLGetLowTextureType(_MIN_ EDyTextureStyleType iType) noexcept
{
  switch (iType)
  {
  case EDyTextureStyleType::D1:             return GL_TEXTURE_1D;
  case EDyTextureStyleType::D2:             return GL_TEXTURE_2D; 
  case EDyTextureStyleType::D2Rectangle:    return GL_TEXTURE_RECTANGLE;
  case EDyTextureStyleType::D2Shadow:       return GL_TEXTURE_2D;
  case EDyTextureStyleType::D1Array:        return GL_TEXTURE_1D_ARRAY;
  case EDyTextureStyleType::D2Array:        return GL_TEXTURE_2D_ARRAY;
  case EDyTextureStyleType::D2ShadowArray:  return GL_TEXTURE_2D_ARRAY;
  case EDyTextureStyleType::D2Cubemap:      return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

GLenum DyGlGetLowDataFormatType(_MIN_ EDyGlBufferDataInternalFormat iValue) noexcept
{
  switch (iValue)
  {
  case EDyGlBufferDataInternalFormat::RED8:   return GL_R8;
  case EDyGlBufferDataInternalFormat::RG8:    return GL_RG8;
  case EDyGlBufferDataInternalFormat::RGB8:   return GL_RGB8;
  case EDyGlBufferDataInternalFormat::RGBA8:  return GL_RGBA8;
  case EDyGlBufferDataInternalFormat::RGBA16Float: return GL_RGBA16F;
  case EDyGlBufferDataInternalFormat::RGB32Float: return GL_RGB32F;
  case EDyGlBufferDataInternalFormat::RGBA32Float:return GL_RGBA32F;
  case EDyGlBufferDataInternalFormat::DEPTH32:return GL_DEPTH_COMPONENT32;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

} /// ::dy namespace