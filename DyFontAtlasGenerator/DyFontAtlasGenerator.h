#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DyFontAtlasGenerator.h"
#include "Helper/GlobalEnums.h"

struct DDyFontInformation final
{
  /// File path on focus.
  std::string fontPath  = "";
  ///
  std::string fontName  = "";
  ///
  std::string fontStyle = "";
};

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

signals:
  void SetProgressBarValue(int);

private:
  ///
  /// @brief Update `BT_Create` button activation state by verifying mCharmapFlag, mOptionFlag.
  ///
  void pUpdateBT_CreateActivation();

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
};
