#ifndef GUARD_DY_ELEMENT_ACTOR_H
#define GUARD_DY_ELEMENT_ACTOR_H
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

#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Element/Object.h>
#include <Dy/Element/Abstract/ADyTransformable.h>
#include <Dy/Element/Descriptor/LevelDescriptor.h>
#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Component/CDyScript.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>

namespace dy
{

///
/// @class FDyActor
/// @brief FFF
///
class FDyActor : public FDyObject, public ADyTransformable
{
  using TComponentList = std::vector<std::unique_ptr<ADyGeneralBaseComponent>>;
  using TScriptList    = std::vector<std::unique_ptr<CDyScript>>;

  MDY_SET_CRC32_HASH_WITH_TYPE(FDyActor);
  MDY_SET_TYPEMATCH_FUNCTION(FDyObject, FDyActor);
public:
  FDyActor() = default;

  ///
  /// @brief Release function (virtual) because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  ///
  [[nodiscard]] virtual EDySuccess Release();

  ///
  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  ///
  [[nodiscard]] const std::string& GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  ///
  /// @brief Set FDyActor's hierarchial position to Parent's children FDyActor.
  /// Transform update will be held automatically.
  ///
  /// @param validParentRawPtr
  ///
  void SetParent(NotNull<FDyActor*> validParentRawPtr) noexcept;

  ///
  /// @brief
  ///
  /// @param
  ///
  void SetParentRelocateTransform(NotNull<FDyActor*> validParentRawPtr) noexcept;

  ///
  /// @brief Set FDyActor's hierarchial position to Scene's root.
  /// Transform update will be held automatically.
  ///
  void SetParentToRoot() noexcept;

  ///
  /// @brief
  ///
  void SetParentToRootRelocateTransform() noexcept;

  ///
  /// @brief Return actor's information
  /// @return information string.
  ///
  std::string ToString() override
  {
    return MDY_INITILAIZE_EMPTYSTR;
  }

  ///
  /// @brief  Get actual actor type
  /// @return Object type specifier
  ///
  [[nodiscard]] FORCEINLINE EDyFDyObjectType GetActorType() const noexcept
  {
    return this->mActorType;
  }

#define MDY_TEST_IS_BASE_OF(__MABaseType__, __MADerivedType__) \
  static_assert(std::is_base_of_v<__MABaseType__, __MADerivedType__>, \
                MDY_TO_STRING(__MADerivedType__) " is not a derived type of " MDY_TO_STRING(__MABaseType__) ".")

  ///
  /// @brief  Add component and bind to this object instance.
  /// @tparam TComponent  Component type class argument.
  /// @tparam TArgs       Universal reference. Used to be arguments of Component constructor.
  /// @return Valid component instance pointer.
  ///
  template<class TComponent, typename... TArgs>
  MDY_NODISCARD NotNull<TComponent*> AddComponent(TArgs&&... args)
  {
    MDY_TEST_IS_BASE_OF(ADyBaseComponent, TComponent);

    // Add and initialize component itself.
    auto p = std::make_unique<TComponent>(std::ref(*this), std::forward<TArgs>(args)...);

    if constexpr (std::is_same_v<CDyScript, TComponent>)
    { // If component which just added is CDyScript, Call Initiate script first.
      auto& reference = this->mScriptList.emplace_back(std::move(p));
      reference->Initiate();

      return reference.get();
    }
    else
    { // Otherwise, just return Ptr.
      auto& reference = this->mComponentList.emplace_back(std::move(p));

      return static_cast<TComponent*>(reference.get());
    }
  }

  ///
  /// @brief  Return component raw-pointer from general component list (not script)
  /// @tparam TGeneralComponent Component type argument.
  /// @return If found, return TGeneralComponent* but not found, return no value.
  ///
  template<class TGeneralComponent>
  MDY_NODISCARD std::optional<TGeneralComponent*> GetGeneralComponent()
  {
    MDY_TEST_IS_BASE_OF(ADyGeneralBaseComponent, TGeneralComponent);

    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    for (auto& component : this->mComponentList)
    {
      if (component->IsTypeMatched(TGeneralComponent::__mHashVal))
      {
        return static_cast<TGeneralComponent*>(component.get());
      }
    }

    // If there is no component to find.
    return std::nullopt;
  }

