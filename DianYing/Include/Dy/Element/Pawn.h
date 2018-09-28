#ifndef GUARD_DY_CORE_COMPONENT_OBJECT_OBJECT_H
#define GUARD_DY_CORE_COMPONENT_OBJECT_OBJECT_H
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

#include <memory>
#include <unordered_map>
#include <string>

#include <Dy/Element/Interface/IDyScriptable.h>
#include <Dy/Helper/Type/Matrix4.h>
#include <Dy/Element/Actor.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDyObjectInformation;
} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy {

///
/// @class FDyPawn
/// @brief The class for every object to update and draw.
///
/// Object abstract class stores common values and inheritable by derived each object class.
/// When using each m_object_list you have to use this type as a storing type to avoid cracking of
/// polymorphism.
/// Each object can be called using Update and Draw with shader to use.
///
class FDyPawn : public FDyActor, public IDyScriptable
{
  using TNameCounterMap   = std::unordered_map<std::string, int32_t>;
	using TGameObjectSmtPtr = std::unique_ptr<FDyPawn>;
	using TGameObjectMap    = std::unordered_map<std::string, TGameObjectSmtPtr>;

public:
	FDyPawn() = default;
	virtual ~FDyPawn() = default;

  FDyPawn(const FDyPawn&)             = delete;
  FDyPawn& operator=(const FDyPawn&)  = delete;
  FDyPawn(FDyPawn&&)                  = default;
  FDyPawn& operator=(FDyPawn&&)       = default;

  /// Initialize properties
  [[nodiscard]] EDySuccess Initialize(const DDyObjectInformation& desc);

  /// Release properties
  [[nodiscard]] EDySuccess Release();

  /// Return pawn's information.
  std::string ToString() override
  {
    return MDY_NOT_INITILAIZED_STR;
  }

#ifdef false
  ///
  /// @brief Set tag with tag name. This method will check whether or not exist matched tag name
  /// in SettingManager. If not exist, do nothing and chagne error flag.
  /// @param[in] tag_name Tag name
  ///
  void SetObjectTag(const std::string& tag_name) noexcept;

  ///
  /// @brief Overloaded version of SetObjectTag(tag_name)
  ///
  void SetObjectTag(const int32_t tag_index) noexcept;

  ///
  /// @brief Get tag index of this object.
  /// @return Tag index value.
  ///
  int32_t GetObjectTagIndexOf() const noexcept;

  ///
  /// @brief Get Tag Name of this object. (different with name of object)
  /// This methods return tag name by referencing SettingManager in body.
  /// @return Tag name string.
  ///
  const std::string& GetObjectTagNameOf() const noexcept;
  //!
  //! Object creation.
  //!

	///
	/// @brief Destroy child object has unique tag key but not recursively.
	/// @param[in] child_object_name Object name.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroied, return ture.
	///
	bool DestroyGameObject(const std::string& child_object_name);

  ///
  /// @brief Destory child object with address.
  /// @param[in] child_object Object reference.
  /// @param[in] is_recursive Flag for destruction of specified object recursively.
	/// @return Success/Failed tag.
  /// If arbitary m_object_list has been destroyed, return ture.
  ///
  bool DestroyGameObject(const FDyPawn& child_object, bool is_recursive = false);

	///
	/// @brief Get children tag list.
	/// @return Children's tags container of object.
	///
	std::vector<std::string> GetGameObjectNameList() const;

	///
	/// @brief Get children reference.
	/// @return Children m_object_list component list.
	///
	TGameObjectMap& GetGameObjectList();

	///
	/// @brief Get arbitary child object.
	/// @param[in] subobjectName The name of object to find.
	/// @return Object's raw-pointer instance. this cannot removeable.
	///
	FDyPawn* GetChildSubObject(const std::string& subobjectName);

	///
	/// @brief This initiate object as a child of base object.
	///
	/// This method create & initiate object as a child, make a leaf of object hierarchy tree.
	/// Created type instance is implicitly converted (up-cast) to Object class type.
	///
	/// If you must initiate specific derived class with initialization parameters,
	/// you can type them _args parameter. This method gets variadic inputs initialized with
	/// appropriate c-tor.
	///
	/// You have to <> parenthesis to input specific class type to create.
	///
  template <
    class TCObjectType,
    class... TConstructionArgs,
    class = std::enable_if_t<IsCObjectBase<TCObjectType>>
  >
  TCObjectType* CreateGameObject(const std::string object_name, TConstructionArgs&&... args) {
      const auto object_final_name = pCreateChildTag(object_name);

    auto [result_pair, result] = mChildSubObjectMap.try_emplace(object_final_name, nullptr);
    if (!result) {
      PHITOS_ASSERT(result, "Object did not be made properly.");
      return nullptr;
    }

    // Set hash value and parent as this.
    result_pair->second = std::make_unique<TCObjectType>(std::forward<TConstructionArgs>(args)...);
    TGameObjectSmtPtr& object_ref = result_pair->second;
    object_ref->pSetHash(object_final_name);
    object_ref->mParentPtr = this;

    return static_cast<TCObjectType*>(object_ref.get());
  }

  //!
  //! Component creation.
  //!

  ///
  /// @brief
  /// Add component and bind to this object instance.
  ///
  /// @param[in] TComponent Component type class argument.
  /// @param[in] _Params&& Universal reference. Used to be arguments of Component constructor.
  ///
  using _Component = component::_internal::CComponent;
  template<
    class TComponent,
    typename... _Params,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  TComponent* AddComponent(_Params&&... params) {
    using EComponentType = component::_internal::EComponentType;

    auto type = EComponentType::Normal;
    if constexpr (std::is_base_of_v<component::IDyScriptable, TComponent>) {
      type = EComponentType::Script;
    }
    else if constexpr (std::is_base_of_v<component::CParticleSpawner, TComponent>) {
      type = EComponentType::Particle;
    }

    m_components.push_back(std::make_pair(std::make_unique<TComponent>(std::forward<_Params>(params)...), type));

    if constexpr (std::is_base_of_v<component::IDyScriptable, TComponent>) {
      static_cast<component::IDyScriptable*>(m_components.rbegin()->first.get())
          ->Initiate();
    }

    return GetComponent<TComponent>();
  }

  ///
  /// @brief Return component raw-pointer.
  /// @tparam TComponent Component type argument.
  /// @return If found, return TComponent* but not found, return nullptr.
  ///
  template<
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  TComponent* GetComponent() {
    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    for (auto& [component, type] : m_components) {
      if (component->DoesTypeMatch(TComponent::__hash_val, TComponent::__string_literal))
        return static_cast<TComponent*>(component.get());
    }

    // If there is no component to find.
    return nullptr;
  }

  ///
  /// @brief Return component raw-pointer list.
  /// @tparam TComponent Component type argument.
  /// @return If found, return TComponent* list but not found, return nullptr.
  ///
  template <
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  std::vector<TComponent*> GetComponents() {
    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    std::vector<TComponent*> result_component_list{};

    for (auto& [component, item] : m_components) {
      if (component->DoesTypeMatch(TComponent::__hash_val, TComponent::__string_literal))
        result_component_list.push_back(static_cast<TComponent*>(component.get()));
    }

    // If there is no component to find.
    return result_component_list;
  }

  ///
  /// @brief Get non-owning components list from component list of object.
  /// All components's bound object reference are undefined, so you should control
  /// these components well not touching object reference like a GetBindObject().
  ///
  template <
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  std::vector<std::unique_ptr<TComponent>> pPopComponents() {
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

  ///
  /// @brief Remove component.
  /// @tparam TComponent Component type argument.
  /// @return If found, return true but otherwise false.
  ///
  template <
    class TComponent,
    typename = std::enable_if_t<std::is_base_of_v<_Component, TComponent>>
  >
  bool RemoveComponent() {
    using component::CParticleSpawner;
    using component::_internal::EComponentType;
    using manager::object::pMoveParticleSpawner;

    auto it = std::find_if(m_components.begin(), m_components.end(),
        [](const auto& item) {
          return item.first->DoesTypeMatch(OP16_GET_HASH(TComponent), TComponent::__string_literal);
        }
    );

    if (it != m_components.cend()) {
      if (it->second == EComponentType::Particle) {
        auto ptr = std::unique_ptr<CParticleSpawner>(static_cast<CParticleSpawner*>(it->first.release()));
        pMoveParticleSpawner(ptr);
      }
      else if (it->second == EComponentType::Script) {
        static_cast<component::IDyScriptable*>(it->first.get())->Destroy();
      }

      /// Too much execution time
      m_components.erase(it);
      return true;
    }
    return false;
  }

protected:
  /// The container stores child object (pawn, component etc.).
	TGameObjectMap mChildSubObjectMap;

private:
  ///
  /// @brief Return hash value of this object.
  ///
  FORCEINLINE uint32_t pGetHash() const noexcept
  {
    PHITOS_ASSERT(__mHashInitialized == true, "Must call pSetHash() prior to this function.");
    return this->__mHashValue;
  }

  ///
  /// @brief Set hash value
  ///
  FORCEINLINE void pSetHash(const std::string& name) const noexcept {
    PHITOS_ASSERT(__mHashInitialized == false, "Hash value of object is already defined.");

    mPawnName           = name;
    __mHashValue        = hash::DyToCrc32Hash(name.c_str());
    __mHashInitialized  = true;

    MDY_LOG_DEBUG_D("Create hash value for object, [Name : {0}] [Hash : {1:x}]", name, __mHashValue);
  }



  ///
  /// @brief Create child object name.
  /// @param[in] name
  /// @return
  ///
  std::string pCreateChildTag(const std::string& name) noexcept;

  FDyPawn* pGetGameObjectResursively(const std::string& object_name) noexcept;

  //!
  //! Propagation
  //!

  ///
  /// @brief Get parent world's normal axis value (x, y, z) which been affected by world rotation.
  ///
  const std::array<DDyVector3, 3>& pGetParentWorldPropagateAxisValue() const noexcept;

  ///
  /// @brief
  ///
  const DDyVector3& pGetParentWorldSummedPositionValue() const noexcept;

  ///
  /// @brief Get all parent's summed world rotation euler angle values (x, y, z).
  ///
  const DDyVector3& pGetParentSummedWorldRotationAngle() const noexcept;

  ///
  /// @brief Get all parent's producted world scale values (x, y, z);
  ///
  const DDyVector3& pGetParentSummedProductedScaleValue() const noexcept;

  ///
  /// @brief Get rotation vector of total world angle from object tree root.
  ///
  const DDyVector3& pfGetRotationTotalWorldAngle();

  ///
  /// @brief
  ///
  void pSetWorldPropagatedPositionForcely(const DDyVector3& vector) noexcept;

  ///
  /// @brief
  ///
  void PropagateActivation(phitos::enums::EActivated value) noexcept;

  ///
  /// @brief Calculate and set up new final activation flag.
  ///
  void CalculateActivation();

  ///
  /// @brief
  ///
  void Propagate();
#endif

private:
#ifdef false
  void pCallPhysicsCallback(_internal::EColliderCollisionState call_state, bool is_collision_function, component::_internal::CColliderBase* collider);
  const std::array<DDyVector3, 3>& pfGetObjectWorldSpaceAxis() const noexcept;
#endif

  MDY_TRANSIENT bool        mIsTransformInitialized       = false;
  /// Parent object. if nullptr, this object has no parent and be on scene.
  FDyPawn*                  mParentRawPtr                 = nullptr;

  /// Object name counter to avoid duplicated object name
  TNameCounterMap           __mChildSubobjectNameCounter;
  mutable uint32_t          __mHashValue                  = 0;
  mutable bool              __mHashInitialized            = false;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_OBJECT_OBJECT_H
