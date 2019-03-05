#ifndef GUARD_DY_MANAGEMENT_WORLDMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_WORLDMANAGER_IMPL_INL
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

#include <Dy/Meta/Information/ElementLevelMetaInfo.h>

namespace dy
{

inline void MDyWorld::Impl::Update(TF32 dt)
{
  if (this->mLevel != nullptr)
  {
    this->mLevel->Update(dt);
  }
}

inline void MDyWorld::Impl::pGcAcitvatedComponents()
{
  // Remove activated script update list reversely.
  if (this->mErasionScriptCandidateList.empty() == false)
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionScriptCandidateList), std::greater<TI32>());
    for (const auto& index : this->mErasionScriptCandidateList)
    { // Remove!
      // @TODO CALL SCRIPT MANAGE TO GC.
      //this->mActivatedScripts.erase(this->mActivatedScripts.begin() + index);
    }
    // Clear!
    this->mErasionScriptCandidateList.clear();
  }

  // Remove activated model renderer (CDyModelRenderer) list reversely to avoiding invalidation index.
  if (this->mErasionModelRenderersCandidateList.empty() == false)
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionModelRenderersCandidateList), std::greater<>());
    for (const auto& index : this->mErasionModelRenderersCandidateList)
    { // Remove!
      this->mActivatedModelRenderers.erase(this->mActivatedModelRenderers.begin() + index);
    }
    // Clear!
    this->mErasionModelRenderersCandidateList.clear();
  }

  // Remove activated camera (CDyCamera) list reversely to avoiding invalidation index.
  if (this->mErasionCamerasCandidateList.empty() == false)
  {
    std::sort(MDY_BIND_BEGIN_END(this->mErasionCamerasCandidateList), std::greater<>());
    for (const auto& index : this->mErasionCamerasCandidateList)
    { // Remove!
      this->mActivatedOnRenderingCameras.erase(this->mActivatedOnRenderingCameras.begin() + index);
    }
    // Clear!
    this->mErasionCamerasCandidateList.clear();
  }
}

inline void MDyWorld::Impl::UpdateObjects(TF32 iDt)
{
  if (this->mLevel)
  { 
    // CDyModelRenderer update
    for (auto& modelRenderer : this->mActivatedModelRenderers)
    {
      if (MDY_CHECK_ISNULL(modelRenderer)) { continue; }
      modelRenderer->Update(iDt);
    }

    // CDyCamera update
    for (auto& camera : this->mActivatedOnRenderingCameras)
    {
      if (MDY_CHECK_ISNULL(camera)) { continue; }
      camera->Update(iDt);
    }

    // After update, check
  }
}

inline void MDyWorld::Impl::UpdateAnimator(TF32 dt)
{
  // Update animation transformation information 
  // using linear-interpolation of given skeleton and animation.
  for (auto& ptrCompAnimator : this->mActivatedModelAnimatorPtrs)
  {
    ptrCompAnimator->Update(dt);
  }
}

inline std::vector<NotNull<FDyActor*>> 
MDyWorld::Impl::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
{
  // If level is not constructed, just return empty list.
  if (this->mLevel != nullptr) { return {}; }
  return this->mLevel->GetAllActorsWithTag(iTagSpecifier);
}

inline std::vector<NotNull<FDyActor*>>
MDyWorld::Impl::GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept
{
  // If level is not constructed, just return empty list.
  if (this->mLevel != nullptr) { return {}; }
  return this->mLevel->GetAllActorsWithTagRecursive(iTagSpecifier);
}

inline std::vector<NotNull<FDyActor*>> 
MDyWorld::Impl::GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept
{
  // If iNameSpacifier is empyt, just return empyt list.
  if (iNameSpecifier.empty() == true) { return {}; }
  if (this->mLevel != nullptr)        { return {}; }
  return this->mLevel->GetAllActorsWithName(iNameSpecifier);
}

inline std::vector<NotNull<FDyActor*>> 
MDyWorld::Impl::GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept
{
  if (iNameSpecifier.empty() == true) { return {}; }
  if (this->mLevel != nullptr)        { return {}; }
  return this->mLevel->GetAllActorsWithNameRecursive(iNameSpecifier);
}

