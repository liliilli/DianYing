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
struct PDyGLTextureDescriptor;
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
  static MDY_NODISCARD GLFWwindow* CreateGLWindow(_MIN_ const PDyGLWindowContextDescriptor& descriptor);

  /// @brief Create opengl context.
  static void CreateGLContext(_MIN_ GLFWwindow* window);

  /// @brief Create texture and get texture id. \n
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD std::optional<TU32> CreateTexture(_MIN_ const PDyGLTextureDescriptor& descriptor);

  /// @brief Delete texture if texture id is valid.
  /// This function is thread-safe and get performance down.
  static void DeleteTexture(_MIN_ const TU32 validTextureId);

private:
  static std::mutex mTextureMutex;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H