#include <precompiled.h>
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

/// Header file
#include <Dy/Element/Actor.h>

#include <Dy/Component/CDyTransform.h>
#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyLightDirectional.h>
#include <Dy/Component/CDyModelAnimator.h>
#include <Dy/Component/CDySoundSource.h>
#include <Dy/Component/CDyPhysicsColliderSphere.h>
#include <Dy/Component/CDyPhysicsColliderCapsule.h>
#include <Dy/Component/CDyPhysicsColliderBox.h>
#include <Dy/Component/CDySkybox.h>
#include <Dy/Element/Type/PDyActorCreationDescriptor.h>
#include <Dy/Helper/Internal/FDyNameGenerator.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>

//!
//! Implementation
//!

namespace dy
{

class FDyActor::PImplDesc
{
public:
  PImplDesc(
    const PDyObjectMetaInfo* iObjectMetaDesc,
    const PDyActorCreationDescriptor* iObjectCreationDesc, 
    FDyActor* iPtrParentActor)
    : mObjectMetaDesc(iObjectMetaDesc),
      mObjectCreationDesc(iObjectCreationDesc),
      mPtrParentActor(iPtrParentActor) {}

  const PDyObjectMetaInfo* mObjectMetaDesc = nullptr;
  const PDyActorCreationDescriptor* mObjectCreationDesc = nullptr;
  FDyActor* mPtrParentActor = nullptr;
};

class FDyActor::Impl final : public FDyNameGenerator, public IDyInitializeHelper<PImplDesc>
{
public:
  Impl(FDyActor& iActor);
  virtual ~Impl() = default;

  /// @brief Initialize FDyActor.
  EDySuccess pInitilaize(const PDyObjectMetaInfo& objectMetaDesc, FDyActor* iPtrParent);
  EDySuccess pInitilaize(const PDyActorCreationDescriptor& iDesc, FDyActor* iPtrParent);

  EDySuccess Initialize(const PImplDesc& descriptor) override final
  {
    if (descriptor.mObjectCreationDesc == nullptr)
    {
      return this->pInitilaize(*descriptor.mObjectMetaDesc, descriptor.mPtrParentActor);
    }
    else
    {
      return this->pInitilaize(*descriptor.mObjectCreationDesc, descriptor.mPtrParentActor);
    }
  }

  void Release() override final
  {
    SDyProfilingHelper::DecreaseOnBindActorCount(1);
    for (auto& item : this->mComponentList)
    {
      this->ReleaseComponent(item);
    }
    this->mComponentList.clear();

    // Detach if alreayd attached to picking target of system.
    if (this->mIsAttachedToPickingTarget == true) { this->MDY_PRIVATE(DetachPickingTargetFromSystem)(); }
    // Release rigidbody also.
    if (this->mRigidbody != nullptr) { this->mRigidbody->Release(); }
  }

  FDyActor& mRefActor;

  void DestroySelf();

  void SetParent(FDyActor& refParentActor) noexcept;
  void SetParentAsRoot() noexcept;
  /// @brief  Check FDyActor has a parent FDyActor.
  /// @return If it has valid parent, return true but false.
  bool HasParent() const noexcept;
  /// @brief  Return valid parent FDyActor pointer instance with wrapping optional.
  /// @return If parent is binded and exist, return optional valid pointer but just no value.
  FDyActor* GetPtrParent() const noexcept;

  /// @brief  Return this actor has children object, empty object will be neglected.
  /// @return If having children, return true.
  bool HasChildrenActor() const noexcept;
  /// @brief Return this actor.
  TActorMap& GetChildrenContainer() noexcept;

  /// @brief  Get actual actor type
  /// @return Object type specifier
  EDyMetaObjectType GetActorType() const noexcept;
  /// @brief Get actor's tag name.
  const std::string& GetActorTag() const noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  std::vector<NotNull<FDyActor*>> 
  GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  std::vector<NotNull<FDyActor*>>
  GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  std::vector<NotNull<FDyActor*>>
  GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get pointer of actor with object id.
  /// If not found, just return nullptr.
  FDyActor* GetActorWithObjectId(TU32 iObjectId) noexcept;
  
  /// @brief Helper function for release component.
  void ReleaseComponent(_MINOUT_ TComponentList::value_type& iItem);

  /// @brief Try remove script instances list. \n
  /// But this funtion does not remove script instance actually, but just forward script list to GC-list.
  void MDY_PRIVATE(TryRemoveScriptInstances)() noexcept;
  /// @brief Try detach dependent components from dy level management system.
  void MDY_PRIVATE(TryDetachDependentComponents)() noexcept;

