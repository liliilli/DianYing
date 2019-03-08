#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
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
#include <thread>

#include <Dy/Core/Rendering/Wrapper/PDyGLWindowContextDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLTextureDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLShaderFragmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLBufferDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLAttachmentDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLFrameBufferDescriptor.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLVaoBindDescriptor.h>
#include <Dy/Core/Resource/Internal/ShaderType.h>
#include <Dy/Meta/Type/Mesh/DDyGLVaoBindInformation.h>
#include "Dy/Helper/Type/Area2D.h"

#define MDY_GL_NONE 0
#define MDY_GL_NONE_VAO 0
#if defined(NDEBUG) == false
#define MDY_CHECK_OPENGL() \
  { const auto _ = glGetError(); MDY_ASSERT_MSG(_ == GL_NO_ERROR, "OpenGL Command failed."); }
#else
#define MDY_CHECK_OPENGL() (void)0;
#endif

//!
//! Forward declaration
//! 

namespace
{

MDY_NODISCARD dy::EDyAttributeVariableType 
DyGlGetAttributeVariableTypeFrom(_MIN_ GLenum type) noexcept
{
  switch (type)
  {
  case GL_BOOL:                             return dy::EDyAttributeVariableType::Bool;
  case GL_FLOAT:      case GL_DOUBLE:       return dy::EDyAttributeVariableType::Float;
  case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:  return dy::EDyAttributeVariableType::Vector2;
  case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:  return dy::EDyAttributeVariableType::Vector3;
  case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:  return dy::EDyAttributeVariableType::Vector4;
  case GL_FLOAT_MAT2: case GL_DOUBLE_MAT2:  return dy::EDyAttributeVariableType::Matrix2;
  case GL_FLOAT_MAT3: case GL_DOUBLE_MAT3:  return dy::EDyAttributeVariableType::Matrix3;
  case GL_FLOAT_MAT4: case GL_DOUBLE_MAT4:  return dy::EDyAttributeVariableType::Matrix4;
  case GL_INT:                              return dy::EDyAttributeVariableType::Integer;
  case GL_INT_VEC2:                         return dy::EDyAttributeVariableType::IVec2;
  case GL_INT_VEC3:                         return dy::EDyAttributeVariableType::IVec3;
  case GL_INT_VEC4:                         return dy::EDyAttributeVariableType::IVec4;
  default: return dy::EDyAttributeVariableType::NoneError;
  }
}

MDY_NODISCARD dy::EDyUniformVariableType 
DyGlGetUniformVariableTypeFrom(_MIN_ GLenum type) noexcept
{
  switch (type)
  {
  case GL_BOOL:                             return dy::EDyUniformVariableType::Bool;
  case GL_FLOAT:      case GL_DOUBLE:       return dy::EDyUniformVariableType::Float;
  case GL_FLOAT_VEC2: case GL_DOUBLE_VEC2:  return dy::EDyUniformVariableType::Vector2;
  case GL_FLOAT_VEC3: case GL_DOUBLE_VEC3:  return dy::EDyUniformVariableType::Vector3;
  case GL_FLOAT_VEC4: case GL_DOUBLE_VEC4:  return dy::EDyUniformVariableType::Vector4;
  case GL_FLOAT_MAT2: case GL_DOUBLE_MAT2:  return dy::EDyUniformVariableType::Matrix2;
  case GL_FLOAT_MAT3: case GL_DOUBLE_MAT3:  return dy::EDyUniformVariableType::Matrix3;
  case GL_FLOAT_MAT4: case GL_DOUBLE_MAT4:  return dy::EDyUniformVariableType::Matrix4;
  case GL_INT:                              return dy::EDyUniformVariableType::Integer;
  case GL_UNSIGNED_INT:                     return dy::EDyUniformVariableType::Unsigned;
  case GL_INT_VEC2:                         return dy::EDyUniformVariableType::IVec2;
  case GL_INT_VEC3:                         return dy::EDyUniformVariableType::IVec3;
  case GL_INT_VEC4:                         return dy::EDyUniformVariableType::IVec4;
  case GL_SAMPLER_1D:                       return dy::EDyUniformVariableType::Texture1D;
  case GL_SAMPLER_2D:                       return dy::EDyUniformVariableType::Texture2D;
  case GL_SAMPLER_2D_RECT:                  return dy::EDyUniformVariableType::Texture2DRectangle;
  case GL_SAMPLER_2D_ARRAY:                 return dy::EDyUniformVariableType::Texture2DArray;
  case GL_SAMPLER_2D_ARRAY_SHADOW:          return dy::EDyUniformVariableType::Texture2DShadowArray;
  case GL_SAMPLER_CUBE:                     return dy::EDyUniformVariableType::Texture2DCubemap;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(dy::EDyUniformVariableType::NoneError);
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

std::mutex FDyGLWrapper::mGLCriticalSectionMutex;

GLFWwindow* FDyGLWrapper::CreateGLWindow(_MIN_ const PDyGLWindowContextDescriptor& descriptor)
{
  MDY_ASSERT_MSG(descriptor.mWindowName.empty() == false, "Window name must not be empty.");
  MDY_ASSERT_MSG(descriptor.mWindowSize.X > 0 && descriptor.mWindowSize.Y > 0, "Window size must be valid.");

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
  MDY_ASSERT_MSG(descriptor.mImageFormat != GL_NONE, "Texture Image format must be specified.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer), "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(descriptor.mTextureSize.X > 0 && descriptor.mTextureSize.Y > 0, "Texture size must be positive value.");
  MDY_ASSERT_MSG(descriptor.mType != EDyTextureStyleType::NoneError, "Texture Image type must be specified.");
  if (descriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrParameterList), "Parameter list must not be null.");
    MDY_ASSERT_MSG(DyCheckTextureParameterList(*descriptor.mPtrParameterList) == DY_SUCCESS, "Texture Parameter validation failed.");
  }

  TU32 mTextureResourceId = MDY_INITIALIZE_DEFUINT;

  // Make texture.
  switch (descriptor.mType)
  {
  case EDyTextureStyleType::D1:
  {
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, descriptor.mTextureSize.X, 0, 
        descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mPtrBuffer->data());
  } break;
  case EDyTextureStyleType::D2:
  { // Border parameter must be 0.
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mTextureSize), 0, 
        descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mPtrBuffer->data());
  } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }
  MDY_CHECK_OPENGL();

  // Make mipmap by following option.
  const GLenum glTextureType = DyGLGetLowTextureType(descriptor.mType);
  if (descriptor.mIsUsingDefaultMipmap == true) { glGenerateMipmap(glTextureType); }
  MDY_CHECK_OPENGL();

  // Set texture parameters.
  if (descriptor.mIsUsingCustomizedParameter == true)
  { // Apply parameter option list to attachment.
    bool isUsingClampToBorder = false;
    for (const auto& parameter : *descriptor.mPtrParameterList)
    { // Check there is ClmapToBorder for border coloring and set parameter
      if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isUsingClampToBorder = true; }
      glTexParameteri(glTextureType, DyGetTexParameterNameValue(parameter.mParameterOption), DyGetTexParameterValueValue(parameter.mParameterValue));
    }

    if (isUsingClampToBorder == true)
    { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
      glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, descriptor.mBorderColor.Data());
    }
  }
  glBindTexture(glTextureType, 0);
  glFlush();
  MDY_CHECK_OPENGL();

  return mTextureResourceId;
}

