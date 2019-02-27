#pragma once

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
  ~DyWindowAbout();

  /// @brief
  /// @param parentWindow
  void SetParentMainWindow(DyFontAtlasGenerator& parentWindow);

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
