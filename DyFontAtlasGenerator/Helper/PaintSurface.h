#ifndef EXAMPLEPAINTSURFACE_H
#define EXAMPLEPAINTSURFACE_H

#include "GLOffscreenSurface.h"
#include "CoordinateBounds.h"

class CPaintSurface : public GlOffscreenSurface, public QOpenGLFunctions
{
public:
  explicit CPaintSurface(QScreen* targetScreen = nullptr, const QSize& size = QSize(1, 1));

  virtual ~CPaintSurface() override;

  ///
  /// @brief
  /// @param  texCoord
  ///
  void UpdateBufferInformation(const dy::DDyCoordinateBounds& texCoord);

  ///
  /// @brief
  /// @param  textureImage
  ///
  void BindTexturePointer(const QImage& textureImage);

  ///
  /// @brief
  ///
  void ClearSurface();

protected:
  virtual void initializeGL() override;

  virtual void resizeGL(int width, int height) override;

  virtual void paintGL() override;

private:
  ///
  /// @brief
  ///
  void pCreateVertexBufferObject();

  /// OpenGL Shader program
  QOpenGLShaderProgram* mShaderProgram  = nullptr;
  ///
  QOpenGLTexture*       mTexturePointer = nullptr;
  /// OpenGL vbo (vec2, vec2) instance.
  QOpenGLBuffer         mVboInstance{QOpenGLBuffer::VertexBuffer};
};

#endif  // EXAMPLEPAINTSURFACE_H
