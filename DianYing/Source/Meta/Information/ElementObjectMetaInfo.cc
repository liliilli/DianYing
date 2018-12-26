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
#include <Dy/Meta/Information/ElementObjectMetaInfo.h>
#include <Dy/Helper/Library/HelperJson.h>

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
  ///
  /// @brief
  /// @param validComponentAtlas
  /// @param desc
  ///
  static auto GetMetaComponentInformation = [](_MIN_ const auto& validComponentAtlas, _MOUT_ PDyObjectMetaInfo& desc)
  {
    for (const auto& componentMetaInfo : validComponentAtlas)
    {
      const auto typeEnum = DyJsonGetValueFrom<EDyComponentMetaType>(componentMetaInfo, sHeader_Type);
      switch (typeEnum)
      {
      default: MDY_UNEXPECTED_BRANCH(); break;
      case EDyComponentMetaType::Transform: // Create and insert CDyTransform meta information descriptor.
        desc.mMetaComponentInfo.emplace_back(typeEnum, componentMetaInfo.template get<PDyTransformComponentMetaInfo>());
        break;
      case EDyComponentMetaType::Script:    // Create and insert CDyScript meta information descriptor.
        desc.mMetaComponentInfo.emplace_back(typeEnum, componentMetaInfo.template get<PDyScriptComponentMetaInfo>());
        break;
      case EDyComponentMetaType::DirectionalLight:  // Create and insert CDyDirectionalLight meta information descriptor.
        desc.mMetaComponentInfo.emplace_back(typeEnum, componentMetaInfo.template get<PDyDirLightComponentMetaInfo>());
        break;
      case EDyComponentMetaType::ModelFilter:
        desc.mMetaComponentInfo.emplace_back(typeEnum, componentMetaInfo.template get<PDyModelFilterComponentMetaInfo>());
        break;
      case EDyComponentMetaType::ModelRenderer:
        desc.mMetaComponentInfo.emplace_back(typeEnum, componentMetaInfo.template get<PDyModelRendererComponentMetaInfo>());
        break;
      case EDyComponentMetaType::Camera:
        desc.mMetaComponentInfo.emplace_back(typeEnum, componentMetaInfo.template get<PDyCameraComponentMetaInfo>());
        break;
      }
    }
  };

  DyJsonGetValueFromTo(j, sHeader_Object_Name,          p.mSpecifierName);
  DyJsonGetValueFromTo(j, sHeader_Object_Type,          p.mObjectType);
  DyJsonGetValueFromTo(j, sHeader_Object_CommonProp,    p.mProperties);
  DyJsonGetValueFromTo(j, sHeader_Object_ComponentList, p.mMetaComponentInfo);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const PDyObjectMetaInfo::DCommonProperties& p)
{
  MDY_NOT_IMPLEMENTED_ASSERT();
}

void from_json(_MIN_ const nlohmann::json& j, _MINOUT_ PDyObjectMetaInfo::DCommonProperties& p)
{
  DyJsonGetValueFromTo(j, sHeader_Prop_ParentName,  p.mParentSpecifierName);
  DyJsonGetValueFromTo(j, sHeader_Prop_PrefabName,  p.mPrefabSpecifierName);
  DyJsonGetValueFromTo(j, sHeader_Prop_Activated,   p.mInitialActivated);
  DyJsonGetValueFromTo(j, sHeader_Prop_IsFromPrefab,p.mIsUsingPrefab);
}

} /// ::dy namespace