inline FDyActor* MDyWorld::Impl::GetActorWithObjectId(TU32 iObjectId) noexcept
{
  // Check
  if (MDY_CHECK_ISNULL(this->mLevel)) { return nullptr; }

  // Trying to get pointer of given obejct id.
  return this->mLevel->GetActorWithObjectId(iObjectId);
}

inline DDyActorBinder MDyWorld::Impl::CreateActor(
  const std::string& iActorName, 
  const std::string& iPrefabName,
  const DDyTransform& iSpawnTransform, 
  FDyActor* iPtrParent, 
  const std::string& iObjectTag, 
  bool iDoSweep)
{
  // Check prefab is exist on meta information manager.
  MDY_ASSERT_MSG_FORCE(
    MDyMetaInfo::GetInstance().IsPrefabMetaInformationExist(iPrefabName) == true,
    "Failed to find prefab with specified `iPrefabName`.");

  PDyActorCreationDescriptor descriptor = {};
  descriptor.mParentFullSpecifierName = iPtrParent != nullptr ? iPtrParent->GetActorFullName() : "";
  // Actor speciier name will be auto-generated when creation.
  descriptor.mActorSpecifierName = iActorName;
  descriptor.mTransform = iSpawnTransform;
  descriptor.mIsDoSweep = iDoSweep;
  descriptor.mPrefabSpecifierName = iPrefabName;
  // Check tag is exist, when tag is not empty.
  if (iObjectTag.empty() == false)
  { 
    MDY_CALL_ASSERT_SUCCESS(MDySetting::GetInstance().MDY_PRIVATE(CheckObjectTagIsExist)(iObjectTag));
    descriptor.mObjectTag = iObjectTag;
  }

  // Push requisition actor item.
  DySafeUniquePtrEmplaceBack(this->mActorCreationDescList, descriptor);

  // Bind actor.
  DDyActorBinder resultBinder {};
  resultBinder.MDY_PRIVATE(BindDescriptor)(this->mActorCreationDescList.back().get());
  return resultBinder;
}

inline void MDyWorld::Impl::DestroyActor(FDyActor& iRefActor)
{
  if (iRefActor.HasParent() == true)
  { // If iRefActor has parent, let parent detach and remove iRefActor from object tree.
    // and move it to MDyWorld::Impl::GC LIST.
    auto& container = iRefActor.GetPtrParent()->GetChildrenContainer();
    auto  it = std::find_if(MDY_BIND_BEGIN_END(container), 
        [ptr = &iRefActor](const std::decay_t<decltype(container)>::value_type& iPair)
        {
          return iPair.second.get() == ptr;
        }
    );
    // If not found, (might be requiring duplicated destruction) just do nothing and return.
    if (it == container.end()) { return; }

    this->mGCedActorList.emplace_back(std::move(it->second));
    this->mGCedActorList.back()->MDY_PRIVATE(TryDetachDependentComponents)();
    container.erase(it);
  }
  else
  { // If iRefActor has not parent, regard it is on root, and remove iRefActor from level.
    // and move it to MDYWorld::Impl::GC List.
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mLevel), "Unexpected error occurred.");
    auto& container = this->mLevel->GetActorContainer();
    auto  it = std::find_if(MDY_BIND_BEGIN_END(container),
        [ptr = &iRefActor](const std::decay_t<decltype(container)>::value_type& iPair)
        {
          return iPair.second.get() == ptr;
        }
    );
    // If not found, (might be requiring duplicated destruction) just do nothing and return.
    if (it == container.end()) { return; }

    this->mGCedActorList.emplace_back(std::move(it->second));
    this->mGCedActorList.back()->MDY_PRIVATE(TryDetachDependentComponents)();
    container.erase(it);
  }
}

