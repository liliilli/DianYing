#ifndef GUARD_DY_BUILTIN_SHADERGL_Render_OpaqueEmissiveStatic_H
#define GUARD_DY_BUILTIN_SHADERGL_Render_OpaqueEmissiveStatic_H
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

#include <Dy/Builtin/Abstract/ADyGLShaderResource.h>

namespace dy::builtin
{

/// @class FDyBtShGlEmissiveStatic
/// @brief Builtin default deferred opqaue but emit fluorescent.
class FDyBtShGlEmissiveStatic final : public ADyGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(FDyBtShGlEmissiveStatic, "dyBtShGlEmissvieStatic")
public:
  FDyBtShGlEmissiveStatic();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_Render_BtShUvWorldPos_H