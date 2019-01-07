#ifndef GUARD_DY_BUILTIN_MESH_FDYBTMSUIIMGQUAD_H
#define GUARD_DY_BUILTIN_MESH_FDYBTMSUIIMGQUAD_H
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

#include <Dy/Builtin/Abstract/ADyMeshResource.h>

namespace dy
{

///
/// @class FDyBtMsUiImageQuad
/// @brief Create mesh for ui image rendering information and resources.
///
class FDyBtMsUiImageQuad final : public ADyMeshResource
{
  MDY_REGISTER_RESOURCE_MESH(FDyBtMsUiImageQuad, "dyBtMsUiImageQuad");
};

} /// ::dy namespace

#endif /// GUARD_DY_BUILTIN_MESH_FDYBTMSUIIMGQUAD_H