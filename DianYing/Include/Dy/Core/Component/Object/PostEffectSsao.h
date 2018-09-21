#ifndef GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H
#define GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H
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

#include <Dy/Core/Component/MeshRenderer.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>

namespace dy
{

class FDyPostEffectSsao final
{
public:
  FDyPostEffectSsao();
  ~FDyPostEffectSsao();

  ///
  /// @brief Rendering deferred contexts to default framebuffer.
  ///
  void RenderScreen();

private:
  ///
  void pDeleteFrameBufferComponents();

  ///
  void pDeleteSsaoShaderResource();

  ///
  void pDeleteSsaoBlurShaderResource();

  ///
  void pCreateSsaoFrameBufferComponents();

  ///
  void pCreateSsaoShaderResource();

  ///
  void pCreateSsaoBlurShaderResource();

  ///
  void pCreateMesh();

  ///
  void pCreateBlurFrameBufferComponent();

  ///
  void pDeleteBlurFrameBufferComponent();

  //!
  //! Mesh
  //!

  TU32                mTriangleVao            = MDY_NOT_INITIALIZED_0;
  TU32                mTriangleVbo            = MDY_NOT_INITIALIZED_0;

  //!
  //! General framebuffer
  //!

  TU32                mSsaoFrameBufferId      = MDY_NOT_INITIALIZED_0;
  TU32                mSsaoColorBuffer        = MDY_NOT_INITIALIZED_0;

  std::vector<DDyVector3> mSsaoKernel         = {};
  std::vector<DDyVector3> mSsaoNoise          = {};
  TU32                mSsaoNoiseTextureId     = MDY_NOT_INITIALIZED_0;

  TI32                mUniformSamples         = MDY_NOT_INITIALIZED_M1;
  TI32                mUniformKernelSize      = MDY_NOT_INITIALIZED_M1;
  TI32                mUniformRadius          = MDY_NOT_INITIALIZED_M1;
  TI32                mUniformBias            = MDY_NOT_INITIALIZED_M1;
  TI32                mUniformScreenSize      = MDY_NOT_INITIALIZED_M1;
  TI32                mUniformProjection      = MDY_NOT_INITIALIZED_M1;

  //!
  //! Blurring framebuffer
  //!

  TU32                mSsaoBlurFrameBufferId  = MDY_NOT_INITIALIZED_0;
  TU32                mSsaoBlurColorBuffer    = MDY_NOT_INITIALIZED_0;

  CDyMeshRenderer     mRenderer;
  CDyShaderResource*  mSsaoShaderPtr          = nullptr;
  CDyShaderResource*  mSsaoBlurShaderPtr      = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H