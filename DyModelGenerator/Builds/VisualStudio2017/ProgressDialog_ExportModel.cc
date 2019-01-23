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
#include "ProgressDialog_ExportModel.h"
#include "HelperConstant.h"
#include "HelperMacroes.h"
#include "Singleton_ModelInstance.h"

ProgressDialog_ExportModel::ProgressDialog_ExportModel(const std::string& iSpecifierName, EExportFlags iFlags) :
    ThreadWithProgressWindow("busy doing some important things...", true, true),
    mSpecifierName{iSpecifierName}
{
  if (iFlags & Flag_Model) { this->mExportMeshes = true; }
}

void ProgressDialog_ExportModel::run()
{
  setProgress(kDialog_SpinningBar); 
  setStatusMessage("Exporting..."); 

  auto& modelInstance = Singleton_ModelInstance::GetInstance();
  const auto numModelMeshes = modelInstance.GetNumModelMeshes();
  if (this->mExportMeshes == true)
  {
    for (unsigned i = 0; i < numModelMeshes; ++i)
    {
      // Get exported specifier name.
      const auto meshSpecifierName = modelInstance.GetExportedMeshSpecifierName(this->mSpecifierName, i);
      setStatusMessage("Exporting meshes..\n" + meshSpecifierName); 
      
      // Export model mesh on fullpath name.
      const auto flag = modelInstance.ExportModelMesh(meshSpecifierName, i, true);
      if (flag == DY_FAILURE)
      { // If failed, just return with failure signal.
        this->signalThreadShouldExit();
        return;
      }
    }
  }

  if (threadShouldExit() == true) { return; }

  /*
   * vec3 pos, vec3 normal, vec3 tangent, vec3 bitangent, vec2 texcoord0 (st만), boneId vec4, weight vec4, 
   * 본 오프셋 매트릭스, *  해당 본의 파이널 트랜스폼. 
   * 뒤쪽 두개 요소는 메쉬별로 나누면 될 것 같고, 앞의 요소들은 버텍스마다 해서 시리얼라이즈 해줘야겠네 ㅠ
   */

  wait(500);
}

void ProgressDialog_ExportModel::threadComplete(bool userPressedCancel)
{
  if (userPressedCancel == false)
  { // thread finished normally..
    AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
      "Progress window",
      "Exporting model completed.");
  }
  else
  { // If `threadShouldExit` is true, (abnormally)
    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
      "Progress window",
      "Canceled Exporting model process.");
  }

  delete this;
}
