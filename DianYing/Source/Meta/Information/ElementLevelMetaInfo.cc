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

void GetLevelResourceFromActor(
    _MIN_ const dy::TComponentMetaList& list, 
    _MINOUT_ dy::TDDyResourceNameSet& iSet)
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
    case EDyComponentMetaType::ModelRenderer:
    {
      const auto& desc = std::any_cast<const PDyModelRendererComponentMetaInfo&>(componentInfo);
      if (desc.mDetails.mMaterialName.empty() == true) { break; }

      for (const auto& materialSpecifier : desc.mDetails.mMaterialName)
      {
        if (materialSpecifier.empty() == true) { continue; }
        iSet.emplace(EDyResourceType::Material, materialSpecifier);
      }
    } break;
    default: /* Do nothing */ break;
    }
  }
}

void MoveMetaObjectIntoParentRecursively(
    _MINOUT_ dy::TObjectMetaInfoList& p, 
    _MIN_ const std::vector<std::string>& parentSpecifierList,
    _MIN_ const TU32 level,
    _MINOUT_ dy::TObjectMetaInfoList::value_type& object)
{
  for (auto& parentObject : p)
  { 
    MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(object), "Unexpected error occurred");
    if (parentObject == object)           { continue; }
    if (MDY_CHECK_ISEMPTY(parentObject))  { continue; }

    if (parentObject->mSpecifierName == parentSpecifierList[level])
    { // 
      if (parentSpecifierList.size() == level + 1)
      { // Move it and return.
        parentObject->mChildrenList.emplace_back(std::move(object));
        return;
      }
      else
      { // Call function recursively.
        return MoveMetaObjectIntoParentRecursively(p, parentSpecifierList, level + 1, object);
      }
    }
  }

  MDY_UNEXPECTED_BRANCH();
  return;
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

void to_json(nlohmann::json& j, const TObjectMetaInfoList& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, TObjectMetaInfoList& p)
{
  // (1) Make object list to all object.
  for (auto jsonIt = j.begin(); jsonIt != j.end(); ++jsonIt)
  {
    auto objectMetaInfo = std::make_unique<PDyObjectMetaInfo>(jsonIt.value().get<PDyObjectMetaInfo>());
    p.emplace_back(std::move(objectMetaInfo));
  }

  // (2) Make object list tree.
  for (auto& object : p)
  {
    if (MDY_CHECK_ISEMPTY(object))                                { continue; }
    if (object->mObjectType == EDyMetaObjectType::Actor
    &&  object->mProperties.mParentSpecifierName.empty() == false)
    { // If object type is Actor, and have parents specifier name as dec
      std::vector<std::string> parentSpecifierList = {};
      std::string::size_type startId = 0;
      do
      { // Make parent specifier list. Separation is `.` comma.
        auto endId = object->mProperties.mParentSpecifierName.find_first_of('.', startId);
        if (endId != std::string::npos) 
        { endId = object->mProperties.mParentSpecifierName.length() - startId; }

        parentSpecifierList.emplace_back(object->mProperties.mParentSpecifierName.substr(startId, endId));
        startId = endId;
      }
      while (startId != std::string::npos);

      // Try move object into any parent's children list.
      MoveMetaObjectIntoParentRecursively(p, parentSpecifierList, 0, object);
    }
  }

  // (3) Realign object meta list.
  p.erase( std::remove( p.begin(), p.end(), nullptr ), p.end() );
}

} /// ::dy namespace