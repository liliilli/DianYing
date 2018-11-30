#ifndef GUARD_DY_BUILTIN_MODEL_SPHERE_H
#define GUARD_DY_BUILTIN_MODEL_SPHERE_H
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
#include <Dy/Builtin/Abstract/ADyModelResource.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDySubmeshInformationDescriptor;
struct DDyVector3;
struct DDyVector2;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy::builtin
{

///
/// @class FDyBuiltinModelSphere
/// @brief Model sphere
///
class FDyBuiltinModelSphere final : public ADyModelResource
{
  MDY_REGISTER_RESOURCE_MODEL(FDyBuiltinModelSphere, "dyBtModelSphere");

private:
  ///
  /// @brief
  ///
  void pEmplaceVertex(
      _MIN_ const DDyVector3& normalized_position,
      _MIN_ const DDyVector2&,
      _MOUT_ PDySubmeshInformationDescriptor& container);

  ///
  /// @brief
  ///
  void pCreateVertexRecursively(
      _MIN_ const DDyVector3& a, _MIN_ const DDyVector3& b, _MIN_ const DDyVector3& c,
      _MIN_ const DDyVector2& ta, _MIN_ const DDyVector2& tb, _MIN_ const DDyVector2& tc,
      _MIN_ const TI32 level,
      _MIO_ PDySubmeshInformationDescriptor& container
  );
};

} /// ::dy::builtin namespace

#endif /// GUARD_DY_BUILTIN_MODEL_SPHERE_H