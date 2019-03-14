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

/// Header files
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Type/ColorRGB.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>

#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Meta/Components/PCompDirLightMetaInfo.h>

//!
//! Local translation unit function & varaible data
//!

namespace
{

//!
//! PDyLevelConstructMetaInfo::Categories
//!

MDY_SET_IMMUTABLE_STRING(sCategoryMeta, "Meta");
MDY_SET_IMMUTABLE_STRING(sCategoryObject, "Object");

//!
//! PDyLevelConstructMetaInfo::Meta
//!

MDY_SET_IMMUTABLE_STRING(sHeader_SpecifierName,               "SpecifierName");
MDY_SET_IMMUTABLE_STRING(sHeader_IsUsingUUIDForSpecification, "IsUsingUUIDForSpecification");
MDY_SET_IMMUTABLE_STRING(sHeader_BackgroundColor,             "BackgroundColor");

/// @brief Set initial level resource from actor component list etc.
void GetLevelResourceFromActor(_MIN_ const dy::TComponentMetaList& list, _MINOUT_ dy::TDDyResourceNameSet& iSet)
{
  using namespace dy;
  for (const auto& [type, componentInfo] : list)
  { // List
    switch (type)
    {
    case EDyComponentMetaType::ModelFilter:
    {
      const auto& desc = std::any_cast<const PDyModelFilterComponentMetaInfo&>(componentInfo);
      if (desc.mDetails.mModelSpecifierName.empty() == true) { break; }
      
      iSet.emplace(EDyResourceType::Model, desc.mDetails.mModelSpecifierName);
    } break;
    case EDyComponentMetaType::ModelAnimator:
    {
      const auto& desc = std::any_cast<const PDyModelAnimatorComponentMetaInfo&>(componentInfo);

      if (desc.mDetails.mSkeletonSpecifier.empty() == false)
      { iSet.emplace(EDyResourceType::Skeleton, desc.mDetails.mSkeletonSpecifier); }

      if (desc.mDetails.mTempAnimationScrap.empty() == false)
      { iSet.emplace(EDyResourceType::AnimationScrap, desc.mDetails.mTempAnimationScrap); }
    } break;
    case EDyComponentMetaType::Skybox:
    {
      const auto& desc = std::any_cast<const PDySkyboxComponentMetaInfo&>(componentInfo);

      if (desc.mDetails.mCubemapSpecifier.empty() == false)
      { iSet.emplace(EDyResourceType::Texture, desc.mDetails.mCubemapSpecifier); }
    } break;
    default: /* Do nothing */ break;
    }
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

TDDyResourceNameSet PDyLevelConstructMetaInfo::GetLevelResourceSet() const noexcept
{
  TDDyResourceNameSet result;
  for (const auto& objectInfo : this->mLevelObjectMetaInfoList)
  {
    const auto type = objectInfo->mObjectType;
    switch (type)
    {
    case EDyMetaObjectType::Actor: 
    {
      GetLevelResourceFromActor(objectInfo->mMetaComponentInfo, result);
    } break;
    case EDyMetaObjectType::SceneScriptor: { MDY_NOT_IMPLEMENTED_ASSERT(); } break;
    case EDyMetaObjectType::Object: { MDY_NOT_IMPLEMENTED_ASSERT(); } break;
    default: MDY_UNEXPECTED_BRANCH();
    }
  }

  return result;
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyLevelConstructMetaInfo& p)
{
  j = nlohmann::json
  {
      {(sCategoryMeta),   p.mMeta},
      {(sCategoryObject), p.mLevelObjectMetaInfoList},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo& p)
{
  DyJsonGetValueFromTo(j, sCategoryMeta,    p.mMeta);
  DyJsonGetValueFromTo(j, sCategoryObject,  p.mLevelObjectMetaInfoList);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyLevelConstructMetaInfo::DMeta& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();

  // @TODO p.mLevelBackgroundColor to DDyColorRGB24.
  j = nlohmann::json
  {
    {(sHeader_BackgroundColor), p.mLevelBackgroundColor},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo::DMeta& p)
{
  DyJsonGetValueFromTo(j, sHeader_BackgroundColor, p.mLevelBackgroundColor);
}

} /// ::dy namespace