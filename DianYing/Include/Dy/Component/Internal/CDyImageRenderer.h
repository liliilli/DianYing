#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYIMAGERENDERER_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYIMAGERENDERER_H
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

#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Ctor/PDyImageRenderer.h>
#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

namespace dy
{

///
/// @class CDyImageRenderer
/// @brief Renderer for basic gauge bar. `FDyImage`.
///
class CDyImageRenderer final
{
public:
  CDyImageRenderer(_MIN_ FDyImage& mRefObject);
  virtual ~CDyImageRenderer() = default;

  CDyImageRenderer(const CDyImageRenderer&) = delete;
  CDyImageRenderer& operator=(const CDyImageRenderer&) = delete;
  CDyImageRenderer(CDyImageRenderer&& instance)            noexcept = default;
  CDyImageRenderer& operator=(CDyImageRenderer&& instance) noexcept = default;

  /// @brief Render guage var.
  void Render();

  /// @brief Update shader binding.
  void UpdateMaterial() noexcept;

private:
  /// LAZY NOT NULLABLE POINTER RAW PTR;
  FDyImage*                   mPtrObject = MDY_INITIALIZE_NULL;
  TDyLResourceBinderMesh      mBinderQuadMesh{};
  TDyLResourceBinderMaterial  mBinderMaterial{};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYIMAGERENDERER_H