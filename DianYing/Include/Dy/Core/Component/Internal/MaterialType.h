#ifndef GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
#define GUARD_DY_CORE_COMPONENT_INTERNAL_MATERIAL_TYPE_H
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

#include <vector>

//!
//! Forward declaration
//!

namespace dy
{
class CDyShaderResource;
class CDyTextureResource;
}

//!
//! Implementation
//!

namespace dy
{

///
/// @enum EDyMaterialBlendMode
/// @brief
///
enum class EDyMaterialBlendMode
{
  Opaque,
  Translucent,
  Custom,
};

///
/// @struct PDyMaterialConstructionDescriptor
/// @brief
///
struct PDyMaterialConstructionDescriptor final
{
  std::string                           mMaterialName     = "";
  std::string                           mShaderName       = "";
  std::vector<std::string>              mTextureNames;
  EDyMaterialBlendMode                  mBlendMode        = EDyMaterialBlendMode::Opaque;
  bool                                  mIsShaderLazyInitialized = false;
};

struct DDyMaterialShaderTuple final
{
  std::string                           mShaderName       = "";
  CDyShaderResource*                    mShaderPointer    = nullptr;
};

struct DDyMaterialTextureTuple final
{
  std::string                           mTextureName      = "";
  CDyTextureResource*                   mTexturePointer   = nullptr;
};

///
/// @struct PDyMaterialResourceDescriptor
/// @brief
///
struct PDyMaterialResourceDescriptor final
{
  std::string                           mMaterialName     = "";
  DDyMaterialShaderTuple                mShaderTuple      = {};
  std::vector<DDyMaterialTextureTuple>  mTextureTuples;

  EDyMaterialBlendMode                  mBlendMode        = EDyMaterialBlendMode::Opaque;
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