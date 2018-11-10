#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H
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

//!
//! Forward declaration
//!

namespace dy
{
class FDyText;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @struct DDyFontRendererMetaInformation
/// @brief
///
struct DDyFontRendererMetaInformation final
{ /// Font renderer pointer address.
  FDyText* mFontComponentPtr = nullptr;
};

///
/// @class CDyFontRenderer
/// @brief
///
class CDyFontRenderer final : public IDyInitializeHelper<DDyFontRendererMetaInformation>
{
public:
  CDyFontRenderer()           = default;
  virtual ~CDyFontRenderer()  = default;

  CDyFontRenderer(const CDyFontRenderer&)                                 = delete;
  CDyFontRenderer& operator=(const CDyFontRenderer&)                      = delete;
  CDyFontRenderer(CDyFontRenderer&& instance)                   noexcept  = default;
  CDyFontRenderer& operator=(CDyFontRenderer&& instance)        noexcept  = default;

  ///
  /// @brief  Initialize with descriptor.
  /// @param  descriptor
  /// @return If successful, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess Initialize(const DDyFontRendererMetaInformation& descriptor) override final;

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
  FDyText* mFontObjectRawPtr = MDY_INITIALIZE_NULL;
  ///
  CDyShaderResource* mSampleShaderPtr = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H