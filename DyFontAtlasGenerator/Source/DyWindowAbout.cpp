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
#include <DyWindowAbout.h>
#include "Include/Library/HelperString.h"
#include "Include/Constant.h"

#include <DyFontAtlasGenerator.h>
#include <QListWidget>

//!
//! Forward declaration
//!

namespace
{

std::array<std::pair<std::string, std::string>, 5> sLibraryDescriptors = 
{
  std::pair
  {"freetype", ""},
  {"msdfgen", ""},
  {"nlohmann", ""},
  {"spdlog & fmt", ""},
  {"zlib", ""},
};

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

DyWindowAbout::DyWindowAbout(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  // Connect signal and slot.
  ui.LB_Version->setText(MakeStringU8("Version v{}", kVersionValue).c_str());

  connect(ui.PB_OK,     &QPushButton::clicked, this, &QWidget::close);
  connect(ui.BT_Github, &QPushButton::clicked, this, &DyWindowAbout::CbOpenGithubUrl);
  connect(ui.BT_Home,   &QPushButton::clicked, this, &DyWindowAbout::CbOpenBlogUrl);

  for (const auto& [libraryName, libraryUrl] : sLibraryDescriptors)
  {
    ui.LW_Library->addItem(libraryName.c_str());
  }
}

void DyWindowAbout::SetParentMainWindow(DyFontAtlasGenerator& parentWindow)
{
  this->mParentMainWindow = &parentWindow;
}

void DyWindowAbout::CbOpenGithubUrl([[maybe_unused]] int value)
{
  // Open github url with default web browser.
  QDesktopServices::openUrl(QUrl(R"dy(https://github.com/liliilli)dy"));
}

void DyWindowAbout::CbOpenBlogUrl([[maybe_unused]] int value)
{
  // Open blog url with default web browser.
  QDesktopServices::openUrl(QUrl(R"dy(https://neuliliilli.hatenablog.com/)dy"));
}

void DyWindowAbout::closeEvent(QCloseEvent* event)
{
  event->accept();
}

} /// dy namespace