inline std::optional<DDyUiBinder> 
MDyWorld::Impl::CreateUiObject(
  const std::string& iUiName, 
  const std::string& iNewCustomizedName, 
  bool isForcedZOrder,
  TU32 ZOrder)
{
  // Check
  const auto& refMetaInfo = MDyMetaInfo::GetInstance();
  if (refMetaInfo.IsWidgetMetaInfoExist(iUiName) == false)
  {
    DyPushLogError("Failed to create UI Widget object, {}.", iUiName);
    return std::nullopt;
  }

  if (this->mUiInstanceContainer.IsUiObjectExist(iUiName) == true && iNewCustomizedName.empty() == true)
  {
    DyPushLogError(
        "Failed to create UI Widget object, meta information is exist but already exist on Container. {}", 
        iUiName);
    return std::nullopt;
  }

  // Get anyway
  const auto& refDescriptor = refMetaInfo.GetWidgetMetaInformation(iUiName);
  const auto keyName = iNewCustomizedName.empty() == true ? refDescriptor.mWidgetSpecifierName : iNewCustomizedName;

  // If zorder must be customized, do that.
  TU32 insertZorder = refDescriptor.mZOrder;
  if (isForcedZOrder == true) { insertZorder = ZOrder; }

  return this->mUiInstanceContainer.CreateUiObject(keyName, refDescriptor, insertZorder);
}

inline std::optional<DDyUiBinder> MDyWorld::Impl::GetUiObject(const std::string& iUiName)
{
  return this->mUiInstanceContainer.GetUiObject(iUiName);
}

inline EDySuccess MDyWorld::Impl::DestoryUiObject(DDyUiBinder& ioRefUi)
{
  // Check
  if (ioRefUi.IsUiObjectValid() == false) 
  { 
    DyPushLogError("Failed to destroy Ui object. Ui binder does not bind anything.");
    return DY_FAILURE; 
  }

  return this->mUiInstanceContainer.RemoveUiObject((*ioRefUi).GetUiObjectName());
}

inline EDySuccess MDyWorld::Impl::DestroyUiObject(const std::string& iUiName)
{
  return this->mUiInstanceContainer.RemoveUiObject(iUiName);
}

inline CDyCamera* MDyWorld::Impl::GetPtrMainLevelCamera() const noexcept
{
  // @TODO TEMPORARY.
  if (this->mActivatedOnRenderingCameras.empty() == true) 
  { 
    return nullptr; 
  }

  if (MDY_CHECK_ISNULL(this->mActivatedOnRenderingCameras.front())) 
  { 
    return nullptr; 
  }
  else { return this->mActivatedOnRenderingCameras.front(); }
}

inline TI32 MDyWorld::Impl::GetFocusedCameraCount() const noexcept
{
  return static_cast<TI32>(this->mActivatedOnRenderingCameras.size());
}

inline std::optional<CDyCamera*> MDyWorld::Impl::GetFocusedCameraValidReference(const TI32 index) const noexcept
{
  MDY_ASSERT_MSG(
      index < this->mActivatedOnRenderingCameras.size(),
      R"dy(Input parameter "index" for "MDyWorld::Impl::GetFocusedCameraValidReferenc" must be equal or less than "MDyWorld::Impl::mActivatedOnRenderingCameras".)dy");

  auto* camera = this->mActivatedOnRenderingCameras[index];
  if (MDY_CHECK_ISNULL(camera)) { return std::nullopt; }
  else                          { return camera; }
}

inline EDySuccess MDyWorld::Impl::OpenLevel(const std::string& levelName)
{
  if (MDyMetaInfo::GetInstance().GetLevelMetaInformation(levelName) == nullptr)
  {
    DyPushLogError("{} | Failed to find and travel next level. Level name is not found. | Level name : {}", levelName);
    return DY_FAILURE;
  }

  this->SetLevelTransition(levelName);
  return DY_SUCCESS;
}

