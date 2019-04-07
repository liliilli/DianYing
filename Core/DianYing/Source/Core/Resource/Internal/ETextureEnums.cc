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
#include <Dy/Core/Resource/Internal/ETextureEnums.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Internal/XStringSwitch.h>
#include <Dy/Include/GlInclude.h>

namespace dy
{
  
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const ETextureMapType& p)
{
  std::string serializedValue;
  switch (p)
  {
  case ETextureMapType::Diffuse:  serializedValue = "Diffuse";  break;
  case ETextureMapType::Specular: serializedValue = "Specular"; break;
  case ETextureMapType::Ambient:  serializedValue = "Ambient";  break;
  case ETextureMapType::Emissive: serializedValue = "Emissive"; break;
  case ETextureMapType::Height:   serializedValue = "Height";   break;
  case ETextureMapType::Normal:   serializedValue = "Normal";   break;
  case ETextureMapType::Shininess:  serializedValue = "Shiniess"; break;
  case ETextureMapType::Opacity:    serializedValue = "Opacity";  break;
  case ETextureMapType::Displacement: serializedValue = "Displacement"; break;
  case ETextureMapType::LightMap:     serializedValue = "LightMap";     break;
  case ETextureMapType::Reflection:   serializedValue = "Reflection";   break;
  case ETextureMapType::Unknown:      serializedValue = "Unknown";      break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  j = {"DefaultType", serializedValue};
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ ETextureMapType& p)
{
  const auto serializedString = j.get<std::string>();
  switch (string::Input(serializedString))
  {
  case string::Case("Diffuse"):  p = ETextureMapType::Diffuse; break;
  case string::Case("Specular"): p = ETextureMapType::Specular; break;
  case string::Case("Ambient"):  p = ETextureMapType::Ambient; break;
  case string::Case("Emissive"): p = ETextureMapType::Emissive; break;
  case string::Case("Height"):   p = ETextureMapType::Height; break;
  case string::Case("Normal"):   p = ETextureMapType::Normal; break;
  case string::Case("Shininess"):p = ETextureMapType::Shininess; break;
  case string::Case("Opacity"):  p = ETextureMapType::Opacity; break;
  case string::Case("Displacement"): p = ETextureMapType::Displacement; break;
  case string::Case("LightMap"):   p = ETextureMapType::LightMap; break;
  case string::Case("Reflection"): p = ETextureMapType::Reflection; break;
  case string::Case("Unknown"):    p = ETextureMapType::Unknown; break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const ETextureStyleType& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ ETextureStyleType& p)
{
  const auto& deserializedStringValue = j.get_ref<const std::string&>();
  if (deserializedStringValue == "D1")                  { p = ETextureStyleType::D1; }
  else if (deserializedStringValue == "D2")             { p = ETextureStyleType::D2; }
  else if (deserializedStringValue == "D2Rectangle")    { p = ETextureStyleType::D2Rectangle; }
  else if (deserializedStringValue == "D1Array")        { p = ETextureStyleType::D1Array; }
  else if (deserializedStringValue == "D2Array")        { p = ETextureStyleType::D2Array; }
  else if (deserializedStringValue == "D2Shadow")       { p = ETextureStyleType::D2Shadow; }
  else if (deserializedStringValue == "D2ShadowArray")  { p = ETextureStyleType::D2ShadowArray; }
  else if (deserializedStringValue == "D2Cubemap")   { p = ETextureStyleType::D2Cubemap; }
  else { MDY_UNEXPECTED_BRANCH(); }
}

TGlEnum GlGetLowTextureType(ETextureStyleType iType) noexcept
{
  switch (iType)
  {
  case ETextureStyleType::D1:             return GL_TEXTURE_1D;
  case ETextureStyleType::D2:             return GL_TEXTURE_2D; 
  case ETextureStyleType::D2Rectangle:    return GL_TEXTURE_RECTANGLE;
  case ETextureStyleType::D2Shadow:       return GL_TEXTURE_2D;
  case ETextureStyleType::D1Array:        return GL_TEXTURE_1D_ARRAY;
  case ETextureStyleType::D2Array:        return GL_TEXTURE_2D_ARRAY;
  case ETextureStyleType::D2ShadowArray:  return GL_TEXTURE_2D_ARRAY;
  case ETextureStyleType::D2Cubemap:      return GL_TEXTURE_CUBE_MAP;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

TGlEnum GlGetLowDataFormatType(EGlBufferDataInternalFormat iValue) noexcept
{
  switch (iValue)
  {
  case EGlBufferDataInternalFormat::RED8:   return GL_R8;
  case EGlBufferDataInternalFormat::RED32Float: return GL_R32F;
  case EGlBufferDataInternalFormat::RED32UnsignedInt: return GL_R32UI;
  case EGlBufferDataInternalFormat::RG8:    return GL_RG8;
  case EGlBufferDataInternalFormat::RGB8:   return GL_RGB8;
  case EGlBufferDataInternalFormat::RGBA8:  return GL_RGBA8;
  case EGlBufferDataInternalFormat::RGBA16Float: return GL_RGBA16F;
  case EGlBufferDataInternalFormat::RGB32Float: return GL_RGB32F;
  case EGlBufferDataInternalFormat::RGBA32Float:return GL_RGBA32F;
  case EGlBufferDataInternalFormat::DEPTH32:return GL_DEPTH_COMPONENT32;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(GL_NONE);
  }
}

} /// ::dy namespace