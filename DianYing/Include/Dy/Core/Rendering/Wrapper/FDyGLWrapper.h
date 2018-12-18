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

#include <Dy/Meta/Type/EDyResourceTypes.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyGLVaoBindDescriptor;
struct PDyGLShaderFragmentDescriptor;
struct PDyGLTextureDescriptor;
struct PDyGLWindowContextDescriptor;
struct PDyGLBufferDescriptor;
struct PDyGLAttachmentDescriptor;
struct PDyGLFrameBufferDescriptor;
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

  /// @brief Create shader fragment and get fragment id. \n
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD std::optional<TU32> CreateShaderFragment(_MIN_ const PDyGLShaderFragmentDescriptor& descriptor);

  /// @brief Delete shader fragment with valid shader fragment id. \n
  /// This function is thread-safe and get performance down.
  static void DeleteShaderFragment(_MIN_ const TU32 shaderFragmentId);

  /// @brief Create shader program and get shader program id. \n
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD std::optional<TU32> CreateShaderProgram(_MIN_ const TFragmentList& fragmentList);

  /// @brief Delete shader program with valid shader program id.
  /// This function is thread-safe and get performance down.
  static void DeleteShaderProgram(_MIN_ const TU32 shaderProgramId);

  /// @brief Create direct buffer which can be shared between OpenGL contexts. \n
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD std::optional<TU32> CreateBuffer(_MIN_ const PDyGLBufferDescriptor& descriptor);

  /// @brief Delete shader program with valid shader program id.
  /// This function is thread-safe and get performance down.
  static void DeleteBuffer(_MIN_ const TU32 directBufferId);

  /// @brief Just create and return created vertex array object id.
  static MDY_NODISCARD TU32 CreateVertexArrayObject(void);

  /// @brief Bind properties with valid Vertex Array Object Id.
  static void BindVertexArrayObject(_MIN_ const PDyGLVaoBindDescriptor& descriptor);

  /// @brief Delete Vertex Array Object id.
  static void DeleteVertexArrayObject(_MIN_ const TU32 vertexArrayObjectId);

  /// @brief Create attachment (texture or renderbuffer) and get texture id. \n
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD std::optional<TU32> CreateAttachment(_MIN_ const PDyGLAttachmentDescriptor& iDescriptor);

  /// @brief Delete attachment (texture or renderbuffer) if texture id is valid.
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD EDySuccess DeleteAttachment(_MIN_ const TU32 attachmentId);

  /// @brief Create frame buffer. \n
  /// Creating frame buffer object must be held on main thread context.
  static MDY_NODISCARD std::optional<TU32> CreateFrameBuffer(_MIN_ const PDyGLFrameBufferDescriptor& iDescriptor);

  /// @brief Delete frame buffer. \n
  /// Deleting frame buffer object must be held on main thread context.
  static MDY_NODISCARD EDySuccess DeleteFrameBuffer(_MIN_ const TU32 framebufferId);

private:
  static std::mutex mGLMutex;

  /// @brief
  static MDY_NODISCARD const PDyGLVaoBindDescriptor& GetDefaultAttributeFormatDescriptor() noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H