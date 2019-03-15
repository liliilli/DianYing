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
#include <Dy/Management/MWorld.h>
#include <Dy/Management/MLog.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MSetting.h>
#include <Dy/Management/MScript.h>
#include <Dy/Management/MPhysics.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/Resource/Type/EDyScope.h>
#include <Dy/Core/DyEngine.h>
#include <Dy/Component/CModelRenderer.h>
#include <Dy/Component/CCamera.h>
#include <Dy/Component/CModelAnimator.h>
#include <Dy/Element/Type/DDyUiBinder.h>
#include <Dy/Element/Type/PDyActorCreationDescriptor.h>
#include <Dy/Element/Level.h>
#include <Dy/Management/Internal/World/MWorldUIContainers.h>

//!
//! Implementation
//!

namespace dy
{

class MWorld::Impl final : public IUpdatable
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
  CCamera* GetPtrMainLevelCamera() const noexcept;
  /// @brief Get binded level camra counts.
  TI32 GetFocusedCameraCount() const noexcept;
  /// @brief
  /// @param  index
  /// @return
  std::optional<CCamera*> GetFocusedCameraValidReference(const TI32 index) const noexcept;

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
  EDySuccess MDY_PRIVATE(TryDetachActiveModelRenderer)(CModelRenderer* iPtrRenderer);

  /// @brief Bind (Enroll) active model animator component. This function must be called in `CModelAnimator`.
  void MDY_PRIVATE(BindActiveModelAnimator)(CModelAnimator& iRefComponent);
  /// @brief Unbind deactivated model animator component This function must be called in `CModelAnimator`.
  EDySuccess MDY_PRIVATE(UnbindActiveModelAnimator)(CModelAnimator& iRefComponent);

  /// @brief Try get valid pointer instance (not-null) of CDySkybox target to be rendered on renderer.
  std::optional<NotNull<CDySkybox*>> GetPtrMainLevelSkybox() const noexcept;
  /// @brief Bind (Enroll) active skybox component. This function must be called in `CDySkybox`.
  void MDY_PRIVATE(BindActiveSkybox)(CDySkybox& iRefComponent);
  /// @brief Unbind deactivated skybox component. This function must be called in `CDySkybox`.
  EDySuccess MDY_PRIVATE(UnbindActiveSkybox)(CDySkybox& iRefComponent);

  /// @brief Get Internal World UI container list.
  MWorldUIContainers& MDY_PRIVATE(GetUiContainer)() noexcept;
  /// @brief Bind (Enroll) active Ui object (widget) into internal container.
  /// This function must be called in `FDyUiWidget`.
  void MDY_PRIVATE(BindActiveUiObject)(FDyUiWidget& iRefWidget);
  /// @brief Unbind deactivated ui object component. This function must be called in `FDyUiWidget`.
  EDySuccess MDY_PRIVATE(UnbindActiveUiObject)(FDyUiWidget& iRefWidget);

  /// @brief Set level transition. Flag will be set and all dependent processing will be halted.
  /// Until level is translated.
  void SetLevelTransition(const std::string& iSpecifier);

  /// @brief This function must be called in MWorld::Update() function.
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
  TI32 pfEnrollActiveModelRenderer(CModelRenderer& validComponent) noexcept;

  /// @brief
  /// @param  validComponent
  TI32 pfEnrollActiveCamera(CCamera& validComponent) noexcept;

private:
  std::string mNextLevelName     = MDY_INITIALIZE_EMPTYSTR;
  std::string mPresentLevelName  = MDY_INITIALIZE_EMPTYSTR;
  std::string mPreviousLevelName = MDY_INITIALIZE_EMPTYSTR;

  /// Present valid level instance.
  std::unique_ptr<FDyLevel> mLevel = nullptr;

  /// Erasion (activated) script candidate list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32> mErasionScriptCandidateList = {};

  /// Activated CModelRenderer component list.
  /// this list must not be invalidated when iterating list, but except for unenrolling.
  std::vector<CModelRenderer*>  mActivatedModelRenderers = {};
  /// Erasion (activated) model rendrerer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32> mErasionModelRenderersCandidateList = {};

  /// Valid camera ptr which to be used rendering sequence.
  /// this list must not be invalidated when interating list, but except for unenrolling.
  std::vector<CCamera*>   mActivatedOnRenderingCameras = {};
  /// Erasion (activated) model renderer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32> mErasionCamerasCandidateList = {};

  /// Valid animator ptr which to be used update animation sequence.
  /// This list is not invalidated because animation updating is not change list order.
  std::vector<NotNull<CModelAnimator*>> mActivatedModelAnimatorPtrs = {};

