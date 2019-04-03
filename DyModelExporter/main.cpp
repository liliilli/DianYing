#include "stdafx.h"
#include "FMain.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FMain w;
    w.show();
    return a.exec();
}
