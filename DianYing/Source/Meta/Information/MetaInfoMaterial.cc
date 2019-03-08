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
#include <Dy/Helper/StringSwitch.h>
#include <Dy/Helper/Library/HelperFilesystem.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Core/Resource/Type/Uniform/UniformValueTypes.h>

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
  auto log = MakeStringU8(
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
      { "ExternalPath": "./data/Material/M_nanosuit_Arm.json", "IsCompressed": false }
   */

  /* External file template.
   *{
      "BlendMode": "Opaque",
      "ShaderSpecifier": "nanosuit_DNS", 
      "TextureSpecifierList": [
        { "DefaultType": "Diffuse", "Specifier": "nanosuit_Body_body_dif" },
        { "DefaultType": "Normal", "Specifier": "nanosuit_Body_body_showroom_ddn" },
        { "DefaultType": "Shininess", "Specifier": "nanosuit_Body_body_showroom_spec" }
      ],
      "UniformValues": {
        "uRoughness": { "Type": "float", "Value": 1.0 },
        ...
      }
    }
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
    // If `IsCompressed` is true, load file from `ExternalPath` and decompress.
    if (j["IsCompressed"].get<bool>() == true)
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
    }
    else
    {
      //
      const auto matPath = j["ExternalPath"].get<std::string>();
      MDY_ASSERT_MSG_FORCE(
        DyFsIsFileExist(matPath) == true, 
        "Material file must be exist on specified path.");

      //
      const auto optJson = DyGetJsonAtlasFromFile(matPath);
      MDY_ASSERT_MSG_FORCE(
        optJson.has_value() == true, 
        "Failed to load extenral material meta information file.");

      //
      const auto& jsonAtlas = optJson.value();
      DyJsonGetValueFromTo(jsonAtlas, "BlendMode", p.mBlendMode);
      DyJsonGetValueFromTo(jsonAtlas, "ShaderSpecifier", p.mShaderSpecifier);

      // nlohmann::json does not support serialization between std::array, 
      // so we need to convert list to vector, and convert vector to array again.
      std::vector<DDyMaterialTextureItem> textureSpecifierList;
      DyJsonGetValueFromTo(jsonAtlas, "TextureSpecifierList", textureSpecifierList);
      for (size_t i = 0, size = textureSpecifierList.size(); i < size; ++i) 
      { 
        p.mTextureNames[i] = textureSpecifierList[i]; 
      }

      // Get "UniformValues" and get chunk of uniform initial values.
      const auto& jsonUniform = jsonAtlas["UniformValues"];
      for (const auto& uniformItem : jsonUniform.items())
      {
        const auto& uniformName = uniformItem.key();
        const auto& itemValue   = uniformItem.value();
        const auto type = DyJsonGetValueFrom<EDyUniformVariableType>(itemValue, "Type");

        switch (type)
        {
        case EDyUniformVariableType::Matrix4: 
        { 
          const auto value = DyJsonGetValueFrom<DDyMatrix4x4>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Matrix4>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Matrix3: 
        {
          MDY_NOT_IMPLEMENTED_ASSERT();
        } break;
        case EDyUniformVariableType::Matrix2:
        {
          MDY_NOT_IMPLEMENTED_ASSERT();
        } break;
        case EDyUniformVariableType::Vector4:
        {
          const auto value = DyJsonGetValueFrom<DDyVector4>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Vector4>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Vector3:
        {
          const auto value = DyJsonGetValueFrom<DDyVector3>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Vector3>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Vector2:
        {
          const auto value = DyJsonGetValueFrom<DDyVector2>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Vector2>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::IVec4: 
        case EDyUniformVariableType::IVec3: 
        case EDyUniformVariableType::IVec2: 
        {
          MDY_NOT_IMPLEMENTED_ASSERT();
        } break;
        case EDyUniformVariableType::Integer:
        {
          const auto value = DyJsonGetValueFrom<TI32>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Integer>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Unsigned:
        {
          const auto value = DyJsonGetValueFrom<TU32>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Unsigned>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Float: 
        {
          const auto value = DyJsonGetValueFrom<TF32>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Float>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Bool:
        {
          const auto value = DyJsonGetValueFrom<bool>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Bool>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Matrix4Array: 
        {
          const auto value = DyJsonGetValueFrom<std::vector<DDyMatrix4x4>>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Matrix4Array>>(-1, value)
          );
        } break;
        case EDyUniformVariableType::Vector3Array: 
        {
          const auto value = DyJsonGetValueFrom<std::vector<DDyVector3>>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<FDyUniformValue<EDyUniformVariableType::Vector3Array>>(-1, value)
          );
        }
        default: MDY_UNEXPECTED_BRANCH(); break;
        }
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