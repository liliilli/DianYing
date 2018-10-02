#ifndef GUARD_DY_ELEMENT_ABSTRACT_ADYBASECOMPONENT_H
#define GUARD_DY_ELEMENT_ABSTRACT_ADYBASECOMPONENT_H
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

#include <Dy/Helper/HashCompileCrc32.h>
#include "Dy/Helper/Type/ThreeStateBool.h"

//!
//! Forward declaration
//!

namespace dy
{
class FDyActor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class ADyBaseComponent
/// @brief Base component type
///
MDY_ABSTRACT ADyBaseComponent
{
public:
  ADyBaseComponent(FDyActor& actorReference);
  virtual ~ADyBaseComponent() = default;

  ///
  /// @brief Activate component, but component will be operated only when parent, and cursor FDyActor is enabled too.
  ///
  virtual void Activate() noexcept;

  ///
  /// @brief Deactive component.
  ///
  virtual void Deactivate() noexcept;

  ///
  /// @brief  Update parent boolean value from FDyActor 3-state boolean variable.
  /// @param  actorBool 3-state boolean function from FDyActor.
  ///
  void pPropagateParentActorActivation(const DDy3StateBool& actorBool) noexcept;

  ///
  /// @brief  Check component is activated.
  /// @return Check flag whether component is activated or not.
  ///
  MDY_NODISCARD bool IsComponentActivated() const noexcept;

private:

  /// Activate flag for operating component.
  DDy3StateBool mActivateFlag = {};

  /// Transient variable, list id for updating
  MDY_TRANSIENT TI32 mActivatedUpdateListId = MDY_INITIALIZE_DEFINT;

  ///
  /// @brief  Type match for static casting of instance in runtime
  /// @param  hashVal CRC32 hashValue
  /// @return If type is matched return true but false.
  ///
  virtual bool IsTypeMatched(const TU32 hashVal) const noexcept
  {
    return ADyBaseComponent::__mHashVal == hashVal;
  }

  MDY_SET_CRC32_HASH_WITH_TYPE(ADyBaseComponent);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ABSTRACT_ADYBASECOMPONENT_H