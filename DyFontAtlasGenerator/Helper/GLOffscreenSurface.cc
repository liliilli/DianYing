#include "precompiled.h"
/// Header file
#include "GlOffscreenSurface.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QPainter>

GlOffscreenSurface::GlOffscreenSurface(QScreen* targetScreen, const QSize& size) : QOffscreenSurface(targetScreen) , m_size(size)
{
  this->setFormat(QSurfaceFormat::defaultFormat());

  this->m_initialized = false;
  this->m_updatePending = false;

  this->create();  // Some platforms require this function to be called on the main (GUI) thread
  this->initializeInternal();
}

GlOffscreenSurface::~GlOffscreenSurface()
{
  // to delete the FBOs we first need to make the context current
  this->mGLContext->makeCurrent(this);

  // destroy framebuffer objects
  if (this->mFboInstance != nullptr)
  {
    this->mFboInstance->release();
    delete this->mFboInstance;
    this->mFboInstance = nullptr;
  }

  // destroy shader
  if (this->m_blitShader != nullptr)
  {
    this->m_blitShader->release();
    delete this->m_blitShader;
    this->m_blitShader = nullptr;
  }

  // free context
  if (this->mGLContext != nullptr)
  {
    this->mGLContext->doneCurrent();
    delete this->mGLContext;
    this->mGLContext = nullptr;
  }

  // free paint device
  if (this->mPaintDeviceInstance != nullptr)
  {
    delete this->mPaintDeviceInstance;
    this->mPaintDeviceInstance = nullptr;
  }

  this->m_initialized   = false;
  this->m_updatePending = false;
  this->destroy();
}

QOpenGLContext* GlOffscreenSurface::GetGLContext() const { return mGLContext; }

QOpenGLFunctions* GlOffscreenSurface::GetGLFunctions() const { return mGLFunctions; }

GLuint GlOffscreenSurface::GetGLFBOHandleId() const { return mFboInstance ? mFboInstance->handle() : 0; }

const QOpenGLFramebufferObject* GlOffscreenSurface::GetGLFBORawPointer() const { return mFboInstance; }

QPaintDevice* GlOffscreenSurface::GetPaintDevice() const { return mPaintDeviceInstance; }

bool GlOffscreenSurface::IsValid() const { return (m_initialized && mGLContext && mFboInstance); }

void GlOffscreenSurface::MakeRenderingCurrentContext() { MakeCurrentInternalContext(); }

void GlOffscreenSurface::DoneRenderingCurrentContext() { if (mGLContext) { mGLContext->doneCurrent(); } }

void GlOffscreenSurface::BindGLFBO()
{
  if (mFboInstance)  { mFboInstance->bind(); }
  else        { QOpenGLFramebufferObject::bindDefault(); }
}

void GlOffscreenSurface::MakeCurrentInternalContext()
{
  if (IsValid())  { mGLContext->makeCurrent(this); }
  else            { throw ("GlOffscreenSurface::makeCurrent() - Window not yet properly initialized!"); }
}

QImage GlOffscreenSurface::GetImageFromGLFBO()
{
  std::lock_guard<std::mutex> locker{m_mutex};
  MakeCurrentInternalContext();
  // blit framebuffer to resolve framebuffer first if needed
  return GrabFramebufferInternal(mFboInstance);
}

