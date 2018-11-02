#include "precompiled.h"
/// Header file
#include "PaintSurface.h"

#include <QPainter>

CPaintSurface::CPaintSurface(QScreen* targetScreen, const QSize& size) : GlOffscreenSurface(targetScreen, size)
{}

CPaintSurface::~CPaintSurface() {}

void CPaintSurface::initializeGL() {}

void CPaintSurface::resizeGL(int width, int height) {}

void CPaintSurface::paintGL()
{
  QPainter painter(getPaintDevice());
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  painter.drawText(20, 40, "Test"); // <-- drawing here

  painter.end();
}