#ifndef GUARD_DY_BUILTIN_SHADERGL_PostEffect_Emissive_BtShEmissiveBlur_H
#define GUARD_DY_BUILTIN_SHADERGL_PostEffect_Emissive_BtShEmissiveBlur_H
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

#include <Dy/Builtin/Abstract/AGLShaderResource.h>

namespace dy::builtin
{

/// @class FDyBuiltinShaderGLEmissiveBlur
/// @brief OpenGL Shader for post-processing blurring HDR emissive attachment.
class FDyBuiltinShaderGLEmissiveBlur final : public AGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(
    FDyBuiltinShaderGLEmissiveBlur, 
    "dyBtShEmissiveBlur")
public:
  FDyBuiltinShaderGLEmissiveBlur();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_PostEffect_Emissive_BtShEmissiveBlur_H