#ifndef GUARD_DY_BUITLIN_MESH_DEBUG_FDyBtMsDebugCapsule_H
#define GUARD_DY_BUITLIN_MESH_DEBUG_FDyBtMsDebugCapsule_H
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

#include <Dy/Builtin/Abstract/AMeshResource.h>

namespace dy::builtin
{

///
/// @class FDyBtMsDebugCapsule
/// @brief mesh Capsule for debugging collision shape. (not AABB)
///
class FDyBtMsDebugCapsule final : public AMeshResource
{
  MDY_REGISTER_RESOURCE_MESH(FDyBtMsDebugCapsule, "dyBtMsDebugCapsule");
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUITLIN_MESH_DEBUG_FDyBtMsDebugCapsule_H
