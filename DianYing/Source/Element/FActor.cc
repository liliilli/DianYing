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
#include <Dy/Element/FActor.h>

#include <Dy/Component/CTransform.h>
#include <Dy/Component/CModelFilter.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Component/CCamera.h>
#include <Dy/Component/CLightDirectional.h>
#include <Dy/Component/CModelAnimator.h>
#include <Dy/Component/CSoundSource.h>
#include <Dy/Component/CPhysicsColliderSphere.h>
#include <Dy/Component/CPhysicsColliderCapsule.h>
#include <Dy/Component/CPhysicsColliderBox.h>
#include <Dy/Component/CSkybox.h>
#include <Dy/Element/Type/PActorCreationDescriptor.h>
#include <Dy/Helper/Internal/FNameGenerator.h>
#include <Dy/Management/MInput.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/Helper/SProfilingHelper.h>

//!
//! Implementation
//!

namespace dy
{

class FActor::PImplDesc
{
public:
  PImplDesc(
    const PDyObjectMetaInfo* iObjectMetaDesc,
    const PActorCreationDescriptor* iObjectCreationDesc, 
    FActor* iPtrParentActor)
    : mObjectMetaDesc(iObjectMetaDesc),
      mObjectCreationDesc(iObjectCreationDesc),
      mPtrParentActor(iPtrParentActor) {}

  const PDyObjectMetaInfo* mObjectMetaDesc = nullptr;
  const PActorCreationDescriptor* mObjectCreationDesc = nullptr;
  FActor* mPtrParentActor = nullptr;
};

class FActor::Impl final : public FNameGenerator, public IInitializeHelper<PImplDesc>
{
public:
  Impl(FActor& iActor);
  virtual ~Impl() = default;

  /// @brief Initialize FActor.
  EDySuccess pInitilaize(const PDyObjectMetaInfo& objectMetaDesc, FActor* iPtrParent);
  EDySuccess pInitilaize(const PActorCreationDescriptor& iDesc, FActor* iPtrParent);

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
    SProfilingHelper::DecreaseOnBindActorCount(1);
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

  FActor& mRefActor;

  void DestroySelf();

  void SetParent(FActor& refParentActor) noexcept;
  void SetParentAsRoot() noexcept;
  /// @brief  Check FActor has a parent FActor.
  /// @return If it has valid parent, return true but false.
  bool HasParent() const noexcept;
  /// @brief  Return valid parent FActor pointer instance with wrapping optional.
  /// @return If parent is binded and exist, return optional valid pointer but just no value.
  FActor* GetPtrParent() const noexcept;

  /// @brief  Return this actor has children object, empty object will be neglected.
  /// @return If having children, return true.
  bool HasChildrenActor() const noexcept;
  /// @brief Return this actor.
  TActorMap& GetChildrenContainer() noexcept;

  /// @brief  Get actual actor type
  /// @return Object type specifier
  EWorldObjectType GetActorType() const noexcept;
  /// @brief Get actor's tag name.
  const std::string& GetActorTag() const noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  std::vector<NotNull<FActor*>> 
  GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  std::vector<NotNull<FActor*>>
  GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept;
  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  std::vector<NotNull<FActor*>>
  GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  std::vector<NotNull<FActor*>>
  GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept; 
  /// @brief Get pointer of actor with object id.
  /// If not found, just return nullptr.
  FActor* GetActorWithObjectId(TU32 iObjectId) noexcept;
  
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
  MDY_NODISCARD CActorScript* GetScriptComponent(_MIN_ const std::string& scriptName) noexcept;
  /// @brief  Remove script component manually from script list using scriptName to verify.
  /// @param  scriptName  Script name to verify and remove from FActor.
  /// @return The pointer instance of CDyScript. If not found, return just no value.
  MDY_NODISCARD EDySuccess RemoveScriptComponent(_MIN_ const std::string& scriptName) noexcept;

  /// @brief  Get tranform component pointer from FActor instance.
  /// @return Valid transform pointer instance.
  MDY_NODISCARD NotNull<CTransform*> GetTransform() noexcept;
  /// @brief  Get rigidbody component pointer from FActor instance.
  /// @return Valid rigidbody component pointer instance.
  MDY_NODISCARD CPhysicsRigidbody* GetRigidbody() noexcept;

