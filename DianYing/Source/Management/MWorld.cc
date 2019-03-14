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
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/MScript.h>
#include <Dy/Management/MPhysics.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Resource/Type/EDyScope.h>
#include <Dy/Core/DyEngine.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyModelAnimator.h>
#include <Dy/Element/Type/DDyUiBinder.h>
#include <Dy/Element/Type/PDyActorCreationDescriptor.h>
#include <Dy/Element/Level.h>
#include <Dy/Management/Internal/World/FDyWorldUIContainer.h>

//!
//! Implementation
//!

namespace dy
{

class MDyWorld::Impl final : public IDyUpdatable
{
public:
  /// @brief Update scene structures prior to dive in scene objects tree hierarchy.
  /// Level transition will be executed maybe.
  void Update(TF32 dt) override final;

  /// @brief Update valid objects. this function must be called after this->Update().
  /// @param dt Delta time
  void UpdateObjects(TF32 dt);
  /// @brief Update animator component (pre-render phase).
  void UpdateAnimator(TF32 dt);

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag.
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

  /// @brief Create actor.
  DDyActorBinder CreateActor(
    const std::string& iActorName, 
    const std::string& iPrefabName, 
    const DDyTransform& iSpawnTransform,
    FDyActor* iPtrParent = MDY_INITIALIZE_NULL,
    const std::string& iObjectTag = MDY_INITIALIZE_EMPTYSTR,
    bool iDoSweep = false);

  /// @brief Destroy Actor
  void DestroyActor(FDyActor& ioRefActor);

  /// @brief Create UI Object. 
  /// If ui object wraps over exist Ui Object, but flag is true, create anyway with auto-generated UI name.
  std::optional<DDyUiBinder> CreateUiObject(
    const std::string& iUiName,
    const std::string& iNewCustomizedName = "",
    bool isForcedZOrder = false,
    TU32 ZOrder = 0);
  /// @brief Get UI Object as binder.
  std::optional<DDyUiBinder> GetUiObject(const std::string& iUiName);
  /// @brief Destroy UI Object.
  EDySuccess DestoryUiObject(DDyUiBinder& ioRefUi);
  /// @brief Destroy UI Object.
  EDySuccess DestroyUiObject(const std::string& iUiName);

  /// @brief Get `Focused` and `Main` camera of level.
  CDyCamera* GetPtrMainLevelCamera() const noexcept;
  /// @brief Get binded level camra counts.
  TI32 GetFocusedCameraCount() const noexcept;
  /// @brief
  /// @param  index
  /// @return
  std::optional<CDyCamera*> GetFocusedCameraValidReference(const TI32 index) const noexcept;

  /// @brief  Ask it for opening level with levelName next frame.
  /// @param  levelName valid level meta information name
  /// @return If level is created successfully, return true or false.
  EDySuccess OpenLevel(const std::string& levelName);
  /// @brief Check we must to transit to next level.
  bool IsNeedTransitNextLevel() const noexcept;

  /// @brief Open first level. This function must be called in first-loading level.
  EDySuccess MDY_PRIVATE(OpenFirstLevel)();
  /// @brief Try Remove level. If level is not exist, just return DY_FAILURE.
  EDySuccess MDY_PRIVATE(RemoveLevel)();
  /// @brief Populate next level resource. If next level specifier is not exist, do nothing and return DY_FAILURE.
  EDySuccess MDY_PRIVATE(PopulateNextLevelResources)();

  /// @brief 
  void MDY_PRIVATE(BuildNextLevel)();
  /// @brief
  EDySuccess MDY_PRIVATE(TransitionToNextLevel)();

  /// @brief  Check scene is initialized and valid.
  /// @return
  bool IsLevelPresentValid() const noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  FDyLevel& GetValidLevelReference() noexcept;

  /// @brief Try create debug ui layout to screen as highest priority. (10xx) \n
  /// If debug ui is already spawned, just return DY_FAILURE, or DY_SUCCESS
  EDySuccess TryCreateDebugUi();
  /// @brief Check debug Ui is now on exist.
  bool IsDebugUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already removed, just return DY_FAILURE
  EDySuccess TryRemoveDebugUi();

  /// @brief Try create loading ui layout. \n
  /// If Loading UI Widget meta information is not exist, just return DY_FAILURE doing nothing.
  EDySuccess  TryCreateLoadingUi();
  /// @brief Check loading ui is now on exist.
  bool        IsLoadingUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already remove or not exist, return DY_FAILURE.
  EDySuccess  TryRemoveLoadingUi();

