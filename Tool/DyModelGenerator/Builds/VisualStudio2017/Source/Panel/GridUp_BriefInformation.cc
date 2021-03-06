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
#include "../../Include/Panel/GridUp_BriefInformation.h"
#include <filesystem>
#include "../../Include/Helper/Singleton_ModelInstance.h"
#include "../../Include/Panel/ProgressDialog_ExportModel.h"

GridUpPanel_BriefInformation::GridUpPanel_BriefInformation()
{
  this->addAndMakeVisible(this->mName);
  this->addAndMakeVisible(this->mSpecifierEditor);
  this->addAndMakeVisible(this->mCreateButton);

  this->mName.setFont(Font (16.0f, Font::plain));
  this->mName.setText("Model export specifier name", dontSendNotification);
  this->mCreateButton.setButtonText("Export");

  this->addAndMakeVisible(this->mTgBtn_MakeFileWithSkeleton);
  this->mTgBtn_MakeFileWithSkeleton.setEnabled(false);
  // Set callback function when `activated` skeleton button has changed state.
  this->mTgBtn_MakeFileWithSkeleton.onStateChange = [this]
  {
    auto& instance = Singleton_ModelInstance::GetInstance();

    if (this->mTgBtn_MakeFileWithSkeleton.getToggleState() == true)
    { 
      instance.SetExportFlag(Flag_WithSkeleton, true); 
      this->mTgBtn_OptionSkeleton.setEnabled(true);

      // Check loaded model has a animation.
      if (instance.IsModelHasAnimations() == true)
      { this->mTgBtn_OptionAnimation.setEnabled(true); }
    }
    else
    { 
      instance.SetExportFlag(Flag_WithSkeleton, false); 

      // If disabled, skeleton option has been checked, change toggle state disabled.
      if (this->mTgBtn_OptionSkeleton.getToggleState() == true)
      { this->mTgBtn_OptionSkeleton.setToggleState(false, NotificationType::sendNotification); }
      this->mTgBtn_OptionSkeleton.setEnabled(false);

      if (this->mTgBtn_OptionAnimation.getToggleState() == true)
      { this->mTgBtn_OptionAnimation.setToggleState(false, NotificationType::sendNotification); }
      this->mTgBtn_OptionAnimation.setEnabled(false);
    }
  };

  this->addAndMakeVisible(this->mTgBtn_MakeFileWithCompression);
  this->mTgBtn_MakeFileWithCompression.onStateChange = [this]
  {
    auto& instance = Singleton_ModelInstance::GetInstance();

    if (this->mTgBtn_MakeFileWithCompression.getToggleState() == true)
    { instance.SetExportFlag(Flag_WithCompress, true); }
    else
    { instance.SetExportFlag(Flag_WithCompress, false); }
  };

  this->addAndMakeVisible(this->mTgBtn_OptionMesh);
  this->mTgBtn_OptionMesh.setEnabled(false);
  this->mTgBtn_OptionMesh.onStateChange = [this]
  {
    auto& instance = Singleton_ModelInstance::GetInstance();

    if (this->mTgBtn_OptionMesh.getToggleState() == true)
    { instance.SetExportFlag(Flag_OptionMesh, true); }
    else
    { instance.SetExportFlag(Flag_OptionMesh, false); }
  };

  this->addAndMakeVisible(this->mTgBtn_OptionSkeleton);
  this->mTgBtn_OptionSkeleton.setEnabled(false);
  this->mTgBtn_OptionSkeleton.onStateChange = [this]
  {
    auto& instance = Singleton_ModelInstance::GetInstance();

    if (this->mTgBtn_OptionSkeleton.getToggleState() == true)
    { instance.SetExportFlag(Flag_OptionSkeleton, true); }
    else
    { instance.SetExportFlag(Flag_OptionSkeleton, false); }
  };

  this->addAndMakeVisible(this->mTgBtn_OptionMaterial);
  this->mTgBtn_OptionMaterial.setEnabled(false);
  this->mTgBtn_OptionMaterial.onStateChange = [this]
  {
    auto& instance = Singleton_ModelInstance::GetInstance();

    if (this->mTgBtn_OptionMaterial.getToggleState() == true)
    { instance.SetExportFlag(Flag_OptionMaterial, true); }
    else
    { instance.SetExportFlag(Flag_OptionMaterial, false); }
  };

  this->addAndMakeVisible(this->mTgBtn_OptionAnimation);
  this->mTgBtn_OptionAnimation.setEnabled(false);
  this->mTgBtn_OptionAnimation.onStateChange = [this]
  {
    auto& instance = Singleton_ModelInstance::GetInstance();

    if (this->mTgBtn_OptionAnimation.getToggleState() == true)
    { instance.SetExportFlag(Flag_OptionAnimation, true); }
    else
    { instance.SetExportFlag(Flag_OptionAnimation, false); }
  };

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

  bounds.removeFromTop(8);

  // Toggle options.
  {
    auto buttonBound = bounds.removeFromTop(25);
    this->mTgBtn_MakeFileWithCompression.setBounds(buttonBound);

    buttonBound = bounds.removeFromTop(25);
    this->mTgBtn_MakeFileWithSkeleton.setBounds(buttonBound);

    buttonBound = bounds.removeFromTop(25);
    this->mTgBtn_OptionMesh.setBounds(buttonBound);

    buttonBound = bounds.removeFromTop(25);
    this->mTgBtn_OptionSkeleton.setBounds(buttonBound);

    buttonBound = bounds.removeFromTop(25);
    this->mTgBtn_OptionMaterial.setBounds(buttonBound);

    buttonBound = bounds.removeFromTop(25);
    this->mTgBtn_OptionAnimation.setBounds(buttonBound);
  }

  // Button
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

  this->mTgBtn_OptionMesh.setEnabled(true);

  // If model has a material, enabled export material button.
  if (modelInstance.GetNumModelMaterials() > 0)
  {
    this->mTgBtn_OptionMaterial.setEnabled(true);
  }

  // If model has a bone. enable export skeleton and with-skeleton button.
  if (modelInstance.IsModelHasBones() == true)
  {
    this->mTgBtn_MakeFileWithSkeleton.setEnabled(true);
  }
  else
  {
    this->mTgBtn_MakeFileWithSkeleton.setToggleState(false, NotificationType::sendNotification);
    this->mTgBtn_MakeFileWithSkeleton.setEnabled(false);
  }
}

void GridUpPanel_BriefInformation::DeactivateModelEditor()
{
  this->mSpecifierEditor.setTextToShowWhenEmpty("Load model file first.", Colours::grey);
  this->mSpecifierEditor.setText("");
  this->mSpecifierEditor.setReadOnly(true);

  this->mCreateButton.setEnabled(false);
  this->mCreateButton.removeListener(this);

  this->mTgBtn_MakeFileWithSkeleton.setEnabled(false);
  this->mTgBtn_OptionMesh.setEnabled(false);
  this->mTgBtn_OptionMaterial.setEnabled(false);
  this->mTgBtn_OptionSkeleton.setEnabled(false);
  this->mTgBtn_OptionAnimation.setEnabled(false);
}

void GridUpPanel_BriefInformation::buttonClicked(Button* button)
{
  if (&this->mCreateButton == button)
  { // If `mCreateButton` is pressed.
    const auto& modelInstance = Singleton_ModelInstance::GetInstance();

    auto ptr = new ProgressDialog_ExportModel(this->mSpecifierEditor.getText().toStdString(), modelInstance.GetExportFlags());
    ptr->launchThread();
  }
}
