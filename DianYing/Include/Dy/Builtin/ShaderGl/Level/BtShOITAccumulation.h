#ifndef GUARD_DY_BUILTIN_SHADERGL_LEVEL_BTSHOITACCUMULATION_H
#define GUARD_DY_BUILTIN_SHADERGL_LEVEL_BTSHOITACCUMULATION_H
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
/// @class FDyBtShOITAccumulation
/// @brief Render potentially transparent meshes which have transparent material,
/// and accumulate properties into A-buffer using Weight-blended Order Independent Transparency.
///
/// @reference https://github.com/NVIDIAGameWorks/GraphicsSamples/tree/master/samples/gl4-kepler/WeightedBlendedOIT
///
class FDyBtShOITAccumulation final : public AGLShaderResource
{
  MDY_REGISTER_RESOURCE_WITH_SPECIFIER(FDyBtShOITAccumulation, "dyBtShOITAccumulation")
public:
  FDyBtShOITAccumulation();
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_SHADERGL_LEVEL_BTSHOITACCUMULATION_H