  /// @brief
  bool CheckCreationActorExist() const noexcept;
  /// @brief
  void TryCreateActorsOfCreationActorList() noexcept;
  /// @brief
  void CleanCreationActorList() noexcept;
  /// @brief Check Gc-candidate actor list is not empty and exist something.
  bool CheckIsGcActorExist() const noexcept;
  /// @brief Try remove actor gc list anyway. \n
  /// If there is something to do actors in GC list, call something to do prior to this.
  void MDY_PRIVATE(TryRemoveActorGCList)() noexcept;

  /// @brief Try detach active model renderer.
  EDySuccess MDY_PRIVATE(TryDetachActiveModelRenderer)(CDyModelRenderer* iPtrRenderer);

  /// @brief Bind (Enroll) active model animator component. This function must be called in `CDyModelAnimator`.
  void MDY_PRIVATE(BindActiveModelAnimator)(CDyModelAnimator& iRefComponent);
  /// @brief Unbind deactivated model animator component This function must be called in `CDyModelAnimator`.
  EDySuccess MDY_PRIVATE(UnbindActiveModelAnimator)(CDyModelAnimator& iRefComponent);

  /// @brief Try get valid pointer instance (not-null) of CDySkybox target to be rendered on renderer.
  std::optional<NotNull<CDySkybox*>> GetPtrMainLevelSkybox() const noexcept;
  /// @brief Bind (Enroll) active skybox component. This function must be called in `CDySkybox`.
  void MDY_PRIVATE(BindActiveSkybox)(CDySkybox& iRefComponent);
  /// @brief Unbind deactivated skybox component. This function must be called in `CDySkybox`.
  EDySuccess MDY_PRIVATE(UnbindActiveSkybox)(CDySkybox& iRefComponent);

  /// @brief Get Internal World UI container list.
  FDyWorldUIContainer& MDY_PRIVATE(GetUiContainer)() noexcept;
  /// @brief Bind (Enroll) active Ui object (widget) into internal container.
  /// This function must be called in `FDyUiWidget`.
  void MDY_PRIVATE(BindActiveUiObject)(FDyUiWidget& iRefWidget);
  /// @brief Unbind deactivated ui object component. This function must be called in `FDyUiWidget`.
  EDySuccess MDY_PRIVATE(UnbindActiveUiObject)(FDyUiWidget& iRefWidget);

  /// @brief Set level transition. Flag will be set and all dependent processing will be halted.
  /// Until level is translated.
  void SetLevelTransition(const std::string& iSpecifier);

  /// @brief This function must be called in MDyWorld::Update() function.
  /// Wipe out deactivated components from activated component lists.
  void pGcAcitvatedComponents();

  /// @brief  Move FDyActor instance to gc.
  /// @param  actorRawPtr Valid FDyActor pointer instance.
  void pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept;

  /// @brief
  /// @param  index
  void pfUnenrollActiveModelRenderer(TI32 index) noexcept;

  /// @brief
  /// @param  index
  void pfUnenrollActiveCamera(TI32& index) noexcept;

  /// @brief
  /// @param  validComponent
  TI32 pfEnrollActiveModelRenderer(CDyModelRenderer& validComponent) noexcept;

  /// @brief
  /// @param  validComponent
  TI32 pfEnrollActiveCamera(CDyCamera& validComponent) noexcept;

private:
  std::string mNextLevelName     = MDY_INITIALIZE_EMPTYSTR;
  std::string mPresentLevelName  = MDY_INITIALIZE_EMPTYSTR;
  std::string mPreviousLevelName = MDY_INITIALIZE_EMPTYSTR;

  /// Present valid level instance.
  std::unique_ptr<FDyLevel> mLevel = nullptr;

  /// Erasion (activated) script candidate list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32> mErasionScriptCandidateList = {};

  /// Activated CDyModelRenderer component list.
  /// this list must not be invalidated when iterating list, but except for unenrolling.
  std::vector<CDyModelRenderer*>  mActivatedModelRenderers = {};
  /// Erasion (activated) model rendrerer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32> mErasionModelRenderersCandidateList = {};

  /// Valid camera ptr which to be used rendering sequence.
  /// this list must not be invalidated when interating list, but except for unenrolling.
  std::vector<CDyCamera*>   mActivatedOnRenderingCameras = {};
  /// Erasion (activated) model renderer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32> mErasionCamerasCandidateList = {};

  /// Valid animator ptr which to be used update animation sequence.
  /// This list is not invalidated because animation updating is not change list order.
  std::vector<NotNull<CDyModelAnimator*>> mActivatedModelAnimatorPtrs = {};

  /// Valid & Activated skybox component pointer list. 
  std::vector<NotNull<CDySkybox*>> mActivatedSkyboxPtrList {};

