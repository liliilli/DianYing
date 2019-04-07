#ifndef GUARD_DY_CORE_RENDERING_WRAPPER_XGLWrapper_H
#define GUARD_DY_CORE_RENDERING_WRAPPER_XGLWrapper_H
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
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/PGLBufferDescriptor.h>
#include <Dy/Core/Resource/Internal/XShaderTypes.h>
#include <Dy/Helper/Internal/FCallStack.h>
#include <Dy/Management/Type/Render/DGlGlobalStates.h>
#include <Dy/Management/Type/AttachmentInformation.h>
#include <Dy/Helper/Type/DMatrix3x3.h>

//!
//! Forward declaration
//!

struct GLFWwindow;

namespace dy
{
struct PDyGLTextureCubemapDescriptor;
enum class ETextureStyleType : unsigned char;
enum class EDrawType;
struct PDyGLVaoBindDescriptor;
struct PDyGLShaderFragmentDescriptor;
struct PDyGLTextureDescriptor;
struct PDyGLWindowContextDescriptor;
struct PGLBufferDescriptor;
struct PGLAttachmentDescriptor;
struct PDyGLFrameBufferDescriptor;
struct DGlVaoBindInformation;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class XGLWrapper
/// @brief OpenGL Wrapping container.
class XGLWrapper
{
public:
  /// @brief Create window context following descriptor. \n\n
  /// Window name and Window size must be valid. \n
  /// If mSharingContext is valid, created window is shared by mSharingContext. \n
  /// If sharing context deleted but derived window is alive, this occurs undefined behaviour.
  MDY_NODISCARD static GLFWwindow* 
  CreateGLWindow(const PDyGLWindowContextDescriptor& descriptor);
  /// @brief Create opengl context.
  static void CreateGLContext(GLFWwindow* window);

  /// @brief Create texture and get texture id. \n
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static std::optional<TU32> 
  CreateTexture(const PDyGLTextureDescriptor& descriptor);
  /// @brief Create cubemap texture and get cubemap texture id. \n
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static std::optional<TU32> 
  CreateTexture(const PDyGLTextureCubemapDescriptor& descriptor);
  /// @brief Delete texture if texture id is valid.
  /// This function is thread-safe and get performance down.
  static void DeleteTexture(const TU32 validTextureId);

  /// @brief Create shader fragment and get fragment id. \n
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static std::optional<TU32> 
  CreateShaderFragment(const PDyGLShaderFragmentDescriptor& descriptor);
  /// @brief Delete shader fragment with valid shader fragment id. \n
  /// This function is thread-safe and get performance down.
  static void DeleteShaderFragment(const TU32 shaderFragmentId);

  /// @brief Create shader program and get shader program id. \n
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static std::optional<TU32> 
  CreateShaderProgram(const TFragmentList& fragmentList);
  /// @brief Delete shader program with valid shader program id.
  /// This function is thread-safe and get performance down.
  static void DeleteShaderProgram(const TU32 shaderProgramId);

  /// @brief Use shader program.
  static void UseShaderProgram(TU32 iShaderProgramId);
  /// @brief Disuse shader program.
  static void DisuseShaderProgram();

  /// @brief Create direct buffer which can be shared between OpenGL contexts. \n
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static std::optional<TU32> 
  CreateBuffer(const PGLBufferDescriptor& descriptor);
  /// @brief Delete shader program with valid shader program id.
  /// This function is thread-safe and get performance down.
  static void DeleteBuffer(const TU32 directBufferId);

  /// @brief Map data to arbitary buffer from zero. This function is needed to bind
  /// related vertex array object prior to function.
  /// This function thread-safe and might get performance down.
  static void MapBuffer(
    EDirectBufferType iBufferType, TU32 iBufferId, const void* iPtrBuffer, TU32 iBufferSize);
  /// @brief Map data to arbitary buffer with start point, input buffer size, and gap of each input target pointer.
  /// This funciton is needed to bind related vertex array object prior to function.
  /// This function thread-safe and might get performance down.
  static void MapBufferExt(
    EDirectBufferType iBufferType, TU32 iBufferId, const void* iPtrBuffer, TU32 iBufferSize, 
    TU32 iItemByteSize, TU32 iGapByteSize, TU32 iStartPoint);

  /// @brief Just create and return created vertex array object id.
  MDY_NODISCARD static TU32 CreateVertexArrayObject();
  /// @brief Bind properties with valid Vertex Array Object Id.
  static void BindVertexArrayObject(const PDyGLVaoBindDescriptor& descriptor);
  /// @brief Delete Vertex Array Object id.
  static void DeleteVertexArrayObject(const TU32 vertexArrayObjectId);

