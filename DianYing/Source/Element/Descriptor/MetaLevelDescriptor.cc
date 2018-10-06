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
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Component/Descriptor/ComponentMetaDescriptor.h>

#include <bitset>
#include <nlohmann/json.hpp>
#include <Dy/Helper/Type/Color32.h>
#include "Dy/Component/CDyModelFilter.h"
#include "Dy/Component/CDyModelRenderer.h"

//!
//! Local translation unit function & varaible data
//!

namespace
{

//!
//! Categories
//!

MDY_SET_IMMUTABLE_STRING(sCategoryMeta, "Meta");
MDY_SET_IMMUTABLE_STRING(sCategoryObject, "Object");
MDY_SET_IMMUTABLE_STRING(sCategoryComponent, "Component");

//!
//! Common headers
//!

MDY_SET_IMMUTABLE_STRING(sHeaderName,         "Name");
MDY_SET_IMMUTABLE_STRING(sHeaderColor,        "Color");
MDY_SET_IMMUTABLE_STRING(sHeaderType,         "Type");
MDY_SET_IMMUTABLE_STRING(sHeaderObjectHash,   "Hash");
MDY_SET_IMMUTABLE_STRING(sHeaderToComponent,  "Component");
MDY_SET_IMMUTABLE_STRING(sHeaderParentHash,   "Parent");
MDY_SET_IMMUTABLE_STRING(sHeaderIsInitiallyActivated, "Activated");

//!
//! CDyTransform
//!

MDY_SET_IMMUTABLE_STRING(sHeaderLocalPosition,"LPosition");
MDY_SET_IMMUTABLE_STRING(sHeaderLocalAngle,   "LRotation");
MDY_SET_IMMUTABLE_STRING(sHeaderLocalScale,   "LScale");
MDY_SET_IMMUTABLE_STRING(sHeaderWorldPosition,"WPosition");
MDY_SET_IMMUTABLE_STRING(sHeaderWorldAngle,   "WRotation");
MDY_SET_IMMUTABLE_STRING(sHeaderWorldScale,   "WScale");

//!
//! CDyScript
//!

MDY_SET_IMMUTABLE_STRING(sHeaderScriptPath,   "Path");

//!
//! CDyDirectionalLight
//!

MDY_SET_IMMUTABLE_STRING(sHeaderLightDirection, "LightDirection");
MDY_SET_IMMUTABLE_STRING(sHeaderLightIntensity, "LightIntensity");
MDY_SET_IMMUTABLE_STRING(sHeaderLightTintColor, "LightTintColor");

//!
//! CDyModelFilter
//!

MDY_SET_IMMUTABLE_STRING(sHeaderModelName,      "ModelName");

//!
//! CDyModelRenderer
//!

MDY_SET_IMMUTABLE_STRING(sHeaderShadow,         "Shadow");
MDY_SET_IMMUTABLE_STRING(sHeaderMaterials,      "Materials");

//!
//! Functions
//!

///
/// @brief Get DDyColor32 RGB color from TU32 24 bit value.
/// @param bit24Value 24bit value, [23...16] R [15...8] G [7...0] B
/// @return RGB Color with alpha 1.0 (always alpha 1.0)
///
MDY_NODISCARD dy::DDyColor32 DyGetRGBColorFromTU32(_MIN_ const TU32 bit24Value) noexcept
{
  std::bitset<24> backgroundColorBit  = bit24Value;
  dy::DDyColor32  background8BitColor = dy::DDyColor32{0, 0, 0, 1};
  {
    TU08 r = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 16 + i;
      r += backgroundColorBit[index] << i;
    }
    background8BitColor.R = r;

    TU08 g = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = 8 + i;
      g += backgroundColorBit[index] << i;
    }
    background8BitColor.G = g;

    TU08 b = 0;
    for (TI32 i = 7; i >= 0; --i)
    {
      const auto index = i;
      b += backgroundColorBit[index] << i;
    }
    background8BitColor.B = b;
  }

  return background8BitColor;
}

///
/// @brief  Get FDyObject derived class type helper enumration.
/// @param  typeString Object type string from json.
/// @return Object type
///
MDY_NODISCARD dy::EDyMetaObjectType DyGetObjectTypeFrom(_MIN_ const std::string& typeString) noexcept
{
  static MDY_SET_IMMUTABLE_STRING(sActor,           "Actor");
  static MDY_SET_IMMUTABLE_STRING(sObject,          "Object");
  static MDY_SET_IMMUTABLE_STRING(sSceneScriptor,   "SceneScriptor");

  if (typeString == sActor)         { return dy::EDyMetaObjectType::Actor; }
  if (typeString == sObject)        { return dy::EDyMetaObjectType::Object; }
  if (typeString == sSceneScriptor) { return dy::EDyMetaObjectType::SceneScriptor; }
  else                              { return dy::EDyMetaObjectType::NoneError; }
}

