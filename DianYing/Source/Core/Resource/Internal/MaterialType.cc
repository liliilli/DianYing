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

#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <nlohmann/json.hpp>

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyMaterialBlendMode& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyMaterialBlendMode& p)
{
  const auto enumString = j.get<std::string>();
  if      (j == "Opaque")       { p = EDyMaterialBlendMode::Opaque; }
  else if (j == "Translucent")  { p = EDyMaterialBlendMode::Translucent; }
  else if (j == "Custom")       { p = EDyMaterialBlendMode::Custom; }
  else                          { MDY_UNEXPECTED_BRANCH(); }
}

  DDyMaterialShaderTuple::DDyMaterialShaderTuple(const std::string& shaderName) :
    mShaderName(shaderName),
    mValidShaderPointer(MDyIOResource::GetInstance().GetShaderResource(shaderName))
{ }

DDyMaterialTextureTuple::DDyMaterialTextureTuple(const std::string& textureName) :
    mTextureName(textureName),
    mValidTexturePointer(MDyIOResource::GetInstance().GetTextureResource(textureName))
{ }

} /// ::dy namespace