  /// @brief Action creation descriptor list for present level. \n
  /// This list must be processed and cleaned each frame prior to update of logic.
  std::vector<std::unique_ptr<PDyActorCreationDescriptor>> mActorCreationDescList = {};
  /// Garbage collection actor instance list.
  std::vector<std::unique_ptr<FDyActor>> mGCedActorList = {};

  /// @brief UI Instance container.
  FDyWorldUIContainer mUiInstanceContainer;

  bool mIsNeedTransitNextLevel = false;
};

} /// ::dy namespace
#include <Dy/Management/Inline/MWorldImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MDyWorld::pfInitialize()
{
  this->mInternal = new Impl();
  return DY_SUCCESS;
}

EDySuccess MDyWorld::pfRelease()
{
  delete this->mInternal; this->mInternal = nullptr;
  return DY_SUCCESS;
}

void MDyWorld::Update(TF32 iDt)         { this->mInternal->Update(iDt); }
void MDyWorld::UpdateObjects(TF32 iDt)  { this->mInternal->UpdateObjects(iDt); }
void MDyWorld::UpdateAnimator(TF32 iDt) { this->mInternal->UpdateAnimator(iDt); }

std::vector<NotNull<FDyActor*>> 
MDyWorld::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithTag(iTagSpecifier);
}

std::vector<NotNull<FDyActor*>>
MDyWorld::GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithTagRecursive(iTagSpecifier);
}

std::vector<NotNull<FDyActor*>> 
MDyWorld::GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithName(iNameSpecifier);
}

std::vector<NotNull<FDyActor*>> 
MDyWorld::GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithNameRecursive(iNameSpecifier);
}

FDyActor* MDyWorld::GetActorWithObjectId(TU32 iObjectId) noexcept
{
  return this->mInternal->GetActorWithObjectId(iObjectId);
}

DDyActorBinder MDyWorld::CreateActor(
  const std::string& iActorName, 
  const std::string& iPrefabName,
  const DDyTransform& iSpawnTransform, 
  FDyActor* iPtrParent, 
  const std::string& iObjectTag, 
  bool iDoSweep)
{
  return this->mInternal->CreateActor(iActorName, iPrefabName, iSpawnTransform, iPtrParent, iObjectTag, iDoSweep);
}

void MDyWorld::DestroyActor(_MIN_ FDyActor& iRefActor)
{
  return this->mInternal->DestroyActor(iRefActor);
}

std::optional<DDyUiBinder> 
MDyWorld::CreateUiObject(
  const std::string& iUiName, 
  const std::string& iNewCustomizedName, 
  bool isForcedZOrder,
  TU32 ZOrder)
{
  return this->mInternal->CreateUiObject(iUiName, iNewCustomizedName, isForcedZOrder, ZOrder);
}

std::optional<DDyUiBinder> MDyWorld::GetUiObject(const std::string& iUiName)
{
  return this->mInternal->GetUiObject(iUiName);
}

EDySuccess MDyWorld::DestoryUiObject(DDyUiBinder& ioUiBInder)
{
  return this->mInternal->DestoryUiObject(ioUiBInder);
}

EDySuccess MDyWorld::DestroyUiObject(const std::string& iUiName)
{
  return this->mInternal->DestroyUiObject(iUiName);
}

CDyCamera* MDyWorld::GetPtrMainLevelCamera() const noexcept
{
  return this->mInternal->GetPtrMainLevelCamera();
}

TI32 MDyWorld::GetFocusedCameraCount() const noexcept
{
  return this->mInternal->GetFocusedCameraCount();
}

std::optional<CDyCamera*> MDyWorld::GetFocusedCameraValidReference(const TI32 index) const noexcept
{
  return this->mInternal->GetFocusedCameraValidReference(index);
}

EDySuccess MDyWorld::OpenLevel(const std::string& levelName)
{
  return this->mInternal->OpenLevel(levelName);
}

bool MDyWorld::IsNeedTransitNextLevel() const noexcept
{
  return this->mInternal->IsNeedTransitNextLevel();
}

EDySuccess MDyWorld::MDY_PRIVATE(OpenFirstLevel)()
{
  return this->mInternal->MDY_PRIVATE(OpenFirstLevel)();
}

EDySuccess MDyWorld::MDY_PRIVATE(RemoveLevel)()
{
  return this->mInternal->MDY_PRIVATE(RemoveLevel)();
}

EDySuccess MDyWorld::MDY_PRIVATE(PopulateNextLevelResources)()
{
  return this->mInternal->MDY_PRIVATE(PopulateNextLevelResources)();
}

