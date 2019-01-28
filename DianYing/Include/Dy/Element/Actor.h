#ifndef GUARD_DY_ELEMENT_ACTOR_H
#define GUARD_DY_ELEMENT_ACTOR_H
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

#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Component/Helper/TmpCheckRemoveParams.h>
#include <Dy/Element/Object.h>
#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Element/Abstract/Actor/ADyActorBinderContainer.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Component/Actor/CDyActorScript.h>
#include <Dy/Helper/Internal/FDyNameGenerator.h>

namespace dy
{

/// @class FDyActor
/// @brief Dy Actor type which consist of almost every object in Level of Dy.
class FDyActor : public FDyObject, public FDyNameGenerator, public ADyActorBinderContainer<FDyActor>, public FDy3WaySwitcher
{
  using TComponentList = std::vector<std::unique_ptr<ADyGeneralBaseComponent>>;
  using TScriptList    = std::vector<std::unique_ptr<CDyActorScript>>;

  MDY_SET_CRC32_HASH_WITH_TYPE(FDyActor);
  MDY_SET_TYPEMATCH_FUNCTION(FDyObject, FDyActor);
public:
  using TActorSmtPtr   = std::unique_ptr<FDyActor>;
  using TActorMap      = std::unordered_map<std::string, TActorSmtPtr>;

  /// @brief Initialize FDyActor.
  /// @param objectMetaDesc Meta descriptor information instance for FDyActor.
  FDyActor(_MIN_ const PDyObjectMetaInfo& objectMetaDesc, _MIN_ FDyActor* iPtrParent = nullptr);
  explicit FDyActor(_MIN_ const PDyActorCreationDescriptor& iDesc, _MIN_ FDyActor* iPtrParent = nullptr);

  /// @brief Release function (virtual) because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  virtual ~FDyActor();

  /// @brief Destory self and tree.
  void DestroySelf();

  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  MDY_NODISCARD const std::string& GetActorName() const noexcept;

  /// @brief Get full specifier name from this to root like a `This_Name.Head~1.Head~2`
  MDY_NODISCARD std::string MDY_PRIVATE_SPECIFIER(GetFullSpecifierName)() const noexcept; 

  /// @brief Set FDyActor's parent to valid input actor.
  void SetParent(_MIN_ FDyActor& refParentActor) noexcept;

  /// @brief Set FDyActor's hierarchial position to Level's root.
  /// Transform update will be held automatically.
  void SetParentAsRoot() noexcept;

  /// @brief  Check FDyActor has a parent FDyActor.
  /// @return If it has valid parent, return true but false.
  MDY_NODISCARD bool IsHaveParent() const noexcept;

  /// @brief  Return valid parent FDyActor pointer instance with wrapping optional.
  /// @return If parent is binded and exist, return optional valid pointer but just no value.
  MDY_NODISCARD FDyActor* GetParent() const noexcept;

  /// @brief  Return this actor has children object, empty object will be neglected.
  /// @return If having children, return true.
  MDY_NODISCARD bool IsHavingChildrenObject() const noexcept;

  /// @brief Return this actor.
  MDY_NODISCARD TActorMap& GetChildrenContainer() noexcept;

  /// @brief Return actor's information
  /// @return information string.
  MDY_NODISCARD std::string ToString() override;

  /// @brief  Get actual actor type
  /// @return Object type specifier
  MDY_NODISCARD EDyMetaObjectType GetActorType() const noexcept;

  /// @brief Get actor's tag name.
  MDY_NODISCARD const std::string& GetActorTag() const noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>> 
  GetAllActorsWithTag(_MIN_ const std::string& iTagSpecifier) const noexcept;

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept;
 
  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithName(_MIN_ const std::string& iNameSpecifier) const noexcept; 

  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithNameRecursive(_MIN_ const std::string& iNameSpecifier) const noexcept; 
  
  ///
  /// @brief  Add component and bind to this object instance.
  /// @tparam TComponent  Component type class argument.
  /// @tparam TArgs       Universal reference. Used to be arguments of Component constructor.
  /// @return Valid component instance pointer.
  ///
  template<class TComponent, typename... TArgs>
  NotNull<TComponent*> AddComponent(_MIN_ TArgs&&... args)
  {
    // Validation test
    MDY_TEST_IS_BASE_OF(ADyBaseComponent, TComponent);
    //DyCheckComponentInitializeFunctionParams<TComponent, TArgs...>();

    if constexpr (std::is_same_v<CDyActorScript, TComponent> == true)
    {
      // Add and initialize component itself.
      // If component which just added is CDyScript, Call Initiate script first.
      auto ptr = MDY_PRIVATE_SPECIFIER(MakeScriptComponent)(std::forward<TArgs...>(args)...);
      auto& reference = this->mScriptList.emplace_back(std::move(ptr));
      return DyMakeNotNull(reference.get());
    }
    else
    {
      // Add and initialize component itself.
      auto componentPtr = std::make_unique<TComponent>(std::ref(*this));
      MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(std::forward<TArgs>(args)...));

      if constexpr (std::is_same_v<CDyTransform, TComponent> == true)
      { // If component is not CDyScript but related to ADyBaseTransform (Transform components)
        MDY_ASSERT(MDY_CHECK_ISEMPTY(this->mTransform), "FDyActor::mTransform must be empty when insert transform component.");
        this->mTransform = std::move(componentPtr);
        return DyMakeNotNull(this->mTransform.get());
      }
      else
      { // Otherwise remain, just return Ptr.
        auto& reference = this->mComponentList.emplace_back(std::move(componentPtr));
        return DyMakeNotNull(static_cast<TComponent*>(reference.get()));
      }
    }
  }