  /// @brief Propagate activation flag from parent. This function could not be called independently.
  void pUpdateActivateFlagFromParent() noexcept;

  /// @brief Attach this actor to picking target pointer variable of internal system.
  void MDY_PRIVATE(AttachPickingTargetFromSystem)(_MINOUT_ FActor** iPPtrTarget);
  /// @brief Detach this actor from target pointer variable of internal system.
  /// If already or not attached to pointer, just do nothing but return DY_FAILURE.
  EDySuccess MDY_PRIVATE(DetachPickingTargetFromSystem)();
    
  CActorScript* pAddScriptComponent(const PDyScriptComponentMetaInfo& iInfo)
  {
    // Validation check.
    const auto specifierName = iInfo.mDetails.mSpecifierName;
    auto& metaManager = MIOMeta::GetInstance();
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
  CActorScript* AddScriptComponent(const PDyScriptInstanceMetaInfo& iComponentInfo);
  
  template<class TComponent, typename... TArgs>
  NotNull<TComponent*> AddComponent(TArgs&&... args)
  {
    // Validation test
    static_assert(
      IsInheritancedFrom<TComponent, ABaseComponent>,
      "Failed to create component, required component type is not inheritenced from ABaseComponent");

    // If component is script, process the other subroutine. 
    if constexpr (IsSameClass<CActorScript, TComponent> == true)
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
      if constexpr (IsSameClass<CTransform, TComponent> == true)
      { // If component is not CDyScript but related to ADyBaseTransform (Transform components)
        MDY_ASSERT_MSG_FORCE(
          this->mTransform == nullptr, 
          "FActor::mTransform must be empty when insert transform component.");

        this->mTransform = std::move(componentPtr);
        return DyMakeNotNull(this->mTransform.get());
      }
      else if constexpr (IsSameClass<TComponent, CPhysicsRigidbody> == true)
      { // If component is CPhysicsRigidbody...
        MDY_ASSERT_MSG_FORCE(
          this->mRigidbody == nullptr, 
          "FActor::mRigidbody must be empty when insert rigidbody component.");

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
      IsInheritancedFrom<TGeneralComponent, ABaseComponent>,
      "Failed to get component, required component type is not inheritenced from ABaseComponent");

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
      IsInheritancedFrom<TGeneralComponent, AGeneralBaseComponent>,
      "Failed to get component list, required component type is not inheritenced from ABaseComponent");

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
      IsInheritancedFrom<TComponent, ABaseComponent>,
      "Failed to remove component, required component type is not inheritenced from ABaseComponent");

    if constexpr (std::is_base_of_v<AGeneralBaseComponent, TComponent>)
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
    else if constexpr (std::is_same_v<CActorScript, TComponent>)
    {
      // @TODO IMPLEMENT SCRIPT DELETION USING DESCRIPTOR OR SCRIPT NAME.
      return this->RemoveScriptComponent(std::forward<TArgs>(args)...);
    }

    return DY_FAILURE;
  }

  /// Actual actor type to discriminate actor type is so cast object with statically.
  MDY_TRANSIENT EWorldObjectType mActorType = EWorldObjectType::NoneError;

  void TryActivateInstance();
  void TryDeactivateInstance();
  
  /// @brief
  void CreateComponentsWithList(_MIN_ const TComponentMetaList& iMetaComponentList);
  /// @brief
  void pPropagateActivationFlag() noexcept;

  /// Parent FActor raw-pointer data.
  FActor*      mPtrParentActor = MDY_INITIALIZE_NULL;
  /// Transform component.
  std::unique_ptr<CTransform> mTransform         = MDY_INITIALIZE_NULL;
  /// Rigidbody component.
  std::unique_ptr<CPhysicsRigidbody> mRigidbody  = MDY_INITIALIZE_NULL;
  /// Component list (randomly) which attached to FActor instance (this!)
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
  FActor::FActor(const PDyObjectMetaInfo& objectMetaDesc, FActor* iPtrParent)
  {
    this->mInternal = new Impl(*this);
    this->pSetObjectName(objectMetaDesc.mSpecifierName);
    this->mInternal->Initialize(PImplDesc{&objectMetaDesc, nullptr, iPtrParent});
  }

