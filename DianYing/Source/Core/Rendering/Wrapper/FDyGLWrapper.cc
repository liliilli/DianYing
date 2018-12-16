#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLWindowContextDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLShaderFragmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLBufferDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLVaoBindDescriptor.h>

namespace dy
{

std::mutex FDyGLWrapper::mTextureMutex;

GLFWwindow* FDyGLWrapper::CreateGLWindow(_MIN_ const PDyGLWindowContextDescriptor& descriptor)
{
  MDY_ASSERT(descriptor.mWindowName.empty() == false, "Window name must not be empty.");
  MDY_ASSERT(descriptor.mWindowSize.X > 0 && descriptor.mWindowSize.Y > 0, "Window size must be valid.");

  if (descriptor.mIsUsingDefaultDoubleBuffer == true) { glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); }
  if (descriptor.mIsUsingDefaultMSAA == true)         { glfwWindowHint(GLFW_SAMPLES, 4); }
  if (descriptor.mIsWindowShouldFocus == true)        { glfwWindowHint(GLFW_FOCUSED, GL_TRUE); }

  if (descriptor.mIsWindowResizable == true)          { glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); }
  else                                                { glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); }
  if (descriptor.mIsWindowVisible == true)            { glfwWindowHint(GLFW_VISIBLE, GL_TRUE); }
  else                                                { glfwWindowHint(GLFW_VISIBLE, GL_FALSE); }

  GLFWmonitor* ptrMonitor = nullptr;
  if (descriptor.mIsWindowFullScreen == true) { ptrMonitor = glfwGetPrimaryMonitor(); }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  return glfwCreateWindow(
      descriptor.mWindowSize.X, descriptor.mWindowSize.Y,
      descriptor.mWindowName.c_str(),
      ptrMonitor, descriptor.mSharingContext);
}

void FDyGLWrapper::CreateGLContext(_MIN_ GLFWwindow* window)
{
  glfwMakeContextCurrent(window);
}

std::optional<TU32> FDyGLWrapper::CreateTexture(_MIN_ const PDyGLTextureDescriptor& descriptor)
{
  // Validation check.
  MDY_ASSERT(descriptor.mImageFormat != GL_NONE, "Texture Image format must be specified.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer), "Texture Image buffer must not be null.");
  MDY_ASSERT(descriptor.mTextureSize.X > 0 && descriptor.mTextureSize.Y > 0, "Texture size must be positive value.");
  MDY_ASSERT(descriptor.mType != EDyTextureStyleType::NoneError, "Texture Image type must be specified.");
  if (descriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mPtrParameterList), "Parameter list must not be null.");
    MDY_ASSERT(DyCheckTextureParameterList(*descriptor.mPtrParameterList) == DY_SUCCESS, "Texture Parameter validation failed.");
  }

  TU32 mTextureResourceId = MDY_INITIALIZE_DEFUINT;
  GLenum glTextureType    = GL_NONE;
  switch (descriptor.mType)
  {
  case EDyTextureStyleType::D1: glTextureType = GL_TEXTURE_1D; break;
  case EDyTextureStyleType::D2: glTextureType = GL_TEXTURE_2D; break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  { // Critical section.
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);

    // Make texture.
    switch (descriptor.mType)
    {
    case EDyTextureStyleType::D1:
    {
      glGenTextures(1, &mTextureResourceId);
      glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, descriptor.mTextureSize.X, 0, descriptor.mImageFormat, GL_UNSIGNED_BYTE, descriptor.mPtrBuffer->data());
    } break;
    case EDyTextureStyleType::D2:
    { // Border parameter must be 0.
      glGenTextures(1, &mTextureResourceId);
      glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mTextureSize), 0, descriptor.mImageFormat, GL_UNSIGNED_BYTE, descriptor.mPtrBuffer->data());
    } break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
    }

    // Make mipmap by following option.
    if (descriptor.mIsUsingDefaultMipmap == true) { glGenerateMipmap(glTextureType); }

    // Set texture parameters.
    if (descriptor.mIsUsingCustomizedParameter == true)
    { // Apply parameter option list to attachment.
      bool isUsingClampToBorder = false;
      for (const auto& parameter : *descriptor.mPtrParameterList)
      { // Check there is ClmapToBorder for border coloring and set parameter
        if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isUsingClampToBorder = true; }
        glTexParameteri(glTextureType, DyGetParameterNameValue(parameter.mParameterOption), DyGetParameterValueValue(parameter.mParameterValue));
      }

      if (isUsingClampToBorder == true)
      { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
        glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, descriptor.mBorderColor.Data());
      }
    }
    glBindTexture(glTextureType, 0);
    glFlush();
  }

  return mTextureResourceId;
}

void FDyGLWrapper::DeleteTexture(_MIN_ const TU32 validTextureId)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  glDeleteTextures(1, &validTextureId);
}

