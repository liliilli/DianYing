#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FMain.h"

class FMain : public QMainWindow
{
    Q_OBJECT

public:
    FMain(QWidget *parent = Q_NULLPTR);

private:
    Ui::FMainClass ui;
};
