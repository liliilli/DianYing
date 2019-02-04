#ifndef GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H
#define GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H
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

#include <Dy/Component/Interface/IDyInitializeHelper.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Meta/Information/ComponentMetaInformation.h>

namespace dy
{

///
/// @class CDyPhysicsCollider
/// @brief
/// @TODO SCRIPT THIS
///
class CDyPhysicsCollider final : public ADyGeneralBaseComponent, public IDyInitializeHelper<PDyColliderComponentMetaInfo>
{
public:
  CDyPhysicsCollider(FDyActor& actorReference) : ADyGeneralBaseComponent(actorReference) {};
  virtual ~CDyPhysicsCollider() = default;
      
  MDY_ONLY_MOVEABLE_PROPERTIES_DEFAULT(CDyPhysicsCollider);
  MDY_SET_TYPEMATCH_FUNCTION(::dy::ADyGeneralBaseComponent, CDyPhysicsCollider)
  MDY_SET_CRC32_HASH_WITH_TYPE(CDyPhysicsCollider);

  /// @brief 
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyColliderComponentMetaInfo& desc) override final;
  /// @brief
  void Release() override final;

  /// @brief Update anyway.
  void Update(_MIN_ TF32 dt) override final {};

  /// @brief Get string information of this component.
  MDY_NODISCARD std::string ToString() override final;

private:
  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_CDYPHYSICSSHAPEBOX_H