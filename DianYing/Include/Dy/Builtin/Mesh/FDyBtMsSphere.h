#ifndef GUARD_DY_BUILTIN_MESH_FDYBTMSSPHERE_H
#define GUARD_DY_BUILTIN_MESH_FDYBTMSSPHERE_H
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

#include <Dy/Builtin/Abstract/ADyMeshResource.h>

namespace dy
{

///
/// @class FDyBtMsSphere
/// @brief Create mesh sphere information and resources.
///
class FDyBtMsSphere final : public ADyMeshResource
{
  MDY_REGISTER_RESOURCE_MESH(FDyBtMsSphere, "dyBtMsSphere");

  void pEmplaceVertex(
      _MIN_ const DDyVector3& normalized_position,
      _MIN_ const DDyVector2&,
      _MOUT_ PDyDefaultMeshInformation& container);

  void pCreateVertexRecursively(
      _MIN_ const DDyVector3& a, _MIN_ const DDyVector3& b, _MIN_ const DDyVector3& c,
      _MIN_ const DDyVector2& ta, _MIN_ const DDyVector2& tb, _MIN_ const DDyVector2& tc,
      _MIN_ const TI32 level,
      _MIO_ PDyDefaultMeshInformation& container
  );
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_MESH_FDYBTMSSPHERE_H