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
#include "../../Include/Panel/ProgressDialog_ReadModel.h"
#include "../../Include/Helper/HelperConstant.h"
#include "../../Include/Helper/Singleton_ModelInstance.h"
#include "../../Include/Helper/Global_EnumSuccess.h"

ProgressDialog_ReadModel::ProgressDialog_ReadModel(const juce::File iFileFd) :
    ThreadWithProgressWindow("busy doing some important things...", true, true),
    mModelFileFd{iFileFd}
{
}

void ProgressDialog_ReadModel::run()
{
  setProgress(kDialog_SpinningBar); 
  setStatusMessage("Opening " + mModelFileFd.getFullPathName() + "...");

  auto& modelInstance = Singleton_ModelInstance::GetInstance();
  modelInstance.ReleaseModel();
  const auto errorFlag = modelInstance.ReadModelWithPath(mModelFileFd.getFullPathName().toStdString());
  if (errorFlag == DY_FAILURE || threadShouldExit() == true) { return; }

  wait(100);
}

void ProgressDialog_ReadModel::threadComplete(bool userPressedCancel)
{
  if (userPressedCancel == false)
  {
    // thread finished normally..
    AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
      "Progress window",
      "Loading model completed.");
  }

  // ..and clean up by deleting our thread object..
  this->signalThreadShouldExit();
  delete this;
}
