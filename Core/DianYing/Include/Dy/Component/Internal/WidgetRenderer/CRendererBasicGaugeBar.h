#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYBASICGAUGEBARRENDERER_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYBASICGAUGEBARRENDERER_H
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

#include <Dy/Component/Interface/IInitializeHelper.h>
#include <Dy/Component/Ctor/PBarTypeRenderer.h>
#include <Dy/Core/Resource/Type/TResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PCtorBasicGaugeBarRendererInformation;
class FWidgetBasicGaugeBar;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CRendererBasicGaugeBar
/// @brief Renderer for basic gauge bar. `FWidgetBasicGaugeBar`.
class CRendererBasicGaugeBar final : public IInitializeHelper<PCtorBasicGaugeBarRendererInformation>
{
public:
  CRendererBasicGaugeBar() = default;
  virtual ~CRendererBasicGaugeBar() = default;

  CRendererBasicGaugeBar(const CRendererBasicGaugeBar&) = delete;
  CRendererBasicGaugeBar& operator=(const CRendererBasicGaugeBar&) = delete;
  CRendererBasicGaugeBar(CRendererBasicGaugeBar&& instance)            noexcept = default;
  CRendererBasicGaugeBar& operator=(CRendererBasicGaugeBar&& instance) noexcept = default;

  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess Initialize(const PCtorBasicGaugeBarRendererInformation& descriptor) override final;

  /// @brief Release resource of component.
  void Release() override final;

  /// @brief Render guage var.
  void Render();

private:
  /// LAZY NOT NULLABLE POINTER RAW PTR;
  FWidgetBasicGaugeBar*         mPtrBarObject = MDY_INITIALIZE_NULL;
  TDyResourceBinderMesh    mBinderBarMesh{};
  TDyResourceBinderShader  mBinderShader = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYBASICGAUGEBARRENDERER_H