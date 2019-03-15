#ifndef GUARD_DY_ELEMENT_INLINE_FACTORTMP_H
#define GUARD_DY_ELEMENT_INLINE_FACTORTMP_H
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

namespace dy
{

template<class TComponent, typename... TArgs>
TComponent* FActor::AddComponent(TArgs&&... args)
{
  return nullptr;
#ifdef false
  // Validation test
  MDY_TEST_IS_BASE_OF(ABaseComponent, TComponent);

  // If component is script, process the other subroutine. 
  if constexpr (std::is_same_v<CActorScript, TComponent> == true)
  {
    // Add and initialize component itself.
    // If component which just added is CDyScript, Call Initiate script first.
    auto* ptrComponent = this->pAddScriptComponent(std::forward<TArgs...>(args)...); 
    MDY_ASSERT_MSG_FORCE(ptrComponent != nullptr, "");
    return DyMakeNotNull(ptrComponent);
  }
  else
  {
    // Add and initialize component itself.
    auto componentPtr = std::make_unique<TComponent>(std::ref(*this));
    MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(std::forward<TArgs>(args)...));

    // If it is transform, move it to separated space. 
    if constexpr (std::is_same_v<CTransform, TComponent> == true)
    { // If component is not CDyScript but related to ADyBaseTransform (Transform components)
      MDY_ASSERT_MSG_FORCE(
        this->mTransform != nullptr, 
        "FActor::mTransform must be empty when insert transform component.");
      this->mTransform = std::move(componentPtr);
      return DyMakeNotNull(this->mTransform.get());
    }
    else if constexpr (IsSameClass<TComponent, CPhysicsRigidbody> == true)
    { // If component is CPhysicsRigidbody...
      MDY_ASSERT_MSG_FORCE(
        this->mRigidbody != nullptr, 
        "FActor::mRigidbody must be empty when insert rigidbody component.");
      this->mRigidbody = std::move(componentPtr);
      return DyMakeNotNull(this->mRigidbody.get());
    }
    else
    { // Otherwise remain, just return Ptr.
      auto&[value, reference] = this->mComponentList.emplace_back(
        std::make_pair(TComponentUnbindingType<TComponent>::Value, std::move(componentPtr))
      );
      return DyMakeNotNull(static_cast<TComponent*>(reference.get()));
    }
  }
#endif
}

template<class TGeneralComponent>
std::optional<TGeneralComponent*> FActor::GetGeneralComponent()
{
  static_assert(
    IsInheritancedFrom<TGeneralComponent, AGeneralBaseComponent>,
    "Failed to compile GetGeneralComponent function.");

  // Component matching process is using recursion of each component
  // from last derived component class to highest base component class.
  auto& componentList = this->pGetComponentList();
  for (auto& [type, component] : componentList)
  {
    if (component == nullptr) { continue; }
    if (component->IsTypeMatched(TGeneralComponent::__mHashVal) == true)
    {
      return static_cast<TGeneralComponent*>(component.get());
    }
  }

  // If there is no component to find.
  return std::nullopt;
}

template <class TGeneralComponent>
std::vector<NotNull<TGeneralComponent*>> FActor::GetGeneralComponentList()
{
  static_assert(
    IsInheritancedFrom<TGeneralComponent, AGeneralBaseComponent>,
    "Failed to get component list, required component type is not inheritenced from ABaseComponent");

  // Component matching process is using recursion of each component
  // from last derived component class to highest base component class.
  std::vector<NotNull<TGeneralComponent*>> resultList = {};

  auto& componentList = this->pGetComponentList();
  for (auto& [type, component] : componentList)
  {
    if (MDY_CHECK_ISEMPTY(component)) { continue; }
    if (component->IsTypeMatched(TGeneralComponent::__mHashVal) == true)
    {
      resultList.emplace_back(static_cast<TGeneralComponent*>(component.get()));
    }
  }

  return resultList;
}

template <class TComponent, typename... TArgs>
EDySuccess FActor::RemoveComponent(TArgs&&... args)
{
  static_assert(
    IsInheritancedFrom<TComponent, ABaseComponent>,
    "Failed to remove component, required component type is not inheritenced from ABaseComponent");

  if constexpr (std::is_base_of_v<AGeneralBaseComponent, TComponent>)
  {
    auto& componentList = this->pGetComponentList();
    auto it = std::find_if(
      MDY_BIND_BEGIN_END(componentList),
      [](const auto& item) { return item->IsTypeMatched(TComponent::__mHashVal); }
    );

    if (it == componentList.end()) { return DY_FAILURE; }
    this->pReleaseComponent(*it);
    componentList.erase(it);
    return DY_SUCCESS;
  }
  else if constexpr (std::is_same_v<CActorScript, TComponent>)
  {
    // @TODO IMPLEMENT SCRIPT DELETION USING DESCRIPTOR OR SCRIPT NAME.
    return this->RemoveScriptComponent(std::forward<TArgs>(args)...);
  }

  return DY_FAILURE;
}

}

#endif /// GUARD_DY_ELEMENT_INLINE_FACTORTMP_H
