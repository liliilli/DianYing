#ifndef GUARD_DY_CORE_COMPONENT_MATERIAL_H
#define GUARD_DY_CORE_COMPONENT_MATERIAL_H
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

#include <Dy/Core/Component/Internal/MaterialType.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyShaderResource;
class CDyTextureComponent;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

class CDyMaterialComponent
{
public:
  CDyMaterialComponent()  = default;
  ~CDyMaterialComponent() = default;
  CDyMaterialComponent(const CDyMaterialComponent&)             = delete;
  CDyMaterialComponent(CDyMaterialComponent&&)                  = default;
  CDyMaterialComponent& operator=(const CDyMaterialComponent&)  = delete;
  CDyMaterialComponent& operator=(CDyMaterialComponent&&)       = default;

  [[nodiscard]]
  EDySuccess pInitializeMaterial(const PDyMaterialConstructionDescriptor& materialConstructionDescriptor);

  // @todo TEMPORAL FUNCTION
  void TemporalRender();

private:
  CDyShaderResource* mShaderResourcePtr = nullptr;
  std::vector<CDyTextureComponent*> mTextureResourcePtrs;

  EDyMaterialBlendMode mBlendMode = EDyMaterialBlendMode::Opaque;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_MATERIAL_H