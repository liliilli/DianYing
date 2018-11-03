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
  this->m_context->makeCurrent(this);

  // destroy framebuffer objects
  if (this->m_fbo != nullptr)
  {
    this->m_fbo->release();
    delete this->m_fbo;
    this->m_fbo = nullptr;
  }

  if (this->m_resolvedFbo != nullptr)
  {
    this->m_resolvedFbo->release();
    delete this->m_resolvedFbo;
    this->m_resolvedFbo = nullptr;
  }

  // destroy shader
  if (this->m_blitShader != nullptr)
  {
    this->m_blitShader->release();
    delete this->m_blitShader;
    this->m_blitShader = nullptr;
  }

  // free context
  if (this->m_context != nullptr)
  {
    this->m_context->doneCurrent();
    delete this->m_context;
    this->m_context = nullptr;
  }

  // free paint device
  if (this->m_paintDevice != nullptr)
  {
    delete this->m_paintDevice;
    this->m_paintDevice = nullptr;
  }

  this->m_initialized   = false;
  this->m_updatePending = false;
  this->destroy();
}

QOpenGLContext* GlOffscreenSurface::context() const { return m_context; }

QOpenGLFunctions* GlOffscreenSurface::functions() const { return m_functions; }

GLuint GlOffscreenSurface::framebufferObjectHandle() const { return m_fbo ? m_fbo->handle() : 0; }

const QOpenGLFramebufferObject* GlOffscreenSurface::getFramebufferObject() const { return m_fbo; }

QPaintDevice* GlOffscreenSurface::getPaintDevice() const { return m_paintDevice; }

bool GlOffscreenSurface::isValid() const { return (m_initialized && m_context && m_fbo); }

void GlOffscreenSurface::makeCurrent() { makeCurrentInternal(); }

void GlOffscreenSurface::doneCurrent() { if (m_context) { m_context->doneCurrent(); } }

void GlOffscreenSurface::bindFramebufferObject()
{
  if (m_fbo)  { m_fbo->bind(); }
  else        { QOpenGLFramebufferObject::bindDefault(); }
}

void GlOffscreenSurface::makeCurrentInternal()
{
  if (isValid())  { m_context->makeCurrent(this); }
  else            { throw ("GlOffscreenSurface::makeCurrent() - Window not yet properly initialized!"); }
}

