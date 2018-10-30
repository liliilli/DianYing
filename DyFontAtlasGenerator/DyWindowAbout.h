#pragma once

#include <QWidget>
#include "ui_DyWindowAbout.h"

class DyWindowAbout : public QWidget
{
    Q_OBJECT

public:
    DyWindowAbout(QWidget *parent = Q_NULLPTR);
    ~DyWindowAbout();

private:
    Ui::DyWindowAbout ui;
};
