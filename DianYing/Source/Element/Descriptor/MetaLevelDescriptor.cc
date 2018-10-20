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
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Helper/JsonHelper.h>
#include <Dy/Element/Helper/DescriptorHelperFunctions.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>

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

MDY_SET_IMMUTABLE_STRING(sHeaderColor,                "Color");
MDY_SET_IMMUTABLE_STRING(sHeaderType,                 "Type");
MDY_SET_IMMUTABLE_STRING(sHeaderObjectHash,           "Hash");
MDY_SET_IMMUTABLE_STRING(sHeaderToComponent,          "Component");
MDY_SET_IMMUTABLE_STRING(sHeaderParentHash,           "Parent");
MDY_SET_IMMUTABLE_STRING(sHeaderIsFromPrefab,         "IsFromPrefab");
MDY_SET_IMMUTABLE_STRING(sHeaderPrefabSpecifierName,  "PrefabSpecifierName");

//!
//! Functions
//!

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
      default: MDY_UNEXPECTED_BRANCH(); break;
      case EDyComponentMetaType::Transform:         // Create and insert CDyTransform meta information descriptor.
      {
        const auto meta = CreateTransformMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::Script:            // Create and insert CDyScript meta information descriptor.
      {
        const auto meta = CreateScriptMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::DirectionalLight:  // Create and insert CDyDirectionalLight meta information descriptor.
      {
        const auto meta = CreateDirectionalLightMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::ModelFilter:
      {
        const auto meta = CreateModelFilterMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::ModelRenderer:
      {
        const auto meta = CreateModelRendererMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::Camera:
      {
        const auto meta = CreateCameraMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
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
    objInfo.mObjectType       = DyGetMetaObjectTypeFrom(DyGetValue<std::string>(*jsonIt, sHeaderType));
    objInfo.mObjectName       = DyGetValue<std::string>(*jsonIt, sHeaderName);
    objInfo.mHashValue        = DyGetValue<std::string>(*jsonIt, sHeaderObjectHash);
    objInfo.mParentHashValue  = DyGetValue<std::string>(*jsonIt, sHeaderParentHash);
    objInfo.mToComponentHash  = DyGetValue<std::string>(*jsonIt, sHeaderToComponent);
    objInfo.mInitialActivated = DyGetValue<bool>(*jsonIt, sHeaderActivated);

    objInfo.mIsUsingPrefab    = DyGetValue<bool>(*jsonIt, sHeaderIsFromPrefab);
    if (objInfo.mIsUsingPrefab == true)
    { // If using prefab, do not create component meta information.
      objInfo.mPrefabSpecifierName = DyGetValue<std::string>(*jsonIt, sHeaderPrefabSpecifierName);
    }
    else
    { // Set component meta dependency information to mMetaComponentInfo;
      const auto componentMap = jsonAtlas.at(MSVSTR(sCategoryComponent));
      if (const auto it = componentMap.find(objInfo.mToComponentHash); it != componentMap.end())
      {
        GetMetaComponentInformation(it.value(), objInfo);
      }
    }

    desc.mLevelObjectInformations.emplace_back(objInfo);
  }

  return desc;
}

} /// ::dy namespace