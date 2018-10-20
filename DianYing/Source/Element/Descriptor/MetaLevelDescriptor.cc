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

MDY_SET_IMMUTABLE_STRING(sHeaderName,         "Name");
MDY_SET_IMMUTABLE_STRING(sHeaderColor,        "Color");
MDY_SET_IMMUTABLE_STRING(sHeaderType,         "Type");
MDY_SET_IMMUTABLE_STRING(sHeaderObjectHash,   "Hash");
MDY_SET_IMMUTABLE_STRING(sHeaderToComponent,  "Component");
MDY_SET_IMMUTABLE_STRING(sHeaderParentHash,   "Parent");
MDY_SET_IMMUTABLE_STRING(sHeaderIsInitiallyActivated, "Activated");

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
    ///
    /// @brief
    /// @capture  desc
    /// @param    componentMetaInfo
    /// @return
    ///
    static auto CreateTransformMetaInfo = [&desc](_MIN_ const auto& componentMetaInfo) -> DDyTransformMetaInformation
    {
      DDyTransformMetaInformation transformMeta;

      transformMeta.mType           = EDyComponentMetaType::Transform;
      transformMeta.mBindHashTo     = desc.mHashValue;

      transformMeta.mLocalPosition  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalPosition)));
      transformMeta.mLocalRotation  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalAngle)));
      transformMeta.mLocalScale     = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalScale)));

      transformMeta.mWorldPosition  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldPosition)));
      transformMeta.mWorldRotation  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldAngle)));
      transformMeta.mWorldScale     = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldScale)));

      return transformMeta;
    };

    ///
    /// @brief
    /// @capture  desc
    /// @param    componentMetaInfo
    /// @return
    ///
    static auto CreateScriptMetaInfo    = [&desc](_MIN_ const auto& componentMetaInfo) -> PDyScriptComponentMetaInformation
    {
      PDyScriptComponentMetaInformation scriptMeta;
      scriptMeta.mType                  = EDyComponentMetaType::Script;
      scriptMeta.mScriptSpecifierName   = DyGetValue<std::string>(componentMetaInfo, sHeaderName);
      scriptMeta.mBindHashTo            = desc.mHashValue;
      scriptMeta.mInitiallyActivated    = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

      return scriptMeta;
    };

    ///
    /// @brief
    /// @capture  desc
    /// @param    componentMetaInfo
    /// @return
    ///
    static auto CreateDirectionalLightMetaInfo = [&desc](_MIN_ const auto& componentMetaInfo) -> DDyDirectionalLightMetaInformation
    {
      DDyDirectionalLightMetaInformation meta;
      meta.mType        = EDyComponentMetaType::DirectionalLight;
      meta.mBindHashTo  = desc.mHashValue;
      meta.mDirection   = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLightDirection)));
      meta.mIntensity   = DyGetValue<TF32>(componentMetaInfo, sHeaderLightIntensity);
      meta.mTintColor   = DyGetRGBColorFromTU32(componentMetaInfo.at(MSVSTR(sHeaderLightTintColor)).template get<TU32>());
      meta.mInitiallyActivated = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

      return meta;
    };

    ///
    /// @brief
    /// @capture
    /// @param
    /// @return
    ///
    static auto CreateModelFilterMetaInfo = [](_MIN_ const auto& componentMetaInfo) -> DDyModelFilterMetaInformation
    {
      DDyModelFilterMetaInformation modelFilterMeta;
      modelFilterMeta.mType               = EDyComponentMetaType::ModelFilter;
      modelFilterMeta.mModelName          = DyGetValue<std::string>(componentMetaInfo, sHeaderModelName);
      modelFilterMeta.mInitiallyActivated = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);

      return modelFilterMeta;
    };

    /// FunctionBody ∨

    for (const auto& componentMetaInfo : validComponentAtlas)
    {
      const auto typeEnum = DyGetComponentTypeFrom(DyGetValue<std::string>(componentMetaInfo, sHeaderType));

      switch (typeEnum)
      {
      default: MDY_UNEXPECTED_BRANCH(); break;
      case EDyComponentMetaType::Transform:         // Create and insert CDyTransform meta information descriptor.
      {
        const DDyTransformMetaInformation meta = CreateTransformMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::Script:            // Create and insert CDyScript meta information descriptor.
      {
        const PDyScriptComponentMetaInformation meta = CreateScriptMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::DirectionalLight:  // Create and insert CDyDirectionalLight meta information descriptor.
      {
        const DDyDirectionalLightMetaInformation meta = CreateDirectionalLightMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
      } break;
      case EDyComponentMetaType::ModelFilter:
      {
        const DDyModelFilterMetaInformation meta = CreateModelFilterMetaInfo(componentMetaInfo);
        desc.mMetaComponentInfo.emplace_back(meta.mType, meta);
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
      case EDyComponentMetaType::Camera:
      {
        DDyCameraMetaInformation cameraMeta;
        cameraMeta.mType                = typeEnum;
        //
        cameraMeta.mInitialFieldOfView  = DyGetValue<TF32>(componentMetaInfo, sHeaderFieldOfView);
        DyGetViewportRectFromJson(componentMetaInfo.at(MSVSTR(sHeaderViewportRect)), cameraMeta);

        //
        const auto cameraModeString = DyGetValue<std::string>(componentMetaInfo, sHeaderProjection);
        if (cameraModeString == sValuePerspective)  { cameraMeta.mIsOrthographic = false; }
        else                                        { cameraMeta.mIsOrthographic = true; }
        //
        cameraMeta.mNear                = DyGetValue<TF32>(componentMetaInfo, sHeaderClippingNear);
        cameraMeta.mFar                 = DyGetValue<TF32>(componentMetaInfo, sHeaderClippingFar);
        cameraMeta.mInitiallyActivated  = DyGetValue<bool>(componentMetaInfo, sHeaderIsInitiallyActivated);
        cameraMeta.mIsFocusInstantly    = DyGetValue<bool>(componentMetaInfo, sHeaderIsFocusInstantly);

        desc.mMetaComponentInfo.emplace_back(cameraMeta.mType, cameraMeta);
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