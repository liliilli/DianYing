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
  
void FDyBtMsSphere::ConstructBuffer(_MOUT_ PDyBtMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName = sName;

  // Position
  const auto a = DDyVector3{ 0, 1, 0 };
  const auto b = DDyVector3{ 0, 0, 1 };
  const auto c = DDyVector3{ 1, 0, 0 };
  const auto d = DDyVector3{-1,-1,-1 }.Normalize();
  // Texture Coordination
  const auto ta = DDyVector2{ 1, 1 };
  const auto tb = DDyVector2{ 0, 1 };
  const auto tc = DDyVector2{ 1, 0 };
  const auto td = DDyVector2{ -1, -1 };

  // Make vertex information
  const TI32 level = 3;
  auto& submeshDesc = property.mDefaultMeshBuffer;
  pCreateVertexRecursively(a, b, c, ta, tb, tc, level, submeshDesc);
  pCreateVertexRecursively(b, c, d, tb, tc, td, level, submeshDesc);
  pCreateVertexRecursively(c, d, a, tc, td, ta, level, submeshDesc);
  pCreateVertexRecursively(d, a, b, td, ta, tb, level, submeshDesc);
}

void FDyBtMsSphere::pEmplaceVertex(
    _MIN_ const DDyVector3& position, _MIN_ const DDyVector2& texcoord,
    _MOUT_ PDyDefaultMeshInformation& container)
{
  DDyVertexInformation vertex;
  vertex.mPosition  = position;
  vertex.mNormal    = position;
  vertex.mTexCoords = texcoord;
  container.mVertexList.emplace_back(vertex);
}

void FDyBtMsSphere::pCreateVertexRecursively(
    _MIN_ const DDyVector3& a, _MIN_ const DDyVector3& b, _MIN_ const DDyVector3& c,
    _MIN_ const DDyVector2& ta, _MIN_ const DDyVector2& tb, _MIN_ const DDyVector2& tc,
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