  /// @brief Create attachment (texture or renderbuffer) and get texture id. \n
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static std::optional<TU32> 
  CreateAttachment(const PGLAttachmentDescriptor& iDescriptor);
  /// @brief Delete attachment (texture or renderbuffer) if texture id is valid.
  /// This function is thread-safe and get performance down.
  MDY_NODISCARD static EDySuccess DeleteAttachment(TU32 iAttachmentId, bool iIsRenderBuffer);

  /// @brief Create frame buffer. \n
  /// Creating frame buffer object must be held on main thread context.
  MDY_NODISCARD static std::optional<TU32> 
  CreateFrameBuffer(const PDyGLFrameBufferDescriptor& iDescriptor);
  /// @brief Delete frame buffer. \n
  /// Deleting frame buffer object must be held on main thread context.
  MDY_NODISCARD static EDySuccess DeleteFrameBuffer(const TU32 framebufferId);

  /// @brief Set global viewport region.
  /// ...You always need to call glViewport() before starting to draw to a framebuffer with a different size. 
  /// This is necessary because the viewport is not part of the framebuffer state...
  static void SetViewport(const DArea2D& iViewportRegion);
  /// @brief Set global indexed viewport region. Keep caution to use this, it's different from general `SetViewport`.
  /// ...You always need to call glViewport() before starting to draw to a framebuffer with a different size. 
  /// This is necessary because the viewport is not part of the framebuffer state...
  static void SetViewportIndexed(TU32 iIndex, const DArea2D& iViewportRegion);

  /// @brief Bind frame buffer object.
  static void BindFrameBufferObject(TU32 iFboId);
  /// @brief Unbind frame buffer object.
  static void UnbindFrameBufferObject();
  /// @brief
  static void ClearColorFrameBuffer(
    TU32 frameBufferId, const DColorRGBA& color, TU32 drawBufferId = 0);
  /// @brief
  static void ClearDepthFrameBuffer(TU32 frameBufferId, TF32 depthValue);

  /// @brief Bind vertex array object.
  static void BindVertexArrayObject(TU32 iVaoId);
  /// @brief Unbind vertex array object.
  static void UnbindVertexArrayObject();

  /// @brief Bind texture to activated shader. Shader must be actiated before use this.
  static void BindTexture(TU32 activeTextureIndex, ETextureStyleType type, TU32 textureId);
  /// @brief Unbind texture from GL_TEXTURE_XX from activated shader.
  static void UnbindTexture(TU32 textureIndex, ETextureStyleType type);
  /// @brief Unbind textures of `textureIndexList` from activated shader.
  static void UnbindTextureList(const std::vector<TU32>& textureIndexList);

  /// @brief Draw command.
  static void Draw(EDrawType iType, bool iIsElement, TU32 iCount);
  /// @brief Instancing drawing command.
  static void DrawInstanced(EDrawType iType, bool iIsElement, TU32 iCount, TU32 iInstanceCount);

  /// @brief Query command. Shader program id must be valid and query enum also.
  MDY_NODISCARD static TI32 QueryShaderProgramIV(TU32 iShaderProgramId, TGlEnum iQueryEnum);
  /// @brief Query Float vector command.
  static void QueryFloatVector(TGlEnum iGLLowEnumCommand, TF32* iPtrRawFloatVector);

  /// @brief 
  MDY_NODISCARD static std::optional<std::tuple<std::string, TGlSize, TI32, EAttributeVariableType, TU32>>
  GetShaderProgramAttributeInfo(TU32 iShaderProgramId, TU32 iAttrIndex);
  /// @brief
  MDY_NODISCARD static std::optional<std::tuple<std::string, TGlSize, TI32, EUniformVariableType, TU32>>
  GetShaderProgramUniformInfo(TU32 iShaderProgramId, TU32 iUniformIndex);
  /// @brief
  MDY_NODISCARD static std::optional<std::string>
  GetShaderProgramUniformBlockInfo(TU32 iShaderProgramId, TU32 iUniformBlockIndex);

  /// @brief Update uniform matrix4x4 item. Specified shader must be valid and activated.
  static void UpdateUniformMatrix4(TU32 iId, const DMat4& iBuffer, bool iTransposed = false);
  /// @brief Update uniform matrix3x3 item. Specified shader must be valid and activated.
  static void UpdateUniformMatrix3(TU32 iId, const DMat3& iBuffer, bool iTransposed = false);
  /// @brief Update uniform matrix4x4 array. Specified shader must be valid and activated.
  static void UpdateUniformMatrix4Array(
    TU32 iId, 
    const std::vector<DMat4>& iBuffer, 
    bool iIransposed = false);
  /// @brief Update uniform vector3 (float) array. Specified shader must be valid and activated.
  static void UpdateUniformVector3Array(TU32 iId, const std::vector<DVec3>& iBfuffer);
  /// @brief Update uniform vector4 (float) item. Specified shader must be valid and activated.
  static void UpdateUniformVector4(TU32 iId, const DVec4& iBuffer);
  /// @brief Update uniform vector3 (float) item. Specified shader must be valid and activated.
  static void UpdateUniformVector3(TU32 iId, const DVec3& iBuffer);
  /// @brief Update uniform vector2 (float) item. Specified shader must be valid and activated.
  static void UpdateUniformVector2(TU32 iId, const DVec2& iBuffer);
  /// @brief Update uniform integer item. Specified shader must be valid and activated.
  static void UpdateUniformInteger(TU32 iId, const TI32& iBuffer);
  /// @brief Update uniform one unsigend (32-bit) value. 
  /// Specified shader must be valid and activated when updating.
  static void UpdateUniformUnsigned(TU32 iId, const TU32& iBuffer);
  /// @brief Update uniform one float value. Specified shader must be valid and activated.
  static void UpdateUniformFloat(TU32 iId, const TF32& iBuffer);
  /// @brief Update uniform float array. Specified shader must be valid and activated.
  static void UpdateUniformFloatArray(TU32 iId, const std::vector<TF32>& iBuffer);

  /// @struct __OutsideLockguard
  /// @brief GL graphics locking mutex instance.
  /// If this instance is out of scope, automatically resolve locking of mutex.
  struct MDY_PRIVATE(OutsideLockguard) final
  {
  public:
    MDY_PRIVATE(OutsideLockguard)() = default;
    ~MDY_PRIVATE(OutsideLockguard)() noexcept;

    MDY_PRIVATE(OutsideLockguard)(MDY_PRIVATE(OutsideLockguard)&& _) noexcept;
    MDY_PRIVATE(OutsideLockguard) operator=(MDY_PRIVATE(OutsideLockguard)&&) noexcept = delete;

  private:
    bool mIsMoved = false;
  };

  static MDY_PRIVATE(OutsideLockguard) MDY_PRIVATE(LockMutex)();
  static void MDY_PRIVATE(UnlockMutex)();

  /// @brief 
  static void SetupInitialGlobalStatus();

  /// @brief Insert global status of GL.
  static void PushInternalGlobalState(const DGlGlobalStates& iNewStatus);
  static void PopInternalGlobalState();

private:
  static std::mutex mGLCriticalSectionMutex;

  /// @brief Default format descriptor of VAO.
  MDY_NODISCARD static const DGlVaoBindInformation& GetDefaultAttributeFormatDescriptor() noexcept;

  /// @brief Global status stack for management. \n
  /// This container will be push & popped automatically by following rendering pipeline.
  /// This container must not be empty before termination of Dy application.
  static FCallStack<DGlGlobalStates> mInternalGlobalStatusStack;

  /// ▽ Actual state machine change logic will be operated in these stack.
  static FCallStack<bool> mInternal_FeatBlendStack;
  static FCallStack<bool> mInternal_FeatCullfaceStack;
  static FCallStack<bool> mInternal_FeatDepthTestStack;
  static FCallStack<bool> mInternal_FeatScissorTestStack;
  static FCallStack<DGlGlobalStates::DPolygonMode>   mInternal_PolygonModeStack;
  static FCallStack<DGlGlobalStates::DBlendMode>     mInternal_BlendModeStack;
  static FCallStack<DGlGlobalStates::DCullfaceMode>  mInternal_CullfaceModeStack;
  static FCallStack<DGlGlobalStates::DViewport>      mInternal_ViewportStack;
  static FCallStack<std::vector<PBlendingEquation>> sAttachmentBlendings;
};

/// @brief Critical section macro for graphic GL API.
#define MDY_GRAPHIC_SET_CRITICALSECITON() \
  const auto MDY_TOKENPASTE2(_, __LINE__) = ::dy::XGLWrapper::MDY_PRIVATE(LockMutex)()

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_WRAPPER_XGLWrapper_H
