#ifndef GUARD_DY_ELEMENT_ABSTRACT_ABaseComponent_H
#define GUARD_DY_ELEMENT_ABSTRACT_ABaseComponent_H
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

#include <Dy/Helper/System/HashCompileCrc32.h>
#include <Dy/Helper/Type/DTristateBool.h>
#include <Dy/Element/Interface/IToString.h>
#include <Dy/Element/Type/ATristateSwitcher.h>

//!
//! Forward declaration
//!

namespace dy
{
class FActor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class ABaseComponent
/// @brief Base component type
MDY_ABSTRACT ABaseComponent : public IToString, public ATristateSwitcher
{
public:
  ABaseComponent(FActor& actorReference);

  ABaseComponent(const ABaseComponent&)             = delete;
  ABaseComponent& operator=(const ABaseComponent&)  = delete;
  ABaseComponent(ABaseComponent&& instance) noexcept;
  ABaseComponent& operator=(ABaseComponent&& instance) noexcept;

  virtual ~ABaseComponent() = default;

  /// @brief  Check component is activated.
  /// @return Check flag whether component is activated or not.
  MDY_NODISCARD bool IsComponentActivated() const noexcept;

  /// @brief  Type match for static casting of instance in runtime
  /// @param  hashVal CRC32 hashValue
  /// @return If type is matched return true but false.
  virtual bool IsTypeMatched(const TU32 hashVal) const noexcept
  {
    return ABaseComponent::__mHashVal == hashVal;
  }

  /// @brief  Get binded FActor's pointer instance.
  /// @return Binded FActor pointer instance.
  MDY_NODISCARD FActor* GetBindedActor() noexcept
  {
    return this->mBindedActor;
  }

  /// @brief  Get binded FActor's pointer instance.
  /// @return Binded FActor pointer instance.
  MDY_NODISCARD const FActor* GetBindedActor() const noexcept
  {
    return this->mBindedActor;
  }

protected:
  /// Transient variable, list id for updating
  MDY_TRANSIENT TI32  mActivatedUpdateListId  = MDY_INITIALIZE_DEFINT;

private:
  /// Binded actor raw pointer.
  FActor*           mBindedActor            = MDY_INITIALIZE_NULL;

  MDY_SET_CRC32_HASH_WITH_TYPE(ABaseComponent);
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ABSTRACT_ABaseComponent_H