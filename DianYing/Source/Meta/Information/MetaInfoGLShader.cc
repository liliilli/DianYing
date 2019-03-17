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
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include "Dy/Helper/Library/HelperFilesystem.h"

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_Vertex,    "Vertex");
MDY_SET_IMMUTABLE_STRING(sHeader_Hull,      "Hull");
MDY_SET_IMMUTABLE_STRING(sHeader_Domain,    "Domain");
MDY_SET_IMMUTABLE_STRING(sHeader_Geometry,  "Geometry");
MDY_SET_IMMUTABLE_STRING(sHeader_Pixel,     "Pixel");
MDY_SET_IMMUTABLE_STRING(sHeader_Compute,   "Compute");

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyGLShaderInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyGLShaderInstanceMetaInfo& p)
{
  /* Template
   * "Sh_Wall1":
     {
       "Fragments":
       {
         "Vert": { "ShaderCode": "", "ExternalShaderPath": "" },
         "Geom": { "ShaderCode": "", "ExternalShaderPath": "" },
         "Frag": { "ShaderCode": "", "ExternalShaderPath": "" }
       }
     }
   * "Fragments" are "Vert", "Hull", "Domain", "Geom", "Frag", "Comp"
   * If "Comp" is exist, remove all fragments and just read "Comp" as compute shader.
   */

  p.mSourceType = EDyResourceSource::External;

  auto& j_ = const_cast<nlohmann::json&>(j)["Fragments"];

  for (auto it = j_.begin(); it != j_.end(); ++it)
  {
    const auto fragmentType = DyConvertStringToEDyShaderFragmentType(it.key());
    if (fragmentType == decltype(fragmentType)::Compute)
    { // If compute shader, turn on the flag.
      p.mIsComputeShader = true;
    }

    // Read builtin shader code or external file path of shader fragment.
    auto&       refFragment = p.GetFragment(fragmentType);
    const auto& refFragmentItemValue = it.value();
    const auto  compressedShaderCode = json::GetValueFrom<std::string>(refFragmentItemValue, "ShaderCode");
    if (compressedShaderCode.empty() == false)
    { // If builtin shader code is exist, insert it and pass external path.
      refFragment.mBuiltinBuffer = compressedShaderCode;
    }
    else
    { // If builtin shader code is not exist, read external path.
      const auto filePath = json::GetValueFrom<std::string>(refFragmentItemValue, "ExternalShaderPath");
      MDY_ASSERT_MSG_FORCE(
          filePath.empty() == false && IsFileExist(filePath) == true, 
          "External shader fragment file path is not valid.");
      refFragment.mExternalFilePath = filePath;
    }
  }
}

} /// ::dy namespace