std::optional<TU32> FDyGLWrapper::CreateTexture(const PDyGLTextureCubemapDescriptor& descriptor)
{
  // Validation check.
  MDY_ASSERT_MSG(descriptor.mImageFormat != GL_NONE, "Texture Image format must be specified.");
  MDY_ASSERT_MSG(descriptor.mType == EDyTextureStyleType::D2Cubemap, "Texture Image type must be D2Cubemap.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mTopBuffer),    "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mBottomBuffer), "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mFrontBuffer),  "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mBackBuffer),   "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mRightBuffer),  "Texture Image buffer must not be null.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mLeftBuffer),   "Texture Image buffer must not be null.");

  if (descriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrParameterList), "Parameter list must not be null.");
    MDY_ASSERT_MSG(DyCheckTextureParameterList(*descriptor.mPtrParameterList) == DY_SUCCESS, "Texture Parameter validation failed.");
  }

  TU32 mTextureResourceId = MDY_INITIALIZE_DEFUINT;
  { // Critical section.
    

    // Make cubemap texture.  
    glGenTextures(1, &mTextureResourceId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureResourceId);
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mTopSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mTopBuffer->data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mBottomSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mBottomBuffer->data());
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mRightSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mRightBuffer->data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mLeftSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mLeftBuffer->data());
    //
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mFrontSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mFrontBuffer->data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mBackSize), 0, 
          descriptor.mImageFormat, descriptor.mImagePixelType, descriptor.mBackBuffer->data());
    MDY_CHECK_OPENGL();

    // Make mipmap by following option.
    const GLenum glTextureType = DyGLGetLowTextureType(descriptor.mType);
    if (descriptor.mIsUsingDefaultMipmap == true) { glGenerateMipmap(glTextureType); }
    MDY_CHECK_OPENGL();

    // Set texture parameters.
    if (descriptor.mIsUsingCustomizedParameter == true)
    { // Apply parameter option list to attachment.
      bool isUsingClampToBorder = false;
      for (const auto& parameter : *descriptor.mPtrParameterList)
      { // Check there is ClmapToBorder for border coloring and set parameter
        if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isUsingClampToBorder = true; }
        glTexParameteri(glTextureType, DyGetTexParameterNameValue(parameter.mParameterOption), DyGetTexParameterValueValue(parameter.mParameterValue));
      }

      if (isUsingClampToBorder == true)
      { // If isThisAttachmentUsingClampToBorder is true, apply border color to texture.
        glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, descriptor.mBorderColor.Data());
      }
    }
    glBindTexture(glTextureType, 0);
    glFlush();
    MDY_CHECK_OPENGL();
  }

  return mTextureResourceId;
}

