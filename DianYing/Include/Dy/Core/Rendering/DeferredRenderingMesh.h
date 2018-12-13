#ifndef GUARD_DY_RENDERING_MESH_H
#define GUARD_DY_RENDERING_MESH_H
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

#include <queue>
#include <Dy/Core/Resource/Resource_Deprecated/ShaderResource_Deprecated.h>

//!
//! Forward declaration
//!

namespace dy
{
struct  PDyGlAttachmentInformation;
struct  DDyUboDirectionalLight;
struct  PDyGlFrameBufferInformation;
class   CDyModelResource_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class FDyDeferredRenderingMesh final
{
public:
  FDyDeferredRenderingMesh();
  ~FDyDeferredRenderingMesh();

  ///
  /// @brief Rendering deferred contexts to default framebuffer.
  ///
  void RenderScreen();

  ///
  /// @brief Clear properties of given framebuffer.
  ///
  void Clear();

  ///
  /// @brief  Issue available directional light index if not available, just return nullopt;
  /// @return Directional light index value or nullopt (no value).
  ///
  MDY_NODISCARD std::optional<TI32> GetAvailableDirectionalLightIndex() noexcept;

  ///
  /// @brief  Update FUCKFUCKFUCKFCUFKCUKFUCK
  /// @param  index
  /// @param  container
  /// @return
  ///
  MDY_NODISCARD EDySuccess UpdateDirectionalLightValueToGpu(
      _MIN_ const TI32 index,
      _MIN_ const DDyUboDirectionalLight& container);

  ///
  /// @brief
  /// @param  index
  /// @return
  ///
  MDY_NODISCARD EDySuccess UnbindDirectionalLight(_MIN_ const TI32 index);

private:
  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pInitializeShaderSetting();

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD EDySuccess pInitializeUboBuffers();

  ///
  /// @brief  Try to getting attachment instance pointers from management instance.
  /// @return If successful to getting all attachment pointers, return true.
  ///
  MDY_NODISCARD EDySuccess pTryGetAttachmentPointers();

  inline static constexpr TI32 sDirectionalLightCount = 5;

  CDyShaderResource_Deprecated*  mShaderPtr      = MDY_INITIALIZE_NULL;

  /// Attachment information pointer

  PDyGlAttachmentInformation* mAttachmentPtr_Unlit          = MDY_INITIALIZE_NULL;
  PDyGlAttachmentInformation* mAttachmentPtr_Normal         = MDY_INITIALIZE_NULL;
  PDyGlAttachmentInformation* mAttachmentPtr_Specular       = MDY_INITIALIZE_NULL;
  PDyGlAttachmentInformation* mAttachmentPtr_ModelPosition  = MDY_INITIALIZE_NULL;
  PDyGlAttachmentInformation* mAttachmentPtr_Shadow         = MDY_INITIALIZE_NULL;
  bool                        mIsAttachmentPtrBinded        = false;

  /// mDyBtFbUiBasic
  PDyGlFrameBufferInformation*  mDyBtFbScrFin               = MDY_INITIALIZE_NULL;
  CDyModelResource_Deprecated*             mScreenRenderTrianglePtr    = MDY_INITIALIZE_NULL;

  std::queue<TI32>    mDirLightAvailableList  = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MESH_H