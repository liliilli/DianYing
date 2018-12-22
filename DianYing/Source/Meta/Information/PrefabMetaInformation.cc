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

/// Header file
#include <Dy/Meta/Information/PrefabMetaInformation.h>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Library/HelperJson.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Meta/Information/ComponentLightMetaInfo.h>

//!
//! Forward declaration
//!

namespace
{

MDY_SET_IMMUTABLE_STRING(sHeader_SpecifierName,     "SpecifierName");
MDY_SET_IMMUTABLE_STRING(sHeader_UUID,              "UUID");
MDY_SET_IMMUTABLE_STRING(sHeader_CommonProperties,  "CommonProperties");
MDY_SET_IMMUTABLE_STRING(sHeader_Type,              "Type");
MDY_SET_IMMUTABLE_STRING(sHeader_ComponentList,     "ComponentList");

//! PDyPrefabInstanceMetaInfo::DCommonProperties

MDY_SET_IMMUTABLE_STRING(sHeader_ParentSpecifierName,   "ParentSpecifierName");
MDY_SET_IMMUTABLE_STRING(sHeader_ParentSpecifierUUID,   "ParentSpecifierUUID");
MDY_SET_IMMUTABLE_STRING(sHeader_IsInitiallyActivated,  "IsInitiallyActivated");

//! PDyPrefabInstanceMetaInfo::TComponentMetaList

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

std::unique_ptr<PDyPrefabInstanceMetaInfo>
PDyPrefabInstanceMetaInfo::CreateMetaInformation(_MIN_ const nlohmann::json& itemAtlas)
{
  return std::make_unique<PDyPrefabInstanceMetaInfo>(itemAtlas.get<PDyPrefabInstanceMetaInfo>());
}

void to_json(nlohmann::json& j, const PDyPrefabInstanceMetaInfo& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, PDyPrefabInstanceMetaInfo& p)
{
  using TDCommonProperties  = PDyPrefabInstanceMetaInfo::DCommonProperties;

  DyJsonGetValueFromTo(j, sHeader_SpecifierName,    p.mSpecifierName);
  DyJsonGetValueFromTo(j, sHeader_UUID,             p.mUUID);
  DyJsonGetValueFromTo(j, sHeader_Type,             p.mPrefabType);
  DyJsonGetValueFromTo(j, sHeader_CommonProperties, p.mCommonProperties);
  DyJsonGetValueFromTo(j, sHeader_ComponentList,    p.mMetaComponentInfo);
}

void to_json(nlohmann::json& j, const PDyPrefabInstanceMetaInfo::DCommonProperties& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(const nlohmann::json& j, PDyPrefabInstanceMetaInfo::DCommonProperties& p)
{
  DyJsonGetValueFromTo<std::string> (j, sHeader_ParentSpecifierName, p.mParentSpecifierName);
  DyJsonGetValueFromTo<std::string> (j, sHeader_ParentSpecifierUUID, p.mParentUUID);
  DyJsonGetValueFromTo<bool>        (j, sHeader_IsInitiallyActivated, p.mIsInitiallyActivated);
}

} /// ::dy namespace