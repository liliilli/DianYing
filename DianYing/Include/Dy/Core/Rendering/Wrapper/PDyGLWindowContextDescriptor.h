#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLWINDOWCONTEXTDESCRIPTOR_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLWINDOWCONTEXTDESCRIPTOR_H
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

#include <Dy/Helper/Type/VectorInt2.h>

namespace dy
{

///
/// @struct PDyGLWindowContextDescriptor
/// @brief Descriptor instance for creating window context of opengl.
///
struct PDyGLWindowContextDescriptor final
{
  bool mIsWindowResizable   = false;
  bool mIsWindowVisible     = false;
  bool mIsWindowFullScreen  = false;
  bool mIsWindowShouldFocus = false;

  bool mIsUsingDefaultMSAA  = false;
  bool mIsUsingDefaultDoubleBuffer = false;

  GLFWwindow* mSharingContext = nullptr;

  DDyVectorInt2 mWindowSize   = {};
  std::string   mWindowName   = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_PDYGLWINDOWCONTEXTDESCRIPTOR_H