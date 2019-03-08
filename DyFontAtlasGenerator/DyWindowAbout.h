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

#include <QWidget>
#include "ui_DyWindowAbout.h"

//!
//! Forward declaration
//!

namespace dy
{
class DyFontAtlasGenerator;
}

//!
//! Implementation
//!

namespace dy
{

class DyWindowAbout : public QWidget
{
  Q_OBJECT

public:
  DyWindowAbout(QWidget *parent = Q_NULLPTR);
  ~DyWindowAbout() = default;

  /// @brief
  /// @param parentWindow
  void SetParentMainWindow(DyFontAtlasGenerator& parentWindow);

  /// @brief Callback function for link to developer github URL with default web browser.
  void CbOpenGithubUrl(int value);
  /// @brief Callback function for link to developer blog URL with default web browser.
  void CbOpenBlogUrl(int value);

signals:
  void ReturnFocusToParent();

private:
  Ui::DyWindowAbout ui;

  ///
  DyFontAtlasGenerator* mParentMainWindow = nullptr;

  /// Close Event
  void closeEvent(QCloseEvent* event) override;
};

} /// dy namespace
