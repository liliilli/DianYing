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
#include <Dy/Helper/Internal/XStringSwitch.h>
#include <Dy/Helper/Library/HelperFilesystem.h>
#include <Dy/Helper/Type/DMatrix4x4.h>
#include <Dy/Core/Resource/Type/Uniform/TUniformValue.h>
#include <Dy/Helper/Library/HelperString.h>

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

  json::GetValueFromTo(j, "Specifier", p.mTextureSpecifier);
  p.mTextureMapType = json::GetValueFrom<ETextureMapType>(j, "DefaultType");
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

  const auto loadingTypeString = json::GetValueFrom<std::string>(j, "LoadingType");
  switch(SwitchStrInput(loadingTypeString))
  {
  case CaseStr("Internal"):
  {
    json::GetValueFromTo(j, "ShaderSpecifier", p.mShaderSpecifier);
    json::GetValueFromTo(j, "BlendMode",       p.mBlendMode);

    // nlohmann::json does not support serialization between std::array, 
    // so we need to convert list to vector, and convert vector to array again.
    std::vector<DDyMaterialTextureItem> texList;
    json::GetValueFromTo(j, "TextureSpecifierList", texList);
    for (TU32 i = 0, size = static_cast<TU32>(texList.size()); i < size; ++i) { p.mTextureNames[i] = texList[i]; }
  } break;
  case CaseStr("External"):
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
        IsFileExist(matPath) == true, 
        "Material file must be exist on specified path.");

      //
      const auto optJson = json::GetAtlasFromFile(matPath);
      MDY_ASSERT_MSG_FORCE(
        optJson.has_value() == true, 
        "Failed to load extenral material meta information file.");

      //
      const auto& jsonAtlas = optJson.value();
      json::GetValueFromTo(jsonAtlas, "BlendMode", p.mBlendMode);
      json::GetValueFromTo(jsonAtlas, "ShaderSpecifier", p.mShaderSpecifier);

      // nlohmann::json does not support serialization between std::array, 
      // so we need to convert list to vector, and convert vector to array again.
      std::vector<DDyMaterialTextureItem> textureSpecifierList;
      json::GetValueFromTo(jsonAtlas, "TextureSpecifierList", textureSpecifierList);
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
        const auto type = json::GetValueFrom<EUniformVariableType>(itemValue, "Type");

        switch (type)
        {
        case EUniformVariableType::Matrix4: 
        { 
          const auto value = json::GetValueFrom<DMat4>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Matrix4>>(-1, value)
          );
        } break;
        case EUniformVariableType::Matrix3: 
        {
          MDY_NOT_IMPLEMENTED_ASSERT();
        } break;
        case EUniformVariableType::Matrix2:
        {
          MDY_NOT_IMPLEMENTED_ASSERT();
        } break;
        case EUniformVariableType::Vector4:
        {
          const auto value = json::GetValueFrom<DVec4>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Vector4>>(-1, value)
          );
        } break;
        case EUniformVariableType::Vector3:
        {
          const auto value = json::GetValueFrom<DVec3>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Vector3>>(-1, value)
          );
        } break;
        case EUniformVariableType::Vector2:
        {
          const auto value = json::GetValueFrom<DVec2>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Vector2>>(-1, value)
          );
        } break;
        case EUniformVariableType::IVec4: 
        case EUniformVariableType::IVec3: 
        case EUniformVariableType::IVec2: 
        {
          MDY_NOT_IMPLEMENTED_ASSERT();
        } break;
        case EUniformVariableType::Integer:
        {
          const auto value = json::GetValueFrom<TI32>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Integer>>(-1, value)
          );
        } break;
        case EUniformVariableType::Unsigned:
        {
          const auto value = json::GetValueFrom<TU32>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Unsigned>>(-1, value)
          );
        } break;
        case EUniformVariableType::Float: 
        {
          const auto value = json::GetValueFrom<TF32>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Float>>(-1, value)
          );
        } break;
        case EUniformVariableType::Bool:
        {
          const auto value = json::GetValueFrom<bool>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Bool>>(-1, value)
          );
        } break;
        case EUniformVariableType::Matrix4Array: 
        {
          const auto value = json::GetValueFrom<std::vector<DMat4>>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Matrix4Array>>(-1, value)
          );
        } break;
        case EUniformVariableType::Vector3Array: 
        {
          const auto value = json::GetValueFrom<std::vector<DVec3>>(itemValue, "Value");
          p.mUniformValues.try_emplace(
            uniformName,
            std::make_unique<TUniformValue<EUniformVariableType::Vector3Array>>(-1, value)
          );
        }
        default: MDY_UNEXPECTED_BRANCH(); break;
        }
      }
    }
  } break;
  case CaseStr("Binary"):
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  default: ;
  }


}

} /// ::dy namespace