inline bool MDyWorld::Impl::IsNeedTransitNextLevel() const noexcept
{
  return this->mIsNeedTransitNextLevel;
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(OpenFirstLevel)()
{
  this->SetLevelTransition(MDySetting::GetInstance().GetInitialSceneInformationName());

  // Let present level do release sequence
  // Game Status Sequence 12-13.
  this->MDY_PRIVATE(RemoveLevel)();
  this->MDY_PRIVATE(PopulateNextLevelResources)();
  return DY_SUCCESS;
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(RemoveLevel)()
{
  // Let present level do release sequence
  // And level must be nullptr. and... Remove RI and Resource & Informations with Scope is `Level`.
  if (MDY_CHECK_ISEMPTY(this->mLevel)) { return DY_FAILURE; }
  
  // Release physx components which are dependent on physx::PxScene, FDyLevel.
  this->mLevel = nullptr;
  this->MDY_PRIVATE(TryRemoveActorGCList)();
  this->mActivatedModelRenderers.clear();
  this->mActivatedOnRenderingCameras.clear();
  this->mActivatedModelAnimatorPtrs.clear();
  this->mActivatedSkyboxPtrList.clear();
  this->mUiInstanceContainer.ClearGeneralUiObjectList();

  // Just remove script instance without `Destroy` function intentionally.
  MDyScript::GetInstance().ClearWidgetScriptGCList();
  MDyScript::GetInstance().ClearActorScriptGCList();

  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Temporal, EDyResourceStyle::Information);

  SDyIOConnectionHelper::TryGC(EDyScope::Level, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Level, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Level, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Level, EDyResourceStyle::Information);
  SDyIOConnectionHelper::TryGC(EDyScope::Level, EDyResourceStyle::Resource);
  SDyIOConnectionHelper::TryGC(EDyScope::Level, EDyResourceStyle::Information);

  // Must reset depedent manager on this.
  MDyPhysics::GetInstance().ReleaseScene();
  return DY_SUCCESS;
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(PopulateNextLevelResources)()
{
  if (this->mNextLevelName.empty() == true) { return DY_FAILURE; }
  if (MDyMetaInfo::GetInstance().IsLevelMetaInformation(this->mNextLevelName) == false) { return DY_FAILURE; }

  // Get level meta information, and construct resource list.
  const auto& levMetaInfo = *MDyMetaInfo::GetInstance().GetLevelMetaInformation(this->mNextLevelName);
  const TDDyResourceNameSet levelResourceSet = levMetaInfo.GetLevelResourceSet();

  // Populate resource and wait until resource populating is done.
  // If done, call `build next level` in outside (MDySync). (GSS 12-13)
  SDyIOConnectionHelper::PopulateResourceList(
      levelResourceSet, 
      EDyScope::Level,
      []() 
  { 
    auto& mWorld = MDyWorld::GetInstance();
    mWorld.MDY_PRIVATE(BuildNextLevel)(); 
    mWorld.MDY_PRIVATE(TransitionToNextLevel)();
    DyEngine::GetInstance().SetNextGameStatus(EDyGlobalGameStatus::GameRuntime);
  });
  return DY_SUCCESS;
}

inline void MDyWorld::Impl::MDY_PRIVATE(BuildNextLevel)()
{
  // GSS 14
  DyPushLogDebugDebug("Building Next Level : {}", this->mNextLevelName);

  const auto* levelMetaInfo = MDyMetaInfo::GetInstance().GetLevelMetaInformation(this->mNextLevelName);
  // Must reset depedent manager on this.
  MDyPhysics::GetInstance().InitScene();
  this->mLevel = std::make_unique<FDyLevel>(*levelMetaInfo);
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(TransitionToNextLevel)()
{
  // GSS 15
  this->mPreviousLevelName  = this->mPresentLevelName;
  this->mPresentLevelName   = this->mNextLevelName;
  this->mNextLevelName      = MDY_INITIALIZE_EMPTYSTR;
  this->mIsNeedTransitNextLevel = false;
  DyPushLogDebugDebug("Present  Level Name : {}", this->mPresentLevelName);
  DyPushLogDebugDebug("Previous Level Name : {}", this->mPreviousLevelName);

  // Need to call initiate funciton maually.
  DyPushLogDebugDebug("Initiate Actor script : {}", this->mPresentLevelName);
  MDyScript::GetInstance().UpdateActorScript(0.0f, EDyScriptState::CalledNothing);
  MDyScript::GetInstance().TryMoveInsertActorScriptToMainContainer();

  // Need to realign position following actor tree.
  DyPushLogDebugDebug("Align Position of Actors on level : {}", this->mPresentLevelName);
  this->mLevel->MDY_PRIVATE(AlignActorsPosition)();

  return DY_SUCCESS;
}

inline bool MDyWorld::Impl::IsLevelPresentValid() const noexcept
{
  return MDY_CHECK_ISNOTEMPTY(this->mLevel);
}

inline FDyLevel& MDyWorld::Impl::GetValidLevelReference() noexcept
{
  MDY_ASSERT_MSG(IsLevelPresentValid() == true, "Level must be valid when retrieving level reference.");
  return *this->mLevel;
}

inline EDySuccess MDyWorld::Impl::TryCreateDebugUi()
{
  return this->mUiInstanceContainer.TryCreateDebugUi();
}

inline bool MDyWorld::Impl::IsDebugUiExist() const noexcept
{
  return this->mUiInstanceContainer.IsDebugUiExist();
}

inline EDySuccess MDyWorld::Impl::TryRemoveDebugUi()
{
  return this->mUiInstanceContainer.TryRemoveDebugUi();
}

inline EDySuccess MDyWorld::Impl::TryCreateLoadingUi()
{
  return this->mUiInstanceContainer.TryCreateLoadingUi();
}

inline bool MDyWorld::Impl::IsLoadingUiExist() const noexcept
{
  return this->mUiInstanceContainer.IsLoadingUiExist();
}

inline EDySuccess MDyWorld::Impl::TryRemoveLoadingUi()
{
  return this->mUiInstanceContainer.TryRemoveLoadingUi();
}

inline bool MDyWorld::Impl::CheckCreationActorExist() const noexcept
{
  return this->mActorCreationDescList.empty() == false;
}

inline void MDyWorld::Impl::TryCreateActorsOfCreationActorList() noexcept
{
  // Check level is not empty.
  if (MDY_CHECK_ISEMPTY(this->mLevel)) { return; }

  //
  for (const auto& ptrsmtDescriptor : this->mActorCreationDescList)
  {
    this->mLevel->CreateActorInstantly(*ptrsmtDescriptor);
  }
}

inline void MDyWorld::Impl::CleanCreationActorList() noexcept
{
  this->mActorCreationDescList.clear();
}

inline bool MDyWorld::Impl::CheckIsGcActorExist() const noexcept
{
  return this->mGCedActorList.empty() == false;
}

inline void MDyWorld::Impl::MDY_PRIVATE(TryRemoveActorGCList)() noexcept
{
  this->mGCedActorList.clear();
}

inline void MDyWorld::Impl::SetLevelTransition(const std::string& iSpecifier)
{
  if (MDyMetaInfo::GetInstance().IsLevelMetaInformation(iSpecifier) == false)
  {
    DyPushLogError("Failed to transit next level, `{0}`. `{0}` level is not exist.", iSpecifier);
    return;
  }

  this->mNextLevelName          = iSpecifier;
  this->mIsNeedTransitNextLevel = true;
}

inline void MDyWorld::Impl::pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept
{
  this->mGCedActorList.emplace_back(std::unique_ptr<FDyActor>(actorRawPtr));
  this->mGCedActorList.back()->MDY_PRIVATE(TryRemoveScriptInstances)();
}

inline void MDyWorld::Impl::pfUnenrollActiveModelRenderer(TI32 index) noexcept
{
  MDY_ASSERT_MSG(
    index < this->mActivatedModelRenderers.size(), 
    "index must be smaller than this->mActivatedModelRenderers.size().");

  this->mActivatedModelRenderers[index] = MDY_INITIALIZE_NULL;
  this->mErasionModelRenderersCandidateList.emplace_back(index);
}

inline void MDyWorld::Impl::pfUnenrollActiveCamera(TI32& ioIndex) noexcept
{
  MDY_ASSERT_MSG(
    ioIndex < this->mActivatedOnRenderingCameras.size(), 
    "ioIndex must be smaller than this->mActivatedOnRenderingCameras.size().");

  this->mActivatedOnRenderingCameras[ioIndex] = MDY_INITIALIZE_NULL;
  this->mErasionCamerasCandidateList.emplace_back(ioIndex);

  ioIndex = MDY_INITIALIZE_DEFINT;
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(TryDetachActiveModelRenderer)(CDyModelRenderer* iPtrRenderer)
{
  auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActivatedModelRenderers), 
      [iPtrRenderer](_MIN_ const CDyModelRenderer* ptrRenderer) { return iPtrRenderer == ptrRenderer; });
  if (it == this->mActivatedModelRenderers.end()) { return DY_SUCCESS; }

  DyFastErase(this->mActivatedModelRenderers, std::distance(this->mActivatedModelRenderers.begin(), it));
  return DY_SUCCESS;
}

inline void MDyWorld::Impl::MDY_PRIVATE(BindActiveModelAnimator)(CDyModelAnimator& iRefComponent)
{
  this->mActivatedModelAnimatorPtrs.emplace_back(DyMakeNotNull(&iRefComponent));
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(UnbindActiveModelAnimator)(CDyModelAnimator& iRefComponent)
{
  // Check address. component's address is not changed unless actor is destroyed.
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActivatedModelAnimatorPtrs), 
      [ptr = &iRefComponent](const auto& ptrValidComponent)
    {
      return ptrValidComponent.Get() == ptr;
    });

  if (it == this->mActivatedModelAnimatorPtrs.end()) { return DY_FAILURE; }

  // Erase pointer of found component.
  this->mActivatedModelAnimatorPtrs.erase(it);
  return DY_SUCCESS;
}

inline std::optional<NotNull<CDySkybox*>> MDyWorld::Impl::GetPtrMainLevelSkybox() const noexcept
{
  // If activated skybox instance is not exist, just return null value.
  if (this->mActivatedSkyboxPtrList.empty() == true) { return std::nullopt; }

  // Otherwise, always get first pointer of CDySkybox.
  return this->mActivatedSkyboxPtrList.front();
}

inline void MDyWorld::Impl::MDY_PRIVATE(BindActiveSkybox)(CDySkybox& iRefComponent)
{
  this->mActivatedSkyboxPtrList.emplace_back(DyMakeNotNull(&iRefComponent));
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(UnbindActiveSkybox)(CDySkybox& iRefComponent)
{
  // Check address. component's address is not changed unless actor is destroyed.
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mActivatedSkyboxPtrList), 
      [ptr = &iRefComponent](const auto& ptrValidComponent)
    {
      return ptrValidComponent.Get() == ptr;
    });

  if (it == this->mActivatedSkyboxPtrList.end()) { return DY_FAILURE; }

  // Erase pointer of found component.
  this->mActivatedSkyboxPtrList.erase(it);
  return DY_SUCCESS;
}

