#include <precompiled.h>
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

/// Header files
#include <Dy/Element/Descriptor/LevelDescriptor.h>

#include <bitset>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Type/Color32.h>

//!
//! Local translation unit function & varaible data
//!

namespace
{

///
/// @brief Get DDyColor32 RGB color from TU32 24 bit value.
/// @param bit24Value 24bit value, [23...16] R [15...8] G [7...0] B
/// @return RGB Color with alpha 1.0 (always alpha 1.0)
///
[[nodiscard]] dy::DDyColor32 DyGetRGBColorFromTU32(const TU32 bit24Value) noexcept
{
  std::bitset<24> backgroundColorBit  = bit24Value;
  dy::DDyColor32  background8BitColor = dy::DDyColor32{0, 0, 0, 1};
  {
    TU08 r = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 16 + i;
      r += backgroundColorBit[index] << i;
    }
    background8BitColor.R = r;

    TU08 g = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 8 + i;
      g += backgroundColorBit[index] << i;
    }
    background8BitColor.G = g;

    TU08 b = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = i;
      b += backgroundColorBit[index] << i;
    }
    background8BitColor.B = b;
  }

  return background8BitColor;
}

///
/// @brief  Get FDyObject derived class type helper enumration.
/// @param  typeString Object type string from json.
/// @return Object type
///
[[nodiscard]] dy::EDyFDyObjectType DyGetObjectTypeFrom(const std::string& typeString) noexcept
{
  static constexpr std::string_view sPawn             = "Pawn";
  static constexpr std::string_view sDirectionalLight = "LightDirectional";

  if (typeString == sPawn)              return dy::EDyFDyObjectType::FDyPawn;
  if (typeString == sDirectionalLight)  return dy::EDyFDyObjectType::FDyDirectionalLight;
  else return dy::EDyFDyObjectType::Error;
}

///
/// @brief  Get additional type for constructing properties of object.
/// @param  typeString Dependency type string from json.
/// @return Dependency component properties type
///
[[nodiscard]] dy::EDyCDyComponentType DyGetComponentTypeFrom(const std::string& typeString) noexcept
{
  static constexpr std::string_view sScript           = "Script";
  static constexpr std::string_view sDirLight         = "DirectionalLight";

  if (typeString == sScript)            return dy::EDyCDyComponentType::Script;
  if (typeString == sDirLight)          return dy::EDyCDyComponentType::DirectionalLight;
  else return dy::EDyCDyComponentType::NoneError;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

PDyLevelConstructDescriptor PDyLevelConstructDescriptor::GetDescriptor(const nlohmann::json& jsonAtlas)
{
  PDyLevelConstructDescriptor desc;

  // Make meta information
  // Make color RGB. alpha is always 1.
  const auto& metaAtlas       = jsonAtlas.at("Meta");
  desc.mLevelName             = metaAtlas.at("Name").get<std::string>();
  desc.mLevelBackgroundColor  = DyGetRGBColorFromTU32(metaAtlas.at("Color").get<TU32>());

  // Make object information.
  const auto& objAtlas = jsonAtlas.at("Object");
  for (auto jsonIt = objAtlas.begin(); jsonIt != objAtlas.end(); ++jsonIt)
  {
    DDyObjectInformation objInfo;

    objInfo.mMetaIndex        = static_cast<TI32>(std::distance(objAtlas.begin(), jsonIt));
    objInfo.mType             = DyGetObjectTypeFrom(jsonIt->at("Type").get<std::string>());
    objInfo.mName             = jsonIt->at("Name").get<std::string>();
    objInfo.mHashValue        = jsonIt->at("Hash").get<std::string>();
    objInfo.mParentMetaIndex  = jsonIt->at("Parent").get<TI32>();

    // Get position, rotation, and scale information.
    // @TODO WE NEED SEPERATE WORLD, LOCAL TRANSFORM AND ETC SADLY X_X
    DDyObjectInformation::DDyTransform transform;
    {
      {
        const auto& positionAtlas = jsonIt->at("Position");
        transform.mPosition.X = positionAtlas.at("X").get<TF32>();
        transform.mPosition.Y = positionAtlas.at("Y").get<TF32>();
        transform.mPosition.Z = positionAtlas.at("Z").get<TF32>();
      }
      {
        const auto& rotationAtlas = jsonIt->at("Rotation");
        transform.mRotation.X = rotationAtlas.at("X").get<TF32>();
        transform.mRotation.Y = rotationAtlas.at("Y").get<TF32>();
        transform.mRotation.Z = rotationAtlas.at("Z").get<TF32>();
      }
      {
        const auto& scaleAtlas = jsonIt->at("Scale");
        transform.mScale.X = scaleAtlas.at("X").get<TF32>();
        transform.mScale.Y = scaleAtlas.at("Y").get<TF32>();
        transform.mScale.Z = scaleAtlas.at("Z").get<TF32>();
      }
    }
    objInfo.mTransform = transform;

    // Set dependency information to mDependencyInfo;
    {
      const auto dependency = jsonAtlas.at("Dependency");
      if (const auto it = dependency.find(objInfo.mHashValue); it != dependency.end())
      {
        const auto dependencyInfo = dependency.at(objInfo.mHashValue);
        const auto typeString     = it.value().at("Type").get<std::string>();
        const auto typeEnum       = DyGetComponentTypeFrom(typeString);

        switch (typeEnum)
        {
        case EDyCDyComponentType::Script:
          {
            DDyFdyPawnDependencyInformation DepInfo;
            DepInfo.mType       = typeEnum;
            DepInfo.mBindHashTo = objInfo.mHashValue;
            DepInfo.mScriptPath = "NOT IMPLEMENTED YET";
            objInfo.mDependencyInfo = DepInfo;
          }
          break;
        case EDyCDyComponentType::DirectionalLight:
          {
            DDyFDyDirLightDependencyInformation DepInfo;
            DepInfo.mType       = typeEnum;
            DepInfo.mBindHashTo = objInfo.mHashValue;
            DepInfo.mIntensity  = it.value().at("LightIntensity").get<TF32>();
            /// @TODO mDIRECTION AND mTINTCOLOR
            objInfo.mDependencyInfo = DepInfo;
          }
          break;
        default: PHITOS_UNEXPECTED_BRANCH(); break;
        }
      }
    }

    desc.mLevelObjectInformations.emplace_back(objInfo);
  }

  return desc;
}

} /// ::dy namespace