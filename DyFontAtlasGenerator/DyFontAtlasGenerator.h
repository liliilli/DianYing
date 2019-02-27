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

#include <array>
#include <optional>
#include <QtWidgets/QMainWindow>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <Include/Type/GlobalEnums.h>
#include <Include/Type/DFileInformations.h>
#include <Include/Type/TCallVar.h>
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
  /// @brief Callback function that find first(primary) font file to populate.
  void FindFirstFontFile();
  /// @brief Callback function that find second(secondary) font file to populate.
  void FindSecondFontFile();
  /// @brief That Find text file callback function.
  void FindTextFile();

  /// @brief
  void UpdateCharmapFlag(int value);
  /// @brief
  void UpdateOptionFlag(int value);

  /// @brief Make batch file. from BT_
  void CreateBatchFile();
  /// @brief
  void CreationTaskFinished();

  /// @brief
  void ShowAbout();
  /// @brief
  void ResumeFocus();

signals:
  void SetProgressBarValue(int);

private:
  /// @brief 
  bool CreateFontFileInformation(uint32_t id);
  /// @brief This function must be called when single thread is running.
  [[nodiscard]] std::optional<DDyFontInformation> GetFontGeneralInformation(const QString& iFontPath);

  /// @brief
  void CreateFontBuffer(const std::vector<DDyFontInformation>& informations,
                        const std::vector<FT_ULong>& targetCharMap,
                        const EDyOptionCollections& option);

  ///
  Ui::DyFontAtlasGeneratorClass ui;
  ///
  QFutureWatcher<void> mFutureWatcher;

  /// @brief Charmap flag.
  EDyCharmapCollections   mCharmapFlag = EDyCharmapCollections::None;
  /// @brief Option flag.
  EDyOptionCollections    mOptionFlag  = EDyOptionCollections::None;
  /// @brief Loaded font file (primary, secondary) information.
  std::array<DDyFontInformation, 2> mFontInformations = {};
  /// @brief If true, secondary font will be accessed when creating atlas.
  bool                    mIsSecondaryFontAvailable = false;
  /// @brief Loaded text file information.
  DDyTextFileInformation  mTextGlyphs;
  /// @brief Callback for `mIsCanSelectWithTextGlyph` variable.
  void UpdateFlags(const bool& iNewValue);
  /// @brief If true, charmap will be start
  FDyCallVar<bool>        mIsCanSelectCharmap;
  /// @brief Callback for `mIsCanSelectWithTextGlyph` variable.
  void UpdateTextFlag(const bool& iNewValue);
  /// @brief If true, charmap will be created with text file information.
  FDyCallVar<bool>        mIsCanSelectWithTextGlyph;
  /// @brief 
  DyWindowAbout*          mChildAbout = nullptr;

  /// @brief
  std::mutex mMutexProgressIncrement;
}; 

} /// dy namespace
