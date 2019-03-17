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

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Helper/Enum_ExportFlags.h"

class ProgressDialog_ExportModel : public ThreadWithProgressWindow
{
public:
  ProgressDialog_ExportModel(const std::string& iSpecifierName, EExportFlags iFlags);

  void run() override final;

  // This method gets called on the message thread once our thread has finished..
  void threadComplete(bool userPressedCancel) override final;

private:
  std::string   mSpecifierName;
  EExportFlags  mExportFlag;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProgressDialog_ExportModel)
};
