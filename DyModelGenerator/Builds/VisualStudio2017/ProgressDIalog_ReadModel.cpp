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
#include "ProgressDialog_ReadModel.h"
#include "HelperConstant.h"
#include "HelperAssimp.h"

ProgressDialog_ReadModel::ProgressDialog_ReadModel(const juce::File iFileFd) :
    ThreadWithProgressWindow("busy doing some important things...", true, true),
    mModelFileFd{iFileFd}
{
}

void ProgressDialog_ReadModel::run()
{
  setProgress(kDialog_SpinningBar); 
  setStatusMessage(mModelFileFd.getFullPathName() + " opened.");

  auto optResult = ReadModel(mModelFileFd.getFullPathName().toStdString());
  if (optResult.has_value() == false) { return; }
  if (threadShouldExit() == true)     { return; }

  setProgress(kDialog_SpinningBar); 
  setStatusMessage("Finishing off the last few bits and pieces!");
  wait(1000);
}

void ProgressDialog_ReadModel::threadComplete(bool userPressedCancel)
{
  if (userPressedCancel)
  {
    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
      "Progress window",
      "You pressed cancel!");
  }
  else
  {
    // thread finished normally..
    AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
      "Progress window",
      "Thread finished ok!");
  }

  // ..and clean up by deleting our thread object..
  delete this;
}
