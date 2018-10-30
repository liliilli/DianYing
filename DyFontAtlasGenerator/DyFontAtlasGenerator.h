#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DyFontAtlasGenerator.h"
#include "GlobalEnums.h"

class DyFontAtlasGenerator : public QMainWindow
{
  Q_OBJECT

public:
  DyFontAtlasGenerator(QWidget *parent = Q_NULLPTR);

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

private:
  ///
  /// @brief Update `BT_Create` button activation state by verifying mCharmapFlag, mOptionFlag.
  ///
  void pUpdateBT_CreateActivation();

  Ui::DyFontAtlasGeneratorClass ui;

  /// Charmap flag.
  dy::EDyCharmapCollections mCharmapFlag = dy::EDyCharmapCollections::None;
  /// Option flag.
  dy::EDyOptionCollections  mOptionFlag  = dy::EDyOptionCollections::None;
  /// File path on focus.
  QString mFilePath = "";
};