  /// Valid & Activated skybox component pointer list. 
  std::vector<NotNull<CDySkybox*>> mActivatedSkyboxPtrList {};

  /// @brief Action creation descriptor list for present level. \n
  /// This list must be processed and cleaned each frame prior to update of logic.
  std::vector<std::unique_ptr<PDyActorCreationDescriptor>> mActorCreationDescList = {};
  /// Garbage collection actor instance list.
  std::vector<std::unique_ptr<FDyActor>> mGCedActorList = {};

  /// @brief UI Instance container.
  MWorldUIContainers mUiInstanceContainer;

  bool mIsNeedTransitNextLevel = false;
};

} /// ::dy namespace
#include <Dy/Management/Inline/MWorldImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MWorld::pfInitialize()
{
  this->mInternal = new Impl();
  return DY_SUCCESS;
}

EDySuccess MWorld::pfRelease()
{
  delete this->mInternal; this->mInternal = nullptr;
  return DY_SUCCESS;
}

void MWorld::Update(TF32 iDt)         { this->mInternal->Update(iDt); }
void MWorld::UpdateObjects(TF32 iDt)  { this->mInternal->UpdateObjects(iDt); }
void MWorld::UpdateAnimator(TF32 iDt) { this->mInternal->UpdateAnimator(iDt); }

std::vector<NotNull<FDyActor*>> 
MWorld::GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithTag(iTagSpecifier);
}

std::vector<NotNull<FDyActor*>>
MWorld::GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithTagRecursive(iTagSpecifier);
}

std::vector<NotNull<FDyActor*>> 
MWorld::GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithName(iNameSpecifier);
}

std::vector<NotNull<FDyActor*>> 
MWorld::GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept
{
  return this->mInternal->GetAllActorsWithNameRecursive(iNameSpecifier);
}

FDyActor* MWorld::GetActorWithObjectId(TU32 iObjectId) noexcept
{
  return this->mInternal->GetActorWithObjectId(iObjectId);
}

DDyActorBinder MWorld::CreateActor(
  const std::string& iActorName, 
  const std::string& iPrefabName,
  const DDyTransform& iSpawnTransform, 
  FDyActor* iPtrParent, 
  const std::string& iObjectTag, 
  bool iDoSweep)
{
  return this->mInternal->CreateActor(iActorName, iPrefabName, iSpawnTransform, iPtrParent, iObjectTag, iDoSweep);
}

void MWorld::DestroyActor(_MIN_ FDyActor& iRefActor)
{
  return this->mInternal->DestroyActor(iRefActor);
}

std::optional<DDyUiBinder> 
MWorld::CreateUiObject(
  const std::string& iUiName, 
  const std::string& iNewCustomizedName, 
  bool isForcedZOrder,
  TU32 ZOrder)
{
  return this->mInternal->CreateUiObject(iUiName, iNewCustomizedName, isForcedZOrder, ZOrder);
}

std::optional<DDyUiBinder> MWorld::GetUiObject(const std::string& iUiName)
{
  return this->mInternal->GetUiObject(iUiName);
}

EDySuccess MWorld::DestoryUiObject(DDyUiBinder& ioUiBInder)
{
  return this->mInternal->DestoryUiObject(ioUiBInder);
}

EDySuccess MWorld::DestroyUiObject(const std::string& iUiName)
{
  return this->mInternal->DestroyUiObject(iUiName);
}

CCamera* MWorld::GetPtrMainLevelCamera() const noexcept
{
  return this->mInternal->GetPtrMainLevelCamera();
}

TI32 MWorld::GetFocusedCameraCount() const noexcept
{
  return this->mInternal->GetFocusedCameraCount();
}

std::optional<CCamera*> MWorld::GetFocusedCameraValidReference(const TI32 index) const noexcept
{
  return this->mInternal->GetFocusedCameraValidReference(index);
}

EDySuccess MWorld::OpenLevel(const std::string& levelName)
{
  return this->mInternal->OpenLevel(levelName);
}

bool MWorld::IsNeedTransitNextLevel() const noexcept
{
  return this->mInternal->IsNeedTransitNextLevel();
}

EDySuccess MWorld::MDY_PRIVATE(OpenFirstLevel)()
{
  return this->mInternal->MDY_PRIVATE(OpenFirstLevel)();
}

EDySuccess MWorld::MDY_PRIVATE(RemoveLevel)()
{
  return this->mInternal->MDY_PRIVATE(RemoveLevel)();
}

