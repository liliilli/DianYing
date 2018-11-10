#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_DEPRECATED_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_DEPRECATED_H
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

#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Component/Ctor/PDyFontRenderer.h>

//!
//! Forward declaration
//!

namespace dy
{
class FDyText_Deprecated;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class CDyFontRenderer_Deprecated
/// @brief
///
class CDyFontRenderer_Deprecated final : public IDyInitializeHelper<PDyFontRendererCtorInformation_Deprecated>
{
public:
  CDyFontRenderer_Deprecated()           = default;
  virtual ~CDyFontRenderer_Deprecated()  = default;

  CDyFontRenderer_Deprecated(const CDyFontRenderer_Deprecated&)                                 = delete;
  CDyFontRenderer_Deprecated& operator=(const CDyFontRenderer_Deprecated&)                      = delete;
  CDyFontRenderer_Deprecated(CDyFontRenderer_Deprecated&& instance)                   noexcept  = default;
  CDyFontRenderer_Deprecated& operator=(CDyFontRenderer_Deprecated&& instance)        noexcept  = default;

  ///
  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(const PDyFontRendererCtorInformation_Deprecated& descriptor) override final;

  ///
  /// @brief Release resource of component.
  ///
  void Release() override final;

  ///
  /// @brief
  ///
  void Render();

private:
  /// LAZY NOT NULLABLE POINTER RAW PTR;
  FDyText_Deprecated* mFontObjectRawPtr = MDY_INITIALIZE_NULL;
  ///
  CDyShaderResource* mSampleShaderPtr = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_DEPRECATED_H