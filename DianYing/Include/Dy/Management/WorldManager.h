#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Internal/World/FDyWorldUIContainer.h>
#include <Dy/Element/Type/DDyActorBinder.h>
#include <Dy/Element/Type/DDyUiBinder.h>
#include <Dy/Element/Type/PDyActorCreationDescriptor.h>
#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Element/Level.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyModelRenderer;
class CDyModelAnimator;
class CDyLegacyCamera;
class CDyCamera;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class MDyWorld
/// @brief Manages scene and helping access each scene's unique properties.
///
class MDyWorld final : public IDySingleton<MDyWorld>, public IDyUpdatable
{
  MDY_SINGLETON_DERIVED(MDyWorld);
  MDY_SINGLETON_PROPERTIES(MDyWorld);
public:
  /// @brief Update scene structures prior to dive in scene objects tree hierarchy.
  /// Level transition will be executed maybe.
  /// @param dt
  void Update(_MIN_ float dt) override final;

  /// @brief Update valid objects. this function must be called after this->Update().
  /// @param dt Delta time
  void UpdateObjects(_MIN_ float dt);
  /// @brief Update animator component (pre-render phase).
  void UpdateAnimator(_MIN_ TF32 dt);

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTag(_MIN_ const std::string& iTagSpecifier) const noexcept;

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept;

  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithName(_MIN_ const std::string& iNameSpecifier) const noexcept; 

  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithNameRecursive(_MIN_ const std::string& iNameSpecifier) const noexcept; 

  /// @brief Create actor.
  DDyActorBinder CreateActor(
      _MIN_ const std::string& iActorName, 
      _MIN_ const std::string& iPrefabName, 
      _MIN_ const DDyTransform& iSpawnTransform,
      _MIN_ FDyActor* iPtrParent = MDY_INITIALIZE_NULL,
      _MIN_ const std::string& iObjectTag = MDY_INITIALIZE_EMPTYSTR,
      _MIN_ bool iDoSweep = false);

  /// @brief Destroy Actor
  void DestroyActor(_MINOUT_ FDyActor& iRefActor);

  /// @brief Create UI Object. 
  /// If ui object wraps over exist Ui Object, but flag is true, create anyway with auto-generated UI name.
  std::optional<DDyUiBinder> CreateUiObject(
      _MIN_ const std::string& iUiName,
      _MIN_ const std::string& iNewCustomizedName = "",
      _MIN_ bool isForcedZOrder = false,
      _MIN_ TU32 ZOrder = 0);

  /// @brief Destroy UI Object.
  EDySuccess DestoryUiObject(_MINOUT_ DDyUiBinder& iRefUi);
  /// @brief Destroy UI Object.
  EDySuccess DestroyUiObject(_MIN_ const std::string& iUiName);

  /// @brief Get `Focused` and `Main` camera of level.
  MDY_NODISCARD CDyCamera* GetPtrMainLevelCamera() const noexcept;
  /// @brief Get binded level camra counts.
  MDY_NODISCARD TI32 GetFocusedCameraCount() const noexcept;
  /// @brief
  /// @param  index
  /// @return
  MDY_NODISCARD std::optional<CDyCamera*> GetFocusedCameraValidReference(_MIN_ const TI32 index) const noexcept;

  /// @brief  Ask it for opening level with levelName next frame.
  /// @param  levelName valid level meta information name
  /// @return If level is created successfully, return true or false.
  EDySuccess OpenLevel(const std::string& levelName);
  /// @brief Check we must to transit to next level.
  MDY_NODISCARD bool IsNeedTransitNextLevel() const noexcept;

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
  MDY_NODISCARD bool IsLevelPresentValid() const noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  MDY_NODISCARD FDyLevel& GetValidLevelReference() noexcept;

  /// @brief Try create debug ui layout to screen as highest priority. (10xx) \n
  /// If debug ui is already spawned, just return DY_FAILURE, or DY_SUCCESS
  MDY_NODISCARD EDySuccess TryCreateDebugUi();
  /// @brief Check debug Ui is now on exist.
  MDY_NODISCARD bool IsDebugUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already removed, just return DY_FAILURE
  MDY_NODISCARD EDySuccess TryRemoveDebugUi();

