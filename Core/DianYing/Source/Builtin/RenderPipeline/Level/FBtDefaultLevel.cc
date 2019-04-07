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

#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevel.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MSetting.h>

#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevelRender.h>
#include <Dy/Builtin/RenderPipeline/Level/FBtDefaultLevelPostProcess.h>
#include <Dy/Builtin/RenderItem/FBtLevelIntegeration.h>

namespace dy
{

void FBtRenderPipelineDefaultLevel::__ConstructionHelper
  ::ConstructBuffer(PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  oMeta.mSpecifierName = sName;
  oMeta.mUuid          = DUuid("4b60255e-19ac-401c-b376-26cf56dab980");

  oMeta.mChildPipelineNames.emplace_back(FBtRenderPipelineDefaultLevelRender::sName);
  oMeta.mChildPipelineNames.emplace_back(FBtRenderPipelineDefaultLevelPostProcess::sName);

  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderItemLevelIntegeration::sName);
}

EDySuccess FBtRenderPipelineDefaultLevel::OnPreRenderCheckCondition()
{
  if (MWorld::GetInstance().IsLevelPresentValid() == false) 
  { 
    return EDySuccess::DY_FAILURE; 
  }

  // If main camera is not exist, do not render level.
  const auto* ptrCamera = MWorld::GetInstance().GetPtrMainLevelCamera();
  if (ptrCamera == nullptr) 
  { 
    return EDySuccess::DY_FAILURE; 
  }

  return EDySuccess::DY_SUCCESS;
}

} /// ::dy namespace