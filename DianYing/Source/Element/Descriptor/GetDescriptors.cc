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

namespace dy
{

PDyLevelConstructDescriptor PDyLevelConstructDescriptor::GetDescriptor(const nlohmann::json& jsonAtlas)
{
  PDyLevelConstructDescriptor desc;

  // Make meta information
  const auto& metaAtlas   = jsonAtlas.at("Meta");
  desc.mLevelName         = metaAtlas.at("Name").get<std::string>();

  std::bitset<24> backgroundColorBit  = metaAtlas.at("Color").get<TU32>();
  DDyColor32      background8BitColor = DDyColor32{0, 0, 0, 1};

  { // Make color RGB. alpha is always 1.
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
  desc.mLevelBackgroundColor = background8BitColor;

  // Make object information.
  const auto& objAtlas = jsonAtlas.at("Object");
  for (auto jsonIt = objAtlas.begin(); jsonIt != objAtlas.end(); ++jsonIt)
  {
    PDyLevelConstructDescriptor::DDyObjectInformation objInfo;

    objInfo.mMetaIndex  = static_cast<TI32>(std::distance(objAtlas.begin(), jsonIt));
    objInfo.mName       = jsonIt->at("Name").get<std::string>();
    objInfo.mType       = jsonIt->at("Type").get<std::string>();
    objInfo.mParentMetaIndex = jsonIt->at("Parent").get<TI32>();

    // Get position, rotation, and scale information.
    // @TODO WE NEED SEPERATE WORLD, LOCAL TRANSFORM AND ETC SADLY X_X
    {
      const auto& positionAtlas = jsonIt->at("Position");
      objInfo.mPosition.X = positionAtlas.at("X").get<TF32>();
      objInfo.mPosition.Y = positionAtlas.at("Y").get<TF32>();
      objInfo.mPosition.Z = positionAtlas.at("Z").get<TF32>();
    }
    {
      const auto& rotationAtlas = jsonIt->at("Rotation");
      objInfo.mRotation.X = rotationAtlas.at("X").get<TF32>();
      objInfo.mRotation.Y = rotationAtlas.at("Y").get<TF32>();
      objInfo.mRotation.Z = rotationAtlas.at("Z").get<TF32>();
    }
    {
      const auto& scaleAtlas = jsonIt->at("Scale");
      objInfo.mScale.X = scaleAtlas.at("X").get<TF32>();
      objInfo.mScale.Y = scaleAtlas.at("Y").get<TF32>();
      objInfo.mScale.Z = scaleAtlas.at("Z").get<TF32>();
    }

    desc.mLevelObjectInformations.emplace_back(objInfo);
  }

  return desc;
}

} /// ::dy namespace