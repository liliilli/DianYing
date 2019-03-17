#ifndef GUARD_DY_ELEMENT_INLINE_FACTORIMPL_H
#define GUARD_DY_ELEMENT_INLINE_FACTORIMPL_H
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

#include <Dy/Meta/Information/ElementObjectMetaInfo.h>
#include <Dy/Meta/Components/PCompSpotLightMetaInfo.h>
#include <Dy/Component/CLightPoint.h>
#include <Dy/Component/CLightSpot.h>

namespace dy
{

inline FActor::Impl::Impl(FActor& iActor) : mRefActor{iActor} { }

inline EDySuccess FActor::Impl::pInitilaize(
  const PDyObjectMetaInfo& objectMetaDesc, 
  FActor* iPtrParent)
{
  // (1-1) Insert prefab information if prefab exist.
  auto metaComponentInfo = objectMetaDesc.mMetaComponentInfo;
  if (objectMetaDesc.mProperties.mIsUsingPrefab == true)
  {
    MDY_ASSERT_MSG(
      objectMetaDesc.mProperties.mPrefabSpecifierName.empty() == false, 
      "Unexpected error occurred.");

    const auto& prefab = MIOMeta::GetInstance().GetPrefabMetaInformation(
      objectMetaDesc.mProperties.mPrefabSpecifierName);

    metaComponentInfo.insert(metaComponentInfo.end(), MDY_BIND_BEGIN_END(prefab.mMetaComponentInfo));

    if (objectMetaDesc.mProperties.mIsOverridePrefabTag == true)
    { this->mActorTagSpecifier = objectMetaDesc.mProperties.mTagSpecifier; }
    else
    { this->mActorTagSpecifier = prefab.mCommonProperties.mTagSpecifier; }
  }
  else { this->mActorTagSpecifier = objectMetaDesc.mProperties.mTagSpecifier; }

  // (2) Create components
  // Check activation flags and execute sub-routines of each components.
  this->CreateComponentsWithList(metaComponentInfo);
  // If transform is not exist, just create default transform.
  if (this->mTransform == nullptr)
  {
    const PDyTransformComponentMetaInfo defaultTransform{};
    this->AddComponent<CTransform>(defaultTransform);
  }

  if (iPtrParent == nullptr) 
  { 
    this->SetParent(*iPtrParent); 
  }
  this->pUpdateActivateFlagFromParent();
  if (objectMetaDesc.mProperties.mInitialActivated == true) 
  { 
    this->mRefActor.Activate(); 
  }

  // (3) Make children actors.
  for (const auto& objectInformation : objectMetaDesc.mChildrenList)
  { // Create object, FActor
    switch (objectInformation->mObjectType)
    {
    case EWorldObjectType::Actor:
    { // General object type. Make FActor instance.
      auto smtChild = std::make_unique<FActor>(*objectInformation, &mRefActor);
      auto [it, result] = this->mChildrenActors.try_emplace(smtChild->GetActorName(), std::move(smtChild));
      MDY_ASSERT_MSG_FORCE(
        result == true, 
        "Unexpected error occured in inserting FActor to object map.");
    } break;
    case EWorldObjectType::SceneScriptor: MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EWorldObjectType::Object:        MDY_NOT_IMPLEMENTED_ASSERT(); break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  SProfilingHelper::IncreaseOnBindActorCount(1);
  return DY_SUCCESS;
}

inline EDySuccess FActor::Impl::pInitilaize(const PActorCreationDescriptor& iDesc, FActor* iPtrParent)
{
  // (1-1) Insert prefab information if prefab exist.
  TComponentMetaList metaComponentInfo;
  if (iDesc.mPrefabSpecifierName.empty() == false)
  { 
    const auto& prefab = MIOMeta::GetInstance().GetPrefabMetaInformation(iDesc.mPrefabSpecifierName);
    metaComponentInfo.insert(metaComponentInfo.end(), MDY_BIND_BEGIN_END(prefab.mMetaComponentInfo));

    if (iDesc.mIsOverridePrefabTag == true)
    { this->mActorTagSpecifier = iDesc.mObjectTag; }
    else
    { this->mActorTagSpecifier = prefab.mCommonProperties.mTagSpecifier; }
  }
  else { this->mActorTagSpecifier = iDesc.mObjectTag; }

  // (2) Create components
  // Check activation flags and execute sub-routines of each components.
  // (3) Create Transform component using Given transform
  this->CreateComponentsWithList(metaComponentInfo);
  this->AddComponent<CTransform>(iDesc.mTransform);

  if (iPtrParent != nullptr) 
  { 
    this->SetParent(*iPtrParent); 
  }
  this->pUpdateActivateFlagFromParent();
  this->mRefActor.Activate(); 

  MDY_ASSERT_MSG(MDY_CHECK_ISNOTEMPTY(this->mTransform), "CTransform component must be created to all FActor.");
  SProfilingHelper::IncreaseOnBindActorCount(1);
  
  return DY_SUCCESS;
}

inline void FActor::Impl::CreateComponentsWithList(const TComponentMetaList& iMetaComponentList)
{
  for (const auto& [type, componentInfo] : iMetaComponentList)
  {
    switch (type)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyComponentMetaType::Transform:
      this->AddComponent<CTransform>(std::any_cast<const PDyTransformComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Script:
      this->AddComponent<CActorScript>(std::any_cast<const PDyScriptComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::DirectionalLight:
      this->AddComponent<CLightDirectional>(std::any_cast<const PDirLightComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::PointLight:
      this->AddComponent<CLightPoint>(std::any_cast<const PCompPointLightMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::SpotLight:
      this->AddComponent<CLightSpot>(std::any_cast<const PCompSpotLightMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::ModelFilter:
      this->AddComponent<CModelFilter>(std::any_cast<const PDyModelFilterComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::ModelRenderer:
      this->AddComponent<CModelRenderer>(std::any_cast<const PDyModelRendererComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::ModelAnimator:
      this->AddComponent<CModelAnimator>(std::any_cast<const PDyModelAnimatorComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Camera:
      this->AddComponent<CCamera>(std::any_cast<const PDyCameraComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::SoundSource:
      this->AddComponent<CSoundSource>(std::any_cast<const PDySoundSourceComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Rigidbody:
      this->AddComponent<CPhysicsRigidbody>(std::any_cast<const PDyRigidbodyComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Skybox:
      this->AddComponent<CSkybox>(std::any_cast<const PDySkyboxComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Collider:
      {
        const auto& refMetaInfo = std::any_cast<const PDyColliderComponentMetaInfo&>(componentInfo);
        switch (refMetaInfo.mDetails.mColliderType)
        {
        case EDyColliderType::Sphere: 
          {
            this->AddComponent<CPhysicsColliderSphere>(refMetaInfo);
          } break;
        case EDyColliderType::Capsule: 
          {
            this->AddComponent<CyPhysicsColliderCapsule>(refMetaInfo);
          } break;
        case EDyColliderType::Box:
          {
            this->AddComponent<CPhysicsColliderBox>(refMetaInfo);
          } break;
        default: MDY_UNEXPECTED_BRANCH(); break;
        }
      }
      break;
    }
  }

  // If rigidbody is not exist, just create default rigidbody.
  if (MDY_CHECK_ISNULL(this->mRigidbody))
  {
    const PDyRigidbodyComponentMetaInfo defaultMetaInfo{};
    this->AddComponent<CPhysicsRigidbody>(defaultMetaInfo);
  }
}

inline void FActor::Impl::ReleaseComponent(_MINOUT_ TComponentList::value_type& iItem)
{
  auto& [typeVal, ptrsmtComponent] = iItem;
  if (MDY_CHECK_ISEMPTY(ptrsmtComponent)) { return; }

  using EType = EComponentType;
  // ActorScript, Transform does not release in this logic.
  // We use downcasting intentionally to call Release() function.
  switch (typeVal)
  {
  case EComponentType::DirectionalLight: 
    { static_cast<TComponentBindingType<EType::DirectionalLight>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::PointLight: 
    { static_cast<TComponentBindingType<EType::PointLight>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::SpotLight: 
    { static_cast<TComponentBindingType<EType::SpotLight>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::Camera:
    { static_cast<TComponentBindingType<EType::Camera>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::ModelAnimator:
    { static_cast<TComponentBindingType<EType::ModelAnimator>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::ModelFilter:
    { static_cast<TComponentBindingType<EType::ModelFilter>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::ModelRenderer:
    { static_cast<TComponentBindingType<EType::ModelRenderer>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::SoundSource:
    { static_cast<TComponentBindingType<EType::SoundSource>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::Rigidbody:
    { static_cast<TComponentBindingType<EType::Rigidbody>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::ColliderSphere:
    { static_cast<TComponentBindingType<EType::ColliderSphere>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::ColliderBox:
    { static_cast<TComponentBindingType<EType::ColliderBox>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::ColliderCapsule:
    { static_cast<TComponentBindingType<EType::ColliderCapsule>::Type&>(*ptrsmtComponent).Release();
    } break;
  case EComponentType::Skybox:
    { static_cast<TComponentBindingType<EType::Skybox>::Type&>(*ptrsmtComponent).Release();
    } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

inline void FActor::Impl::DestroySelf()
{
  MWorld::GetInstance().DestroyActor(this->mRefActor);
}

inline void FActor::Impl::pUpdateActivateFlagFromParent() noexcept
{
  // If parent actor is not exist, it regards as a root object.
  // Otherwise, get parent actor flag.
  if (this->mPtrParentActor == nullptr)
  { 
    this->mRefActor.SetupFlagAsParent(true);
  }
  else
  { 
    this->mRefActor.SetupFlagAsParent(this->mPtrParentActor->IsActivated()); 
  }
}

inline void FActor::Impl::MDY_PRIVATE(AttachPickingTargetFromSystem)(_MINOUT_ FActor** iPPtrTarget)
{
  *iPPtrTarget = &this->mRefActor;
  this->mIsAttachedToPickingTarget = true;
}

inline EDySuccess FActor::Impl::MDY_PRIVATE(DetachPickingTargetFromSystem)()
{
  // Check 1 : flag
  if (this->mIsAttachedToPickingTarget == false) { return DY_FAILURE; }

  // Check 2 : address
  auto& refInput = MInput::GetInstance();
  auto** pptrPickingTarget = refInput.MDY_PRIVATE(GetPPtrPickingTarget)();
  if (*pptrPickingTarget != &this->mRefActor) { return DY_FAILURE; }
  
  // Detach actually.
  *pptrPickingTarget = nullptr;
  this->mIsAttachedToPickingTarget = false;
  return DY_SUCCESS;
}

inline CActorScript* FActor::Impl::AddScriptComponent(const PDyScriptInstanceMetaInfo& iComponentInfo)
{
  auto component = std::make_unique<CActorScript>(mRefActor, iComponentInfo.mSpecifierName);

  auto& ref = this->mScriptList.emplace_back(std::move(component));
  return ref.get();
}

inline void FActor::Impl::TryActivateInstance()
{
  this->pPropagateActivationFlag();
}

inline void FActor::Impl::TryDeactivateInstance()
{
  this->pPropagateActivationFlag();
}

inline void FActor::Impl::pPropagateActivationFlag() noexcept
{
  const auto activated = this->mRefActor.IsActivated();
  for (auto& [type, smtComponent] : mComponentList)
  {
    if (smtComponent == nullptr) { continue; }
    smtComponent->SetupFlagAsParent(activated);
  }

  for (auto& smtScript : this->mScriptList)
  {
    if (smtScript == nullptr) { continue; }
    smtScript->SetupFlagAsParent(activated);
  }

  if (this->mRigidbody != nullptr) 
  { 
    this->mRigidbody->SetupFlagAsParent(activated); 
  }

  for (const auto& [specifier, instance] : this->mChildrenActors)
  {
    if (instance == nullptr) { continue; }
    instance->SetupFlagAsParent(activated);
  }
}

inline void FActor::Impl::SetParent(FActor& iParentActor) noexcept
{
  this->mPtrParentActor = &iParentActor;
}

inline void FActor::Impl::SetParentAsRoot() noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mPtrParentActor = nullptr;
}

inline EWorldObjectType FActor::Impl::GetActorType() const noexcept
{
  return this->mActorType;
}

inline const std::string& FActor::Impl::GetActorTag() const noexcept
{
  return this->mActorTagSpecifier;
}

inline std::vector<NotNull<FActor*>> 
FActor::Impl::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
{
  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildrenActors)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorTag() == iTagSpecifier) { result.emplace_back(ptrsmtActor.get()); }
  }

  return result;
}

inline std::vector<NotNull<FActor*>> 
FActor::Impl::GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept
{
  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildrenActors)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorTag() == iTagSpecifier) { result.emplace_back(ptrsmtActor.get()); }
    if (ptrsmtActor->HasChildrenActor() == true)
    {
      const auto subResult = ptrsmtActor->GetAllActorsWithTagRecursive(iTagSpecifier);
      result.insert(result.end(), MDY_BIND_BEGIN_END(subResult));
    }
  }

  return result;
}

inline std::vector<NotNull<FActor*>> 
FActor::Impl::GetAllActorsWithName(_MIN_ const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true) { return {}; }

  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildrenActors)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorName() == iNameSpecifier) { result.emplace_back(ptrsmtActor.get()); }
  }

  return result;
}

inline std::vector<NotNull<FActor*>> 
FActor::Impl::GetAllActorsWithNameRecursive(_MIN_ const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true) { return {}; }

  std::vector<NotNull<FActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildrenActors)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorName() == iNameSpecifier) { result.emplace_back(ptrsmtActor.get()); }
    if (ptrsmtActor->HasChildrenActor() == true)
    {
      const auto subResult = ptrsmtActor->GetAllActorsWithNameRecursive(iNameSpecifier);
      result.insert(result.end(), MDY_BIND_BEGIN_END(subResult));
    }
  }

  return result;
}

inline FActor* FActor::Impl::GetActorWithObjectId(_MIN_ TU32 iObjectId) noexcept
{
  for (auto& [actorName, smtptrObject] : this->mChildrenActors)
  {
    if (smtptrObject == nullptr) { continue; }
    // Check id.
    if (smtptrObject->GetId() == iObjectId) { return smtptrObject.get(); }
    // Iterate id.
    auto* ptrReturnedObject = smtptrObject->GetActorWithObjectId(iObjectId);
    if (ptrReturnedObject != nullptr) { return ptrReturnedObject; }
  }
  return nullptr;
}

inline bool FActor::Impl::HasParent() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrParentActor);
}

inline FActor* FActor::Impl::GetPtrParent() const noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrParentActor)) { return nullptr; }
  else                                         { return this->mPtrParentActor; }
}

inline bool FActor::Impl::HasChildrenActor() const noexcept
{
  return std::any_of(MDY_BIND_BEGIN_END(this->mChildrenActors), [](const auto& iPair) 
  {
    return MDY_CHECK_ISNOTEMPTY(iPair.second);
  });
}

inline FActor::TActorMap& FActor::Impl::GetChildrenContainer() noexcept
{
  return this->mChildrenActors;
}

#ifdef false
CActorScript* FActor::Impl::GetScriptComponent(_MIN_ const std::string& scriptName) noexcept
{
  MDY_ASSERT_MSG(scriptName.empty() == false, "scriptName must not be empty at FActor::Impl::GetScriptComponent()");

  using TInstanceType = decltype(this->mScriptList)::value_type;
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mScriptList), [&scriptName](const TInstanceType& instance)
  {
    return instance->GetScriptVerificationName() == scriptName;
  });

  if (it == this->mScriptList.end())  { return std::nullopt; }
  else                                { return it->get(); }
}

EDySuccess FActor::Impl::RemoveScriptComponent(_MIN_ const std::string& scriptName) noexcept
{
  MDY_ASSERT_MSG(scriptName.empty() == false, "scriptName must not be empty at FActor::Impl::GetScriptComponent()");

  // Find script instance that has scriptName.
  using TInstanceType = decltype(this->mScriptList)::value_type;
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mScriptList), [&scriptName](const TInstanceType& instance)
  {
    if (MDY_CHECK_ISEMPTY(instance))  { return false; }
    else                              { return instance->GetScriptVerificationName() == scriptName; }
  });

  if (it == this->mScriptList.end())
  {
    DyPushLogWarning("FActor::Impl::RemoveComponent | Can not find script to remove. | Name : {}", scriptName);
    return DY_FAILURE;
  }
  else
  {
    // @TODO MOVE DESTROY FUNCTION CALL TO AFTERWARD COMPONENT UPDATE() (BEFORE TRANFORM SYNCHRONIZATION)
// @TODO FUCTION
    //(*it)->Destroy();
    (*it)->Release();
    this->mScriptList.erase(it);
    return DY_SUCCESS;
  }
}
#endif

inline void FActor::Impl::MDY_PRIVATE(TryRemoveScriptInstances)() noexcept
{
  return this->mScriptList.clear();
}

inline void FActor::Impl::MDY_PRIVATE(TryDetachDependentComponents)() noexcept
{
  this->MDY_PRIVATE(TryRemoveScriptInstances)();
  auto rendererList = this->GetGeneralComponentList<CModelRenderer>();
  for (auto& ptrRenderer : rendererList)
  {
    // Try detach renderer from list, if not exist already just do nothing.
    MWorld::GetInstance().MDY_PRIVATE(TryDetachActiveModelRenderer)(ptrRenderer);
  }

  for (auto& [specifier, ptrsmtChild] : this->mChildrenActors)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtChild)) { return; }
    ptrsmtChild->MDY_PRIVATE(TryDetachDependentComponents)();
  }
}

inline NotNull<CTransform*> FActor::Impl::GetTransform() noexcept
{
  return DyMakeNotNull(this->mTransform.get());
}

inline CPhysicsRigidbody* FActor::Impl::GetRigidbody() noexcept
{
  if (this->mRigidbody == nullptr) { return nullptr; } 

  return DyMakeNotNull(this->mRigidbody.get()); 
}

inline CActorScript* FActor::Impl::GetScriptComponent(const std::string& scriptName) noexcept
{
  for (auto& smtptrScript : this->mScriptList)
  {
    if (smtptrScript == nullptr) { continue; }
    MDY_NOT_IMPLEMENTED_ASSERT();
  }
  return nullptr;
}

inline EDySuccess FActor::Impl::RemoveScriptComponent(const std::string& scriptName) noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_FAILURE;
}

} /// ::dy namespace

#endif /// GUARD_DY_ELEMENT_INLINE_FACTORIMPL_H