QImage GlOffscreenSurface::GrabFramebufferInternal(QOpenGLFramebufferObject* fbo)
{
  QImage resultImage;

  // bind framebuffer first
  mGLFunctions->glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->handle());
  if (mGLFunctionsAbove30 != nullptr) { mGLFunctionsAbove30->glReadBuffer(GL_COLOR_ATTACHMENT0); }

  const GLenum  internalFormat  = fbo->format().internalTextureFormat();
  const bool    hasAlpha        = internalFormat == GL_RGBA || internalFormat == GL_BGRA || internalFormat == GL_RGBA8;

  if (internalFormat == GL_BGRA)
  {
    resultImage = QImage(fbo->size(), hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    mGLFunctions->glReadPixels(0, 0, fbo->size().width(), fbo->size().height(), GL_BGRA, GL_UNSIGNED_BYTE, resultImage.bits());
  }
  else if (internalFormat == GL_RGBA || internalFormat == GL_RGBA8)
  {
    resultImage = QImage(fbo->size(), hasAlpha ? QImage::Format_RGBA8888 : QImage::Format_RGBX8888);
    mGLFunctions->glReadPixels(0, 0, fbo->size().width(), fbo->size().height(), GL_RGBA, GL_UNSIGNED_BYTE, resultImage.bits());
  }
  else { qDebug() << "GlOffscreenSurface::grabFramebuffer() - Unsupported framebuffer format" << internalFormat << "!"; }

  mGLFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFboInstance->handle());
  return resultImage.mirrored();
}

void GlOffscreenSurface::swapBuffers()
{
  swapBuffersInternal();
  emit frameSwapped();
}

void GlOffscreenSurface::swapBuffersInternal()
{
  // blit framebuffer to back buffer
  this->mGLContext->makeCurrent(this);
  // make sure all paint operation have been processed
  this->mGLFunctions->glFlush();

  // check if we have glFrameBufferBlit support. this is true for desktop OpenGL 3.0+, but not OpenGL ES 2.0
  if (this->mGLFunctionsAbove30 != nullptr)
  {
    // if our framebuffer has multi-sampling, the resolve should be done automagically
    this->mGLFunctionsAbove30->glBindFramebuffer(GL_READ_FRAMEBUFFER, mFboInstance->handle());
    this->mGLFunctionsAbove30->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // blit all buffers including depth buffer for further rendering
    this->mGLFunctionsAbove30->glBlitFramebuffer(0, 0, GetBufferSize().width(),
        GetBufferSize().height(), 0, 0, GetBufferSize().width(),
        GetBufferSize().height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
        GL_NEAREST);
    this->mGLFunctionsAbove30->glBindFramebuffer(GL_FRAMEBUFFER, mFboInstance->handle());
  }
  else
  {
    // we must unbind the FBO here, so we can use its texture and bind the default back-buffer
    // now use its texture for drawing in the shader and bind shader and draw textured quad here bind regular FBO again
    this->mGLFunctions->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->mGLFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFboInstance->handle());
  }

  // check if OpenGL errors happened
  if (const GLenum error = this->mGLFunctions->glGetError(); error != GL_NO_ERROR)
  {
    qDebug() << "GlOffscreenSurface::swapBuffersInternal() - OpenGL error" << error;
  }

  // now swap back buffer to front buffer
  this->mGLContext->swapBuffers(this);
}

void GlOffscreenSurface::RecreateFBOAndPaintDevice()
{
  if (mGLContext && ((mFboInstance == nullptr) || (mFboInstance->size() != GetBufferSize())))
  {
    mGLContext->makeCurrent(this);
    // free old FBOs
    if (mFboInstance)
    {
      mFboInstance->release();
      delete mFboInstance;
      mFboInstance = nullptr;
    }

    // Create new frame buffer
    QOpenGLFramebufferObjectFormat fboFormat {}; fboFormat.setSamples(0);

    this->mFboInstance = new QOpenGLFramebufferObject(GetBufferSize(), fboFormat);
    if (this->mFboInstance->isValid() == false)
    {
      throw "GlOffscreenSurface::recreateFbo() - Failed to create background FBO!";
    }
    // Add color attachment (GL_COLOR_ATTACHMENT1) for ping-pong.
    // @TODO UNCOMMENT THIS
    //this->mFboInstance->addColorAttachment(GetBufferSize());

    // Clear framebuffer
    this->mFboInstance->bind();
    this->mGLFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    this->mFboInstance->release();
  }
  // Create paint device for painting with QPainter if needed
  if (this->mPaintDeviceInstance == nullptr)
  {
    this->mPaintDeviceInstance = new QOpenGLPaintDevice();
  }

  // Update paint device size if needed
  if (this->mPaintDeviceInstance->size() != this->GetBufferSize())
  {
    this->mPaintDeviceInstance->setSize(GetBufferSize());
  }
}

