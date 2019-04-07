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

#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevelRender.h>
#include <Dy/Management/Type/Render/DGlGlobalStates.h>
#include <Dy/Management/Type/Render/ERenderingModelMode.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Management/MSetting.h>

#include <Dy/Builtin/RenderItem/Level/FBtOpaqueDefault.h>
#include <Dy/Builtin/RenderItem/Level/FBtLevelOITDefault.h>

namespace dy
{

void FBtRenderPipelineDefaultLevelRender::__ConstructionHelper
  ::ConstructBuffer(PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  oMeta.mSpecifierName = sName;
  oMeta.mUuid          = DUuid("6071de28-791b-4e2d-9312-00c2591e9cb9");

  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderItemOpaqueDefault::sName);
  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderLevelOitDefault::sName);
}

EDySuccess FBtRenderPipelineDefaultLevelRender::OnPreRenderCheckCondition()
{
  // Whether camera is focused by main camera is true, by parent RenderPipeline
  // `FBtDefaultLevel`, we do not need to check more setting.
  return EDySuccess::DY_SUCCESS;
}

void FBtRenderPipelineDefaultLevelRender::OnFailedCheckCondition()
{
  /* Do nothing */
}

void FBtRenderPipelineDefaultLevelRender::OnSetupRenderingSetting()
{
  { // Set overall rendering mode.
    DGlGlobalStates initialStatus{};
    using DPolygonMode = DGlGlobalStates::DPolygonMode;
    using EMode  = DGlGlobalStates::DPolygonMode::EMode;
    using EValue = DGlGlobalStates::DPolygonMode::EValue;
    //
    switch (MSetting::GetInstance().GetRenderingMode())
    {
    case ERenderingModelMode::FillNormal: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Triangle}; 
      { 
        XGLWrapper::PushInternalGlobalState(initialStatus);
      }
    } break;
    case ERenderingModelMode::WireFrame: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Line}; 
      { 
        XGLWrapper::PushInternalGlobalState(initialStatus);
      }
    } break;
    }
  }
}

void FBtRenderPipelineDefaultLevelRender::OnReleaseRenderingSetting()
{
  XGLWrapper::PopInternalGlobalState();
}

void FBtRenderPipelineDefaultLevelRender::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace