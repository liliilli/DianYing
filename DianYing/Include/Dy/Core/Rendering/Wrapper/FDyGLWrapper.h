#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H
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

#include <Dy/Meta/Type/EDyResourceTypes.h>
#include <Dy/Core/Rendering/Type/EDyDrawType.h>
#include <Dy/Core/Rendering/Wrapper/PDyGLBufferDescriptor.h>
#include <Dy/Core/Resource/Internal/ShaderType.h>

//!
//! Forward declaration
//!

namespace dy
{
  struct DDyArea2D;
  enum class EDyTextureStyleType : unsigned char;
enum class EDyDrawType;
struct PDyGLVaoBindDescriptor;
struct PDyGLShaderFragmentDescriptor;
struct PDyGLTextureDescriptor;
struct PDyGLWindowContextDescriptor;
struct PDyGLBufferDescriptor;
struct PDyGLAttachmentDescriptor;
struct PDyGLFrameBufferDescriptor;
struct DDyGLVaoBindInformation;
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

  /// @brief Use shader program.
  static void UseShaderProgram(_MIN_ TU32 iShaderProgramId);
  /// @brief Disuse shader program.
  static void DisuseShaderProgram();

  /// @brief Create direct buffer which can be shared between OpenGL contexts. \n
  /// This function is thread-safe and get performance down.
  static MDY_NODISCARD std::optional<TU32> CreateBuffer(_MIN_ const PDyGLBufferDescriptor& descriptor);
  /// @brief Delete shader program with valid shader program id.
  /// This function is thread-safe and get performance down.
  static void DeleteBuffer(_MIN_ const TU32 directBufferId);

  /// @brief Map data to arbitary buffer from zero. This function is needed to bind
  /// related vertex array object prior to function.
  /// This function thread-safe and might get performance down.
  static void MapBuffer(_MIN_ EDyDirectBufferType iBufferType, _MIN_ TU32 iBufferId, _MIN_ void* iPtrBuffer, _MIN_ TU32 iBufferSize);
  /// @brief Map data to arbitary buffer with start point, input buffer size, and gap of each input target pointer.
  /// This funciton is needed to bind related vertex array object prior to function.
  /// This function thread-safe and might get performance down.
  static void MapBufferExt(
      _MIN_ EDyDirectBufferType iBufferType, _MIN_ TU32 iBufferId, _MIN_ void* iPtrBuffer, _MIN_ TU32 iBufferSize, 
      _MIN_ TU32 iItemByteSize, _MIN_ TU32 iGapByteSize, _MIN_ TU32 iStartPoint);

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
  static MDY_NODISCARD EDySuccess DeleteAttachment(_MIN_ TU32 iAttachmentId, _MIN_ bool iIsRenderBuffer);

  /// @brief Create frame buffer. \n
  /// Creating frame buffer object must be held on main thread context.
  static MDY_NODISCARD std::optional<TU32> CreateFrameBuffer(_MIN_ const PDyGLFrameBufferDescriptor& iDescriptor);
  /// @brief Delete frame buffer. \n
  /// Deleting frame buffer object must be held on main thread context.
  MDY_NODISCARD static EDySuccess DeleteFrameBuffer(_MIN_ const TU32 framebufferId);

  /// @brief Set global viewport region.
  /// ...You always need to call glViewport() before starting to draw to a framebuffer with a different size. 
  /// This is necessary because the viewport is not part of the framebuffer state...
  static void SetViewport(_MIN_ const std::array<TI32, 4>& iViewportRegion);
  static void SetViewport(_MIN_ const DDyArea2D& iViewportRegion);
  /// @brief Set global indexed viewport region. Keep caution to use this, it's different from general `SetViewport`.
  /// ...You always need to call glViewport() before starting to draw to a framebuffer with a different size. 
  /// This is necessary because the viewport is not part of the framebuffer state...
  static void SetViewportIndexed(_MIN_ TU32 iIndex, _MIN_ const std::array<TI32, 4>& iViewportRegion);
  static void SetViewportIndexed(_MIN_ TU32 iIndex, _MIN_ const DDyArea2D& iViewportRegion);

  /// @brief Bind frame buffer object.
  static void BindFrameBufferObject(_MIN_ TU32 iFboId);
  /// @brief Unbind frame buffer object.
  static void UnbindFrameBufferObject();

  /// @brief Bind vertex array object.
  static void BindVertexArrayObject(_MIN_ TU32 iVaoId);
  /// @brief Unbind vertex array object.
  static void UnbindVertexArrayObject();

