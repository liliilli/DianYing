#ifndef GUARD_DY_BUILTIN_SHADERGL_LEVEL_BTSHOITINTEGRATION_H
#define GUARD_DY_BUILTIN_SHADERGL_LEVEL_BTSHOITINTEGRATION_H
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
  
///
/// @class FDyBtShOITIntegration
/// @brief Integrate Weight-blended OIT A-buffer and opaque rendered screen texture.
/// @reference https://github.com/NVIDIAGameWorks/GraphicsSamples/tree/master/samples/gl4-kepler/WeightedBlendedOIT
///
class FDyBtShOITIntegration final : public AGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(FDyBtShOITIntegration, "dyBtShOITIntegration")
public:
  FDyBtShOITIntegration();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_LEVEL_BTSHOITINTEGRATION_H