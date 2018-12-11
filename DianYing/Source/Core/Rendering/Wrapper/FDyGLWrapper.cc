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

namespace dy
{

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

} /// ::dy namespace