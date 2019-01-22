#pragma once
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
/// This file is inspired by GSL pointer.h file.
/// @reference https://github.com/Microsoft/GSL/blob/master/include/gsl/pointers
///

#include "../JuceLibraryCode/JuceHeader.h"

class ProgressDialog_ReadModel : public ThreadWithProgressWindow
{
public:
  ProgressDialog_ReadModel() : ThreadWithProgressWindow("busy doing some important things...", true, true)
  {
    setStatusMessage("Getting ready...");
  }

  void run() override
  {
    setProgress(-1.0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
    setStatusMessage("Preparing to do some stuff...");
    wait(2000);

    int thingsToDo = 10;

    for (int i = 0; i < thingsToDo; ++i)
    {
      // must check this as often as possible, because this is
      // how we know if the user's pressed 'cancel'
      if (threadShouldExit())
        return;

      // this will update the progress bar on the dialog box
      setProgress(i / (double)thingsToDo);

      setStatusMessage(String(thingsToDo - i) + " things left to do...");

      wait(500);
    }

    setProgress(-1.0); // setting a value beyond the range 0 -> 1 will show a spinning bar..
    setStatusMessage("Finishing off the last few bits and pieces!");
    wait(2000);
  }

  // This method gets called on the message thread once our thread has finished..
  void threadComplete(bool userPressedCancel) override
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
};

