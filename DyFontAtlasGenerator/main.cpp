#include "precompiled.h"
#include "DyFontAtlasGenerator.h"
#include <QtWidgets/QApplication>
#include "Helper/PaintSurface.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  DyFontAtlasGenerator w;
  w.show();

#ifdef false
  CPaintSurface paintSurface;
  paintSurface.resize(1024, 1024);
  paintSurface.render();
  QImage image = paintSurface.grabFramebuffer();
  image.save(QString("image.png"));
#endif

  return a.exec();
}