  ///
  /// @brief  Return component raw-pointer list from general component list.
  /// @tparam TGeneralComponent Component type argument.
  /// @return If found, return TGeneralComponent* list but not found, return empty list.
  ///
  template <class TGeneralComponent>
  MDY_NODISCARD std::vector<NotNull<TGeneralComponent*>> GetGeneralComponents()
  {
    MDY_TEST_IS_BASE_OF(ADyGeneralBaseComponent, TGeneralComponent);

    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    std::vector<NotNull<TGeneralComponent*>> resultList = {};

    for (auto& component : this->mComponentList)
    {
      if (component->IsTypeMatched(TGeneralComponent::__mHashVal))
      {
        resultList.emplace_back(static_cast<TGeneralComponent*>(component.get()));
      }
    }

    return resultList;
  }

  ///
  /// @brief Remove component.
  /// @tparam TComponent Component type argument.
  /// @return If found, return true but otherwise false.
  ///
  template <class TComponent>
  MDY_NODISCARD EDySuccess RemoveComponent()
  {
    MDY_TEST_IS_BASE_OF(ADyBaseComponent, TComponent);
#ifdef false
    using component::CParticleSpawner;
    using component::_internal::EComponentType;
    using manager::object::pMoveParticleSpawner;
#endif

    if constexpr (std::is_base_of_v<ADyGeneralBaseComponent, TComponent>)
    {
      auto it = std::find_if(MDY_BIND_BEGIN_END(this->mComponentList),
          [](const auto& item) { return item->IsTypeMatched(TComponent::__mHashVal); }
      );

      if (it == this->mComponentList.end()) { return DY_FAILURE; }
      this->mComponentList.erase(it);
      return DY_SUCCESS;
    }
    else if constexpr (std::is_same_v<CDyScript, TComponent>)
    {
      // @TODO IMPLEMENT SCRIPT DELETION USING DESCRIPTOR OR SCRIPT NAME.

      static_cast<CDyScript*>(it->get())->Destroy();

      this->mScriptList.erase(it);
      return DY_SUCCESS;
    }
#ifdef false
    if (it->second == EComponentType::Particle) {
      auto ptr = std::unique_ptr<CParticleSpawner>(static_cast<CParticleSpawner*>(it->first.release()));
      pMoveParticleSpawner(ptr);
    }
    else if (it->second == EComponentType::Script) {
      static_cast<component::CScriptFrame*>(it->first.get())->Destroy();
    }
#endif

    return DY_FAILURE;
  }

#ifdef false
  ///
  /// @brief Get non-owning components list from component list of object.
  /// All components's bound object reference are undefined, so you should control
  /// these components well not touching object reference like a GetBindObject().
  ///
  template <class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  std::vector<std::unique_ptr<TComponent>> pPopComponents()
  {

    using opgs16::component::_internal::EComponentType;
    if (m_components.empty()) return decltype(pPopComponents<TComponent>()){};

    std::vector<std::unique_ptr<TComponent>> result_list;

    // m_componentsからTTypeであるコンポネントに対して他のところに移す。
    auto it = --m_components.end();
    int32_t remove_back_count = 0;
    for (auto& [component, item] : m_components) {
      if (!component) continue;
      if (component->DoesTypeMatch(OP16_GET_HASH(TComponent), TComponent::__string_literal)) {
        result_list.push_back(std::unique_ptr<TComponent>(static_cast<TComponent*>(component.release())));
        ++remove_back_count;

        while (it->second == EComponentType::Particle && &(it->first) != &component) {
          --it;
        }
        if (&(it->first) == &component) continue;
        component.swap(it->first);
        // ReSharper disable CppAssignedValueIsNeverUsed
        item = it->second;
        // ReSharper restore CppAssignedValueIsNeverUsed
        --it;
      }
    }
    // 移動した分だけ真後ろから削除する。
    while (remove_back_count > 0) {
      m_components.pop_back();
      --remove_back_count;
    }
    return std::move(result_list);
  }
#endif


protected:
  /// Actual actor type to discriminate actor type is so cast object with statically.
  MDY_TRANSIENT EDyFDyObjectType mActorType = EDyFDyObjectType::Error;

private:
  /// Parent FDyActor raw-pointer data.
  FDyActor*         mParentFDyActorRawPtr   = MDY_INITIALIZE_NULL;

  /// Component list (randomly) which attached to FDyActor instance (this!)
  TComponentList    mComponentList          = {};
  /// Script list (specialized!)
  TScriptList       mScriptList             = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ACTOR_H