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

struct GridLeftPanel_Controller : public Component
{
  GridLeftPanel_Controller(const Colour& iColour) : mColour{iColour}
  {}

  void paint(Graphics& g) override
  {
    g.fillAll(this->mColour.withAlpha(0.5f));

    //g.setColour(Colours::black);
    //g.drawText(text, getLocalBounds().withSizeKeepingCentre(100, 100), Justification::centred, false);
  }

  Colour mColour;
};
