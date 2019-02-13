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
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Helper/Library/HelperJson.h>
#include "Dy/Helper/StringSwitch.h"
#include "Dy/Helper/Library/HelperFilesystem.h"

namespace dy
{
  
void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyMaterialTextureItem& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ DDyMaterialTextureItem& p)
{
  /* Template
   * {"Specifier": "T_BrickWall1_Diffuse", "DefaultType": "Unknown"},
   */

  DyJsonGetValueFromTo(j, "Specifier", p.mTextureSpecifier);
  p.mTextureMapType = DyJsonGetValueFrom<EDyTextureMapType>(j, "DefaultType");
}

std::string PDyMaterialInstanceMetaInfo::ToString()
{
  auto log = fmt::format(
R"dy(PDyMaterialInstanceMetaInfo
Material Name : {0}
Shader Name : {1}
)dy", this->mSpecifierName, this->mShaderSpecifier);

  //for (const auto& textureName : this->mTextureNames_Deprecated) { log += textureName; }
  return log;
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyMaterialInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyMaterialInstanceMetaInfo& p)
{
  /*  Template
   *  "M_Wall1": 
      {
        "ShaderSpecifier": "Sh_Wall1",
        "TextureSpecifierList": [ 
          {"Specifier": "T_BrickWall1_Diffuse", "DefaultType": "Unknown"},
          {"Specifier": "T_BrickWall1_Normal", "DefaultType": "Unknown"},
          {"Specifier": "T_BrickWall1_Disp", "DefaultType": "Unknown"}
        ]
      },
      "M_nanosuit_Arm": 
      { "ShaderSpecifier": "", "ExternalPath": "./data/Material/M_nanosuit_Arm.json", "IsCompressed": false }
   */

  const auto loadingTypeString = DyJsonGetValueFrom<std::string>(j, "LoadingType");
  switch(DyStrSwitchInput(loadingTypeString))
  {
  case DyStrCase("Internal"):
  {
    DyJsonGetValueFromTo(j, "ShaderSpecifier", p.mShaderSpecifier);
    DyJsonGetValueFromTo(j, "BlendMode",       p.mBlendMode);

    // nlohmann::json does not support serialization between std::array, 
    // so we need to convert list to vector, and convert vector to array again.
    std::vector<DDyMaterialTextureItem> texList;
    DyJsonGetValueFromTo(j, "TextureSpecifierList", texList);
    for (TU32 i = 0, size = static_cast<TU32>(texList.size()); i < size; ++i) { p.mTextureNames[i] = texList[i]; }
  } break;
  case DyStrCase("External"):
  {
    DyJsonGetValueFromTo(j, "ShaderSpecifier", p.mShaderSpecifier);
    // If `IsCompressed` is true, load file from `ExternalPath` and decompress.
    if (j["IsCompressed"].get<bool>() == true)
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
    }
    else
    {
      //
      const auto matPath = j["ExternalPath"].get<std::string>();
      MDY_ASSERT_FORCE(DyFsIsFileExist(matPath) == true, "Material file must be exist on specified path.");

      //
      const auto optJson = DyGetJsonAtlasFromFile(matPath);
      MDY_ASSERT_FORCE(optJson.has_value() == true, "Failed to load extenral material meta information file.");

      //
      {
        const auto& jsonAtlas = optJson.value();
        DyJsonGetValueFromTo(jsonAtlas, "BlendMode",       p.mBlendMode);

        // nlohmann::json does not support serialization between std::array, 
        // so we need to convert list to vector, and convert vector to array again.
        std::vector<DDyMaterialTextureItem> texList;
        DyJsonGetValueFromTo(jsonAtlas, "TextureSpecifierList", texList);
        for (TU32 i = 0, size = static_cast<TU32>(texList.size()); i < size; ++i) { p.mTextureNames[i] = texList[i]; }
      }
    }
  } break;
  case DyStrCase("Binary"):
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: ;
  }


}

} /// ::dy namespace