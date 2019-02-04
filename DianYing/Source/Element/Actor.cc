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
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Component/CDyTransform.h>
#include <Dy/Component/CDyModelFilter.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Element/Type/PDyActorCreationDescriptor.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Component/CDyModelAnimator.h>
#include <Dy/Component/CDySoundSource.h>
#include <Dy/Component/CDyPhysicsCollider.h>

//!
//! Implementation
//!

namespace dy
{

FDyActor::FDyActor(_MIN_ const PDyObjectMetaInfo& objectMetaDesc, _MIN_ FDyActor* iPtrParent)
{
  // (1) Set properties.
  this->pSetObjectName(objectMetaDesc.mSpecifierName);

  // (1-1) Insert prefab information if prefab exist.
  auto metaComponentInfo = objectMetaDesc.mMetaComponentInfo;
  if (objectMetaDesc.mProperties.mIsUsingPrefab == true)
  { 
    MDY_ASSERT(objectMetaDesc.mProperties.mPrefabSpecifierName.empty() == false, "Unexpected error occurred.");
    const auto& prefab = MDyMetaInfo::GetInstance().GetPrefabMetaInformation(objectMetaDesc.mProperties.mPrefabSpecifierName);

    metaComponentInfo.insert(metaComponentInfo.end(), MDY_BIND_BEGIN_END(prefab.mMetaComponentInfo));

    if (objectMetaDesc.mProperties.mIsOverridePrefabTag == true)
    { this->mActorTagSpecifier = objectMetaDesc.mProperties.mTagSpecifier; }
    else
    { this->mActorTagSpecifier = prefab.mCommonProperties.mTagSpecifier; }
  }
  else { this->mActorTagSpecifier = objectMetaDesc.mProperties.mTagSpecifier; } 

  // (2) Create components
  // Check activation flags and execute sub-routines of each components.
  this->MDY_PRIVATE_SPECIFIER(CreateComponentList)(metaComponentInfo);

  if (MDY_CHECK_ISNOTNULL(iPtrParent)) { this->SetParent(*iPtrParent); }
  this->pUpdateActivateFlagFromParent();
  if (objectMetaDesc.mProperties.mInitialActivated == true) { this->Activate(); }

  // (3) Make children actors.
  for (const auto& objectInformation : objectMetaDesc.mChildrenList)
  { // Create object, FDyActor
    switch (objectInformation->mObjectType)
    {
    case EDyMetaObjectType::Actor:
    { // General object type. Make FDyActor instance.
      auto instancePtr = std::make_unique<FDyActor>(*objectInformation, this);

      auto [it, result] = this->mChildActorMap.try_emplace(instancePtr->GetActorName(), std::move(instancePtr));
      MDY_ASSERT(result == true, "Unexpected error occured in inserting FDyActor to object map.");
    } break;
    case EDyMetaObjectType::SceneScriptor:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
    case EDyMetaObjectType::Object:         MDY_NOT_IMPLEMENTED_ASSERT(); break;
    default: MDY_UNEXPECTED_BRANCH(); break;
    }
  }

  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mTransform), "CDyTransform component must be created to all FDyActor.");
  SDyProfilingHelper::IncreaseOnBindActorCount(1);
}

FDyActor::FDyActor(_MIN_ const PDyActorCreationDescriptor& iDesc, _MIN_ FDyActor* iPtrParent)
{
  // (1) Set properties.
  this->pSetObjectName(iDesc.mActorSpecifierName);

  // (1-1) Insert prefab information if prefab exist.
  TComponentMetaList metaComponentInfo;
  if (iDesc.mPrefabSpecifierName.empty() == false)
  { 
    const auto& prefab = MDyMetaInfo::GetInstance().GetPrefabMetaInformation(iDesc.mPrefabSpecifierName);
    metaComponentInfo.insert(metaComponentInfo.end(), MDY_BIND_BEGIN_END(prefab.mMetaComponentInfo));

    if (iDesc.mIsOverridePrefabTag == true)
    { this->mActorTagSpecifier = iDesc.mObjectTag; }
    else
    { this->mActorTagSpecifier = prefab.mCommonProperties.mTagSpecifier; }
  }
  else { this->mActorTagSpecifier = iDesc.mObjectTag; }

  // (2) Create components
  // Check activation flags and execute sub-routines of each components.
  this->MDY_PRIVATE_SPECIFIER(CreateComponentList)(metaComponentInfo);

  if (MDY_CHECK_ISNOTNULL(iPtrParent)) { this->SetParent(*iPtrParent); }
  this->pUpdateActivateFlagFromParent();
  this->Activate();

  // (3) Create Transform component using Given transform
  this->AddComponent<CDyTransform>(iDesc.mTransform);
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mTransform), "CDyTransform component must be created to all FDyActor.");
  SDyProfilingHelper::IncreaseOnBindActorCount(1);
}

