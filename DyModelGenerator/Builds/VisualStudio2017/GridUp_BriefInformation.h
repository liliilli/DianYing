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

struct GridUpPanel_BriefInformation final : 
    public Component, 
    public Button::Listener 
{
public:
  GridUpPanel_BriefInformation();

  void paint(Graphics& g) override final;

  void resized() override final;

  /// @brief activate model editor.
  void ActivateModelEditor();
  /// @brief deactivate model editor.
  void DeactivateModelEditor();

  void buttonClicked(Button* button) override final;

private:
  Label       mName;
  TextEditor  mSpecifierEditor;
  TextButton  mCreateButton;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GridUpPanel_BriefInformation)
};