void FDyGLWrapper::DeleteTexture(_MIN_ const TU32 validTextureId)
{
  glDeleteTextures(1, &validTextureId);
}

std::optional<TU32> FDyGLWrapper::CreateShaderFragment(_MIN_ const PDyGLShaderFragmentDescriptor& descriptor)
{
  TU32 shaderFragmentId;

  // Get OpenGL Shader type
  switch (descriptor.mType)
  {
  case EDyShaderFragmentType::Vertex:   shaderFragmentId = glCreateShader(GL_VERTEX_SHADER);          break;
  case EDyShaderFragmentType::Hull:     shaderFragmentId = glCreateShader(GL_TESS_CONTROL_SHADER);    break;
  case EDyShaderFragmentType::Domain:   shaderFragmentId = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
  case EDyShaderFragmentType::Geometry: shaderFragmentId = glCreateShader(GL_GEOMETRY_SHADER);        break;
  case EDyShaderFragmentType::Pixel:    shaderFragmentId = glCreateShader(GL_FRAGMENT_SHADER);        break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  MDY_ASSERT_MSG(shaderFragmentId > 0,                        "Failed to create shader.");
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer),  "Shader fragment buffer must not be null.");

  // Compile shader fragment.
  glShaderSource(shaderFragmentId, 1, &descriptor.mPtrBuffer, nullptr);
  glCompileShader(shaderFragmentId);
  return shaderFragmentId;
}

void FDyGLWrapper::DeleteShaderFragment(_MIN_ const TU32 shaderFragmentId)
{
  glDeleteShader(shaderFragmentId);
}

std::optional<TU32> FDyGLWrapper::CreateShaderProgram(_MIN_ const TFragmentList& fragmentList)
{
  const TU32 shaderProgramId = glCreateProgram();

  // Create GlShaderProgram, and attach fragments to link them.
  for (auto& [shaderFragmentType, shaderFragmentId] : fragmentList) 
  { 
    glAttachShader(shaderProgramId, shaderFragmentId); 
  }
  glLinkProgram(shaderProgramId);
  // Delete obsolete & redundant fragment item.
  for (auto& [shaderFragmentType, shaderFragmentId] : fragmentList) 
  { 
    glDetachShader(shaderProgramId, shaderFragmentId); 
  }

  return shaderProgramId;
}

void FDyGLWrapper::DeleteShaderProgram(_MIN_ const TU32 shaderProgramId)
{
  glDeleteProgram(shaderProgramId);
}

void FDyGLWrapper::UseShaderProgram(_MIN_ TU32 iShaderProgramId)
{
  glUseProgram(iShaderProgramId);
}

void FDyGLWrapper::DisuseShaderProgram()
{
  glUseProgram(0);
}