  /// @brief Try create loading ui layout. \n
  /// If Loading UI Widget meta information is not exist, just return DY_FAILURE doing nothing.
  MDY_NODISCARD EDySuccess  TryCreateLoadingUi();
  /// @brief Check loading ui is now on exist.
  MDY_NODISCARD bool        IsLoadingUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already remove or not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess  TryRemoveLoadingUi();

  /// @brief
  MDY_NODISCARD bool CheckCreationActorExist() const noexcept;
  /// @brief
  void TryCreateActorsOfCreationActorList() noexcept;
  /// @brief
  void CleanCreationActorList() noexcept;
  /// @brief Check Gc-candidate actor list is not empty and exist something.
  MDY_NODISCARD bool CheckIsGcActorExist() const noexcept;
  /// @brief Try remove actor gc list anyway. \n
  /// If there is something to do actors in GC list, call something to do prior to this.
  void MDY_PRIVATE(TryRemoveActorGCList)() noexcept;

  /// @brief Try detach active model renderer.
  EDySuccess MDY_PRIVATE(TryDetachActiveModelRenderer)(_MIN_ CDyModelRenderer* iPtrRenderer);

  /// @brief Bind (Enroll) active model animator component. This function must be called in `CDyModelAnimator`.
  void MDY_PRIVATE(BindActiveModelAnimator)(_MIN_ CDyModelAnimator& iRefComponent);
  /// @brief Unbind deactivated model animator component This function must be called in `CDyModelAnimator`.
  EDySuccess MDY_PRIVATE(UnbindActiveModelAnimator)(_MIN_ CDyModelAnimator& iRefComponent);

  /// @brief Try get valid pointer instance (not-null) of CDySkybox target to be rendered on renderer.
  MDY_NODISCARD std::optional<NotNull<CDySkybox*>> GetPtrMainLevelSkybox() const noexcept;
  /// @brief Bind (Enroll) active skybox component. This function must be called in `CDySkybox`.
  void MDY_PRIVATE(BindActiveSkybox)(_MIN_ CDySkybox& iRefComponent);
  /// @brief Unbind deactivated skybox component. This function must be called in `CDySkybox`.
  EDySuccess MDY_PRIVATE(UnbindActiveSkybox)(_MIN_ CDySkybox& iRefComponent);

  /// @brief Get Internal World UI container list.
  FDyWorldUIContainer& MDY_PRIVATE(GetUiContainer)() noexcept;
  /// @brief Bind (Enroll) active Ui object (widget) into internal container.
  /// This function must be called in `FDyUiWidget`.
  void MDY_PRIVATE(BindActiveUiObject)(_MIN_ FDyUiWidget& iRefWidget);
  /// @brief Unbind deactivated ui object component. This function must be called in `FDyUiWidget`.
  EDySuccess MDY_PRIVATE(UnbindActiveUiObject)(_MIN_ FDyUiWidget& iRefWidget);

#ifdef false
  ///
  /// @brief  Spawn actor into present scene.
  /// @tparam TActorType
  /// @tparam TParameterType
  ///
  /// @throw  If mLevel is empty, throw runtime_error.
  /// @return Valid actor type.
  ///
  template <typename TActorType, typename TParameterType>
  [[maybe_unused]]
  NotNull<TActorType*> SpawnActor(const std::string& actorName,
                                  const DDyVector3& position, const DDyVector3& rotation, const DDyVector3& scale,
                                  const TParameterType& desc)
  {
    static_assert(std::is_object_v<TActorType>,             "TActorType must be object type, not cv-qualifier and pointer, reference either.");
    static_assert(std::is_base_of_v<FDyActor, TActorType>,  "TActorType must be derivation of FDyActor class type.");
    if (!this->mLevel)
    {
      throw std::runtime_error("mLevel is not initialized and empty.");
    }

    return mLevel->pSpawnActor<TActorType, TParameterType>(actorName, position, rotation, scale, desc);
  }

  ///
  /// @brief  Destroy actor at present scene, but not actually destroy instantly, instead moving heap instance ptr
  /// to garbage collection list to remove it actually at next frame start.
  /// @param  actorName
  /// @return If removed return true but false.
  ///
  [[nodiscard]] EDySuccess DestroyActor(const std::string& actorName);

