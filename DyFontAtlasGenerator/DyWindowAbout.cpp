#include "precompiled.h"
/// Header file
#include "DyWindowAbout.h"

#include "DyFontAtlasGenerator.h"

DyWindowAbout::DyWindowAbout(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  // Connect signal and slot.
  connect(ui.PB_OK, &QPushButton::clicked,                this,                     &QWidget::close);
  connect(this,     &DyWindowAbout::ReturnFocusToParent,  this->mParentMainWindow,  &DyFontAtlasGenerator::ResumeFocus);
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
