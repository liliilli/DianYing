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
#include <Dy/Component/CDyPhysicsRigidbody.h>
#include <Dy/Component/Actor/CActorScript.h>
#include <Dy/Component/Type/EComponentType.h>
#include <Dy/Element/Object.h>
#include <Dy/Element/Abstract/ADyBaseComponent.h>
#include <Dy/Element/Abstract/AGeneralBaseComponent.h>
#include <Dy/Element/Abstract/Actor/ADyActorBinderContainer.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Element/Internal/TDyIdDistributor.h>
#include <Dy/Element/Descriptor/GlobalEnums.h>

//!
//! Forward declaration
//!

namespace dy
{
struct PDyObjectMetaInfo;
class CDyTransform;
}

//!
//! Implementation
//!

namespace dy
{

/// @class FDyActor
/// @brief Dy Actor type which consist of almost every object in Level of Dy.
class FDyActor final : public FDyObject, 
    public ADyActorBinderContainer<FDyActor>, 
    public FDy3WaySwitcher,
    public TDyIdDistributor<TU32, FDyActor>
{
public:
  MDY_SET_CRC32_HASH_WITH_TYPE(FDyActor);
  MDY_SET_TYPEMATCH_FUNCTION(FDyObject, FDyActor);
  using TComponentItem = std::pair<EComponentType, std::unique_ptr<AGeneralBaseComponent>>;
  using TComponentList = std::vector<TComponentItem>;
  using TScriptList    = std::vector<std::unique_ptr<CActorScript>>;
  using TActorSmtPtr   = std::unique_ptr<FDyActor>;
  using TActorMap      = std::unordered_map<std::string, TActorSmtPtr>;

  /// @brief Initialize FDyActor.
  /// @param objectMetaDesc Meta descriptor information instance for FDyActor.
  FDyActor(const PDyObjectMetaInfo& objectMetaDesc, FDyActor* iPtrParent = nullptr);
  explicit FDyActor(const PDyActorCreationDescriptor& iDesc, FDyActor* iPtrParent = nullptr);

  /// @brief Release function (virtual) 
  /// because Initialize function has different parameter but release does not need any parameter.
  /// @return Success flag.
  virtual ~FDyActor();

  /// @brief Destory self and tree.
  void DestroySelf();
  
  /// @brief  Get actual actor type
  /// @return Object type specifier
  MDY_NODISCARD EDyMetaObjectType GetActorType() const noexcept;
  /// @brief Get actor's tag name.
  MDY_NODISCARD const std::string& GetActorTag() const noexcept;

  /// @brief Get present actor name on runtime.
  /// @return Actor name of this instance.
  MDY_NODISCARD const std::string& GetActorName() const noexcept;
  /// @brief Get full specifier name from this to root like a `This_Name.Head~1.Head~2`
  MDY_NODISCARD std::string GetActorFullName() const noexcept; 

  /// @brief Set FDyActor's parent to valid input actor.
  void SetParent(FDyActor& iParentActor) noexcept;
  /// @brief Set FDyActor's hierarchial position to Level's root.
  /// Transform update will be held automatically.
  void SetParentAsRoot() noexcept;
  /// @brief  Check FDyActor has a parent FDyActor.
  /// @return If it has valid parent, return true but false.
  MDY_NODISCARD bool HasParent() const noexcept;
  /// @brief  Return valid parent FDyActor pointer instance with wrapping optional.
  /// @return If parent is binded and exist, return optional valid pointer but just no value.
  MDY_NODISCARD FDyActor* GetPtrParent() const noexcept;

  /// @brief  Return this actor has children object, empty object will be neglected.
  /// @return If having children, return true.
  MDY_NODISCARD bool HasChildrenActor() const noexcept;
  /// @brief Return this actor.
  MDY_NODISCARD TActorMap& GetChildrenContainer() noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>> 
  GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get pointer of actor with object id.
  /// If not found, just return nullptr.
  FDyActor* GetActorWithObjectId(TU32 iObjectId) noexcept;
  
  /// @brief Return actor's information
  /// @return information string.
  MDY_NODISCARD std::string ToString() override;

  /// @brief  Add component and bind to this object instance.
  /// @tparam TComponent  Component type class argument.
  /// @tparam TArgs       Universal reference. Used to be arguments of Component constructor.
  /// @return Valid component instance pointer.
  template<class TComponent, typename... TArgs>
  TComponent* AddComponent(TArgs&&... args);

  /// @brief  Return component raw-pointer from general component list (not script)
  /// @tparam TGeneralComponent Component type argument.
  /// @return If found, return TGeneralComponent* but not found, return no value.
  template<class TGeneralComponent>
  MDY_NODISCARD std::optional<TGeneralComponent*> GetGeneralComponent();

  /// @brief  Return component raw-pointer list from general component list.
  /// @tparam TGeneralComponent Component type argument.
  /// @return If found, return TGeneralComponent* list but not found, return empty list.
  template <class TGeneralComponent>
  MDY_NODISCARD std::vector<NotNull<TGeneralComponent*>> GetGeneralComponentList();

  /// @brief  Remove component.
  /// @tparam TComponent Component type argument.
  /// @tparam TArgs      Arguments
  /// @return If found, return true but otherwise false.
  template <class TComponent, typename... TArgs>
  MDY_NODISCARD EDySuccess RemoveComponent(TArgs&&... args);

  /// @brief Try remove script instances list. \n
  /// But this funtion does not remove script instance actually, but just forward script list to GC-list.
  void __TryRemoveScriptInstances() noexcept;
  /// @brief Try detach dependent components from dy level management system.
  void MDY_PRIVATE(TryDetachDependentComponents)() noexcept;

  /// @brief  Get tranform component pointer from FDyActor instance.
  /// @return Valid transform pointer instance.
  MDY_NODISCARD NotNull<CDyTransform*> GetTransform() noexcept;
  /// @brief  Get rigidbody component pointer from FDyActor instance.
  /// @return Valid rigidbody component pointer instance.
  MDY_NODISCARD CDyPhysicsRigidbody* GetRigidbody() noexcept;

  /// @brief Propagate activation flag from parent. This function could not be called independently.
  void pUpdateActivateFlagFromParent() noexcept;

  /// @brief Attach this actor to picking target pointer variable of internal system.
  void MDY_PRIVATE(AttachPickingTargetFromSystem)(FDyActor** ioPPtrTarget);
  /// @brief Detach this actor from target pointer variable of internal system.
  /// If already or not attached to pointer, just do nothing but return DY_FAILURE.
  EDySuccess MDY_PRIVATE(DetachPickingTargetFromSystem)();

private:
  //!
  //! These functions are needed to communicaet with pimpl instance.
  //!

  CActorScript* pAddScriptComponent(const PDyScriptComponentMetaInfo& iInfo);
  TComponentList& pGetComponentList() noexcept;
  void pReleaseComponent(TComponentItem& ioItem);

  void TryActivateInstance() override final;
  void TryDeactivateInstance() override final;

  class PImplDesc;
  class Impl; Impl* mInternal = nullptr;
};

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_ACTOR_H
#include <Dy/Element/Inline/FActorTmp.inl>