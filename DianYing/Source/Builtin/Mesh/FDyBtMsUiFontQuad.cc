#include <precompiled.h>
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

/// Header file
#include <Dy/Builtin/Mesh/FDyBtMsUiFontQuad.h>

namespace dy
{

void FDyBtMsUiFontQuad::ConstructBuffer(_MOUT_ PDyBtMeshInstanceMetaInfo& property) noexcept
{
  property.mSpecifierName         = sName;
  property.mMeshUsage             = EDyMeshUsage::DynamicDraw;

  // (Position, Texture coord) stride.
  //DDyVector2{}, DDyVector2{1, 1}, DDyVector2{}, DDyVector2{1, 0},
  //DDyVector2{}, DDyVector2{0, 0}, DDyVector2{}, DDyVector2{0, 1}
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(1.f);
  property.mCustomMeshBuffer.emplace_back(1.f);

  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(1.f);
  property.mCustomMeshBuffer.emplace_back(0.f);

  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);

  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(0.f);
  property.mCustomMeshBuffer.emplace_back(1.f);

  property.mVAOBindingInfo.mIsUsingDefaultDyAttributeModel = false;
}

} /// ::dy namespace