void FDyActor::MDY_PRIVATE_SPECIFIER(CreateComponentList)(const TComponentMetaList& iMetaComponentList)
{
  for (const auto& [type, componentInfo] : iMetaComponentList)
  {
    switch (type)
    {
    default: MDY_UNEXPECTED_BRANCH(); break;
    case EDyComponentMetaType::Transform:
      this->AddComponent<CDyTransform>(std::any_cast<const PDyTransformComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Script:
      this->AddComponent<CDyActorScript>(std::any_cast<const PDyScriptComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::DirectionalLight:
      this->AddComponent<CDyDirectionalLight>(std::any_cast<const PDyDirLightComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::ModelFilter:
      this->AddComponent<CDyModelFilter>(std::any_cast<const PDyModelFilterComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::ModelRenderer:
      this->AddComponent<CDyModelRenderer>(std::any_cast<const PDyModelRendererComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::ModelAnimator:
      this->AddComponent<CDyModelAnimator>(std::any_cast<const PDyModelAnimatorComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Camera:
      this->AddComponent<CDyCamera>(std::any_cast<const PDyCameraComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::SoundSource:
      this->AddComponent<CDySoundSource>(std::any_cast<const PDySoundSourceComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Rigidbody:
      this->AddComponent<CDyPhysicsRigidbody>(std::any_cast<const PDyRigidbodyComponentMetaInfo&>(componentInfo));
      break;
    case EDyComponentMetaType::Collider:
      this->AddComponent<CDyPhysicsCollider>(std::any_cast<const PDyColliderComponentMetaInfo&>(componentInfo));
      break;
    }
  }

  // If rigidbody is not exist, just create default rigidbody.
  if (MDY_CHECK_ISNULL(this->mRigidbody))
  {
    const PDyRigidbodyComponentMetaInfo defaultMetaInfo{};
    this->AddComponent<CDyPhysicsRigidbody>(defaultMetaInfo);
  }
}

FDyActor::~FDyActor()
{
  SDyProfilingHelper::DecreaseOnBindActorCount(1);
  for (auto& item : this->mComponentList)
  {
    this->ReleaseComponent(item);
  }
  this->mComponentList.clear();
}

void FDyActor::ReleaseComponent(_MINOUT_ TComponentList::value_type& iItem)
{
  auto& [typeVal, ptrsmtComponent] = iItem;
  if (MDY_CHECK_ISEMPTY(ptrsmtComponent)) { return; }

  using _ = EDyComponentType;
  // ActorScript, Transform does not release in this logic.
  // We use downcasting intentionally to call Release() function.
  switch (typeVal)
  {
  case EDyComponentType::DirectionalLight: 
  { static_cast<TComponentBindingType<_::DirectionalLight>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::Camera:
  { static_cast<TComponentBindingType<_::Camera>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::ModelAnimator:
  { static_cast<TComponentBindingType<_::ModelAnimator>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::ModelFilter:
  { static_cast<TComponentBindingType<_::ModelFilter>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::ModelRenderer:
  { static_cast<TComponentBindingType<_::ModelRenderer>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::SoundSource:
  { static_cast<TComponentBindingType<_::SoundSource>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::Rigidbody:
  { static_cast<TComponentBindingType<_::Rigidbody>::Type&>(*ptrsmtComponent).Release(); } break;
  case EDyComponentType::Collider:
  { static_cast<TComponentBindingType<_::Collider>::Type&>(*ptrsmtComponent).Release(); } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void FDyActor::DestroySelf()
{
  MDyWorld::GetInstance().DestroyActor(*this);
}

const std::string& FDyActor::GetActorName() const noexcept
{
  return this->pGetObjectName();
}

std::string FDyActor::MDY_PRIVATE_SPECIFIER(GetFullSpecifierName)() const noexcept
{
  if (this->IsHaveParent() == false) { return this->GetActorName(); }
  else
  {
    const auto headFullSpecifierName = this->GetParent()->MDY_PRIVATE_SPECIFIER(GetFullSpecifierName)();
    return fmt::format("{}.{}", headFullSpecifierName, this->GetActorName());
  }
}

void FDyActor::pUpdateActivateFlagFromParent() noexcept
{
  // If parent actor is not exist, it regards as a root object.
  // Otherwise, get parent actor flag.
  if (MDY_CHECK_ISNULL(this->mPtrParentActor))
  { this->SetupFlagAsParent(true); }
  else
  { this->SetupFlagAsParent(this->mPtrParentActor->IsActivated()); }

  this->pPropagateActivationFlag();
}

void FDyActor::TryActivateInstance()
{
  this->pPropagateActivationFlag();
}

void FDyActor::TryDeactivateInstance()
{
  this->pPropagateActivationFlag();
}

void FDyActor::pPropagateActivationFlag() noexcept
{
  for (auto& [type, unknownComponent] : mComponentList)
  {
    if (MDY_CHECK_ISEMPTY(unknownComponent)) { continue; }
    unknownComponent->SetupFlagAsParent(this->IsActivated());
  }

  for (auto& unknownScript : this->mScriptList)
  {
    if (MDY_CHECK_ISEMPTY(unknownScript)) { continue; }
    unknownScript->SetupFlagAsParent(this->IsActivated());
  }

  // @TODO PROPAGATE ACTIVATION FLAG TO SUBACTOR ALSO.
}

void FDyActor::SetParent(_MIN_ FDyActor& validParentRawPtr) noexcept
{
  this->mPtrParentActor = &validParentRawPtr;
}

void FDyActor::SetParentAsRoot() noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  this->mPtrParentActor = nullptr;
}

EDyMetaObjectType FDyActor::GetActorType() const noexcept
{
  return this->mActorType;
}

const std::string& FDyActor::GetActorTag() const noexcept
{
  return this->mActorTagSpecifier;
}

std::vector<NotNull<FDyActor*>> 
FDyActor::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
{
  std::vector<NotNull<FDyActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorTag() == iTagSpecifier) { result.emplace_back(ptrsmtActor.get()); }
  }

  return result;
}

std::vector<NotNull<FDyActor*>> 
FDyActor::GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept
{
  std::vector<NotNull<FDyActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorTag() == iTagSpecifier) { result.emplace_back(ptrsmtActor.get()); }
    if (ptrsmtActor->IsHavingChildrenObject() == true)
    {
      const auto subResult = ptrsmtActor->GetAllActorsWithTagRecursive(iTagSpecifier);
      result.insert(result.end(), MDY_BIND_BEGIN_END(subResult));
    }
  }

  return result;
}

std::vector<NotNull<FDyActor*>> 
FDyActor::GetAllActorsWithName(_MIN_ const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true) { return {}; }

  std::vector<NotNull<FDyActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorName() == iNameSpecifier) { result.emplace_back(ptrsmtActor.get()); }
  }

  return result;
}

std::vector<NotNull<FDyActor*>> 
FDyActor::GetAllActorsWithNameRecursive(_MIN_ const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true) { return {}; }

  std::vector<NotNull<FDyActor*>> result;
  for (const auto& [specifier, ptrsmtActor] : this->mChildActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtActor)) { continue; }
    if (ptrsmtActor->GetActorName() == iNameSpecifier) { result.emplace_back(ptrsmtActor.get()); }
    if (ptrsmtActor->IsHavingChildrenObject() == true)
    {
      const auto subResult = ptrsmtActor->GetAllActorsWithNameRecursive(iNameSpecifier);
      result.insert(result.end(), MDY_BIND_BEGIN_END(subResult));
    }
  }

  return result;
}

std::unique_ptr<CDyActorScript> 
FDyActor::MDY_PRIVATE_SPECIFIER(MakeScriptComponent)(_MIN_ const PDyScriptComponentMetaInfo& info)
{
  auto& metaManager = MDyMetaInfo::GetInstance();
  MDY_ASSERT(metaManager.IsScriptMetaInformationExist(info.mDetails.mSpecifierName) == true, "");

  const auto& instanceInfo = metaManager.GetScriptMetaInformation(info.mDetails.mSpecifierName);
  MDY_ASSERT(instanceInfo.mScriptType != EDyScriptType::NoneError, "");

  return std::make_unique<CDyActorScript>(*this, info.mDetails.mSpecifierName);
}

bool FDyActor::IsHaveParent() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrParentActor);
}

FDyActor* FDyActor::GetParent() const noexcept
{
  if (MDY_CHECK_ISNULL(this->mPtrParentActor)) { return nullptr; }
  else                                         { return this->mPtrParentActor; }
}

bool FDyActor::IsHavingChildrenObject() const noexcept
{
  return std::any_of(MDY_BIND_BEGIN_END(this->mChildActorMap), [](const auto& iPair) 
  {
    return MDY_CHECK_ISNOTEMPTY(iPair.second);
  });
}

FDyActor::TActorMap& FDyActor::GetChildrenContainer() noexcept
{
  return this->mChildActorMap;
}

std::string FDyActor::ToString()
{
  return MDY_INITIALIZE_EMPTYSTR;
}

#ifdef false
CDyActorScript* FDyActor::GetScriptComponent(_MIN_ const std::string& scriptName) noexcept
{
  MDY_ASSERT(scriptName.empty() == false, "scriptName must not be empty at FDyActor::GetScriptComponent()");

  using TInstanceType = decltype(this->mScriptList)::value_type;
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mScriptList), [&scriptName](const TInstanceType& instance)
  {
    return instance->GetScriptVerificationName() == scriptName;
  });

  if (it == this->mScriptList.end())  { return std::nullopt; }
  else                                { return it->get(); }
}

EDySuccess FDyActor::RemoveScriptComponent(_MIN_ const std::string& scriptName) noexcept
{
  MDY_ASSERT(scriptName.empty() == false, "scriptName must not be empty at FDyActor::GetScriptComponent()");

  // Find script instance that has scriptName.
  using TInstanceType = decltype(this->mScriptList)::value_type;
  const auto it = std::find_if(MDY_BIND_BEGIN_END(this->mScriptList), [&scriptName](const TInstanceType& instance)
  {
    if (MDY_CHECK_ISEMPTY(instance))  { return false; }
    else                              { return instance->GetScriptVerificationName() == scriptName; }
  });

  if (it == this->mScriptList.end())
  {
    MDY_LOG_WARNING("FDyActor::RemoveComponent | Can not find script to remove. | Name : {}", scriptName);
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

void FDyActor::MDY_PRIVATE_SPECIFIER(TryRemoveScriptInstances)() noexcept
{
  return this->mScriptList.clear();
}

void FDyActor::MDY_PRIVATE_SPECIFIER(TryDetachDependentComponents)() noexcept
{
  this->MDY_PRIVATE_SPECIFIER(TryRemoveScriptInstances)();
  auto rendererList = this->GetGeneralComponentList<CDyModelRenderer>();
  for (auto& ptrRenderer : rendererList)
  { // Try detach renderer from list, if not exist already just do nothing.
    MDyWorld::GetInstance().MDY_PRIVATE_SPECIFIER(TryDetachActiveModelRenderer)(ptrRenderer);
  }

  for (auto& [specifier, ptrsmtChild] : this->mChildActorMap)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtChild)) { return; }
    ptrsmtChild->MDY_PRIVATE_SPECIFIER(TryDetachDependentComponents)();
  }
}

NotNull<CDyTransform*> FDyActor::GetTransform() noexcept
{
  return DyMakeNotNull(this->mTransform.get());
}

NotNull<CDyPhysicsRigidbody*> FDyActor::GetRigidbody() noexcept
{
  return DyMakeNotNull(this->mRigidbody.get());
}

} /// ::dy namespace