void GlOffscreenSurface::initializeInternal()
{
  if (!this->m_initialized.exchange(true))
  {
    // create OpenGL context. we set the format requested by the user (default: QWindow::requestedFormat())
    this->mGLContext = new QOpenGLContext(this);
    this->mGLContext->setFormat(format());

    if (this->mGLContext->create() == true)
    {
      this->mGLContext->makeCurrent(this);

      // initialize the OpenGL 2.1 / ES 2.0 functions for this object
      this->mGLFunctions = this->mGLContext->functions();
      this->mGLFunctions->initializeOpenGLFunctions();

      // try initializing the OpenGL 3.0 functions for this object
      this->mGLFunctionsAbove30 = mGLContext->versionFunctions <QOpenGLFunctions_3_0>();
      if (this->mGLFunctionsAbove30 != nullptr) { mGLFunctionsAbove30->initializeOpenGLFunctions(); }
      else                                      { throw "Failed to create OpenGL function above version 3.0."; }

      // now we have a context, create the FBO
      RecreateFBOAndPaintDevice();
    }
    else
    {
      m_initialized = false;
      delete mGLContext;
      mGLContext = nullptr;
      throw ("Failed to create OpenGL context!");
    }
  }
}

void GlOffscreenSurface::update()
{
  // only queue an update if there's not already an update pending
  if (!m_updatePending.exchange(true))
  {
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
  }
}

void GlOffscreenSurface::render()
{
  std::lock_guard <std::mutex> locker(m_mutex);
  // check if we need to initialize stuff
  initializeInternal();
  // check if we need to call the user initialization
//    makeCurrent(); // TODO: may be makeCurrent() must be here, as noted for QOpenGLWidget.initializeGL()

  if (mIsOpenGLInitialized == false)
  {
    this->initializeGL();
    mIsOpenGLInitialized = true;
  }

  // make context current and bind framebuffer
  this->MakeRenderingCurrentContext();
  this->BindGLFBO();
  // call user paint function
  this->paintGL();
  this->DoneRenderingCurrentContext();

  // mark that we're done with updating
  m_updatePending = false;
}

void GlOffscreenSurface::exposeEvent(QExposeEvent* e)
{ // render window content if window is exposed
  render();
}

void GlOffscreenSurface::resizeEvent(QResizeEvent* e)
{ // call base implementation
  resize(e->size());
  emit resized();
}

void GlOffscreenSurface::resize(const QSize& newSize)
{
  m_mutex.lock();
  // make context current first
  this->MakeRenderingCurrentContext();

  m_size = QSize(newSize);

  // update FBO and paint device
  this->RecreateFBOAndPaintDevice();
  m_mutex.unlock();

  // call user-defined resize method
  this->resizeGL(GetBufferSize().width(), GetBufferSize().height());
}

bool GlOffscreenSurface::event(QEvent* event)
{
  switch (event->type())
  {
  case QEvent::UpdateLater:
    update();
    return true;
  case QEvent::UpdateRequest:
    render();
    return true;
  default:
    return false;
  }
}

void GlOffscreenSurface::resize(int w, int h) { this->resize(QSize(w, h)); }

void GlOffscreenSurface::InitializeContext()
{ // check if we need to initialize stuff
  std::lock_guard <std::mutex> locker(m_mutex);
  this->initializeInternal();

  if (this->mIsOpenGLInitialized == false)
  {
    this->initializeGL();
    this->mIsOpenGLInitialized = true;
  }
}

QSize GlOffscreenSurface::GetBufferSize() const
{
  return this->m_size;
}
