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

#define MDY_VECTOR_XY(__MAVectorType__) __MAVectorType__.X, __MAVectorType__.Y

std::optional<TU32> FDyGLWrapper::CreateTexture(_MIN_ const PDyGLTextureDescriptor& descriptor)
{


  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTION BODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // Validation check.
  MDY_ASSERT(descriptor.mImageFormat != GL_NONE, "Texture Image format must be specified.");
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(descriptor.mPtrBuffer), "Texture Image buffer must not be null.");
  MDY_ASSERT(descriptor.mTextureSize.X > 0 && descriptor.mTextureSize.Y > 0, "Texture size must be positive value.");
  MDY_ASSERT(descriptor.mType != EDyTextureStyleType::None, "Texture Image type must be specified.");
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
  case EDyTextureStyleType::None: MDY_UNEXPECTED_BRANCH_BUT_RETURN(std::nullopt);
  }

  { // Critical section.
    MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);

    // Make texture.
    switch (descriptor.mType)
    {
    case EDyTextureStyleType::D1:
      glGenTextures(1, &mTextureResourceId);
      glBindTexture(GL_TEXTURE_1D, mTextureResourceId);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, descriptor.mTextureSize.X, 0, descriptor.mImageFormat, GL_UNSIGNED_BYTE, descriptor.mPtrBuffer->data());
      break;
    case EDyTextureStyleType::D2:
      glGenTextures(1, &mTextureResourceId);
      glBindTexture(GL_TEXTURE_2D, mTextureResourceId);
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, MDY_VECTOR_XY(descriptor.mTextureSize), descriptor.mImageFormat, GL_UNSIGNED_BYTE, descriptor.mPtrBuffer->data());
      break;
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
  }

  return mTextureResourceId;
}

void FDyGLWrapper::DeleteTexture(_MIN_ const TU32 validTextureId)
{
  MDY_SYNC_LOCK_GUARD(FDyGLWrapper::mTextureMutex);
  glDeleteTextures(1, &validTextureId);
}

} /// ::dy namespace