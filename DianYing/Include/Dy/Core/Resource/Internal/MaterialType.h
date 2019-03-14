#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
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

#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Element/Interface/IDyToString.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyShaderResource_Deprecated;
class CDyTextureResource_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @enum EDyMaterialBlendMode
/// @brief Specify material's blend mode. The way of rendering will be changed by this value.
///
enum class EDyMaterialBlendMode
{
  // Material will be forward to deferred rendering (pass 2)
  Opaque,
  // Material will be forward to forwarding rendering afterward deferred rendering (pass 3)
  TranslucentOIT,
  // I dont know but will be used later. (pass x)
  Custom,
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const EDyMaterialBlendMode& p);
void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ EDyMaterialBlendMode& p);

///
/// @struct DDyMaterialShaderTuple
/// @brief Shader tuple for binding to material resource.
///
struct DDyMaterialShaderTuple final
{
  std::string                           mShaderName         = "";
  CDyShaderResource_Deprecated*                    mValidShaderPointer = nullptr;

  DDyMaterialShaderTuple(const std::string& shaderName);
  DDyMaterialShaderTuple() noexcept = default;;
};

///
/// @struct DDyMaterialTextureTuple
/// @brief Texture tuple for binding to material resource.
///
struct DDyMaterialTextureTuple final
{
  std::string                           mTextureName          = "";
  CDyTextureResource_Deprecated*                   mValidTexturePointer  = nullptr;

  DDyMaterialTextureTuple(const std::string& textureName);
};

///
/// @struct PDyMaterialResourceDescriptor
/// @brief Resource material construction descriptor.
///
struct PDyMaterialResourceDescriptor final
{
  std::string                             mMaterialName     = "";
  DDyMaterialShaderTuple                  mShaderTuple      = {};
  std::vector<DDyMaterialTextureTuple>    mTextureTuples;

  EDyMaterialBlendMode                    mBlendMode        = EDyMaterialBlendMode::Opaque;
};

///
/// @struct PDyMaterialPopulateDescriptor
/// @brief Material descriptor for populating from base material.
/// Populated material will be automatically remove by being removed original base material.
///
struct PDyMaterialPopulateDescriptor final
{
  std::string                           mMaterialOverrideName   = "";
  std::string                           mOverrideShaderName     = "";
  std::vector<std::string>              mTextureOverrideNames   = {};

  bool                                  mIsEnabledMaterialCustomNameOverride  = false;
  bool                                  mIsEnabledShaderOverride              = false;
  bool                                  mIsEnabledTextureOverride             = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H