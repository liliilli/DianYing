#ifndef EXAMPLEPAINTSURFACE_H
#define EXAMPLEPAINTSURFACE_H

#include "GLOffscreenSurface.h"

class CPaintSurface : public GlOffscreenSurface
{
public:
    explicit CPaintSurface(QScreen* targetScreen = nullptr, const QSize& size = QSize (1, 1));

    virtual ~CPaintSurface() override;

protected:
    virtual void initializeGL() override;

    virtual void resizeGL(int width, int height) override;

    virtual void paintGL() override;
};


#endif  // EXAMPLEPAINTSURFACE_H