EDySuccess MWorld::MDY_PRIVATE(PopulateNextLevelResources)()
{
  return this->mInternal->MDY_PRIVATE(PopulateNextLevelResources)();
}

void MWorld::MDY_PRIVATE(BuildNextLevel)()
{
  this->mInternal->__BuildNextLevel();
}

EDySuccess MWorld::MDY_PRIVATE(TransitionToNextLevel)() { return this->mInternal->__TransitionToNextLevel(); }

bool MWorld::IsLevelPresentValid() const noexcept { return this->mInternal->IsLevelPresentValid(); }
FDyLevel& MWorld::GetValidLevelReference() noexcept { return this->mInternal->GetValidLevelReference(); }

EDySuccess MWorld::TryCreateDebugUi()        { return this->mInternal->TryCreateDebugUi(); }
bool MWorld::IsDebugUiExist() const noexcept { return this->mInternal->IsDebugUiExist(); }
EDySuccess MWorld::TryRemoveDebugUi()        { return this->mInternal->TryRemoveDebugUi(); }

EDySuccess MWorld::TryCreateLoadingUi()        { return this->mInternal->TryCreateLoadingUi(); }
bool MWorld::IsLoadingUiExist() const noexcept { return this->mInternal->IsLoadingUiExist(); } 
EDySuccess MWorld::TryRemoveLoadingUi()        { return this->mInternal->TryRemoveLoadingUi(); }

bool MWorld::CheckCreationActorExist() const noexcept { return this->mInternal->CheckCreationActorExist(); }

void MWorld::TryCreateActorsOfCreationActorList() noexcept
{
  this->mInternal->TryCreateActorsOfCreationActorList();
}

void MWorld::CleanCreationActorList() noexcept
{
  this->mInternal->CleanCreationActorList();
}

bool MWorld::CheckIsGcActorExist() const noexcept
{
  return this->mInternal->CheckIsGcActorExist();
}

void MWorld::MDY_PRIVATE(TryRemoveActorGCList)() noexcept
{
  this->mInternal->__TryRemoveActorGCList();
}

void MWorld::pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept
{
  this->mInternal->pfMoveActorToGc(actorRawPtr);
}

void MWorld::pfUnenrollActiveModelRenderer(TI32 index) noexcept
{
  this->mInternal->pfUnenrollActiveModelRenderer(index);
}

void MWorld::pfUnenrollActiveCamera(TI32& index) noexcept
{
  this->mInternal->pfUnenrollActiveCamera(index);
}

EDySuccess MWorld::MDY_PRIVATE(TryDetachActiveModelRenderer)(CModelRenderer* iPtrRenderer)
{
  return this->mInternal->__TryDetachActiveModelRenderer(iPtrRenderer);
}

void MWorld::__BindActiveModelAnimator(CModelAnimator& iRefComponent)
{
  this->mInternal->__BindActiveModelAnimator(iRefComponent);
}

EDySuccess MWorld::__UnbindActiveModelAnimator(CModelAnimator& iRefComponent)
{
  return this->mInternal->__UnbindActiveModelAnimator(iRefComponent);
}

std::optional<NotNull<CDySkybox*>> MWorld::GetPtrMainLevelSkybox() const noexcept
{
  return this->mInternal->GetPtrMainLevelSkybox();
}

void MWorld::__BindActiveSkybox(CDySkybox& iRefComponent)
{
  this->mInternal->__BindActiveSkybox(iRefComponent);
}

EDySuccess MWorld::__UnbindActiveSkybox(CDySkybox& iRefComponent)
{
  return this->mInternal->__UnbindActiveSkybox(iRefComponent);
}

MWorldUIContainers& MWorld::MDY_PRIVATE(GetUiContainer)() noexcept
{
  return this->mInternal->__GetUiContainer();
}

void MWorld::MDY_PRIVATE(BindActiveUiObject)(FDyUiWidget& iRefWidget)
{
  this->mInternal->__BindActiveUiObject(iRefWidget);
}

EDySuccess MWorld::MDY_PRIVATE(UnbindActiveUiObject)(FDyUiWidget& iRefWidget)
{
  return this->mInternal->__UnbindActiveUiObject(iRefWidget);
}

TI32 MWorld::pfEnrollActiveModelRenderer(CModelRenderer& validComponent) noexcept
{
  return this->mInternal->pfEnrollActiveModelRenderer(validComponent);
}

TI32 MWorld::pfEnrollActiveCamera(CCamera& validComponent) noexcept
{
  return this->mInternal->pfEnrollActiveCamera(validComponent);
}

} /// ::dy namespace