std::optional<TU32> FDyGLWrapper::CreateBuffer(_MIN_ const PDyGLBufferDescriptor& descriptor)
{
  TU32 id = MDY_INITIALIZE_DEFUINT;
  GLenum usage = GL_NONE;
  switch (descriptor.mBufferUsage)
  {
  case EDyMeshUsage::StaticDraw:  usage = GL_STATIC_DRAW;   break;
  case EDyMeshUsage::DynamicDraw: usage = GL_DYNAMIC_DRAW;  break;
  case EDyMeshUsage::StreamDraw:  usage = GL_STREAM_DRAW;   break;
  }

  switch (descriptor.mBufferType)
  {
  case EDyDirectBufferType::VertexBuffer:
  { // VBO
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    { // Make buffer space first,
      glBufferData(GL_ARRAY_BUFFER, descriptor.mBufferByteSize, 0, usage);
      // fill out with buffers.
      auto* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
      memcpy(ptr, descriptor.mPtrBuffer, descriptor.mBufferByteSize);
      glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, MDY_GL_NONE);
    glFlush();
  } break;
  case EDyDirectBufferType::ElementBuffer:
  { // EBO
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, descriptor.mBufferByteSize, descriptor.mPtrBuffer, usage);
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
  glDeleteBuffers(1, &directBufferId);
}

void FDyGLWrapper::MapBuffer(
    _MIN_ EDyDirectBufferType iBufferType, _MIN_ TU32 iBufferId, 
    _MIN_ void* iPtrBuffer, _MIN_ TU32 iBufferSize)
{
  GLenum bufferType = GL_NONE;
  switch (iBufferType)
  {
  case EDyDirectBufferType::VertexBuffer:   { bufferType = GL_ARRAY_BUFFER; } break;
  case EDyDirectBufferType::ElementBuffer:  { bufferType = GL_ELEMENT_ARRAY_BUFFER; } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  glBindBuffer(bufferType, iBufferId);
  glBufferSubData(bufferType, 0, iBufferSize, iPtrBuffer);
}

void FDyGLWrapper::MapBufferExt(
    _MIN_ EDyDirectBufferType iBufferType, _MIN_ TU32 iBufferId, _MIN_ void* iPtrBuffer, _MIN_ TU32 iBufferSize,
    _MIN_ TU32 iItemByteSize, _MIN_ TU32 iGapByteSize, _MIN_ TU32 iStartPoint)
{
  GLenum  bufferType = GL_NONE;
  auto*   ptrInput   = static_cast<char*>(iPtrBuffer);
  switch (iBufferType)
  {
  case EDyDirectBufferType::VertexBuffer:   { bufferType = GL_ARRAY_BUFFER; } break;
  case EDyDirectBufferType::ElementBuffer:  { bufferType = GL_ELEMENT_ARRAY_BUFFER; } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  glBindBuffer(bufferType, iBufferId);
  char* ptr = static_cast<char*>(glMapBuffer(bufferType, GL_WRITE_ONLY));
  ptr += iStartPoint;
  while (ptrdiff_t(ptrInput) - ptrdiff_t(iPtrBuffer) < iBufferSize)
  {
    memcpy(ptr, ptrInput, iItemByteSize);
    ptr       += iItemByteSize + iGapByteSize;
    ptrInput  += iItemByteSize;
  }
  glUnmapBuffer(bufferType);
}

TU32 FDyGLWrapper::CreateVertexArrayObject()
{
  TU32 vaoId = MDY_INITIALIZE_DEFUINT;
  glGenVertexArrays(1, &vaoId);
  return vaoId;
}

void FDyGLWrapper::BindVertexArrayObject(_MIN_ const PDyGLVaoBindDescriptor& iDescriptor)
{
  if (iDescriptor.mAttributeInfo.mIsUsingDefaultDyAttributeModel == true)
  { // If descriptor using default attribute structure binding model, retrieve information from another dimesion
    // and call `BindVertexArrayobject` Function recursively.
    PDyGLVaoBindDescriptor descriptor;
    descriptor.mVaoId         = iDescriptor.mVaoId;
    descriptor.mBoundVboId    = iDescriptor.mBoundVboId;
    descriptor.mBoundEboId    = iDescriptor.mBoundEboId;
    descriptor.mAttributeInfo = GetDefaultAttributeFormatDescriptor();
    BindVertexArrayObject(descriptor);
    return;
  }

  const auto& attributeInfo = iDescriptor.mAttributeInfo;
  const auto attributeCount = attributeInfo.mAttributeFormatList.size();

  glBindVertexArray(iDescriptor.mVaoId);
  glBindBuffer(GL_ARRAY_BUFFER, iDescriptor.mBoundVboId);
  glBindVertexBuffer(0, iDescriptor.mBoundVboId, attributeInfo.mOffsetByteSize, attributeInfo.mStrideByteSize);
  if (iDescriptor.mBoundEboId > 0) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iDescriptor.mBoundEboId); }

  for (auto i = 0u; i < attributeCount; ++i)
  { // Set up vbo attribute formats.
    const auto&   attributeFormat = attributeInfo.mAttributeFormatList[i];
    const GLenum  type            = DyGetGLTypeFrom(attributeFormat.mType);
    const GLenum  isMustNormalized= attributeFormat.mIsMustNormalized == true ? GL_TRUE : GL_FALSE;

    glEnableVertexAttribArray(i);
    if (type == GL_INT) 
    { 
      glVertexAttribIFormat(i, attributeFormat.mElementCount, type, 
          attributeFormat.mOffsetByteSize); 
    }
    else 
    { 
      glVertexAttribFormat(i, attributeFormat.mElementCount, type, isMustNormalized, 
          attributeFormat.mOffsetByteSize); 
    }
    glVertexAttribBinding(i, 0);
  }
  glBindVertexArray(MDY_GL_NONE_VAO);
  glFlush();
}

void FDyGLWrapper::DeleteVertexArrayObject(_MIN_ const TU32 vertexArrayObjectId)
{
  glDeleteVertexArrays(1, &vertexArrayObjectId);
}

std::optional<TU32> FDyGLWrapper::CreateAttachment(_MIN_ const PDyGLAttachmentDescriptor& iDescriptor)
{
  // Validation check.
  MDY_ASSERT_MSG(iDescriptor.mBufferSize.X > 0 && iDescriptor.mBufferSize.Y > 0, 
      "Buffer size must be positive value.");
  MDY_ASSERT_MSG(iDescriptor.mBufferFormat != EDyGlBufferDataInternalFormat::NoneError, 
      "Attachment buffer format must be specified.");
  if (iDescriptor.mIsUsingCustomizedParameter == true)
  {
    MDY_ASSERT_MSG(DyCheckTextureParameterList(iDescriptor.mParameterList) == DY_SUCCESS, 
        "Attachment Parameter validation failed.");
  }

  TU32 attachmentId = MDY_INITIALIZE_DEFUINT;
  const auto glTextureType    = DyGLGetLowTextureType(iDescriptor.mAttachmentType);
  const auto mipmapLv         = iDescriptor.mSpecifiedMipmapLevel;
  const auto glInternalFormat = DyGlGetLowDataFormatType(iDescriptor.mBufferFormat);

  // Create attachment (texture only now)
  glGenTextures(1, &attachmentId);
  glBindTexture(glTextureType, attachmentId);

  switch (iDescriptor.mAttachmentType)
  {
  case EDyTextureStyleType::D2: 
  case EDyTextureStyleType::D2Shadow:
  case EDyTextureStyleType::D2Rectangle:
  {
    glTexStorage2D(glTextureType, mipmapLv, glInternalFormat, 
        MDY_VECTOR_XY(iDescriptor.mBufferSize));
  } break;
  case EDyTextureStyleType::D1:
  case EDyTextureStyleType::D1Array: 
  {
    glTexStorage2D(glTextureType, mipmapLv, glInternalFormat, 
        iDescriptor.mBufferSize.X, iDescriptor.mDepthNumber);
  } break;
  case EDyTextureStyleType::D2Array:
  case EDyTextureStyleType::D2ShadowArray: 
  {
    glTexStorage3D(glTextureType, mipmapLv, glInternalFormat, 
        MDY_VECTOR_XY(iDescriptor.mBufferSize), iDescriptor.mDepthNumber);
  } break;
  default: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  // Apply parameter option list to attachment.
  bool isThisAttachmentUsingClampToBorder = false;
  for (const auto& parameter : iDescriptor.mParameterList)
  {
    if (parameter.mParameterValue == EDyGlParameterValue::ClampToBorder) { isThisAttachmentUsingClampToBorder = true; }
    glTexParameteri(glTextureType, 
        DyGetTexParameterNameValue(parameter.mParameterOption), 
        DyGetTexParameterValueValue(parameter.mParameterValue));
  }
  if (isThisAttachmentUsingClampToBorder == true)
  {
    glTexParameterfv(glTextureType, GL_TEXTURE_BORDER_COLOR, iDescriptor.mBorderColor.Data());
  }

  glBindTexture(glTextureType, 0);
  glFlush();

  MDY_CHECK_OPENGL();
  return attachmentId;
}

EDySuccess FDyGLWrapper::DeleteAttachment(_MIN_ TU32 iAttachmentId, _MIN_ bool iIsRenderBuffer)
{
  // Delete attachment (only texture attachment now)
  if (iIsRenderBuffer == true) 
  { 
    glDeleteRenderbuffers(1, &iAttachmentId); 
  }
  else 
  { 
    glDeleteTextures(1, &iAttachmentId); 
  }
  return DY_SUCCESS;
}

std::optional<TU32> FDyGLWrapper::CreateFrameBuffer(_MIN_ const PDyGLFrameBufferDescriptor& iDescriptor)
{
  const auto attachmentBindingSize = TU32(iDescriptor.mAttachmentBindingList.size());
  std::vector<GLenum> attachmentTypeList = {};
  attachmentTypeList.reserve(attachmentBindingSize);

  // Make frame buffer resource and attachment.
  TU32 framebufferId = MDY_INITIALIZE_DEFUINT;
  glGenFramebuffers(1, &framebufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

  for (TU32 i = 0; i < attachmentBindingSize; ++i)
  {
    const auto [attachmentId, attachmentType, attachmentFormat, isRenderBuffer] = iDescriptor.mAttachmentBindingList[i];
    if (isRenderBuffer == false)
    { // If attachment is texture.
      glBindTexture(DyGLGetLowTextureType(attachmentType), attachmentId);
      const auto typeValue = DyGetAttachmentTypeValue(attachmentFormat);
      glFramebufferTexture(GL_FRAMEBUFFER, typeValue, attachmentId, 0);
      attachmentTypeList.emplace_back(typeValue);
    }
    else
    { // @TODO RENDER BUFFER IS NOT SUPPORTED YET.
      MDY_NOT_IMPLEMENTED_ASSERT();
    }
  }

  if (iDescriptor.mIsUsingDepthBuffer == true)
  { // Bind Depth Buffer
    const auto [depthId, attachmentType, attachmentFormat, isRenderBuffer] = iDescriptor.mDepthBufferBinding;
    if (isRenderBuffer == false)
    {
      glBindTexture(DyGLGetLowTextureType(attachmentType), depthId);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthId, 0);
    }
    else
    {
      MDY_NOT_IMPLEMENTED_ASSERT();
#ifdef false
      glGenRenderbuffers(1, &depthId);
      glBindRenderbuffer(GL_RENDERBUFFER, depthId);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, MDY_VECTOR_XY(iDescriptor.mFrameBufferSize));
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthId);
#endif
    }
  }

  // Let framebuffer know that attachmentBuffer's id will be drawn at framebuffer.
  // @WARNING TODO BE CAREFUL OF INSERTING DEPTH ATTACHMENTS AS COLOR ATTACHMENT!.
  if (iDescriptor.mIsUsingPixelShader == false) { glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE); }
  else { glDrawBuffers(attachmentBindingSize, attachmentTypeList.data()); }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glFlush();

  MDY_CHECK_OPENGL();
  return framebufferId;
}

