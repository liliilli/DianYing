#ifndef GUARD_DY_BUILTIN_SHADERGL_RENDERBASICSHADOW_H
#define GUARD_DY_BUILTIN_SHADERGL_RENDERBASICSHADOW_H
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

namespace dy::builtin
{

///
/// @class FDyBuiltinShaderGLRenderBasicShadow
/// @brief Instantiate and create resource for basic shadow rendering.
///
class FDyBuiltinShaderGLRenderBasicShadow final
{
public:
  FDyBuiltinShaderGLRenderBasicShadow();

  /// Constant name for creating DDyShaderInformation
  inline static MDY_SET_IMMUTABLE_STRING(sName, "dyBtShaderGlRenderBscShadow");
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_RENDERBASICSHADOW_H