inline FDyWorldUIContainer& MDyWorld::Impl::MDY_PRIVATE(GetUiContainer)() noexcept
{
  return this->mUiInstanceContainer;
}

inline void MDyWorld::Impl::MDY_PRIVATE(BindActiveUiObject)(FDyUiWidget& iRefWidget)
{
  this->mUiInstanceContainer.BindActiveUiObject(iRefWidget);
}

inline EDySuccess MDyWorld::Impl::MDY_PRIVATE(UnbindActiveUiObject)(FDyUiWidget& iRefWidget)
{
  return this->mUiInstanceContainer.UnbindActiveUiObject(iRefWidget);
}

#ifdef false
void MDyWorld::Impl::pfUnenrollActiveScript(_MIN_ TI32 index) noexcept
{
  MDY_ASSERT_MSG(index < this->mActivatedScripts.size(), "index must be smaller than this->mActivatedScripts.size().");

  this->mActivatedScripts[index] = MDY_INITIALIZE_NULL;
  this->mErasionScriptCandidateList.emplace_back(index);
}

TI32 MDyWorld::Impl::pfEnrollActiveScript(_MIN_ const NotNull<CDyScript*>& pawnRawPtr) noexcept
{
  this->mActivatedScripts.emplace_back(pawnRawPtr);
  return static_cast<TI32>(this->mActivatedScripts.size()) - 1;
}
#endif

inline TI32 MDyWorld::Impl::pfEnrollActiveModelRenderer(CDyModelRenderer& validComponent) noexcept
{
  this->mActivatedModelRenderers.emplace_back(&validComponent);
  return static_cast<TI32>(this->mActivatedModelRenderers.size()) - 1;
}

inline TI32 MDyWorld::Impl::pfEnrollActiveCamera(CDyCamera& validComponent) noexcept
{
  this->mActivatedOnRenderingCameras.emplace_back(&validComponent);
  return static_cast<TI32>(this->mActivatedOnRenderingCameras.size()) - 1;
}

} /// ::dy namespace 

#endif /// GUARD_DY_MANAGEMENT_WORLDMANAGER_IMPL_INL