  /// @brief Bind texture to activated shader. Shader must be actiated before use this.
  static void BindTexture(_MIN_ TU32 activeTextureIndex, _MIN_ EDyTextureStyleType type, _MIN_ TU32 textureId);
  /// @brief Unbind texture from GL_TEXTURE_XX from activated shader.
  static void UnbindTexture(TU32 textureIndex, _MIN_ EDyTextureStyleType type);
  /// @brief Unbind textures of `textureIndexList` from activated shader.
  static void UnbindTextureList(_MIN_ const std::vector<TU32>& textureIndexList);

  /// @brief Draw command.
  static void Draw(_MIN_ EDyDrawType iType, _MIN_ bool iIsElement, _MIN_ TU32 iCount);

  /// @brief Query command. Shader program id must be valid and query enum also.
  MDY_NODISCARD static TI32 QueryShaderProgramIV(_MIN_ TU32 iShaderProgramId, _MIN_ GLenum iQueryEnum);
  /// @brief Query Float vector command.
  static void QueryFloatVector(_MIN_ GLenum iGLLowEnumCommand, _MIN_ TF32* iPtrRawFloatVector);

  /// @brief 
  MDY_NODISCARD static std::optional<std::tuple<std::string, GLsizei, GLint, EDyAttributeVariableType, TU32>>
  GetShaderProgramAttributeInfo(_MIN_ TU32 iShaderProgramId, _MIN_ TU32 iAttrIndex);
  /// @brief
  MDY_NODISCARD static std::optional<std::tuple<std::string, GLsizei, GLint, EDyUniformVariableType, TU32>>
  GetShaderProgramUniformInfo(_MIN_ TU32 iShaderProgramId, _MIN_ TU32 iUniformIndex);
  /// @brief
  MDY_NODISCARD static std::optional<std::string>
  GetShaderProgramUniformBlockInfo(_MIN_ TU32 iShaderProgramId, _MIN_ TU32 iUniformBlockIndex);

  /// @brief Update uniform matrix4x4 item. Specified shader must be valid and activated.
  static void UpdateUniformMatrix4(_MIN_ TU32 iId, _MIN_ const DDyMatrix4x4& iBuffer, _MIN_ bool iTransposed = false);
  /// @brief Update uniform matrix3x3 item. Specified shader must be valid and activated.
  static void UpdateUniformMatrix3(_MIN_ TU32 iId, _MIN_ const DDyMatrix3x3& iBuffer, _MIN_ bool iTransposed = false);
  /// @brief Update uniform matrix4x4 array. Specified shader must be valid and activated.
  static void UpdateUniformMatrix4Array(_MIN_ TU32 iId, _MIN_ const std::vector<DDyMatrix4x4>& iBuffer, _MIN_ bool iIransposed = false);
  /// @brief Update uniform vector3 (float) array. Specified shader must be valid and activated.
  static void UpdateUniformVector3Array(_MIN_ TU32 iId, _MIN_ const std::vector<DDyVector3>& iBfuffer);
  /// @brief
  static void UpdateUniformVector4(_MIN_ TU32 iId, _MIN_ const DDyVector4& iBuffer);
  /// @brief
  static void UpdateUniformInteger(_MIN_ TU32 iId, _MIN_ const TI32& iBuffer);
  /// @brief Update uniform one float value. Specified shader must be valid and activated.
  static void UpdateUniformFloat(_MIN_ TU32 iId, _MIN_ const TF32& iBuffer);

  struct MDY_PRIVATE_SPECIFIER(OutsideLockguard) final
  {
  public:
    MDY_PRIVATE_SPECIFIER(OutsideLockguard)() = default;
    ~MDY_PRIVATE_SPECIFIER(OutsideLockguard)() noexcept;

    MDY_PRIVATE_SPECIFIER(OutsideLockguard)(MDY_PRIVATE_SPECIFIER(OutsideLockguard)&& _) noexcept;
    MDY_PRIVATE_SPECIFIER(OutsideLockguard) operator=(MDY_PRIVATE_SPECIFIER(OutsideLockguard)&&) noexcept = delete;

  private:
    bool mIsMoved = false;
  };

  static MDY_PRIVATE_SPECIFIER(OutsideLockguard) MDY_PRIVATE_SPECIFIER(LockMutex)();
  static void MDY_PRIVATE_SPECIFIER(UnlockMutex)();

private:
  static std::mutex mGLMutex;
  static std::mutex mGLCriticalSectionMutex;

  /// @brief
  static MDY_NODISCARD const DDyGLVaoBindInformation& GetDefaultAttributeFormatDescriptor() noexcept;
};

#define MDY_GRAPHIC_SET_CRITICALSECITON() const auto MDY_TOKENPASTE2(_, __LINE__) = ::dy::FDyGLWrapper::MDY_PRIVATE_SPECIFIER(LockMutex)()

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_FDYGLWRAPPER_H