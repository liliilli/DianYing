#ifndef GUARD_DY_CORE_COMPONENT_OBJECT_GRID_H
#define GUARD_DY_CORE_COMPONENT_OBJECT_GRID_H
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

//#include <Dy/Core/Resource/MeshRenderer.h>

namespace dy
{

class FDyGrid final
{
public:
  FDyGrid();

  void RenderGrid() noexcept;

private:
  const int32_t       mStep       = 12;

  GLuint              mVao        = 0;
  GLuint              mVbo        = 0;

  //CDyMeshRenderer     mRenderer;
  //CDyShaderResource_Deprecated*  mShaderPtr  = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_OBJECT_GRID_H