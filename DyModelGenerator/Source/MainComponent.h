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
#include "../Builds/VisualStudio2017/GridLeftModelInformation.h"
#include "../Builds/VisualStudio2017/GridRightController.h"
#include "../Builds/VisualStudio2017/GridUp_BriefInformation.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, 
    public FilenameComponentListener, 
    public MenuBarModel,
    public ApplicationCommandTarget,
    public Thread::Listener
{
public:
  //==============================================================================
  MainComponent();
  ~MainComponent();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

private:
  //==============================================================================
  // Your private member variables go here...

  void filenameComponentChanged (FilenameComponent*) override;
 
  void exitSignalSent() override final;

  /// @brief
  FilenameComponent mFileChooser = {
      "Model File", 
      {}, 
      true, false, false, 
      "*.obj;*.fbx;*.blend;*.dae;*.collada;", 
      {},
      R"(Choose a model file to process for reading from `Dy` framework.)"
  };

  /// @brief Get menubar items list.
  [[nodiscard]] StringArray getMenuBarNames() override final;
  /// @brief Get menubar sub-items for each item.
  [[nodiscard]] PopupMenu getMenuForIndex(int menuIndex, const String& /*menuName*/) override final;

  /// @brief
  void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override final {}
  /// @brief
  [[nodiscard]] ApplicationCommandTarget* getNextCommandTarget() override final;
  /// @brief
  void getAllCommands(Array<CommandID>& c) override final;
  /// @brief
  void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override final;
  /// @brief
  [[nodiscard]] bool perform(const InvocationInfo& info) override final;

  /// @brief List of menu commands that responds to.
  enum CommandIDs
  {
    menu_Exit   = 1,
    about_About = 2,
  };

  ApplicationCommandManager mCommandManager;
  std::unique_ptr<MenuBarComponent> mMenuBar = nullptr;

  GridRightPanel_ModelInformation   mRightPanel {"Hello world!"};
  GridLeftPanel_Controller          mLeftPanel {Colours::white};
  GridUpPanel_BriefInformation      mUpPanel {};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
