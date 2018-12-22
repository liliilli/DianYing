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
#include <Dy/Meta/Information/ElementLevelMetaInfo.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Type/ColorRGB24.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>

#include <Dy/Meta/Information/ComponentMetaInformation.h>
#include <Dy/Meta/Information/ComponentLightMetaInfo.h>

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

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyLevelConstructMetaInfo& p)
{
  j = nlohmann::json
  {
      {MSVSTR(sCategoryMeta),   p.mMetaCategory},
      {MSVSTR(sCategoryObject), p.mLevelObjectMetaInfoList},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo& p)
{
  DyJsonGetValueFromTo(j, sCategoryMeta,    p.mMetaCategory);
  DyJsonGetValueFromTo(j, sCategoryObject,  p.mLevelObjectMetaInfoList);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyLevelConstructMetaInfo::DMeta& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();

  // @TODO p.mLevelBackgroundColor to DDyColorRGB24.
  j = nlohmann::json
  {
    {MSVSTR(sHeader_SpecifierName),               p.mLevelName},
    {MSVSTR(sHeader_IsUsingUUIDForSpecification), p.mIsUsingUUIDForSpecification},
    {MSVSTR(sHeader_BackgroundColor),             p.mLevelBackgroundColor},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyLevelConstructMetaInfo::DMeta& p)
{
  DyJsonGetValueFromTo(j, sHeader_SpecifierName,                p.mLevelName);
  DyJsonGetValueFromTo(j, sHeader_IsUsingUUIDForSpecification,  p.mIsUsingUUIDForSpecification);
  p.mLevelBackgroundColor = DyJsonGetValueFrom<DDyColorRGB24>(j, sHeader_BackgroundColor);
}

void to_json(nlohmann::json& j, const PDyLevelConstructMetaInfo::TObjectMetaInfoList& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, PDyLevelConstructMetaInfo::TObjectMetaInfoList& p)
{
  // (1) Make object list.
  PDyLevelConstructMetaInfo::TObjectMetaInfoList tempList;
  for (auto jsonIt = j.begin(); jsonIt != j.end(); ++jsonIt)
  {
    auto objectMetaInfo = std::make_unique<PDyObjectMetaInfo>(jsonIt.value().get<PDyObjectMetaInfo>());
    tempList.emplace_back(std::move(objectMetaInfo));
  }

  // (2) Make object list tree.
  // @TODO ONLY USE ParentSpecifierName to specification.
  for (auto& object : tempList)
  {
    if (MDY_CHECK_ISEMPTY(object))                                { continue; }
    if (object->mProperties.mParentSpecifierName.empty() == true) { continue; }

    const auto parentSpecifierName = object->mProperties.mParentSpecifierName;
    for (auto& parentObject : tempList)
    {
      if (MDY_CHECK_ISEMPTY(parentObject)) { continue; }
      if (parentObject->mSpecifierName == parentSpecifierName)
      {
        parentObject->mChildrenList.emplace_back(std::move(object));
        break;
      }
    }
  }

  // (3) Realign object meta list.
  tempList.erase( std::remove( tempList.begin(), tempList.end(), nullptr ), tempList.end() );
  p = std::move(tempList);
}

} /// ::dy namespace