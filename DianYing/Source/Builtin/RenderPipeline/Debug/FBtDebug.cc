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

#include <Dy/Builtin/RenderPipeline/Debug/FBtDebug.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MSetting.h>

#include <Dy/Management/Type/Render/DGlGlobalStates.h>
#include <Dy/Builtin/RenderItem/Debug/FBtDebugCollisionShape.h>
#include <Dy/Builtin/RenderItem/Debug/FBtDebugCollisionAabb.h>
#include <Dy/Builtin/RenderItem/Debug/FBtDebugPicking.h>

namespace dy
{

void FBtRenderPipelineDebug::__ConstructionHelper
  ::ConstructBuffer(PDyRenderPipelineInstanceMetaInfo& oMeta)
{
  /// @brief Render level debug information. This function must be called in render phase.
  /// @reference https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  oMeta.mSpecifierName = sName;
  oMeta.mUuid          = DUuid("d7abee65-274d-4f4e-813d-cdc873ff3434");

  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderDebugCollisionShape::sName);
  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderDebugCollisionAabb::sName);
  oMeta.mLocalRenderItemNames.emplace_back(FBtRenderDebugPicking::sName);
}

EDySuccess FBtRenderPipelineDebug::OnPreRenderCheckCondition()
{
  // If main camera is not exist, do not render level.
  const auto* ptrCamera = MWorld::GetInstance().GetPtrMainLevelCamera();
  if (ptrCamera == nullptr) 
  { 
    return DY_FAILURE; 
  }

  return DY_SUCCESS;
}

void FBtRenderPipelineDebug::OnSetupRenderingSetting()
{
  // Set status
  DGlGlobalStates statusSetting;
  statusSetting.mIsEnableDepthTest = false;
  glDisable(GL_DEPTH_TEST);
  //this->PushInternalGlobalState(statusSetting);
}

void FBtRenderPipelineDebug::OnReleaseRenderingSetting()
{
  glEnable(GL_DEPTH_TEST);
  //this->PopInternalGlobalState();
}

} /// ::dy namespace