  /// @brief  Get script component pointer from script list using scriptName to verify.
  /// @param  scriptName Script name to verify and get.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  MDY_NODISCARD CDyActorScript* GetScriptComponent(_MIN_ const std::string& scriptName) noexcept;
  /// @brief  Remove script component manually from script list using scriptName to verify.
  /// @param  scriptName  Script name to verify and remove from FDyActor.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  MDY_NODISCARD EDySuccess RemoveScriptComponent(_MIN_ const std::string& scriptName) noexcept;

  /// @brief  Get tranform component pointer from FDyActor instance.
  /// @return Valid transform pointer instance.
  MDY_NODISCARD NotNull<CDyTransform*> GetTransform() noexcept;
  /// @brief  Get rigidbody component pointer from FDyActor instance.
  /// @return Valid rigidbody component pointer instance.
  MDY_NODISCARD CDyPhysicsRigidbody* GetRigidbody() noexcept;

  /// @brief Propagate activation flag from parent. This function could not be called independently.
  void pUpdateActivateFlagFromParent() noexcept;

  /// @brief Attach this actor to picking target pointer variable of internal system.
  void MDY_PRIVATE(AttachPickingTargetFromSystem)(_MINOUT_ FDyActor** iPPtrTarget);
  /// @brief Detach this actor from target pointer variable of internal system.
  /// If already or not attached to pointer, just do nothing but return DY_FAILURE.
  EDySuccess MDY_PRIVATE(DetachPickingTargetFromSystem)();
    
  CDyActorScript* pAddScriptComponent(const PDyScriptComponentMetaInfo& iInfo)
  {
    // Validation check.
    const auto specifierName = iInfo.mDetails.mSpecifierName;
    auto& metaManager = MDyMetaInfo::GetInstance();
    if (metaManager.IsScriptMetaInformationExist(specifierName) == false)
    {
      DyPushLogDebugError("Failed to create script, {}. Script information is not exist.", specifierName);
      return nullptr;
    };

    // Get information of script to be created.
    const auto& instanceInfo = metaManager.GetScriptMetaInformation(specifierName);
    MDY_ASSERT_MSG(
      instanceInfo.mScriptType != EDyScriptType::NoneError, 
      "Script type must be valid.");

    return this->AddScriptComponent(instanceInfo);
  }
  CDyActorScript* AddScriptComponent(const PDyScriptInstanceMetaInfo& iComponentInfo);
  
  template<class TComponent, typename... TArgs>
  NotNull<TComponent*> AddComponent(TArgs&&... args)
  {
    // Validation test
    static_assert(
      IsInheritancedFrom<TComponent, ADyBaseComponent>,
      "Failed to create component, required component type is not inheritenced from ADyBaseComponent");

    // If component is script, process the other subroutine. 
    if constexpr (IsSameClass<CDyActorScript, TComponent> == true)
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
      auto componentPtr = std::make_unique<TComponent>(std::ref(this->mRefActor));
      MDY_CALL_ASSERT_SUCCESS(componentPtr->Initialize(std::forward<TArgs>(args)...));

      // If it is transform, move it to separated space. 
      if constexpr (IsSameClass<CDyTransform, TComponent> == true)
      { // If component is not CDyScript but related to ADyBaseTransform (Transform components)
        MDY_ASSERT_MSG_FORCE(
          this->mTransform == nullptr, 
          "FDyActor::mTransform must be empty when insert transform component.");

        this->mTransform = std::move(componentPtr);
        return DyMakeNotNull(this->mTransform.get());
      }
      else if constexpr (IsSameClass<TComponent, CDyPhysicsRigidbody> == true)
      { // If component is CDyPhysicsRigidbody...
        MDY_ASSERT_MSG_FORCE(
          this->mRigidbody == nullptr, 
          "FDyActor::mRigidbody must be empty when insert rigidbody component.");

        this->mRigidbody = std::move(componentPtr);
        return DyMakeNotNull(this->mRigidbody.get());
      }
      else
      { // Otherwise remain, just return Ptr.
        auto& [value, reference] = this->mComponentList.emplace_back(
          std::make_pair(TComponentUnbindingType<TComponent>::Value, std::move(componentPtr))
        );
        return DyMakeNotNull(static_cast<TComponent*>(reference.get()));
      }
    }
  }