///
/// @brief  Get additional type for constructing properties of object.
/// @param  typeString Dependency type string from json.
/// @return Dependency component properties type
///
MDY_NODISCARD dy::EDyComponentMetaType DyGetComponentTypeFrom(_MIN_ const std::string& typeString) noexcept
{
  static MDY_SET_IMMUTABLE_STRING(sDirectionalLight,  "DirectionalLight");
  static MDY_SET_IMMUTABLE_STRING(sScript,            "Script");
  static MDY_SET_IMMUTABLE_STRING(sTransform,         "Transform");
  static MDY_SET_IMMUTABLE_STRING(sModelFilter,       "ModelFilter");
  static MDY_SET_IMMUTABLE_STRING(sModelRenderer,     "ModelRenderer");

  if (typeString == sScript)           { return dy::EDyComponentMetaType::Script; }
  if (typeString == sDirectionalLight) { return dy::EDyComponentMetaType::DirectionalLight; }
  if (typeString == sTransform)        { return dy::EDyComponentMetaType::Transform; }
  if (typeString == sModelFilter)      { return dy::EDyComponentMetaType::ModelFilter; }
  if (typeString == sModelRenderer)    { return dy::EDyComponentMetaType::ModelRenderer; }
  else                                 { return dy::EDyComponentMetaType::NoneError; }
}

///
/// @brief  Exceptionable.
/// @param  jsonAtlas
/// @param  name
/// @return
/// @TODO SCRIPT THIS.
///
template <typename TReturnType, typename TParam1>
MDY_NODISCARD TReturnType DyGetValue(_MIN_ const TParam1& jsonAtlas, _MIN_ const std::string_view& name)
{
  return jsonAtlas.at(MSVSTR(name)).template get<TReturnType>();
}

