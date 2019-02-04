#ifndef GUARD_DY_COMPONENT_CDYPHYSICSCOLLIDERBOX_H
#define GUARD_DY_COMPONENT_CDYPHYSICSCOLLIDERBOX_H
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

#include <Dy/Component/CDyPhysicsCollider.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @class CDyPhysicsColliderBox
/// @brief Box collider component of physics simuation.
///
class CDyPhysicsColliderBox final : public CDyPhysicsCollider
{
public:
  CDyPhysicsColliderBox(_MIN_ FDyActor& actorReference) : CDyPhysicsCollider(actorReference) {};
  virtual ~CDyPhysicsColliderBox() = default;
      
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyPhysicsColliderBox);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::CDyPhysicsCollider, CDyPhysicsColliderBox)
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsColliderBox);

  /// @brief Initialize component.
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc) override final;
  /// @brief Release component.
  void Release() override final;

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;

  /// @brief Initialize internal (PhysX) resource.
  void InitializeInternalResource() override final; 
  /// @brief Release internal (PhysX) resource.
  void ReleaseInternalResource() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSCOLLIDERBOX_H