EDySuccess FDyGLWrapper::DeleteFrameBuffer(_MIN_ const TU32 framebufferId)
{
  glDeleteFramebuffers(1, &framebufferId);
  return DY_SUCCESS;
}

void FDyGLWrapper::SetViewport(_MIN_ const std::array<TI32, 4>& iViewportRegion)
{
  DDyArea2D area; 
  area.mLeftDown  = DDyVector2{static_cast<TF32>(iViewportRegion[0]), static_cast<TF32>(iViewportRegion[1])};
  area.mRightUp   = DDyVector2{static_cast<TF32>(iViewportRegion[2]), static_cast<TF32>(iViewportRegion[3])};
  FDyGLWrapper::SetViewport(area);
}

void FDyGLWrapper::SetViewport(_MIN_ const DDyArea2D& iViewportRegion)
{
  glViewport(
      static_cast<TI32>(iViewportRegion.mLeftDown.X), 
      static_cast<TI32>(iViewportRegion.mLeftDown.Y), 
      static_cast<TI32>(iViewportRegion.mRightUp.X - iViewportRegion.mLeftDown.X), 
      static_cast<TI32>(iViewportRegion.mRightUp.Y - iViewportRegion.mLeftDown.Y));
}

void FDyGLWrapper::SetViewportIndexed(_MIN_ TU32 iIndex,_MIN_ const std::array<TI32, 4>& iViewportRegion)
{
  DDyArea2D area; 
  area.mLeftDown  = DDyVector2{static_cast<TF32>(iViewportRegion[0]), static_cast<TF32>(iViewportRegion[1])};
  area.mRightUp   = DDyVector2{static_cast<TF32>(iViewportRegion[2]), static_cast<TF32>(iViewportRegion[3])};
  FDyGLWrapper::SetViewportIndexed(iIndex, area);
}