void MDyWorld::MDY_PRIVATE(BuildNextLevel)()
{
  this->mInternal->__BuildNextLevel();
}

EDySuccess MDyWorld::MDY_PRIVATE(TransitionToNextLevel)() { return this->mInternal->__TransitionToNextLevel(); }

bool MDyWorld::IsLevelPresentValid() const noexcept { return this->mInternal->IsLevelPresentValid(); }
FDyLevel& MDyWorld::GetValidLevelReference() noexcept { return this->mInternal->GetValidLevelReference(); }

EDySuccess MDyWorld::TryCreateDebugUi()        { return this->mInternal->TryCreateDebugUi(); }
bool MDyWorld::IsDebugUiExist() const noexcept { return this->mInternal->IsDebugUiExist(); }
EDySuccess MDyWorld::TryRemoveDebugUi()        { return this->mInternal->TryRemoveDebugUi(); }

EDySuccess MDyWorld::TryCreateLoadingUi()        { return this->mInternal->TryCreateLoadingUi(); }
bool MDyWorld::IsLoadingUiExist() const noexcept { return this->mInternal->IsLoadingUiExist(); } 
EDySuccess MDyWorld::TryRemoveLoadingUi()        { return this->mInternal->TryRemoveLoadingUi(); }

bool MDyWorld::CheckCreationActorExist() const noexcept { return this->mInternal->CheckCreationActorExist(); }

void MDyWorld::TryCreateActorsOfCreationActorList() noexcept
{
  this->mInternal->TryCreateActorsOfCreationActorList();
}

void MDyWorld::CleanCreationActorList() noexcept
{
  this->mInternal->CleanCreationActorList();
}

bool MDyWorld::CheckIsGcActorExist() const noexcept
{
  return this->mInternal->CheckIsGcActorExist();
}

void MDyWorld::MDY_PRIVATE(TryRemoveActorGCList)() noexcept
{
  this->mInternal->__TryRemoveActorGCList();
}

void MDyWorld::pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept
{
  this->mInternal->pfMoveActorToGc(actorRawPtr);
}

void MDyWorld::pfUnenrollActiveModelRenderer(TI32 index) noexcept
{
  this->mInternal->pfUnenrollActiveModelRenderer(index);
}

void MDyWorld::pfUnenrollActiveCamera(TI32& index) noexcept
{
  this->mInternal->pfUnenrollActiveCamera(index);
}

EDySuccess MDyWorld::MDY_PRIVATE(TryDetachActiveModelRenderer)(CDyModelRenderer* iPtrRenderer)
{
  return this->mInternal->__TryDetachActiveModelRenderer(iPtrRenderer);
}

void MDyWorld::__BindActiveModelAnimator(CDyModelAnimator& iRefComponent)
{
  this->mInternal->__BindActiveModelAnimator(iRefComponent);
}

EDySuccess MDyWorld::__UnbindActiveModelAnimator(CDyModelAnimator& iRefComponent)
{
  return this->mInternal->__UnbindActiveModelAnimator(iRefComponent);
}

std::optional<NotNull<CDySkybox*>> MDyWorld::GetPtrMainLevelSkybox() const noexcept
{
  return this->mInternal->GetPtrMainLevelSkybox();
}

void MDyWorld::__BindActiveSkybox(CDySkybox& iRefComponent)
{
  this->mInternal->__BindActiveSkybox(iRefComponent);
}

EDySuccess MDyWorld::__UnbindActiveSkybox(CDySkybox& iRefComponent)
{
  return this->mInternal->__UnbindActiveSkybox(iRefComponent);
}

FDyWorldUIContainer& MDyWorld::MDY_PRIVATE(GetUiContainer)() noexcept
{
  return this->mInternal->__GetUiContainer();
}

void MDyWorld::MDY_PRIVATE(BindActiveUiObject)(FDyUiWidget& iRefWidget)
{
  this->mInternal->__BindActiveUiObject(iRefWidget);
}

EDySuccess MDyWorld::MDY_PRIVATE(UnbindActiveUiObject)(FDyUiWidget& iRefWidget)
{
  return this->mInternal->__UnbindActiveUiObject(iRefWidget);
}

TI32 MDyWorld::pfEnrollActiveModelRenderer(CDyModelRenderer& validComponent) noexcept
{
  return this->mInternal->pfEnrollActiveModelRenderer(validComponent);
}

TI32 MDyWorld::pfEnrollActiveCamera(CDyCamera& validComponent) noexcept
{
  return this->mInternal->pfEnrollActiveCamera(validComponent);
}

} /// ::dy namespace