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

#include <Dy/Core/Resource/Type/TDyResourceBinder.h>

namespace dy
{

class FDyText;

///
/// @class CDyFontRenderer
/// @brief
///
class CDyFontRenderer final 
{
public:
  CDyFontRenderer(_MIN_ FDyText& iPtrWidget);
  ~CDyFontRenderer() = default;

  CDyFontRenderer(const CDyFontRenderer&)            = delete;
  CDyFontRenderer& operator=(const CDyFontRenderer&) = delete;

  /// @brief
  void Render();

private:
  FDyText*                  mPtrWidget = nullptr; 
  TDyLResourceBinderShader  mBinderShader{};
  TDyLResourceBinderMesh    mBinderFontMesh{};
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_CDYFONTRENDERER_H