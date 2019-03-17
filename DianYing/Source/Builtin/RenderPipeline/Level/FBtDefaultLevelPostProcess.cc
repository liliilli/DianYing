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
#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevelPostProcess.h>

#include <Dy/Builtin/RenderItem/Level/FBtPpCsmShadow.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpSsao.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpSkybox.h>
#include <Dy/Builtin/RenderItem/Level/FBtPpEmissive.h>

namespace dy
{

void FBtRenderPipelineDefaultLevelPostProcess::__ConstructionHelper
  ::ConstructBuffer(PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  oMeta.mSpecifierName = sName;
  oMeta.mUuid          = DUuid("9fb28bcb-c919-4675-b79b-1f6758c9a9aa");

  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderItemCsmShadow::sName);
  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderItemSsao::sName);
  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderItemSkybox::sName);
  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderItemEmissive::sName);
}

EDySuccess FBtRenderPipelineDefaultLevelPostProcess::OnPreRenderCheckCondition()
{
  // Whether camera is focused by main camera is true, by parent RenderPipeline
  // `FBtDefaultLevel`, we do not need to check more setting.
  return DY_SUCCESS;
}

void FBtRenderPipelineDefaultLevelPostProcess::OnFailedCheckCondition()
{
  /* Do nothing */
}

void FBtRenderPipelineDefaultLevelPostProcess::OnSetupRenderingSetting()
{

}

void FBtRenderPipelineDefaultLevelPostProcess::OnReleaseRenderingSetting()
{

}

void FBtRenderPipelineDefaultLevelPostProcess::OnPostRender()
{
  /* Do nothing */
}

} /// ::dy namespace