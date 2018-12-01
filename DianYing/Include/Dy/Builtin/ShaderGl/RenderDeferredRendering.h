#ifndef GUARD_DY_BUILTIN_SHADERGL_RENDERDEFERREDRENDERING_H
#define GUARD_DY_BUILTIN_SHADERGL_RENDERDEFERREDRENDERING_H
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

#include <string_view>
#include <Dy/Builtin/Abstract/ADyGLShaderResource.h>

namespace dy::builtin
{

///
/// @class FDyBuiltinShaderGLRenderDeferredRendering
/// @brief OpenGL Shader program which collects from other render pipe and render final screen (scene).
///
class FDyBuiltinShaderGLRenderDeferredRendering final : public ADyGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(FDyBuiltinShaderGLRenderDeferredRendering, "dyBtShaderGlDeferred")
public:
  FDyBuiltinShaderGLRenderDeferredRendering();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_RENDERDEFERREDRENDERING_H