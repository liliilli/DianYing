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

#include <Dy/Core/Resource/Internal/MaterialType.h>
#include <Dy/Component/CDyScript.h>
#include <Dy/Component/Internal/CDyActorScriptCpp.h>
#include <Dy/Component/Internal/CDyActorScriptLua.h>
#include <Dy/Component/Helper/TmpCheckInitilizeParams.h>
#include <Dy/Component/Helper/TmpCheckRemoveParams.h>
#include <Dy/Element/Object.h>
#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Abstract/ADyGeneralBaseComponent.h>
#include <Dy/Element/Abstract/ADyTransformable.h>
#include <Dy/Management/IO/MetaInfoManager.h>

namespace dy
{

///
/// @class FDyActor
/// @brief FFF
///
class FDyActor : public FDyObject
{
  using TComponentList = std::vector<std::unique_ptr<ADyGeneralBaseComponent>>;
  using TScriptList    = std::vector<std::unique_ptr<CDyScript>>;

  MDY_SET_CRC32_HASH_WITH_TYPE(FDyActor);
  MDY_SET_TYPEMATCH_FUNCTION(FDyObject, FDyActor);
public:
  FDyActor() = default;

  ///
  /// @brief Initialize FDyActor.
  /// @param objectMetaDesc Meta descriptor information instance for FDyActor.
  /// @return Success / Failure flag.
  ///
  MDY_NODISCARD EDySuccess Initialize(_MIN_ const PDyObjectMetaInfo& objectMetaDesc);

  ///
  /// @brief Release function (virtual) because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  ///
  MDY_NODISCARD virtual EDySuccess Release();

  ///
  /// @brief Activate FDyActor instance.
  ///
  void Activate() noexcept;

  ///
  /// @brief Deactivate FDyActor instance.
  ///
  void Deactivate() noexcept;

  ///
  /// @brief  Check FDyActor is activated or not.
  /// @return Check flag for activation checking.
  ///
  MDY_NODISCARD FORCEINLINE bool IsActivated() const noexcept
  {
    return this->mActivationFlag.GetOutput();
  }

  ///
  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  ///
  MDY_NODISCARD const std::string& GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  ///
  /// @brief Set FDyActor's hierarchial position to Parent's children FDyActor.
  /// Transform update will be held automatically.
  /// @param validParentRawPtr
  /// @TODO SCRIPT THIS
  ///
  void SetParent(_MIN_ NotNull<FDyActor*> validParentRawPtr) noexcept;

  ///
  /// @brief
  /// @param
  /// @TODO SCRIPT THIS
  ///
  void SetParentRelocateTransform(_MIN_ NotNull<FDyActor*> validParentRawPtr) noexcept;

  ///
  /// @brief Set FDyActor's hierarchial position to Scene's root.
  /// Transform update will be held automatically.
  ///
  void SetParentAsRoot() noexcept;

  ///
  /// @brief
  /// @TODO SCRIPT THIS
  ///
  void SetParentToRootRelocateTransform() noexcept;

  ///
  /// @brief  Check FDyActor has a parent FDyActor.
  /// @return If it has valid parent, return true but false.
  ///
  FORCEINLINE MDY_NODISCARD bool IsHaveParent() const noexcept
  {
    return MDY_CHECK_ISNOTNULL(this->mParentFDyActorRawPtr);
  }

  ///
  /// @brief  Return valid parent FDyActor pointer instance with wrapping optional.
  /// @return If parent is binded and exist, return optional valid pointer but just no value.
  ///
  FORCEINLINE MDY_NODISCARD std::optional<FDyActor*> GetParent() const noexcept
  {
    if (MDY_CHECK_ISNULL(this->mParentFDyActorRawPtr))  { return std::nullopt; }
    else                                                { return this->mParentFDyActorRawPtr; }
  }

  ///
  /// @brief Return actor's information
  /// @return information string.
  ///
  MDY_NODISCARD std::string ToString() override
  {
    return MDY_INITIALIZE_EMPTYSTR;
  }

