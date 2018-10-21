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
#include <Dy/Element/Helper/DescriptorHelperFunctions.h>

#include <bitset>
#include <nlohmann/json.hpp>

#include <Dy/Helper/JsonHelper.h>
#include <Dy/Element/Helper/DescriptorComponentHeaderString.h>

namespace dy
{

MDY_NODISCARD dy::EDyComponentMetaType DyGetComponentTypeFrom(_MIN_ const std::string& typeString) noexcept
{
  static MDY_SET_IMMUTABLE_STRING(sDirectionalLight,  "DirectionalLight");
  static MDY_SET_IMMUTABLE_STRING(sScript,            "Script");
  static MDY_SET_IMMUTABLE_STRING(sTransform,         "Transform");
  static MDY_SET_IMMUTABLE_STRING(sModelFilter,       "ModelFilter");
  static MDY_SET_IMMUTABLE_STRING(sModelRenderer,     "ModelRenderer");
  static MDY_SET_IMMUTABLE_STRING(sCamera,            "Camera");

  if (typeString == sScript)           { return dy::EDyComponentMetaType::Script; }
  if (typeString == sDirectionalLight) { return dy::EDyComponentMetaType::DirectionalLight; }
  if (typeString == sTransform)        { return dy::EDyComponentMetaType::Transform; }
  if (typeString == sModelFilter)      { return dy::EDyComponentMetaType::ModelFilter; }
  if (typeString == sModelRenderer)    { return dy::EDyComponentMetaType::ModelRenderer; }
  if (typeString == sCamera)           { return dy::EDyComponentMetaType::Camera; }
  else                                 { return dy::EDyComponentMetaType::NoneError; }
}

void DyGetViewportRectFromJson(_MIN_ const nlohmann::json& jsonAtlas, _MOUT_ DDyCameraMetaInformation& metaInfo)
{
  // Calculate
  dy::DDyVector2 viewportRectXY = {};
  viewportRectXY.X = jsonAtlas.at("X").get<TF32>();
  viewportRectXY.Y = jsonAtlas.at("Y").get<TF32>();

  dy::DDyVector2 viewportRectWH = {};
  viewportRectWH.X = jsonAtlas.at("W").get<TF32>();
  viewportRectWH.Y = jsonAtlas.at("H").get<TF32>();

  // Update value.
  metaInfo.mViewportSizeXY = viewportRectXY;
  metaInfo.mViewportSizeWH = viewportRectWH;
}


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

DDyTransformMetaInformation CreateTransformMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo)
{
  DDyTransformMetaInformation transformMeta;

  transformMeta.mType           = EDyComponentMetaType::Transform;
  transformMeta.mLocalPosition  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalPosition)));
  transformMeta.mLocalRotation  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalAngle)));
  transformMeta.mLocalScale     = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLocalScale)));

  transformMeta.mWorldPosition  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldPosition)));
  transformMeta.mWorldRotation  = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldAngle)));
  transformMeta.mWorldScale     = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderWorldScale)));

  return transformMeta;
};

PDyScriptComponentMetaInformation CreateScriptMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo)
{
  PDyScriptComponentMetaInformation scriptMeta;
  scriptMeta.mType                  = EDyComponentMetaType::Script;
  scriptMeta.mScriptSpecifierName   = DyGetValue<std::string>(componentMetaInfo, sHeaderName);
  scriptMeta.mInitiallyActivated    = DyGetValue<bool>(componentMetaInfo, sHeaderActivated);

  return scriptMeta;
};

DDyDirectionalLightMetaInformation CreateDirectionalLightMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo)
{
  DDyDirectionalLightMetaInformation meta;
  meta.mType        = EDyComponentMetaType::DirectionalLight;
  meta.mDirection   = DyGetDDyVector3FromJson(componentMetaInfo.at(MSVSTR(sHeaderLightDirection)));
  meta.mIntensity   = DyGetValue<TF32>(componentMetaInfo, sHeaderLightIntensity);
  meta.mTintColor   = DyGetRGBColorFromTU32(componentMetaInfo.at(MSVSTR(sHeaderLightTintColor)).get<TU32>());
  meta.mInitiallyActivated = DyGetValue<bool>(componentMetaInfo, sHeaderActivated);

  return meta;
};

DDyModelFilterMetaInformation CreateModelFilterMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo)
{
  DDyModelFilterMetaInformation modelFilterMeta;
  modelFilterMeta.mType               = EDyComponentMetaType::ModelFilter;
  modelFilterMeta.mModelName          = DyGetValue<std::string>(componentMetaInfo, sHeaderModelName);
  modelFilterMeta.mInitiallyActivated = DyGetValue<bool>(componentMetaInfo, sHeaderActivated);

  return modelFilterMeta;
}

DDyModelRendererMetaInformation CreateModelRendererMetaInfo(_MIN_ const nlohmann::json& componentMetaInfo)
{
  DDyModelRendererMetaInformation modelRendererMeta;
  modelRendererMeta.mType                   = EDyComponentMetaType::ModelRenderer;
  modelRendererMeta.mIsEnabledCreateShadow  = DyGetValue<bool>(componentMetaInfo, sHeaderShadow);
  modelRendererMeta.mMaterialName           = DyGetValue<std::vector<std::string>>(componentMetaInfo, sHeaderMaterials);
  modelRendererMeta.mInitiallyActivated     = DyGetValue<bool>(componentMetaInfo, sHeaderActivated);

  return modelRendererMeta;
}

DDyCameraMetaInformation CreateCameraMetaInfo(const nlohmann::json& componentMetaInfo)
{
  DDyCameraMetaInformation cameraMeta;
  cameraMeta.mType                = EDyComponentMetaType::Camera;
  cameraMeta.mNear                = DyGetValue<TF32>(componentMetaInfo, sHeaderClippingNear);
  cameraMeta.mFar                 = DyGetValue<TF32>(componentMetaInfo, sHeaderClippingFar);
  cameraMeta.mInitiallyActivated  = DyGetValue<bool>(componentMetaInfo, sHeaderActivated);
  cameraMeta.mIsFocusInstantly    = DyGetValue<bool>(componentMetaInfo, sHeaderIsFocusInstantly);
  cameraMeta.mInitialFieldOfView  = DyGetValue<TF32>(componentMetaInfo, sHeaderFieldOfView);
  DyGetViewportRectFromJson(componentMetaInfo.at(MSVSTR(sHeaderViewportRect)), cameraMeta);

  const auto cameraModeString = DyGetValue<std::string>(componentMetaInfo, sHeaderProjection);
  if (cameraModeString == sValuePerspective)  { cameraMeta.mIsOrthographic = false; }
  else                                        { cameraMeta.mIsOrthographic = true; }

  return cameraMeta;
}

} /// ::dy namespace