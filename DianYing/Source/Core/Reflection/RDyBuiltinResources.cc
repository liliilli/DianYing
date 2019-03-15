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
#include <Dy/Core/Reflection/RBuiltinResources.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Meta/Information/MetaInfoModel.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>
#include <Dy/Meta/Information/MetaInfoAttachment.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>
#include <Dy/Meta/Information/GLShaderMetaInformation.h>
#include <Dy/Meta/Information/MetaInfoMaterial.h>
#include <Dy/Meta/Information/MetaInfoModelMesh.h>
#include <Dy/Meta/Information/ScriptMetaInformation.h>
#include <Dy/Meta/Information/MetaInfoRenderPipeline.h>
#include <Dy/Meta/Information/MetaInfoRenderItem.h>
#include <Dy/Helper/System/Pointer.h>

namespace dy::reflect
{

void RBuiltinResources::BindBuiltinResourcesToMetaManager()
{
  auto& metaManager = MIOMeta::GetInstance();
  auto& map         = GetResourceMapReference();

  for (auto& [_, pair] : map)
  {
    auto& [resourceType, function] = pair;
    switch (resourceType)
    {
    case EResourceType::Script:
    { // CPP Script (Widget and Actor)
      auto metaInfo = std::any_cast<PDyScriptInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddScriptMetaInformation(metaInfo));
    } break;
    case EResourceType::GLShader:
    { // Shader (OpenGL)
      auto metaInfo = std::any_cast<PDyGLShaderInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddGLShaderMetaInfo(metaInfo));
    } break;
    case EResourceType::Model:
    { // Model
      auto metaInfo = std::any_cast<PDyModelInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddModelMetaInfo(metaInfo));
    } break;
    case EResourceType::Texture:
    { // Texture
      auto metaInfo = std::any_cast<PDyTextureInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddTextureMetaInfo(metaInfo));
    } break;
    case EResourceType::Material:
    { // Material. texture and shader must be specified.
      auto metaInfo = std::any_cast<PDyMaterialInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddMaterialMetaInfo(metaInfo));
    } break;
    case EResourceType::GLAttachment:
    { // OpenGL Attachment (Either Texture or RenderBuffer) for FBO.
      auto metaInfo = std::any_cast<PDyGlAttachmentInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddGLAttachmentMetaInfo(metaInfo));
    } break;
    case EResourceType::GLFrameBuffer:
    { // OpenGL FrameBuffer object.
      auto metaInfo = std::any_cast<PDyGlFrameBufferInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddGLFrameBufferMetaInfo(metaInfo));
    } break;
    case EResourceType::WidgetMeta:
    { // WidgetMeta
      auto metaInfo = std::any_cast<std::string_view>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddWidgetMetaInformation(metaInfo.data()));
    } break;
    case EResourceType::Mesh:
    { // Mesh
      auto metaInfo = std::any_cast<PDyMeshInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddBuiltinMeshMetaInfo(metaInfo));
    } break;
    case EResourceType::RenderPipeline:
    { // RenderPipeline.
      auto metaInfo = std::any_cast<PDyRenderPipelineInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddRenderPipelineMetaInfo(metaInfo));
    } break;
    case EResourceType::RenderItem:
    { // RenderItem.
      auto metaInfo = std::any_cast<PDyRenderItemInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddRenderIteMIOMeta(metaInfo));
    } break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }
  map.clear();

  if (IsBootingMetaInfoScriptExist() == true)
  { // If boot resource exist, do that.
    const auto bootScript = DyConvertUniquePtrTo<ALoadingBootResource>(GetBootResourceMetaInfo().second());
    MDY_CALL_ASSERT_SUCCESS(metaManager.MDY_PRIVATE(AddBootResourceSpecifierList)(bootScript->GetResourceMetaList()));
  }

  for (const auto& [type, populateFunc] : GetGlobalResourceMetaInfo())
  { // If global resource exist, do that.
    const auto globalScript = DyConvertUniquePtrTo<ALoadingGlobalResource>(populateFunc());
    MDY_CALL_ASSERT_SUCCESS(metaManager.MDY_PRIVATE(AddGlobalResourceSpecifierList)(globalScript->GetResourceMetaList()));
  }

  if (IsLoadingWidgetMetaInfoExist() == true)
  { // If loading widget is exist, insert it to meta manager.
    auto metaInfo = std::any_cast<std::string_view>(GetLoadingWidgetResourceMetaInfo().second()->GetMetaInfo());
    MDY_CALL_ASSERT_SUCCESS(metaManager.MDY_PRIVATE(AddLoadingWidgetMetaInformation)(metaInfo.data()));
  }
}

bool RBuiltinResources::IsBootingMetaInfoScriptExist() noexcept
{ 
  return MDY_CHECK_ISNOTNULL(GetBootResourceMetaInfo().second); 
}

bool RBuiltinResources::IsLoadingWidgetMetaInfoExist() noexcept
{
  return MDY_CHECK_ISNOTNULL(GetLoadingWidgetResourceMetaInfo().second);
}

RBuiltinResources::TMapType& RBuiltinResources::GetResourceMapReference()
{
  static TMapType typeMap;
  return typeMap;
}

RBuiltinResources::TValueType& RBuiltinResources::GetBootResourceMetaInfo()
{
  static TValueType mLoadingBootingMetaInfoCustomized = {};
  return mLoadingBootingMetaInfoCustomized;
}

std::vector<RBuiltinResources::TValueType>& RBuiltinResources::GetGlobalResourceMetaInfo()
{
  static std::vector<TValueType> mLoadingGlobalMetaInfoCustomizedList = {};
  return mLoadingGlobalMetaInfoCustomizedList;
}

RBuiltinResources::TValueType& RBuiltinResources::GetLoadingWidgetResourceMetaInfo()
{
  static TValueType mLoadingWidgetResourceMetaInfo = {};
  return mLoadingWidgetResourceMetaInfo;
}

} /// ::dy::reflect namespace