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
#include <Dy/Meta/Information/ElementObjectMetaInfo.h>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Helper/Library/HelperRegex.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Management/MSetting.h>

//!
//! Forward declaration & local translation unit data
//!

namespace
{

//!
//! PDyObjectMetaInfo::DCommonProperties
//!

MDY_SET_IMMUTABLE_STRING(sHeader_Object_Name,         "Name");
MDY_SET_IMMUTABLE_STRING(sHeader_Object_Type,         "Type");

MDY_SET_IMMUTABLE_STRING(sHeader_Object_CommonProp,   "CommonProperties");
MDY_SET_IMMUTABLE_STRING(sHeader_Object_ComponentList,"ComponentList");

//!
//! PDyObjectMetaInfo::DCommonProperties
//!

MDY_SET_IMMUTABLE_STRING(sHeader_Prop_ParentName,     "ParentSpecifierName");
MDY_SET_IMMUTABLE_STRING(sHeader_Prop_Activated,      "IsInitiallyActivated");
MDY_SET_IMMUTABLE_STRING(sHeader_Prop_IsFromPrefab,   "IsFromPrefab");
MDY_SET_IMMUTABLE_STRING(sHeader_Prop_PrefabName,     "PrefabSpecifierName");

void MoveMetaObjectIntoParentRecursively(
    _MINOUT_ dy::TObjectMetaInfoList& p, 
    _MIN_ const std::vector<std::string>& parentSpecifierList,
    _MIN_ const TU32 level,
    _MINOUT_ dy::TObjectMetaInfoList::value_type& object)
{
  for (auto& parentObject : p)
  { 
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(object), "Unexpected error occurred");
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
        return MoveMetaObjectIntoParentRecursively(parentObject->mChildrenList, parentSpecifierList, level + 1, object);
      }
    }
  }

  MDY_UNEXPECTED_BRANCH();
  return;
}

} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyObjectMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyObjectMetaInfo& p)
{
  json::GetValueFromTo(j, sHeader_Object_Name,          p.mSpecifierName);
  json::GetValueFromTo(j, sHeader_Object_Type,          p.mObjectType);
  json::GetValueFromTo(j, sHeader_Object_CommonProp,    p.mProperties);
  json::GetValueFromTo(j, sHeader_Object_ComponentList, p.mMetaComponentInfo);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyObjectMetaInfo::DCommonProperties& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyObjectMetaInfo::DCommonProperties& p)
{
  json::GetValueFromTo(j, sHeader_Prop_ParentName,  p.mParentSpecifierName);
  json::GetValueFromTo(j, sHeader_Prop_PrefabName,  p.mPrefabSpecifierName);
  json::GetValueFromTo(j, sHeader_Prop_Activated,   p.mInitialActivated);
  json::GetValueFromTo(j, sHeader_Prop_IsFromPrefab,p.mIsUsingPrefab);
  json::GetValueFromTo(j, "ObjectTag",              p.mTagSpecifier);
  json::GetValueFromTo(j, "IsOverridePrefabTag",    p.mIsOverridePrefabTag);

  /// Validity Test
  MDY_CALL_ASSERT_SUCCESS(MSetting::GetInstance().MDY_PRIVATE(CheckObjectTagIsExist)(p.mTagSpecifier));
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
    if (MDY_CHECK_ISEMPTY(object)) { continue; }
    if (object->mObjectType == EDyMetaObjectType::Actor
    &&  object->mProperties.mParentSpecifierName.empty() == false)
    { // If object type is Actor, and have parents specifier name as dec
      const auto list = regex::CreateObjectParentSpecifierList(object->mProperties.mParentSpecifierName);
      MoveMetaObjectIntoParentRecursively(p, list, 0, object);
    }
  }

  // (3) Realign object meta list.
  EraseRemove(p, nullptr);
}

} /// ::dy namespace