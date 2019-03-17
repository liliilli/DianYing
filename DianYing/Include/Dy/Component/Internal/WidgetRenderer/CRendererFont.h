#ifndef GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H
#define GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H
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

#include <Dy/Core/Resource/Type/TResourceBinder.h>

//!
//! Forward declaration
//!

namespace dy
{
class FWidgetText;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class CRendererFont
/// @brief Rendering instance for font (SDF) Rendering.
class CRendererFont final 
{
public:
  CRendererFont(FWidgetText& iWidget);
  ~CRendererFont() = default;

  CRendererFont(const CRendererFont&)            = delete;
  CRendererFont& operator=(const CRendererFont&) = delete;

  /// @brief
  void Render();

private:
  FWidgetText*                mPtrWidget = nullptr; 
  TDyResourceBinderShader mBinderShader{};
  TDyResourceBinderMesh   mBinderFontMesh{};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H