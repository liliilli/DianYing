#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DyFontAtlasGenerator.h"

class DyFontAtlasGenerator : public QMainWindow
{
    Q_OBJECT

public:
    DyFontAtlasGenerator(QWidget *parent = Q_NULLPTR);

private:
    Ui::DyFontAtlasGeneratorClass ui;
};