QImage GlOffscreenSurface::grabFramebuffer()
{
  std::lock_guard<std::mutex> locker{m_mutex};
  makeCurrentInternal();

  // blit framebuffer to resolve framebuffer first if needed
  if (m_fbo->format().samples() > 0)
  {
    // check if we have glFrameBufferBlit support. this is true for desktop OpenGL 3.0+, but not
    // OpenGL ES 2.0
    if (m_functions_3_0)
    { // only blit the color buffer attachment
      m_functions_3_0->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
      m_functions_3_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolvedFbo->handle());
      m_functions_3_0->glBlitFramebuffer(0, 0, bufferSize().width(),
        bufferSize().height(), 0, 0, bufferSize().width(),
        bufferSize().height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
      m_functions_3_0->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    { // we must unbind the FBO here, so we can use its texture and bind the default back-buffer
      m_functions->glBindFramebuffer(GL_FRAMEBUFFER, m_resolvedFbo->handle());
      // now use its texture for drawing in the shader --> bind shader and draw textured quad here
      // bind regular FBO again
      m_functions->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
    }

    // check if OpenGL errors happened
    if (GLenum error = m_functions->glGetError() != GL_NO_ERROR)
    {
      qDebug() << "GlOffscreenSurface::grabFramebuffer() - OpenGL error" << error;
    }

    // now grab from resolve FBO
    return GrabFramebufferInternal(m_resolvedFbo);
  }
  else
  { // no multi-sampling. grab directly from FBO
    return GrabFramebufferInternal(m_fbo);
  }
}

QImage GlOffscreenSurface::GrabFramebufferInternal(QOpenGLFramebufferObject* fbo)
{
  QImage resultImage;
  // bind framebuffer first
  m_functions->glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->handle());
  if (m_functions_3_0 != nullptr) { m_functions_3_0->glReadBuffer(GL_COLOR_ATTACHMENT0); }

  const GLenum  internalFormat  = fbo->format().internalTextureFormat();
  const bool    hasAlpha        = internalFormat == GL_RGBA || internalFormat == GL_BGRA || internalFormat == GL_RGBA8;

  if (internalFormat == GL_BGRA)
  {
    resultImage = QImage(fbo->size(), hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    m_functions->glReadPixels(0, 0, fbo->size().width(), fbo->size().height(), GL_BGRA, GL_UNSIGNED_BYTE, resultImage.bits());
  }
  else if (internalFormat == GL_RGBA
        || internalFormat == GL_RGBA8)
  {
    resultImage = QImage(fbo->size(), hasAlpha ? QImage::Format_RGBA8888 : QImage::Format_RGBX8888);
    m_functions->glReadPixels(0, 0, fbo->size().width(), fbo->size().height(), GL_RGBA, GL_UNSIGNED_BYTE, resultImage.bits());
  }
  else { qDebug() << "GlOffscreenSurface::grabFramebuffer() - Unsupported framebuffer format" << internalFormat << "!"; }

  m_functions->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
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
  m_context->makeCurrent(this);

  // make sure all paint operation have been processed
  m_functions->glFlush();

  // check if we have glFrameBufferBlit support. this is true for desktop OpenGL 3.0+, but not
  // OpenGL ES 2.0
  if (m_functions_3_0)
  {
    // if our framebuffer has multi-sampling, the resolve should be done automagically
    m_functions_3_0->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo->handle());
    m_functions_3_0->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    // blit all buffers including depth buffer for further rendering
    m_functions_3_0->glBlitFramebuffer(0, 0, bufferSize().width(),
      bufferSize().height(), 0, 0, bufferSize().width(),
      bufferSize().height(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
      GL_NEAREST);
    m_functions_3_0->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  }
  else
  {
    // we must unbind the FBO here, so we can use its texture and bind the default back-buffer
    m_functions->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // now use its texture for drawing in the shader
    // --> bind shader and draw textured quad here
    // bind regular FBO again
    m_functions->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
  }

  // check if OpenGL errors happened
  if (GLenum error = m_functions->glGetError() != GL_NO_ERROR)
  {
    qDebug() << "GlOffscreenSurface::swapBuffersInternal() - OpenGL error" << error;
  }

  // now swap back buffer to front buffer
  m_context->swapBuffers(this);
}  // GlOffscreenSurface::swapBuffersInternal


void GlOffscreenSurface::recreateFBOAndPaintDevice()
{
  if (m_context && ((m_fbo == nullptr) || (m_fbo->size() != bufferSize())))
  {
    m_context->makeCurrent(this);
    // free old FBOs
    if (m_fbo)
    {
      m_fbo->release();
      delete m_fbo;
      m_fbo = nullptr;
    }
    if (m_resolvedFbo)
    {
      m_resolvedFbo->release();
      delete m_resolvedFbo;
      m_resolvedFbo = nullptr;
    }

    // create new frame buffer
//        QOpenGLFramebufferObjectFormat format = QGLInfo::DefaultFramebufferFormat();
//        format.setSamples(QGLInfo::HasMultisamplingSupport(m_context) ? 4 : 0);
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(0);

    m_fbo = new QOpenGLFramebufferObject(bufferSize(), format);
    if (!m_fbo->isValid()) { throw ("GlOffscreenSurface::recreateFbo() - Failed to create background FBO!"); }

    // clear framebuffer
    m_fbo->bind();
    m_functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_fbo->release();

    // if multi sampling is requested and supported we need a resolve FBO
    if (format.samples() > 0)
    {
      // create resolve framebuffer with only a color attachment
      format.setAttachment(QOpenGLFramebufferObject::NoAttachment);
      format.setSamples(0);
      m_resolvedFbo = new QOpenGLFramebufferObject(bufferSize(), format);
      if (!m_resolvedFbo->isValid()) {
        throw ("GlOffscreenSurface::recreateFbo() - Failed to create resolve FBO!");
      }
      // clear resolve framebuffer
      m_resolvedFbo->bind();
      m_functions->glClear(GL_COLOR_BUFFER_BIT);
      m_resolvedFbo->release();
    }
  }
  // create paint device for painting with QPainter if needed
  if (!m_paintDevice) { m_paintDevice = new QOpenGLPaintDevice; }

  // update paint device size if needed
  if (m_paintDevice->size() != bufferSize()) { m_paintDevice->setSize(bufferSize()); }
}

void GlOffscreenSurface::initializeInternal()
{
  if (!m_initialized.exchange(true))
  {
    // create OpenGL context. we set the format requested by the user (default:
    // QWindow::requestedFormat())
    m_context = new QOpenGLContext(this);
    m_context->setFormat(format());

    if (m_context->create())
    {
      m_context->makeCurrent(this);

      // initialize the OpenGL 2.1 / ES 2.0 functions for this object
      m_functions = m_context->functions();
      m_functions->initializeOpenGLFunctions();

      // try initializing the OpenGL 3.0 functions for this object
      m_functions_3_0 = m_context->versionFunctions <QOpenGLFunctions_3_0>();
      if (m_functions_3_0) { m_functions_3_0->initializeOpenGLFunctions(); }
      else
      {
        // if we do not have OpenGL 3.0 functions, glBlitFrameBuffer is not available, so we
        // must do the blit
        // using a shader and the framebuffer texture, so we need to create the shader
        // here...
        // --> allocate m_blitShader, a simple shader for drawing a textured quad
        // --> build quad geometry, VBO, whatever
      }

      // now we have a context, create the FBO
      recreateFBOAndPaintDevice();
    }
    else
    {
      m_initialized = false;
      delete m_context;
      m_context = nullptr;
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

  if (m_initializedGL == false)
  {
    this->initializeGL();
    m_initializedGL = true;
  }

  // make context current and bind framebuffer
  this->makeCurrent();
  this->bindFramebufferObject();
  // call user paint function
  this->paintGL();
  this->doneCurrent();

  // mark that we're done with updating
  m_updatePending = false;
}

void GlOffscreenSurface::exposeEvent(QExposeEvent* e)
{
  // render window content if window is exposed
  render();
}

void GlOffscreenSurface::resizeEvent(QResizeEvent* e)
{
  // call base implementation
  resize(e->size());
  emit resized();
}

void GlOffscreenSurface::resize(const QSize& newSize)
{
  m_mutex.lock();
  // make context current first
  makeCurrent();

  m_size = QSize(newSize);

  // update FBO and paint device
  recreateFBOAndPaintDevice();
  m_mutex.unlock();
  // call user-defined resize method
  resizeGL(bufferSize().width(), bufferSize().height());
}  // GlOffscreenSurface::resize

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

void GlOffscreenSurface::resize(int w, int h) { resize(QSize(w, h)); }

void GlOffscreenSurface::InitializeContext()
{
  // check if we need to initialize stuff
  std::lock_guard <std::mutex> locker(m_mutex);
  initializeInternal();

  if (m_initializedGL == false)
  {
    this->initializeGL();
    m_initializedGL = true;
  }
}

QSize GlOffscreenSurface::bufferSize() const { return (m_size); }