  template<class TGeneralComponent>
  std::optional<TGeneralComponent*> GetGeneralComponent()
  {
    static_assert(
      IsInheritancedFrom<TGeneralComponent, ADyBaseComponent>,
      "Failed to get component, required component type is not inheritenced from ADyBaseComponent");

    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    for (auto& [type, component] : this->mComponentList)
    {
      if (component != nullptr) { continue; }
      if (component->IsTypeMatched(TGeneralComponent::__mHashVal) == true)
      {
        return static_cast<TGeneralComponent*>(component.get());
      }
    }

    // If there is no component to find.
    return std::nullopt;
  }

  template <class TGeneralComponent>
  std::vector<NotNull<TGeneralComponent*>> GetGeneralComponentList()
  {
    static_assert(
      IsInheritancedFrom<TGeneralComponent, ADyGeneralBaseComponent>,
      "Failed to get component list, required component type is not inheritenced from ADyBaseComponent");

    // Component matching process is using recursion of each component
    // from last derived component class to highest base component class.
    std::vector<NotNull<TGeneralComponent*>> resultList = {};

    for (auto& [type, component] : this->mComponentList)
    {
      if (component == nullptr) { continue; }
      if (component->IsTypeMatched(TGeneralComponent::__mHashVal) == true)
      {
        resultList.emplace_back(static_cast<TGeneralComponent*>(component.get()));
      }
    }

    return resultList;
  }

  template <class TComponent, typename... TArgs>
  EDySuccess RemoveComponent(TArgs&&... args)
  {
    static_assert(
      IsInheritancedFrom<TComponent, ADyBaseComponent>,
      "Failed to remove component, required component type is not inheritenced from ADyBaseComponent");
    DyCheckComponentRemoveFunctionParams<TComponent, TArgs...>();

    if constexpr (std::is_base_of_v<ADyGeneralBaseComponent, TComponent>)
    {
      auto it = std::find_if(
        MDY_BIND_BEGIN_END(this->mComponentList),
        [](const auto& item) { return item->IsTypeMatched(TComponent::__mHashVal); }
      );

      if (it == this->mComponentList.end()) { return DY_FAILURE; }
      this->ReleaseComponent(*it);
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

  /// Actual actor type to discriminate actor type is so cast object with statically.
  MDY_TRANSIENT EDyMetaObjectType mActorType = EDyMetaObjectType::NoneError;

  void TryActivateInstance();
  void TryDeactivateInstance();
  
  /// @brief
  void CreateComponentsWithList(_MIN_ const TComponentMetaList& iMetaComponentList);
  /// @brief
  void pPropagateActivationFlag() noexcept;

  /// Parent FDyActor raw-pointer data.
  FDyActor*      mPtrParentActor = MDY_INITIALIZE_NULL;
  /// Transform component.
  std::unique_ptr<CDyTransform> mTransform         = MDY_INITIALIZE_NULL;
  /// Rigidbody component.
  std::unique_ptr<CDyPhysicsRigidbody> mRigidbody  = MDY_INITIALIZE_NULL;
  /// Component list (randomly) which attached to FDyActor instance (this!)
  TComponentList mComponentList  = {};
  /// Script list (specialized!)
  TScriptList    mScriptList     = {};
  /// Actor list (hierarchial version)
  TActorMap      mChildrenActors = {};
  /// @brief Tag specifier 
  std::string    mActorTagSpecifier = MDY_INITIALIZE_EMPTYSTR;

  /// @brief Internal variable.
  bool mIsAttachedToPickingTarget = false;
  };

} /// ::dy namespace
#include <Dy/Element/Inline/FActorImpl.inl>

//!
//! Proxy
//!

namespace dy
{
  FDyActor::FDyActor(const PDyObjectMetaInfo& objectMetaDesc, FDyActor* iPtrParent)
  {
    this->mInternal = new Impl(*this);
    this->pSetObjectName(objectMetaDesc.mSpecifierName);
    this->mInternal->Initialize(PImplDesc{&objectMetaDesc, nullptr, iPtrParent});
  }

  FDyActor::FDyActor(const PDyActorCreationDescriptor& iDesc, FDyActor* iPtrParent)
  {
    this->mInternal = new Impl(*this);
    this->pSetObjectName(iDesc.mActorSpecifierName);
    this->mInternal->Initialize(PImplDesc{nullptr, &iDesc, iPtrParent});
  }

  FDyActor::~FDyActor()
  {
    this->mInternal->Release();
    delete this->mInternal; this->mInternal = nullptr;
  }

  void FDyActor::DestroySelf()
  {
    MDyWorld::GetInstance().DestroyActor(*this);
  }

