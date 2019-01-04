#ifndef GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H
#define GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H
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

#include <Dy/Component/Abstract/ADyPhysicsShape.h>

namespace dy
{

///
/// @class CDyPhysicsShapeBox
/// @brief
/// @TODO SCRIPT THIS
///
class CDyPhysicsShapeBox final : public ADyPhysicsShape
{
public:
  CDyPhysicsShapeBox(FDyActor& actorReference) : ADyPhysicsShape(actorReference) {};
  virtual ~CDyPhysicsShapeBox() = default;

  ///
  /// @brief
  /// @param
  /// @return
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const void* desc);

  ///
  /// @brief
  ///
  void Release();

  CDyPhysicsShapeBox(const CDyPhysicsShapeBox&)                                 = delete;
  CDyPhysicsShapeBox& operator=(const CDyPhysicsShapeBox&)                      = delete;
  CDyPhysicsShapeBox(CDyPhysicsShapeBox&& instance)                   noexcept  = default;
  CDyPhysicsShapeBox& operator=(CDyPhysicsShapeBox&& instance)        noexcept  = default;

  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyPhysicsShape, CDyPhysicsShapeBox);
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsShapeBox);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H