void FDyGLWrapper::SetViewportIndexed(_MIN_ TU32 iIndex, _MIN_ const DDyArea2D& iViewportRegion)
{
  glViewportIndexedf(iIndex, 
      iViewportRegion.mLeftDown.X, 
      iViewportRegion.mLeftDown.Y, 
      iViewportRegion.mRightUp.X - iViewportRegion.mLeftDown.X, 
      iViewportRegion.mRightUp.Y - iViewportRegion.mLeftDown.Y);
}

void FDyGLWrapper::BindFrameBufferObject(_MIN_ TU32 iFboId)
{
  glBindFramebuffer(GL_FRAMEBUFFER, iFboId);
}

void FDyGLWrapper::UnbindFrameBufferObject()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyGLWrapper::BindVertexArrayObject(_MIN_ TU32 iVaoId)
{
  glBindVertexArray(iVaoId);
}

void FDyGLWrapper::UnbindVertexArrayObject()
{
  glBindVertexArray(0);
}

void FDyGLWrapper::BindTexture(
    _MIN_ TU32 activeTextureIndex, 
    _MIN_ EDyTextureStyleType type, _MIN_ TU32 textureId)
{
  #if defined(NDEBUG) == false 
  {
    MDY_ASSERT_MSG(glIsTexture(textureId) == GL_TRUE, "given `textureId` is not texture.");
  }
  #endif

  glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
  switch (type)
  {
  case EDyTextureStyleType::D1: { glBindTexture(GL_TEXTURE_1D, textureId); } break;
  case EDyTextureStyleType::D2: { glBindTexture(GL_TEXTURE_2D, textureId); } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void FDyGLWrapper::UnbindTexture(_MIN_ TU32 textureIndex, _MIN_ EDyTextureStyleType type)
{
  glActiveTexture(GL_TEXTURE0 + textureIndex);
  switch (type)
  {
  case EDyTextureStyleType::D1: { glBindTexture(GL_TEXTURE_1D, 0); } break;
  case EDyTextureStyleType::D2: { glBindTexture(GL_TEXTURE_2D, 0); } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void FDyGLWrapper::Draw(_MIN_ EDyDrawType iType, _MIN_ bool iIsElement, _MIN_ TU32 iCount)
{
  GLenum drawType = GL_POINT;
  switch (iType)
  {
  case EDyDrawType::Point:  drawType = GL_POINT;  break;
  case EDyDrawType::Line:   drawType = GL_LINE;   break;
  case EDyDrawType::LineStrip: drawType = GL_LINE_STRIP; break;
  case EDyDrawType::LineLoop:   drawType = GL_LINE_LOOP; break;
  case EDyDrawType::Triangle:   drawType = GL_TRIANGLES; break;
  case EDyDrawType::TriangleStip: drawType = GL_TRIANGLE_STRIP; break;
  case EDyDrawType::TriangleFan:  drawType = GL_TRIANGLE_FAN;   break;
  }

  if (iIsElement == true)
  {
    glDrawElements(drawType, iCount, GL_UNSIGNED_INT, nullptr);
  }
  else
  {
    glDrawArrays(drawType, 0, iCount);
  }
}

TI32 FDyGLWrapper::QueryShaderProgramIV(_MIN_ TU32 iShaderProgramId, _MIN_ GLenum iQueryEnum)
{
  TI32 result = 0;
  glGetProgramiv(iShaderProgramId, iQueryEnum, &result);
  return result;
}

void FDyGLWrapper::QueryFloatVector(_MIN_ GLenum iGLLowEnumCommand, _MIN_ TF32* iPtrRawFloatVector)
{
  glGetFloatv(iGLLowEnumCommand, iPtrRawFloatVector);
}

std::optional<std::tuple<std::string, GLsizei, GLint, EDyAttributeVariableType, TU32>> 
FDyGLWrapper::GetShaderProgramAttributeInfo(_MIN_ TU32 iShaderProgramId, _MIN_ TU32 iAttrIndex)
{
  const TI32 attrBufferLength = QueryShaderProgramIV(iShaderProgramId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
  auto* attributeName = static_cast<char*>(std::calloc(attrBufferLength, sizeof(GLchar)));

  GLsizei attributelength = 0;
  GLint   attributeSize   = 0;
  GLenum  attributeType   = GL_NONE;

  glGetActiveAttrib(iShaderProgramId, iAttrIndex, attrBufferLength, &attributelength, &attributeSize, &attributeType, attributeName);
  const TI32 attrLocation = glGetAttribLocation(iShaderProgramId, attributeName);

  MDY_ASSERT_MSG(attrLocation != -1, "Attribute location did not find.");
  const auto type = DyGlGetAttributeVariableTypeFrom(attributeType);
  MDY_ASSERT_MSG(type != EDyAttributeVariableType::NoneError, "Not supported attribute type.");

  const auto result = std::make_tuple(std::string(attributeName), attributelength, attributeSize, type, attrLocation);
  free(attributeName); attributeName = nullptr;
  return result;
}

std::optional<std::tuple<std::string, GLsizei, GLint, EDyUniformVariableType, TU32>>
FDyGLWrapper::GetShaderProgramUniformInfo(_MIN_ TU32 iShaderProgramId, _MIN_ TU32 iUniformIndex)
{
  const TI32 uniformBufLength = QueryShaderProgramIV(iShaderProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH);
  auto* uniformName = static_cast<char*>(std::calloc(uniformBufLength, sizeof(GLchar)));

  GLsizei uniLength = 0;
  GLint   uniSize   = 0;
  GLenum  uniType   = GL_NONE;
  glGetActiveUniform(iShaderProgramId, iUniformIndex, uniformBufLength, &uniLength, &uniSize, &uniType, uniformName);
  const auto uniLocId = glGetUniformLocation(iShaderProgramId, uniformName);

  if (uniLocId == -1)
  { // Uniform location will be -1 when uniform varaible is in ubo.
    const GLchar* uboNameLastPtr = std::strchr(uniformName, '.');
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(uboNameLastPtr), "Unsupported uniform value type.");
    free(uniformName); uniformName = nullptr;
    return std::nullopt;
  }

  auto type = DyGlGetUniformVariableTypeFrom(uniType);
  MDY_ASSERT_MSG(type != EDyUniformVariableType::NoneError, "Not supported uniform type.");

  // If array type.. 
  if (std::strchr(uniformName, '[') != nullptr)
  {
    switch (type)
    {
    case EDyUniformVariableType::Matrix4: { type = EDyUniformVariableType::Matrix4Array; } break;
    case EDyUniformVariableType::Vector3: { type = EDyUniformVariableType::Vector3Array; } break;
    default: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    }
  }

  // Insert
  const auto result = std::make_tuple(std::string(uniformName), uniLength, uniSize, type, uniLocId);
  free(uniformName); uniformName = nullptr;
  return result;
}

std::optional<std::string> 
FDyGLWrapper::GetShaderProgramUniformBlockInfo(_MIN_ TU32 iShaderProgramId, _MIN_ TU32 iUniformBlockIndex)
{
  const TI32 uboNameMaxLength = QueryShaderProgramIV(iShaderProgramId, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH);
  auto* uniformName = static_cast<char*>(std::calloc(uboNameMaxLength, sizeof(GLchar)));

  GLsizei uniformLength = 0;
  glGetActiveUniformBlockName(iShaderProgramId, iUniformBlockIndex, uboNameMaxLength, &uniformLength, uniformName);

  if (uniformLength <= 0) 
  { 
    free(uniformName); uniformName = nullptr;
    return std::nullopt; 
  }

  const std::string result = uniformName;
  free(uniformName); uniformName = nullptr;
  return result;
}

void FDyGLWrapper::UpdateUniformMatrix4(TU32 iId, const DDyMatrix4x4& iBuffer, bool iTransposed)
{
  GLenum transposed = GL_FALSE;
  if (iTransposed == true) { transposed = GL_TRUE; }

  glUniformMatrix4fv(iId, 1, transposed, &iBuffer[0].X);
}

void FDyGLWrapper::UpdateUniformMatrix3(TU32 iId, const DDyMatrix3x3& iBuffer, bool iTransposed)
{
  GLenum transposed = GL_FALSE;
  if (iTransposed == true) { transposed = GL_TRUE; }

  glUniformMatrix3fv(iId, 1, transposed, &iBuffer[0].X);
}

void FDyGLWrapper::UpdateUniformMatrix4Array(TU32 iId, const std::vector<DDyMatrix4x4>& iBuffer, bool iIransposed)
{
  if (iBuffer.empty() == true) { return; }

  const TU32 size = static_cast<TU32>(iBuffer.size());
  GLenum transposed = GL_FALSE;
  if (iIransposed == true) { transposed = GL_TRUE; }

  glUniformMatrix4fv(iId, size, transposed, &iBuffer[0][0].X);
}

void FDyGLWrapper::UpdateUniformVector3Array(TU32 iId, const std::vector<DDyVector3>& iBuffer)
{
  if (iBuffer.empty() == true) { return; }

  const TU32 size = static_cast<TU32>(iBuffer.size());
  glUniform3fv(iId, size, iBuffer.front().Data());
}

void FDyGLWrapper::UpdateUniformVector4(TU32 iId, const DDyVector4& iBuffer)
{
  glUniform4fv(iId, 1, iBuffer.Data());
}

void FDyGLWrapper::UpdateUniformVector3(TU32 iId, const DDyVector3& iBuffer)
{
  glUniform3fv(iId, 1, iBuffer.Data());
}

void FDyGLWrapper::UpdateUniformVector2(TU32 iId, const DDyVector2& iBuffer)
{
  glUniform2fv(iId, 1, iBuffer.Data());
}

void FDyGLWrapper::UpdateUniformInteger(TU32 iId, const TI32& iBuffer)
{
  glUniform1i(iId, iBuffer);
}

void FDyGLWrapper::UpdateUniformUnsigned(TU32 iId, const TU32& iBuffer)
{
  glUniform1ui(iId, iBuffer);
}

void FDyGLWrapper::UpdateUniformFloat(TU32 iId, const TF32& iBuffer)
{
  glUniform1f(iId, iBuffer);
}

FDyGLWrapper::__OutsideLockguard::MDY_PRIVATE(OutsideLockguard)
(FDyGLWrapper::MDY_PRIVATE(OutsideLockguard)&& iSource) noexcept
{
  iSource.mIsMoved = true;
}

FDyGLWrapper::__OutsideLockguard FDyGLWrapper::__LockMutex()
{
  FDyGLWrapper::mGLCriticalSectionMutex.lock();
  return __OutsideLockguard{};
}

void FDyGLWrapper::__UnlockMutex()
{
  FDyGLWrapper::mGLCriticalSectionMutex.unlock();
}

FDyGLWrapper::__OutsideLockguard::~MDY_PRIVATE(OutsideLockguard)() noexcept
{
  if (this->mIsMoved == false)
  { // Unlock gl critical section when not moved.
    glFlush();
    FDyGLWrapper::MDY_PRIVATE(UnlockMutex)();
  }
}

} /// ::dy namespace