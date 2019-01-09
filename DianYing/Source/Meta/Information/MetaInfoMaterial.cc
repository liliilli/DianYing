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

//!
//! Forward delcaration
//!

//!
//! Implementation
//!

namespace dy
{

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
  /* Template
   * {
        "SpecifierName":
        {
          "SpecifierName": "Name",
          "ShaderSpecifier": "ValidShaderName",
          "BlendMode": "Opaque" / "Transcluent" / "Custom",
          "TextureList":
          [
            "Texture1",
            "Texture2",
            "Texture3",
            // ...
            "Texture16"
          ]
        }
      }
   */

  DyJsonGetValueFromTo(j, "SpecifierName", p.mSpecifierName);
  DyJsonGetValueFromTo(j, "ShaderSpecifier", p.mShaderSpecifier);
  DyJsonGetValueFromTo(j, "BlendMode", p.mBlendMode);
  DyJsonGetValueFromTo(j, "TextureList", p.mTextureNames);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyMaterialInstanceMetaInfo::TTextureList& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyMaterialInstanceMetaInfo::TTextureList& p)
{
  auto id = 0;
  for (const auto& textureItem : j)
  {
    p[id] = textureItem.get<std::string>();
    id++;
  }
}

} /// ::dy namespace