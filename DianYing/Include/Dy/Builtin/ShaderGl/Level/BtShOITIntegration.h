#ifndef GUARD_DY_BUITLIN_SHADERGL_LEVEL_BTSHOITINTEGRATION_H
#define GUARD_DY_BUITLIN_SHADERGL_LEVEL_BTSHOITINTEGRATION_H
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

///
/// @class FDyBtShOITIntegration
/// @brief Render potentially transparent meshes which have transparent material,
/// and accumulate properties into A-buffer using Weight-blended Order Independent Transparency.
///
/// Integrate Accumulation buffer (color) and (weight) which is GL_TEXTURE_RECTANGLE.
///
/// @reference https://github.com/NVIDIAGameWorks/GraphicsSamples/tree/master/samples/gl4-kepler/WeightedBlendedOIT
///
class FDyBtShOITIntegration final : public ADyGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(FDyBtShOITIntegration, "dyBtShOITIntegration")
public:
  FDyBtShOITIntegration();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUITLIN_SHADERGL_LEVEL_BTSHOITINTEGRATION_H