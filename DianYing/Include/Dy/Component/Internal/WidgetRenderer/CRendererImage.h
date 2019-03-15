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

#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyImage;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CRendererImage
/// @brief Renderer for basic gauge bar. `FDyImage`.
class CRendererImage final
{
public:
  CRendererImage(FDyImage& iRefObject);
  ~CRendererImage() = default;

  CRendererImage(const CRendererImage&) = delete;
  CRendererImage& operator=(const CRendererImage&) = delete;
  CRendererImage(CRendererImage&& instance)            noexcept = default;
  CRendererImage& operator=(CRendererImage&& instance) noexcept = default;

  /// @brief Render guage var.
  void Render();

  /// @brief Update shader binding.
  void UpdateRenderableTarget() noexcept;

  /// @brief Get using mateiral pointer. If using texture, just return nullptr;
  MDY_NODISCARD TDyResourceBinderMaterial* GetUsingMaterial();

private:
  /// LAZY NOT NULLABLE POINTER RAW PTR;
  FDyImage*               mPtrObject = MDY_INITIALIZE_NULL;
  TDyResourceBinderMesh   mBinderQuadMesh{};
  TDyResourceBinderShader mDefaultImageShader{"dyBtShaderGlRenderUiImage"};
  std::unique_ptr<IDyRenderableBinder> mBinderRenderable = nullptr; 
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYIMAGERENDERER_H