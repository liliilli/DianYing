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

#include "MainComponent.h"
#include "../Builds/VisualStudio2017/ProgressDialog_ReadModel.h"
#include "../Builds/VisualStudio2017/Singleton_ModelInstance.h"

//==============================================================================
MainComponent::MainComponent()
{
  const auto errorFlag = Singleton_ModelInstance::Initialize();
  jassert(errorFlag == DY_SUCCESS);

  // Create a file chooser control to load files into it..
  {
    this->addAndMakeVisible(this->mFileChooser);
    this->mFileChooser.addListener(this);
  }

  // Create menu bar.
  {
    this->mMenuBar = std::make_unique<MenuBarComponent>(this);
    this->addAndMakeVisible(this->mMenuBar.get());

    this->setApplicationCommandManagerToWatch(&this->mCommandManager);
    this->mCommandManager.registerAllCommandsForTarget(this);
  }

  // Set grid regions.
  {
    this->addAndMakeVisible(this->mLeftPanel);
    this->addAndMakeVisible(this->mRightPanel);
    this->addAndMakeVisible(this->mUpPanel);
  }

  this->lookAndFeelChanged();
  this->setSize(1280, 720);
}

MainComponent::~MainComponent()
{
  this->mFileChooser.removeListener(this);

  const auto errorFlag = Singleton_ModelInstance::Release();
  jassert(errorFlag == DY_SUCCESS);
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(this->getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

  //g.setFont(Font(16.0f));
  //g.setColour(Colours::white);
  //g.drawText("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
  // This is called when the MainComponent is resized.
  // If you add any child components, this is where you should
  // update their positions.
  const auto menuHeight = LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight();
  auto bounds = this->getLocalBounds();
  { // Menubar resize
    const auto b = bounds.removeFromTop(menuHeight);
    this->mMenuBar->setBounds(b);
  }

  auto r = bounds.reduced(8);
  { // Filechooser resize
    this->mFileChooser.setBounds(r.removeFromTop(25));
  }

  { // Grid resize
    Grid grid;
    grid.rowGap     = 8_px;
    grid.columnGap  = 8_px;

    using Track = Grid::TrackInfo;
    grid.templateRows     = { Track(128_px), Track(1_fr) };
    grid.templateColumns  = { Track(1_fr), Track(1_fr) };

    grid.autoRows     = Track(1_fr);
    grid.autoColumns  = Track(1_fr);
    grid.autoFlow     = Grid::AutoFlow::column;

    grid.items.addArray({ 
      GridItem(this->mUpPanel).withArea(1, 1),
      GridItem(this->mLeftPanel).withArea(2, 1), 
      GridItem(this->mRightPanel).withArea(GridItem::Span{2}, 2)
    });

    r.removeFromTop(8);
    grid.performLayout(r);
  }
}

void MainComponent::filenameComponentChanged(FilenameComponent*)
{
  auto fileFd = this->mFileChooser.getCurrentFile();
  auto ptr = (new ProgressDialog_ReadModel(fileFd));
  ptr->addListener(this);
  ptr->launchThread();
}

void MainComponent::exitSignalSent()
{
  auto& modelInstance = Singleton_ModelInstance::GetInstance();
  
  const auto* ptrModelScene = modelInstance.GetPtrModelScene();
  if (ptrModelScene == nullptr) { return; }

  (void)ptrModelScene;
}

StringArray MainComponent::getMenuBarNames()
{
  return { "Menu", "Help" };
}

PopupMenu MainComponent::getMenuForIndex(int menuIndex, const String&)
{
  PopupMenu menu;

  if (menuIndex == 0)
  {
    menu.addCommandItem(&mCommandManager, CommandIDs::menu_Exit);
  }
  else if (menuIndex == 1)
  {
    menu.addCommandItem(&mCommandManager, CommandIDs::about_About);
  }

  return menu;
}

ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
  return nullptr;
  //return &outerCommandTarget;
}

void MainComponent::getAllCommands(Array<CommandID>& c)
{
  const Array<CommandID> commands{ CommandIDs::menu_Exit, };
  c.addArray(commands);
}

void MainComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
  switch (commandID)
  {
  case CommandIDs::menu_Exit:
  {
    result.setInfo("Exit", "Places the menu bar inside the application window", "Menu", 0);
    result.addDefaultKeypress('w', ModifierKeys::ctrlModifier);
  } break;
  default: break;
  }
}

bool MainComponent::perform(const InvocationInfo& info)
{
  switch (info.commandID)
  {
  case CommandIDs::menu_Exit:
  {
    if (JUCEApplicationBase::isStandaloneApp() == true)
    {
      JUCEApplicationBase::quit();
    }
  } break;
  default: return false;
  }

  return true;
}
