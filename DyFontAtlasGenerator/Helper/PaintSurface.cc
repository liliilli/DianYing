#include "precompiled.h"
/// Header file
#include "PaintSurface.h"

#include <array>
#include <QPainter>

//!
//! Forward declaration & Local translation unit functions
//!

namespace
{

constexpr const char* vertexShader = R"dy(
attribute highp   vec2  vs_vertex;
attribute mediump vec2  vs_texCoord;
varying   mediump vec2  fs_texCoord;

void main()
{
  gl_Position = vec4((vs_vertex * 2) - 1, 0, 1);
  fs_texCoord = vs_texCoord;
}
)dy";

constexpr const char* fragmentShader = R"dy(
varying   mediump vec2  fs_texCoord;
uniform   sampler2D     uCharTexture;

void main()
{
  gl_FragColor = texture(uCharTexture, fs_texCoord);
}
)dy";

} /// ::unnamed namespace

//!
//! Implementation
//!

CPaintSurface::CPaintSurface(QScreen* targetScreen, const QSize& size) : GlOffscreenSurface(targetScreen, size) {}

CPaintSurface::~CPaintSurface()
{
  this->makeCurrent();
  // Destroy vbo instance resource.
  mVboInstance.destroy();

  if (this->mTexturePointer != nullptr)
  {
    delete this->mTexturePointer;
    this->mTexturePointer = nullptr;
  }

  if (this->mShaderProgram != nullptr)
  {
    delete this->mShaderProgram;
    this->mShaderProgram = nullptr;
  }

  this->doneCurrent();
}

void CPaintSurface::UpdateBufferInformation(const dy::DDyCoordinateBounds& texCoord)
{
  this->makeCurrent();
  this->mVboInstance.bind();
  /// Counter-clockwisely...
  /// (vec2) vert.x, vert.y (0)
  /// (vec2) text.x, text.y
  /// (vec2) vert.x, vert.y (1)
  /// ...
  auto* ptr = (float*)this->mVboInstance.map(QOpenGLBuffer::Access::WriteOnly);
  const auto& ld {texCoord.mLeftDown};
  const auto& ru {texCoord.mRightUp};

  // (0)
  *ptr = ld.X; ptr++;
  *ptr = ld.Y; ptr++; ptr += 2;
  // (1)
  *ptr = ru.X; ptr++;
  *ptr = ld.Y; ptr++; ptr += 2;
  // (2)
  *ptr = ru.X; ptr++;
  *ptr = ru.Y; ptr++; ptr += 2;
  // (3)
  *ptr = ld.X; ptr++;
  *ptr = ru.Y; ptr++;

  this->mVboInstance.unmap();
  this->mVboInstance.release();
  this->doneCurrent();
}

void CPaintSurface::BindTexturePointer(const QImage& textureImage)
{
  this->makeCurrent();

  if (this->mTexturePointer != nullptr)
  {
    delete this->mTexturePointer;
    this->mTexturePointer = nullptr;
  }

  this->mTexturePointer = new QOpenGLTexture(textureImage, QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps);
  Q_ASSERT(this->mTexturePointer->textureId() != 0);

  this->doneCurrent();
}

void CPaintSurface::ClearSurface()
{
  // make context current and bind framebuffer
  this->makeCurrent();
  this->bindFramebufferObject();

  auto painter {QPainter{this->getPaintDevice()}};
  painter.beginNativePainting();

  glClearColor(0, 0, 0, 1);
  glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  painter.endNativePainting();
  painter.end();
  this->doneCurrent();
}

void CPaintSurface::initializeGL()
{
  initializeOpenGLFunctions();
  this->pCreateVertexBufferObject();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  QOpenGLShader* vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  vshader->compileSourceCode(vertexShader);
  QOpenGLShader* fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  fshader->compileSourceCode(fragmentShader);

  this->mShaderProgram = new QOpenGLShaderProgram;
  this->mShaderProgram->addShader(vshader);
  this->mShaderProgram->addShader(fshader);
  this->mShaderProgram->bindAttributeLocation("vs_vertex",   0);
  this->mShaderProgram->bindAttributeLocation("vs_texCoord", 1);

  this->mShaderProgram->link();
  this->mShaderProgram->bind();
  //this->mShaderProgram->setUniformValue(this->mShaderProgram->uniformLocation("uCharTexture"), 0);
}

void CPaintSurface::resizeGL(int width, int height) { }

void CPaintSurface::paintGL()
{
  Q_ASSERT(this->mTexturePointer != nullptr);

  auto painter {QPainter{this->getPaintDevice()}};
  painter.beginNativePainting();
  this->mVboInstance.bind();
  this->mShaderProgram->bind();

  this->mShaderProgram->enableAttributeArray(0);
  this->mShaderProgram->enableAttributeArray(1);
  this->mShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0,                    2, 4 * sizeof(GLfloat));
  this->mShaderProgram->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(GLfloat),  2, 4 * sizeof(GLfloat));
  this->mTexturePointer->bind();

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  this->mVboInstance.release();
  this->mShaderProgram->release();
  painter.endNativePainting();
  painter.end();
}

void CPaintSurface::pCreateVertexBufferObject()
{
  static constexpr std::array<std::array<float, 2>, 4> vs_texCoord = { std::array<float, 2>{0, 0}, {1, 0}, {1, 1}, {0, 1} };

  QVector<GLfloat> vertexData;
  for (auto j {0}; j < 4; ++j)
  { // Vertex position    (vec2)
    vertexData.append(0);
    vertexData.append(0);
    // Texture coordinate (vec2)
    vertexData.append(vs_texCoord[j][0]);
    vertexData.append(vs_texCoord[j][1]);
  }

  this->mVboInstance.create();
  this->mVboInstance.bind();
  this->mVboInstance.allocate(vertexData.constData(), vertexData.count() * sizeof(GLfloat));
  this->mVboInstance.release();
}
