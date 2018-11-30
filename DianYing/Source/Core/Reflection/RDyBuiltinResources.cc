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
#include <Dy/Core/Reflection/RDyBuiltinResources.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include "Dy/Meta/Information/ModelMetaInformation.h"

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
      MDY_NOT_IMPLEMENTED_ASSERT();
    } break;
    case EDyResourceType::WidgetMeta:
    { // WidgetMeta
      auto metaInfo = std::any_cast<std::string_view>(function()->GetMetaInfo());
      MDY_CALL_ASSERT_SUCCESS(metaManager.pfAddWidgetMetaInformation(MSVSTR(metaInfo)));
    } break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }
}

} /// ::dy::reflect namespace