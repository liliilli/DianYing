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
#include <Dy/Management/Type/Render/DDyGlGlobalStatus.h>
#include <Dy/Management/Type/Render/EDyModelRenderingMode.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Management/SettingManager.h>

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
  return DY_SUCCESS;
}

void FBtRenderPipelineDefaultLevelRender::OnFailedCheckCondition()
{
  /* Do nothing */
}

void FBtRenderPipelineDefaultLevelRender::OnSetupRenderingSetting()
{
  { // Set overall rendering mode.
    DDyGlGlobalStatus initialStatus{};
    using DPolygonMode = DDyGlGlobalStatus::DPolygonMode;
    using EMode  = DDyGlGlobalStatus::DPolygonMode::EMode;
    using EValue = DDyGlGlobalStatus::DPolygonMode::EValue;
    //
    switch (MDySetting::GetInstance().GetRenderingMode())
    {
    case EDyModelRenderingMode::FillNormal: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Triangle}; 
      { 
        FDyGLWrapper::InsertInternalGlobalStatus(initialStatus);
      }
    } break;
    case EDyModelRenderingMode::WireFrame: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Line}; 
      { 
        FDyGLWrapper::InsertInternalGlobalStatus(initialStatus);
      }
    } break;
    }
  }
}

void FBtRenderPipelineDefaultLevelRender::OnReleaseRenderingSetting()
{
  FDyGLWrapper::PopInternalGlobalStatus();
}

void FBtRenderPipelineDefaultLevelRender::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace