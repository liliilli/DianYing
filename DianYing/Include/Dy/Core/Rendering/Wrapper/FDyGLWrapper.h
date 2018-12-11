#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H
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

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGLWindowContextDescriptor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class FDyGLWrapper
/// @brief OpenGL Wrapping container.
///
class FDyGLWrapper
{
public:
  ///
  /// @brief Create window context following descriptor. \n\n
  /// Window name and Window size must be valid. \n
  /// If mSharingContext is valid, created window is shared by mSharingContext. \n
  /// If sharing context deleted but derived window is alive, this occurs undefined behaviour.
  ///
  static GLFWwindow* CreateGLWindow(_MIN_ const PDyGLWindowContextDescriptor& descriptor);

  /// @brief Create opengl context.
  static void CreateGLContext(_MIN_ GLFWwindow* window);
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H