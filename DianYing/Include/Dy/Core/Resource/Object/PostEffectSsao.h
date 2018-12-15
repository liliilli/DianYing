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

#include <Dy/Helper/Type/Vector3.h>

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

  TU32                mTriangleVao            = MDY_INITIALIZE_DEFUINT;
  TU32                mTriangleVbo            = MDY_INITIALIZE_DEFUINT;

  //!
  //! General framebuffer
  //!

  TU32                mSsaoFrameBufferId      = MDY_INITIALIZE_DEFUINT;
  TU32                mSsaoColorBuffer        = MDY_INITIALIZE_DEFUINT;

  std::vector<DDyVector3> mSsaoKernel         = {};
  std::vector<DDyVector3> mSsaoNoise          = {};
  TU32                mSsaoNoiseTextureId     = MDY_INITIALIZE_DEFUINT;

  TI32                mUniformSamples         = MDY_INITIALIZE_DEFINT;
  TI32                mUniformKernelSize      = MDY_INITIALIZE_DEFINT;
  TI32                mUniformRadius          = MDY_INITIALIZE_DEFINT;
  TI32                mUniformBias            = MDY_INITIALIZE_DEFINT;
  TI32                mUniformScreenSize      = MDY_INITIALIZE_DEFINT;
  TI32                mUniformProjection      = MDY_INITIALIZE_DEFINT;

  //!
  //! Blurring framebuffer
  //!

  TU32                mSsaoBlurFrameBufferId  = MDY_INITIALIZE_DEFUINT;
  TU32                mSsaoBlurColorBuffer    = MDY_INITIALIZE_DEFUINT;

  //CDyShaderResource_Deprecated*  mSsaoShaderPtr          = nullptr;
  //CDyShaderResource_Deprecated*  mSsaoBlurShaderPtr      = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_OBJECT_POSTEFFECT_SSAO_H