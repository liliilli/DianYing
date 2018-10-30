#include "precompiled.h"
#include "DyFontAtlasGenerator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DyFontAtlasGenerator w;
    w.show();
    return a.exec();
}
