#ifndef GUARD_DY_CORE_RENDERING_BASICSHADOW_H
#define GUARD_DY_CORE_RENDERING_BASICSHADOW_H
#include "Dy/Helper/Type/Vector3.h"
#include "Dy/Core/Component/Resource/ShaderResource.h"

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

namespace dy
{
class CDyMeshRenderer;
} /// ::dy namespace

namespace dy
{
///
/// @class FDyBasicShadow
/// @brief Basic shadow effect dkdk
///
class FDyBasicShadow final
{
public:
  FDyBasicShadow();
  ~FDyBasicShadow();

  ///
  /// @brief Rendering deferred contexts to default framebuffer.
  ///
  void RenderScreen(const CDyMeshRenderer& renderer);

  ///
  /// @brief
  ///
  [[nodiscard]] TU32 GetDepthTextureId() const noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] FORCEINLINE TU32 GetShadowFrameBufferId() const noexcept
  {
    return this->mShadowFramebufferId;
  }

  ///
  /// @brief
  ///
  [[nodiscard]] std::pair<TI32, TI32> GetShadowMapSize() const noexcept;

private:
  /// Create shading framebuffer component
  void pCreateFramebufferComponents();
  /// Release shading framebuffer component
  void pReleaseFrameBufferComponents();

  //!
  //! General framebuffer
  //!

  TU32                mShadowFramebufferId    = MDY_INITIALIZE_DEFUINT;
  TU32                mShadowDepthValueBuffer = MDY_INITIALIZE_DEFUINT;
  TI32                mMapWidth               = 512;
  TI32                mMapHeight              = 512;

  CDyShaderResource*  mShaderResource;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_RENDERING_BASICSHADOW_H