  const std::string& FDyActor::GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  std::string FDyActor::GetActorFullName() const noexcept
  {
    if (this->HasParent() == false) 
    { 
      return this->GetActorName(); 
    }
    else
    {
      const auto headFullSpecifierName = this->GetPtrParent()->GetActorFullName();
      return MakeStringU8("{}.{}", headFullSpecifierName, this->GetActorName());
    }
  }

  void FDyActor::pUpdateActivateFlagFromParent() noexcept
  {
    this->mInternal->pUpdateActivateFlagFromParent();
  }

  void FDyActor::MDY_PRIVATE(AttachPickingTargetFromSystem)(_MINOUT_ FDyActor** iPPtrTarget)
  {
    this->mInternal->__AttachPickingTargetFromSystem(iPPtrTarget);
  }

  EDySuccess FDyActor::MDY_PRIVATE(DetachPickingTargetFromSystem)()
  {
    return this->mInternal->__DetachPickingTargetFromSystem();
  }

  void FDyActor::SetParent(FDyActor& iValidParent) noexcept
  {
    return this->mInternal->SetParent(iValidParent);
  }

  void FDyActor::SetParentAsRoot() noexcept
  {
    return this->mInternal->SetParentAsRoot();
  }

  EDyMetaObjectType FDyActor::GetActorType() const noexcept
  {
    return this->mInternal->GetActorType();
  }

  const std::string& FDyActor::GetActorTag() const noexcept
  {
    return this->mInternal->GetActorTag();
  }

  std::vector<NotNull<FDyActor*>> 
  FDyActor::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithTag(iTagSpecifier);
  }

  std::vector<NotNull<FDyActor*>> 
  FDyActor::GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithTagRecursive(iTagSpecifier);
  }

  std::vector<NotNull<FDyActor*>> 
  FDyActor::GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithName(iNameSpecifier);
  }

  std::vector<NotNull<FDyActor*>> 
  FDyActor::GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithNameRecursive(iNameSpecifier);
  }

  FDyActor* FDyActor::GetActorWithObjectId(TU32 iObjectId) noexcept
  {
    return this->mInternal->GetActorWithObjectId(iObjectId);
  }

  bool FDyActor::HasParent() const noexcept { return this->mInternal->HasParent(); }

  FDyActor* FDyActor::GetPtrParent() const noexcept { return this->mInternal->GetPtrParent(); }

  bool FDyActor::HasChildrenActor() const noexcept { return this->mInternal->HasChildrenActor(); }

  FDyActor::TActorMap& FDyActor::GetChildrenContainer() noexcept
  {
    return this->mInternal->GetChildrenContainer();
  }

  std::string FDyActor::ToString()
  {
    return MakeStringU8("Actor name : {}, Id : {}", this->GetActorName(), this->GetId());
  }

  void FDyActor::MDY_PRIVATE(TryRemoveScriptInstances)() noexcept
  {
    this->mInternal->__TryRemoveScriptInstances();
  }

  void FDyActor::MDY_PRIVATE(TryDetachDependentComponents)() noexcept
  {
    this->mInternal->__TryDetachDependentComponents();
  }

  NotNull<CDyTransform*> FDyActor::GetTransform() noexcept
  {
    return this->mInternal->GetTransform();
  }

  CDyPhysicsRigidbody* FDyActor::GetRigidbody() noexcept
  {
    return this->mInternal->GetRigidbody();
  }

  CDyActorScript* FDyActor::pAddScriptComponent(const PDyScriptComponentMetaInfo& iInfo)
  {
    // Validation check.
    const auto specifierName = iInfo.mDetails.mSpecifierName;
    auto& metaManager = MDyMetaInfo::GetInstance();
    if (metaManager.IsScriptMetaInformationExist(specifierName) == false)
    {
      DyPushLogDebugError("Failed to create script, {}. Script information is not exist.", specifierName);
      return nullptr;
    };

    // Get information of script to be created.
    const auto& instanceInfo = metaManager.GetScriptMetaInformation(specifierName);
    MDY_ASSERT_MSG(
      instanceInfo.mScriptType != EDyScriptType::NoneError, 
      "Script type must be valid.");

    return this->mInternal->AddScriptComponent(instanceInfo);
  }

  FDyActor::TComponentList& FDyActor::pGetComponentList() noexcept
  {
    return this->mInternal->mComponentList;
  }

  void FDyActor::pReleaseComponent(TComponentItem& ioItem)
  {
    this->mInternal->ReleaseComponent(ioItem);
  }

  void FDyActor::TryActivateInstance()
  {
    this->mInternal->TryActivateInstance();
  }

  void FDyActor::TryDeactivateInstance()
  {
    this->mInternal->TryDeactivateInstance();
  }

} /// ::dy namespace