///
/// @brief  Exceptionable.
/// @param  jsonAtlas
/// @return
/// @TODO SCRIPT THIS.
///
MDY_NODISCARD dy::DDyVector3 DyGetDDyVector3FromJson(_MIN_ const nlohmann::json& jsonAtlas)
{
  dy::DDyVector3 vector;
  vector.X = jsonAtlas.at("X").get<TF32>();
  vector.Y = jsonAtlas.at("Y").get<TF32>();
  vector.Z = jsonAtlas.at("Z").get<TF32>();

  return vector;
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

PDyLevelConstructDescriptor PDyLevelConstructDescriptor::CreateDescriptor(_MIN_ const nlohmann::json& jsonAtlas)
{
  ///
  /// @function GetMetaInformation
  /// @brief Make meta information
  /// @param metaAtlas
  /// @param desc
  /// @TODO SCRIPT THIS
  ///
  static auto GetMetaInformation = [](_MIN_ const nlohmann::json& metaAtlas, _MOUT_ PDyLevelConstructDescriptor& desc)
  {
    desc.mLevelName             = metaAtlas.at(sHeaderName.data()).get<std::string>();
    // Make color RGB. alpha is always 1.
    desc.mLevelBackgroundColor  = DyGetRGBColorFromTU32(metaAtlas.at(MSVSTR(sHeaderColor)).get<TU32>());
  };

  ///
  /// @function GetMetaComponentInformation
  /// @brief
  /// @param validComponentAtlas
  /// @param desc
  /// @TODO SCRIPT THIS
  ///
  static auto GetMetaComponentInformation = [](_MIN_ const auto& validComponentAtlas, _MOUT_ DDyObjectInformation& desc)
  {
    for (const auto& componentMetaInfo : validComponentAtlas)
    {
      const auto typeEnum = DyGetComponentTypeFrom(DyGetValue<std::string>(componentMetaInfo, sHeaderType));

      switch (typeEnum)
      {
      default: PHITOS_UNEXPECTED_BRANCH(); break;
      case EDyComponentMetaType::Transform:         // Create and insert CDyTransform meta information descriptor.
      {
        DDyTransformMetaInformation transformMeta;
        transformMeta.mType           = typeEnum;
        transformMeta.mBindHashTo     = desc.mHashValue;

        transformMeta.mLocalPosition  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalPosition)));
        transformMeta.mLocalRotation  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalAngle)));
        transformMeta.mLocalScale     = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalScale)));

        transformMeta.mWorldPosition  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldPosition)));
        transformMeta.mWorldRotation  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldAngle)));
        transformMeta.mWorldScale     = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldScale)));

        desc.mMetaComponentInfo.emplace_back(transformMeta.mType, transformMeta);
      } break;
      case EDyComponentMetaType::Script:            // Create and insert CDyScript meta information descriptor.
      {
        DDyScriptMetaInformation scriptMeta;
        scriptMeta.mType              = typeEnum;
        scriptMeta.mScriptName        = DyGetValue<std::string>(componentMetaInfo, sHeaderName);
        scriptMeta.mBindHashTo        = desc.mHashValue;
        scriptMeta.mScriptPath        = DyGetValue<std::string>(componentMetaInfo, sHeaderScriptPath);
        scriptMeta.mInitiallyActivated= DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

        desc.mMetaComponentInfo.emplace_back(scriptMeta.mType, scriptMeta);
      } break;
      case EDyComponentMetaType::DirectionalLight:  // Create and insert CDyDirectionalLight meta information descriptor.
      {
        DDyDirectionalLightMetaInformation DirLightMeta;
        DirLightMeta.mType            = typeEnum;
        DirLightMeta.mBindHashTo      = desc.mHashValue;
        DirLightMeta.mDirection       = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLightDirection)));
        DirLightMeta.mIntensity       = DyGetValue<TF32>(componentMetaInfo, sHeaderLightIntensity);
        DirLightMeta.mTintColor       = DyGetRGBColorFromTU32(componentMetaInfo.at(MSVSTR(sHeaderLightTintColor)).get<TU32>());
        DirLightMeta.mInitiallyActivated = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

        desc.mMetaComponentInfo.emplace_back(DirLightMeta.mType, DirLightMeta);
      } break;
      case EDyComponentMetaType::ModelFilter:
      {
        DDyModelFilterMetaInformation modelFilterMeta;
        modelFilterMeta.mType         = typeEnum;
        modelFilterMeta.mModelName    = DyGetValue<std::string>(componentMetaInfo, sHeaderModelName);
        modelFilterMeta.mInitiallyActivated = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

        desc.mMetaComponentInfo.emplace_back(modelFilterMeta.mType, modelFilterMeta);
      } break;
      case EDyComponentMetaType::ModelRenderer:
      {
        DDyModelRendererMetaInformation modelRendererMeta;
        modelRendererMeta.mType                   = typeEnum;
        modelRendererMeta.mIsEnabledCreateShadow  = DyGetValue<bool>(componentMetaInfo, sHeaderShadow);
        modelRendererMeta.mMaterialName           = DyGetValue<std::vector<std::string>>(componentMetaInfo, sHeaderMaterials);
        modelRendererMeta.mInitiallyActivated     = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

        desc.mMetaComponentInfo.emplace_back(modelRendererMeta.mType, modelRendererMeta);
      } break;
      }
    }
  };

  // FunctionBody ∨

  PDyLevelConstructDescriptor desc;
  GetMetaInformation(jsonAtlas.at(MSVSTR(sCategoryMeta)), desc);

  // Make object information.
  const auto& objAtlas = jsonAtlas.at(MSVSTR(sCategoryObject));
  for (auto jsonIt = objAtlas.begin(); jsonIt != objAtlas.end(); ++jsonIt)
  {
    DDyObjectInformation objInfo;

    objInfo.mMetaIndex        = static_cast<TI32>(std::distance(objAtlas.begin(), jsonIt));
    objInfo.mObjectType       = DyGetObjectTypeFrom(DyGetValue<std::string>(*jsonIt, sHeaderType));
    objInfo.mObjectName       = DyGetValue<std::string>(*jsonIt, sHeaderName);
    objInfo.mHashValue        = DyGetValue<std::string>(*jsonIt, sHeaderObjectHash);
    objInfo.mParentHashValue  = DyGetValue<std::string>(*jsonIt, sHeaderParentHash);
    objInfo.mToComponentHash  = DyGetValue<std::string>(*jsonIt, sHeaderToComponent);
    objInfo.mInitialActivated = DyGetValue<bool>(*jsonIt, sHeaderIsInitiallyActivated);

    // Set component meta dependency information to mMetaComponentInfo;
    const auto componentMap = jsonAtlas.at(MSVSTR(sCategoryComponent));
    if (const auto it = componentMap.find(objInfo.mToComponentHash); it != componentMap.end())
    {
      GetMetaComponentInformation(it.value(), objInfo);
    }

    desc.mLevelObjectInformations.emplace_back(objInfo);
  }

  return desc;
}

} /// ::dy namespace