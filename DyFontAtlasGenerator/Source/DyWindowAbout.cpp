#include "precompiled.h"
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
#include <Include/DyWindowAbout.h>
#include <Include/DyFontAtlasGenerator.h>

namespace dy
{

DyWindowAbout::DyWindowAbout(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  // Connect signal and slot.
  connect(ui.PB_OK, &QPushButton::clicked, this, &QWidget::close);
  connect(this, &DyWindowAbout::ReturnFocusToParent, this->mParentMainWindow, &DyFontAtlasGenerator::ResumeFocus);
}

DyWindowAbout::~DyWindowAbout()
{

}

void DyWindowAbout::SetParentMainWindow(DyFontAtlasGenerator& parentWindow)
{
  this->mParentMainWindow = &parentWindow;
}

void DyWindowAbout::closeEvent(QCloseEvent* event)
{
  event->accept();
}

} /// dy namespace
