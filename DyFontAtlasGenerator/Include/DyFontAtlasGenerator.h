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

#include <QtWidgets/QMainWindow>
#include <Include/Type/GlobalEnums.h>
#include "ui_DyFontAtlasGenerator.h"

//!
//! Forward declaration
//!

namespace dy
{
class DyWindowAbout;
} /// dy namespace

//!
//! Implementation
//!

struct DDyFontInformation final
{
  /// File path on focus.
  std::string fontPath  = "";
  ///
  std::string fontName  = "";
  ///
  std::string fontStyle = "";
};

namespace dy
{

class DyFontAtlasGenerator : public QMainWindow
{
  Q_OBJECT

public:
  DyFontAtlasGenerator(QWidget *parent = Q_NULLPTR);

  ///
  /// @brief
  ///
  void IncrementProgress();

public slots:
  ///
  /// @brief
  ///
  void FindFontFile();
  ///
  /// @brief
  /// @param value NOT USED
  ///
  void UpdateCharmapFlag(int value);
  ///
  /// @brief
  /// @param value NOT USED
  ///
  void UpdateOptionFlag(int value);
  ///
  /// @brief Make batch file. from BT_
  ///
  void CreateBatchFile();
  ///
  /// @brief
  ///
  void CreationTaskFinished();
  ///
  /// @brief
  ///
  void ShowAbout();
  ///
  /// @brief
  ///
  void ResumeFocus();

signals:
  void SetProgressBarValue(int);

private:
  ///
  Ui::DyFontAtlasGeneratorClass ui;
  ///
  QFutureWatcher<void>      mFutureWatcher;

  /// Charmap flag.
  dy::EDyCharmapCollections mCharmapFlag = dy::EDyCharmapCollections::None;
  /// Option flag.
  dy::EDyOptionCollections  mOptionFlag  = dy::EDyOptionCollections::None;
  ///
  DDyFontInformation        mFontInformation = {};
  ///
  DyWindowAbout*            mChildAbout = nullptr;
};

} /// dy namespace