  ///
  /// @brief  Get actual actor type
  /// @return Object type specifier
  ///
  MDY_NODISCARD FORCEINLINE EDyMetaObjectType GetActorType() const noexcept
  {
    return this->mActorType;
  }

  ///
  /// @brief  Add component and bind to this object instance.
  /// @tparam TComponent  Component type class argument.
  /// @tparam TArgs       Universal reference. Used to be arguments of Component constructor.
  /// @return Valid component instance pointer.
  ///
  template<class TComponent, typename... TArgs>
  MDY_NODISCARD NotNull<TComponent*> AddComponent(_MIN_ TArgs&&... args)
  {
    ///
    /// @brief Make script component (lua or cpp)
    ///
    static auto MakeScriptComponent = [this](const PDyScriptComponentMetaInfo& info)
    {
      const auto& instanceInfo = MDyMetaInfo::GetInstance().GetScriptMetaInformation(info.mDetails.mSpecifierName);
      MDY_ASSERT(instanceInfo.mScriptType != EDyScriptType::NoneError, "");

      std::unique_ptr<CDyScript> component = nullptr;
      if (instanceInfo.mScriptType == EDyScriptType::Cpp)
      {
        auto componentPtr = std::make_unique<CDyActorScriptCpp>(std::ref(*this));
        MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(info));
        component = std::move(componentPtr);
      }
      else
      {
        auto componentPtr = std::make_unique<CDyActorScriptLua>(std::ref(*this));
        MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(info));
        component = std::move(componentPtr);
      }

      return component;
    };

    //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //! FUNCTIONBODY ∨
    //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Integrity test
    MDY_TEST_IS_BASE_OF(ADyBaseComponent, TComponent);
    DyCheckComponentInitializeFunctionParams<TComponent, TArgs...>();

    if constexpr (std::is_same_v<CDyScript, TComponent>)
    {
      // Add and initialize component itself.
      // If component which just added is CDyScript, Call Initiate script first.
      auto& reference = this->mScriptList.emplace_back(MakeScriptComponent(std::forward<TArgs...>(args)...));
      return DyMakeNotNull(reference.get());
    }
    else
    {
      // Add and initialize component itself.
      auto componentPtr = std::make_unique<TComponent>(std::ref(*this));
      MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(std::forward<TArgs>(args)...));

      if constexpr (std::is_same_v<CDyTransform, TComponent>)
      { // If component is not CDyScript but related to ADyBaseTransform (Transform components)
        MDY_ASSERT(MDY_CHECK_ISEMPTY(this->mTransform), "FDyActor::mTransform must be empty when insert transform component.");

        this->mTransform.reset(componentPtr.release());
        return DyMakeNotNull(this->mTransform.get());
      }
      else
      { // Otherwise remain, just return Ptr.
        auto& reference = this->mComponentList.emplace_back(std::move(componentPtr));
        return DyMakeNotNull(static_cast<TComponent*>(reference.get()));
      }
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
    else if constexpr (std::is_same_v<CDyScript, TComponent>)
    {
      // @TODO IMPLEMENT SCRIPT DELETION USING DESCRIPTOR OR SCRIPT NAME.
      return this->RemoveScriptComponent(std::forward<TArgs>(args)...);
    }

    return DY_FAILURE;
  }

  ///
  /// @brief  Get script component pointer from script list using scriptName to verify.
  /// @param  scriptName Script name to verify and get.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  ///
  MDY_NODISCARD std::optional<CDyScript*> GetScriptComponent(_MIN_ const std::string& scriptName) noexcept;

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
  /// @brief
  ///
  void pPropagateActivationFlag() noexcept;

  DDy3StateBool                     mActivationFlag = {};
  /// Parent FDyActor raw-pointer data.
  FDyActor*                         mParentFDyActorRawPtr   = MDY_INITIALIZE_NULL;
  /// Transform component.
  std::unique_ptr<CDyTransform>     mTransform              = MDY_INITIALIZE_NULL;
  /// Component list (randomly) which attached to FDyActor instance (this!)
  TComponentList                    mComponentList          = {};
  /// Script list (specialized!)
  TScriptList                       mScriptList             = {};
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ACTOR_H