  ///
  /// @brief
  /// @param
  /// @return
  ///
  [[nodiscard]] EDySuccess DestroyActor(const NotNull<const FDyActor*>& actorRawPtr);
#endif

private:
  /// @brief Set level transition. Flag will be set and all dependent processing will be halted.
  /// Until level is translated.
  void SetLevelTransition(_MIN_ const std::string& iSpecifier);

  /// @brief This function must be called in MDyWorld::Update() function.
  /// Wipe out deactivated components from activated component lists.
  void pGcAcitvatedComponents();

  ///
  /// @brief  Move FDyActor instance to gc.
  /// @param  actorRawPtr Valid FDyActor pointer instance.
  ///
  void pfMoveActorToGc(_MIN_ NotNull<FDyActor*> actorRawPtr) noexcept;

  ///
  /// @brief  Unenroll activated FDyPawn raw pointer from list.
  /// raw pointer value will be nullptr at first, and erase actual memory space at next frame.
  /// erasion pawn candidate index list will be cleared when next frame starts.
  ///
  /// @param  index Index to erase.
  ///
  void pfUnenrollActiveScript(_MIN_ TI32 index) noexcept;

  ///
  /// @brief
  /// @param  index
  /// @return
  /// @TODO SCRIPT THIS!
  ///
  void pfUnenrollActiveModelRenderer(_MIN_ TI32 index) noexcept;

  ///
  /// @brief
  /// @param  index
  /// @return
  /// @TODO SCRIPT THIS!
  ///
  void pfUnenrollActiveCamera(_MIO_ TI32& index) noexcept;

  /// @brief
  /// @param  validComponent
  /// @return
  /// @TODO SCRIPT THIS!
  MDY_NODISCARD TI32 pfEnrollActiveModelRenderer(_MIN_ CDyModelRenderer& validComponent) noexcept;

  /// @brief
  /// @param  validComponent
  /// @return
  /// @TODO SCRIPT THIS!
  MDY_NODISCARD TI32 pfEnrollActiveCamera(_MIN_ CDyCamera& validComponent) noexcept;

  std::string               mNextLevelName      = MDY_INITIALIZE_EMPTYSTR;
  std::string               mPresentLevelName   = MDY_INITIALIZE_EMPTYSTR;
  std::string               mPreviousLevelName  = MDY_INITIALIZE_EMPTYSTR;

  /// Present valid level instance.
  std::unique_ptr<FDyLevel> mLevel              = nullptr;

  /// Erasion (activated) script candidate list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32>         mErasionScriptCandidateList = {};

  /// Activated CDyModelRenderer component list.
  /// this list must not be invalidated when iterating list, but except for unenrolling.
  std::vector<CDyModelRenderer*>  mActivatedModelRenderers = {};
  /// Erasion (activated) model rendrerer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32>               mErasionModelRenderersCandidateList = {};

  /// Valid camera ptr which to be used rendering sequence.
  /// this list must not be invalidated when interating list, but except for unenrolling.
  std::vector<CDyCamera*>   mActivatedOnRenderingCameras = {};
  /// Erasion (activated) model renderer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32>         mErasionCamerasCandidateList = {};

  /// Valid animator ptr which to be used update animation sequence.
  /// This list is not invalidated because animation updating is not change list order.
  std::vector<NotNull<CDyModelAnimator*>> mActivatedModelAnimatorPtrs = {};

  /// Valid & Activated skybox component pointer list. 
  std::vector<NotNull<CDySkybox*>> mActivatedSkyboxPtrList {};

  /// @brief Action creation descriptor list for present level. \n
  /// This list must be processed and cleaned each frame prior to update of logic.
  std::vector<std::unique_ptr<PDyActorCreationDescriptor>> 
  mActorCreationDescList = {};
  /// Garbage collection actor instance list.
  std::vector<std::unique_ptr<FDyActor>> mGCedActorList = {};

  /// @brief UI Instance container.
  FDyWorldUIContainer mUiInstanceContainer;

  bool mIsNeedTransitNextLevel = false;

  friend class CDyLegacyCamera;
  friend class FDyLevel;
  friend class CDyScript;
  friend class CDyModelRenderer;
  friend class CDyCamera;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H