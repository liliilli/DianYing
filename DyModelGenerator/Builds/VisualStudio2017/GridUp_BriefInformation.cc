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
#include "GridUp_BriefInformation.h"
#include "Singleton_ModelInstance.h"
#include <filesystem>
#include "ProgressDialog_ExportModel.h"

GridUpPanel_BriefInformation::GridUpPanel_BriefInformation()
{
  this->addAndMakeVisible(this->mName);
  this->addAndMakeVisible(this->mSpecifierEditor);
  this->addAndMakeVisible(this->mCreateButton);

  this->mName.setFont(Font (16.0f, Font::plain));
  this->mName.setText("Model export specifier name", dontSendNotification);
  this->mCreateButton.setButtonText("Export");

  this->DeactivateModelEditor();
}

void GridUpPanel_BriefInformation::paint(Graphics& g)
{
  g.fillAll(this->getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void GridUpPanel_BriefInformation::resized()
{
  auto bounds = this->getLocalBounds();
  {
    const auto labelBound = bounds.removeFromTop(25);
    this->mName.setBounds(labelBound);
  }
  {
    const auto editorBound = bounds.removeFromTop(25); 
    this->mSpecifierEditor.setBounds(editorBound);
  }
  {
    const auto createButton = bounds.removeFromBottom(32);
    this->mCreateButton.setBounds(createButton);
  }
}

void GridUpPanel_BriefInformation::ActivateModelEditor()
{
  const auto& modelInstance = Singleton_ModelInstance::GetInstance();
  this->mSpecifierEditor.setReadOnly(false);

  std::string fileName;
  { // Get filename, without `.***` type specifier.
    namespace fs = std::filesystem;
    const auto& strPath = modelInstance.GetModelFileFullPath();
    fs::path fsPath = strPath;

    fileName = fsPath.filename().string();
    const auto idSuffixDot = fileName.find_last_of(".");
    fileName = fileName.substr(0, idSuffixDot);
  }

  this->mSpecifierEditor.setText(fileName);
  this->mSpecifierEditor.setTextToShowWhenEmpty("Write proper specifier name to bind all resource with inputted name.", Colours::grey);

  this->mCreateButton.setEnabled(true);
  this->mCreateButton.addListener(this);
}

void GridUpPanel_BriefInformation::DeactivateModelEditor()
{
  this->mSpecifierEditor.setTextToShowWhenEmpty("Load model file first.", Colours::grey);
  this->mSpecifierEditor.setText("");
  this->mSpecifierEditor.setReadOnly(true);

  this->mCreateButton.setEnabled(false);
  this->mCreateButton.removeListener(this);
}

void GridUpPanel_BriefInformation::buttonClicked(Button* button)
{
  if (&this->mCreateButton == button)
  { // If `mCreateButton` is pressed.
    auto ptr = (new ProgressDialog_ExportModel(EExportFlags::Flag_Model));
    ptr->launchThread();
  }
}
