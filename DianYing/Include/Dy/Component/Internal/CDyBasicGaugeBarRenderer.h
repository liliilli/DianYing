#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYBASICGAUGEBARRENDERER_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYBASICGAUGEBARRENDERER_H
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

#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Ctor/PDyBarTypeRenderer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyBasicGaugeBarRendererCtorInformation;
class FDyBasicGaugeBar;
class CDyShaderResource_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyBasicGaugeBarRenderer
/// @brief Renderer for basic gauge bar. `FDyBasicGaugeBar`.
///
class CDyBasicGaugeBarRenderer final : public IDyInitializeHelper<PDyBasicGaugeBarRendererCtorInformation>
{
public:
  CDyBasicGaugeBarRenderer() = default;
  virtual ~CDyBasicGaugeBarRenderer() = default;

  CDyBasicGaugeBarRenderer(const CDyBasicGaugeBarRenderer&) = delete;
  CDyBasicGaugeBarRenderer& operator=(const CDyBasicGaugeBarRenderer&) = delete;
  CDyBasicGaugeBarRenderer(CDyBasicGaugeBarRenderer&& instance)            noexcept = default;
  CDyBasicGaugeBarRenderer& operator=(CDyBasicGaugeBarRenderer&& instance) noexcept = default;

  ///
  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(const PDyBasicGaugeBarRendererCtorInformation& descriptor) override final;

  /// @brief Release resource of component.
  void Release() override final;

  /// @brief Render guage var.
  void Render();

private:
  /// LAZY NOT NULLABLE POINTER RAW PTR;
  FDyBasicGaugeBar*   mPtrBarObject  = MDY_INITIALIZE_NULL;
  ///
  CDyShaderResource_Deprecated*  mPtrShader      = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYBASICGAUGEBARRENDERER_H