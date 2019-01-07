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
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Meta/Information/MetaInfoModel.h>
#include <Dy/Meta/Information/MetaInfoTexture.h>
#include <Dy/Meta/Information/MetaInfoAttachment.h>
#include <Dy/Meta/Information/MetaInfoFrameBuffer.h>

namespace dy::reflect
{

void RDyBuiltinResource::BindBuiltinResourcesToMetaManager()
{
  auto& metaManager = MDyMetaInfo::GetInstance();
  auto& map         = GetResourceMapReference();

  for (auto& [_, pair] : map)
  {
    auto& [resourceType, function] = pair;
    switch (resourceType)
    {
    case EDyResourceType::Script:
    { // CPP Script (Widget and Actor)
      auto metaInfo = std::any_cast<PDyScriptInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddScriptMetaInformation(metaInfo));
    } break;
    case EDyResourceType::GLShader:
    { // Shader (OpenGL)
      auto metaInfo = std::any_cast<PDyGLShaderInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddGLShaderMetaInfo(metaInfo));
    } break;
    case EDyResourceType::Model:
    { // Model
      auto metaInfo = std::any_cast<PDyModelInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddModelMetaInfo(metaInfo));
    } break;
    case EDyResourceType::Texture:
    { // Texture
      auto metaInfo = std::any_cast<PDyTextureInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddTextureMetaInfo(metaInfo));
    } break;
    case EDyResourceType::Material:
    { // Material. texture and shader must be specified.
      auto metaInfo = std::any_cast<PDyMaterialInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddMaterialMetaInfo(metaInfo));
    } break;
    case EDyResourceType::GLAttachment:
    { // OpenGL Attachment (Either Texture or RenderBuffer) for FBO.
      auto metaInfo = std::any_cast<PDyGlAttachmentInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddGLAttachmentMetaInfo(metaInfo));
    } break;
    case EDyResourceType::GLFrameBuffer:
    { // OpenGL FrameBuffer object.
      auto metaInfo = std::any_cast<PDyGlFrameBufferInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddGLFrameBufferMetaInfo(metaInfo));
    } break;
    case EDyResourceType::WidgetMeta:
    { // WidgetMeta
      auto metaInfo = std::any_cast<std::string_view>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddWidgetMetaInformation(MSVSTR(metaInfo)));
    } break;
    case EDyResourceType::Mesh:
    { // Mesh
      auto metaInfo = std::any_cast<PDyBtMeshInstanceMetaInfo>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddBuiltinMeshMetaInfo(metaInfo));
    } break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }
  map.clear();

  if (IsBootingMetaInfoScriptExist() == true)
  { // If boot resource exist, do that.
    const auto bootScript = DyConvertUniquePtrTo<ADyLoadingBootResource>(GetBootResourceMetaInfo().second());
    MDY_CALL_ASSERT_SUCCESS(metaManager.MDY_PRIVATE_SPECIFIER(AddBootResourceSpecifierList)(bootScript->GetResourceMetaList()));
  }

  for (const auto& [type, populateFunc] : GetGlobalResourceMetaInfo())
  { // If global resource exist, do that.
    const auto globalScript = DyConvertUniquePtrTo<ADyLoadingGlobalResource>(populateFunc());
    MDY_CALL_ASSERT_SUCCESS(metaManager.MDY_PRIVATE_SPECIFIER(AddGlobalResourceSpecifierList)(globalScript->GetResourceMetaList()));
  }

  if (IsLoadingWidgetMetaInfoExist() == true)
  { // If loading widget is exist, insert it to meta manager.
    auto metaInfo = std::any_cast<std::string_view>(GetLoadingWidgetResourceMetaInfo().second()->GetMetaInfo());
    MDY_CALL_ASSERT_SUCCESS(metaManager.MDY_PRIVATE_SPECIFIER(AddLoadingWidgetMetaInformation)(MSVSTR(metaInfo)));
  }
}

} /// ::dy::reflect namespace