std::optional<TU32> FDyGLWrapper::CreateShaderFragment(_MIN_ const PDyGLShaderFragmentDescriptor& descriptor)
{
  TU32 shaderFragmentId;

  // Get OpenGL Shader type
  {
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
    switch (descriptor.mType)
    {
    case EDyShaderFragmentType::Vertex:   shaderFragmentId = glCreateShader(GL_VERTEX_SHADER);          break;
    case EDyShaderFragmentType::Hull:     shaderFragmentId = glCreateShader(GL_TESS_CONTROL_SHADER);    break;
    case EDyShaderFragmentType::Domain:   shaderFragmentId = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
    case EDyShaderFragmentType::Geometry: shaderFragmentId = glCreateShader(GL_GEOMETRY_SHADER);        break;
    case EDyShaderFragmentType::Pixel:    shaderFragmentId = glCreateShader(GL_FRAGMENT_SHADER);        break;
    default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
    }
  }

  MDY_ASSERT(shaderFragmentId > 0,                        "Failed to create shader.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer),  "Shader fragment buffer must not be null.");

  {
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
    glShaderSource(shaderFragmentId, 1, &descriptor.mPtrBuffer, nullptr);
    glCompileShader(shaderFragmentId);
  }

  return shaderFragmentId;
}

void FDyGLWrapper::DeleteShaderFragment(_MIN_ const TU32 shaderFragmentId)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  glDeleteShader(shaderFragmentId);
}

std::optional<TU32> FDyGLWrapper::CreateShaderProgram(_MIN_ const TFragmentList& fragmentList)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  const TU32 shaderProgramId = glCreateProgram();

  // Create GlShaderProgram, and attach fragments to link them.
  for (auto& [shaderFragmentType, shaderFragmentId] : fragmentList) { glAttachShader(shaderProgramId, shaderFragmentId); }
  glLinkProgram(shaderProgramId);
  for (auto& [shaderFragmentType, shaderFragmentId] : fragmentList) { glDetachShader(shaderProgramId, shaderFragmentId); }

  return shaderProgramId;
}

void FDyGLWrapper::DeleteShaderProgram(_MIN_ const TU32 shaderProgramId)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  glDeleteShader(shaderProgramId);
}

#define MDY_GL_NONE 0
#define MDY_GL_NONE_VAO 0

std::optional<TU32> FDyGLWrapper::CreateBuffer(_MIN_ const PDyGLBufferDescriptor& descriptor)
{
  TU32 id = MDY_INITIALIZE_DEFUINT;

  switch (descriptor.mBufferType)
  {
  case EDyDirectBufferType::VertexBuffer:
  { // VBO
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, descriptor.mBufferByteSize, descriptor.mPtrBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, MDY_GL_NONE);
    glFlush();
  } break;
  case EDyDirectBufferType::ElementBuffer:
  { // EBO
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, descriptor.mBufferByteSize, descriptor.mPtrBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MDY_GL_NONE);
    glFlush();
  } break;
  case EDyDirectBufferType::UniformBuffer:
  case EDyDirectBufferType::TransformFeedback:
  case EDyDirectBufferType::ShaderStorage:
    MDY_NOT_IMPLEMENTED_ASSERT();
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  return id;
}

void FDyGLWrapper::DeleteBuffer(_MIN_ const TU32 directBufferId)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  glDeleteBuffers(1, &directBufferId);
}

TU32 FDyGLWrapper::CreateVertexArrayObject()
{
  TU32 vaoId = MDY_INITIALIZE_DEFUINT;
  {
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
    glGenVertexArrays(1, &vaoId);
  }
  return vaoId;
}

void FDyGLWrapper::BindVertexArrayObject(const PDyGLVaoBindDescriptor& iDescriptor)
{
  using EFormatType = PDyGLVaoBindDescriptor::EFormatType;

  if (iDescriptor.mIsUsingDefaultDyAttributeModel == true)
  {
    auto descriptor = GetDefaultAttributeFormatDescriptor();
    descriptor.mVaoId      = iDescriptor.mVaoId;
    descriptor.mBoundVboId = iDescriptor.mBoundVboId;
    descriptor.mBoundEboId = iDescriptor.mBoundEboId;
    BindVertexArrayObject(descriptor);
    return;
  }

  const auto size = iDescriptor.mAttributeFormatList.size();
  { // Critical section.
    MDY_SYNC_LOCK_GUARD (FDyGLWrapper::mTextureMutex);
    glBindVertexArray   (iDescriptor.mVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, iDescriptor.mBoundVboId);
    glBindVertexBuffer  (0, iDescriptor.mBoundVboId, iDescriptor.mOffsetByteSize, iDescriptor.mStrideByteSize);
    if (iDescriptor.mBoundEboId > 0) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iDescriptor.mBoundEboId); }

    for (auto i = 0u; i < size; ++i)
    { // Set up vbo attribute formats.
      const auto&   attributeFormat = iDescriptor.mAttributeFormatList[i];
      const GLenum  type            = DyGetGLTypeFrom(attributeFormat.mType);
      const GLenum  isMustNormalized= attributeFormat.mIsMustNormalized == true ? GL_TRUE : GL_FALSE;

      glEnableVertexAttribArray(i);
      if (type == GL_INT) { glVertexAttribIFormat(i, attributeFormat.mElementCount, type, attributeFormat.mOffsetByteSize); }
      else                { glVertexAttribFormat(i, attributeFormat.mElementCount, type, isMustNormalized, attributeFormat.mOffsetByteSize); }
      glVertexAttribBinding(i, 0);
    }
    glBindVertexArray(MDY_GL_NONE_VAO);
    glFlush();
  }
}

void FDyGLWrapper::DeleteVertexArrayObject(_MIN_ const TU32 vertexArrayObjectId)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  glDeleteVertexArrays(1, &vertexArrayObjectId);
}

} /// ::dy namespace