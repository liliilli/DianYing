#ifndef GUARD_DY_BUILTIN_SHADERGL_PostEffect_Emissive_BtShEmissiveToneMap_H
#define GUARD_DY_BUILTIN_SHADERGL_PostEffect_Emissive_BtShEmissiveToneMap_H
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

/// @class FDyBuiltinShaderGLEmissiveToneMap
/// @brief OpenGL Shader for post-processing tone-mapping blurred emissive attachment.
class FDyBuiltinShaderGLEmissiveToneMap final : public ADyGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(
    FDyBuiltinShaderGLEmissiveToneMap, 
    "dyBtShEmissiveToneMap")
public:
  FDyBuiltinShaderGLEmissiveToneMap();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_PostEffect_Emissive_BtShEmissiveBlur_H