  FActor::FActor(const PActorCreationDescriptor& iDesc, FActor* iPtrParent)
  {
    this->mInternal = new Impl(*this);
    this->pSetObjectName(iDesc.mActorSpecifierName);
    this->mInternal->Initialize(PImplDesc{nullptr, &iDesc, iPtrParent});
  }

  FActor::~FActor()
  {
    this->mInternal->Release();
    delete this->mInternal; this->mInternal = nullptr;
  }

  void FActor::DestroySelf()
  {
    MWorld::GetInstance().DestroyActor(*this);
  }

  const std::string& FActor::GetActorName() const noexcept
  {
    return this->pGetObjectName();
  }

  std::string FActor::GetActorFullName() const noexcept
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

  void FActor::pUpdateActivateFlagFromParent() noexcept
  {
    this->mInternal->pUpdateActivateFlagFromParent();
  }

  void FActor::MDY_PRIVATE(AttachPickingTargetFromSystem)(_MINOUT_ FActor** iPPtrTarget)
  {
    this->mInternal->__AttachPickingTargetFromSystem(iPPtrTarget);
  }

  EDySuccess FActor::MDY_PRIVATE(DetachPickingTargetFromSystem)()
  {
    return this->mInternal->__DetachPickingTargetFromSystem();
  }

  void FActor::SetParent(FActor& iValidParent) noexcept
  {
    return this->mInternal->SetParent(iValidParent);
  }

  void FActor::SetParentAsRoot() noexcept
  {
    return this->mInternal->SetParentAsRoot();
  }

  EWorldObjectType FActor::GetActorType() const noexcept
  {
    return this->mInternal->GetActorType();
  }

  const std::string& FActor::GetActorTag() const noexcept
  {
    return this->mInternal->GetActorTag();
  }

  std::vector<NotNull<FActor*>> 
  FActor::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithTag(iTagSpecifier);
  }

  std::vector<NotNull<FActor*>> 
  FActor::GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithTagRecursive(iTagSpecifier);
  }

  std::vector<NotNull<FActor*>> 
  FActor::GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithName(iNameSpecifier);
  }

  std::vector<NotNull<FActor*>> 
  FActor::GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept
  {
    return this->mInternal->GetAllActorsWithNameRecursive(iNameSpecifier);
  }

  FActor* FActor::GetActorWithObjectId(TU32 iObjectId) noexcept
  {
    return this->mInternal->GetActorWithObjectId(iObjectId);
  }

  bool FActor::HasParent() const noexcept { return this->mInternal->HasParent(); }

  FActor* FActor::GetPtrParent() const noexcept { return this->mInternal->GetPtrParent(); }

  bool FActor::HasChildrenActor() const noexcept { return this->mInternal->HasChildrenActor(); }

  FActor::TActorMap& FActor::GetChildrenContainer() noexcept
  {
    return this->mInternal->GetChildrenContainer();
  }

  std::string FActor::ToString()
  {
    return MakeStringU8("Actor name : {}, Id : {}", this->GetActorName(), this->GetId());
  }

  void FActor::MDY_PRIVATE(TryRemoveScriptInstances)() noexcept
  {
    this->mInternal->__TryRemoveScriptInstances();
  }

  void FActor::MDY_PRIVATE(TryDetachDependentComponents)() noexcept
  {
    this->mInternal->__TryDetachDependentComponents();
  }

  NotNull<CTransform*> FActor::GetTransform() noexcept
  {
    return this->mInternal->GetTransform();
  }

  CPhysicsRigidbody* FActor::GetRigidbody() noexcept
  {
    return this->mInternal->GetRigidbody();
  }

  CActorScript* FActor::pAddScriptComponent(const PDyScriptComponentMetaInfo& iInfo)
  {
    // Validation check.
    const auto specifierName = iInfo.mDetails.mSpecifierName;
    auto& metaManager = MIOMeta::GetInstance();
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

  FActor::TComponentList& FActor::pGetComponentList() noexcept
  {
    return this->mInternal->mComponentList;
  }

  void FActor::pReleaseComponent(TComponentItem& ioItem)
  {
    this->mInternal->ReleaseComponent(ioItem);
  }

  void FActor::TryActivateInstance()
  {
    this->mInternal->TryActivateInstance();
  }

  void FActor::TryDeactivateInstance()
  {
    this->mInternal->TryDeactivateInstance();
  }

} /// ::dy namespace