  /// @brief Make script component (lua or cpp)
  MDY_NODISCARD std::unique_ptr<CDyActorScript> 
  MDY_PRIVATE_SPECIFIER(MakeScriptComponent)(_MIN_ const PDyScriptComponentMetaInfo& info);
  
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
      if (component->IsTypeMatched(TGeneralComponent::__mHashVal) == true)
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
  MDY_NODISCARD std::vector<NotNull<TGeneralComponent*>> GetGeneralComponentList()
  {
    MDY_TEST_IS_BASE_OF(ADyGeneralBaseComponent, TGeneralComponent);

    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    std::vector<NotNull<TGeneralComponent*>> resultList = {};

    for (auto& component : this->mComponentList)
    {
      if (component->IsTypeMatched(TGeneralComponent::__mHashVal) == true)
      {
        resultList.emplace_back(static_cast<TGeneralComponent*>(component.get()));
      }
    }

    return resultList;
  }

  ///
  /// @brief  Remove component.
  /// @tparam TComponent Component type argument.
  /// @tparam TArgs      Arguments
  /// @return If found, return true but otherwise false.
  ///
  template <class TComponent, typename... TArgs>
  MDY_NODISCARD EDySuccess RemoveComponent(TArgs&&... args)
  {
    // Integrity test
    MDY_TEST_IS_BASE_OF(ADyBaseComponent, TComponent);
    DyCheckComponentRemoveFunctionParams<TComponent, TArgs...>();

    if constexpr (std::is_base_of_v<ADyGeneralBaseComponent, TComponent>)
    {
      auto it = std::find_if(MDY_BIND_BEGIN_END(this->mComponentList),
          [](const auto& item) { return item->IsTypeMatched(TComponent::__mHashVal); }
      );

      if (it == this->mComponentList.end()) { return DY_FAILURE; }
      this->mComponentList.erase(it);
      return DY_SUCCESS;
    }
    else if constexpr (std::is_same_v<CDyActorScript, TComponent>)
    {
      // @TODO IMPLEMENT SCRIPT DELETION USING DESCRIPTOR OR SCRIPT NAME.
      return this->RemoveScriptComponent(std::forward<TArgs>(args)...);
    }

    return DY_FAILURE;
  }

  /// @brief Try remove script instances list. \n
  /// But this funtion does not remove script instance actually, but just forward script list to GC-list.
  void MDY_PRIVATE_SPECIFIER(TryRemoveScriptInstances)() noexcept;

  /// @brief Try detach dependent components from dy level management system.
  void MDY_PRIVATE_SPECIFIER(TryDetachDependentComponents)() noexcept;

  ///
  /// @brief  Get script component pointer from script list using scriptName to verify.
  /// @param  scriptName Script name to verify and get.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  ///
  MDY_NODISCARD CDyActorScript* GetScriptComponent(_MIN_ const std::string& scriptName) noexcept;

  ///
  /// @brief  Remove script component manually from script list using scriptName to verify.
  /// @param  scriptName  Script name to verify and remove from FDyActor.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  ///
  MDY_NODISCARD EDySuccess RemoveScriptComponent(_MIN_ const std::string& scriptName) noexcept;

  ///
  /// @brief  Get tranform component pointer from FDyActor instance.
  /// @return Valid transform pointer instance.
  ///
  MDY_NODISCARD NotNull<CDyTransform*> GetTransform() noexcept;

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

  ///
  /// @brief
  ///
  void pUpdateActivateFlagFromParent() noexcept;

protected:
  /// Actual actor type to discriminate actor type is so cast object with statically.
  MDY_TRANSIENT EDyMetaObjectType   mActorType = EDyMetaObjectType::NoneError;

private:
  ///
  void TryActivateInstance() override final;
  ///
  void TryDeactivateInstance() override final;
  
  /// @brief
  void MDY_PRIVATE_SPECIFIER(CreateComponentList)(_MIN_ const TComponentMetaList& iMetaComponentList);

  ///
  /// @brief
  ///
  void pPropagateActivationFlag() noexcept;

  /// Parent FDyActor raw-pointer data.
  FDyActor*                     mPtrParentActor    = MDY_INITIALIZE_NULL;
  /// Transform component.
  std::unique_ptr<CDyTransform> mTransform         = MDY_INITIALIZE_NULL;
  /// Component list (randomly) which attached to FDyActor instance (this!)
  TComponentList                mComponentList     = {};
  /// Script list (specialized!)
  TScriptList                   mScriptList        = {};
  /// Actor list (hierarchial version)
  TActorMap                     mChildActorMap     = {};
  /// @brief Tag specifier 
  std::string                   mActorTagSpecifier = MDY_INITIALIZE_EMPTYSTR;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ACTOR_H