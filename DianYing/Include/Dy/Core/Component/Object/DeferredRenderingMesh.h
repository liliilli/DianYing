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

#include <Dy/Core/Component/MeshRenderer.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>

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

private:
  GLuint              mVao        = 0;
  GLuint              mVbo        = 0;

  CDyMeshRenderer     mRenderer;
  CDyShaderResource*  mShaderPtr  = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_RENDERING_MESH_H