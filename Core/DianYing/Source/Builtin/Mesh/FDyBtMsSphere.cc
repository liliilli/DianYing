#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/Mesh/FDyBtMsSphere.h>

namespace dy
{
  
void FDyBtMsSphere::ConstructBuffer(_MOUT_ PDyMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName = sName;

  // Position
  const auto a = DVec3{ 0, 1, 0 };
  const auto b = DVec3{ 0, 0, 1 };
  const auto c = DVec3{ 1, 0, 0 };
  const auto d = DVec3{ -1, -1, -1 }.Normalize();
  // Texture Coordination
  const auto ta = DVec2{ 1, 1 };
  const auto tb = DVec2{ 0, 1 };
  const auto tc = DVec2{ 1, 0 };
  const auto td = DVec2{ -1, -1 };

  // Make vertex information
  const TI32 level = 4;
  auto& submeshDesc = property.mDefaultMeshBuffer;
  pCreateVertexRecursively(a, b, c, ta, tb, tc, level, submeshDesc);
  pCreateVertexRecursively(b, c, d, tb, tc, td, level, submeshDesc);
  pCreateVertexRecursively(c, d, a, tc, td, ta, level, submeshDesc);
  pCreateVertexRecursively(d, a, b, td, ta, tb, level, submeshDesc);
}

void FDyBtMsSphere::pEmplaceVertex(
    _MIN_ const DVec3& position, _MIN_ const DVec2& texcoord,
    _MOUT_ PDyDefaultMeshInformation& container)
{
  DDefaultVertexInfo vertex;
  vertex.mPosition  = position;
  vertex.mNormal    = position;
  vertex.mTexCoord0 = texcoord;
  container.mVertexList.emplace_back(vertex);
}

void FDyBtMsSphere::pCreateVertexRecursively(
    _MIN_ const DVec3& a, _MIN_ const DVec3& b, _MIN_ const DVec3& c,
    _MIN_ const DVec2& ta, _MIN_ const DVec2& tb, _MIN_ const DVec2& tc,
    _MIN_ const TI32 level,
    _MIO_ PDyDefaultMeshInformation& container)
{
  if (level <= 0)
  {
    pEmplaceVertex(a, ta, container);
    pEmplaceVertex(b, tb, container);
    pEmplaceVertex(c, tc, container);
    return;
  }

  const auto ab = ((a + b) / 2.f).Normalize();
  const auto bc = ((b + c) / 2.f).Normalize();
  const auto ca = ((c + a) / 2.f).Normalize();

  const auto tab = ((ta + tb) / 2.f);
  const auto tbc = ((tb + tc) / 2.f);
  const auto tca = ((tc + ta) / 2.f);
  pCreateVertexRecursively(a, ab, ca, ta, tab, tca, level - 1, container);
  pCreateVertexRecursively(ab, b, bc, tab, tb, tbc, level - 1, container);
  pCreateVertexRecursively(c, ca, bc, tc, tca, tbc, level - 1, container);
  pCreateVertexRecursively(ab, bc, ca, tab, tbc, tca, level - 1, container);
}

} /// ::dy namespace