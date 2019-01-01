#ifndef GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H
#define GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H
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

#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Internal/World/FDyWorldUIContainer.h>
#include <Dy/Element/Level.h>

//!
//! Forward declaration
//!

namespace dy
{
class CDyLegacyCamera;
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
  ///
  /// @brief Update scene structures prior to dive in scene objects tree hierarchy.
  /// Level transition will be executed maybe.
  /// @param dt
  ///
  void Update(_MIN_ float dt) override final;

  ///
  /// @brief Update valid objects. this function must be called after this->Update().
  /// @param dt Delta time
  ///
  void UpdateObjects(_MIN_ float dt);

  ///
  /// @brief
  /// @param
  ///
  void RequestDrawCall(_MIN_ float dt);

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD FORCEINLINE TI32 GetFocusedCameraCount() const noexcept
  {
    return static_cast<TI32>(this->mActivatedOnRenderingCameras.size());
  }

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTag(_MIN_ const std::string& iTagSpecifier) const noexcept;

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  MDY_NODISCARD std::vector<NotNull<FDyActor*>>
  GetAllActorsWithTagRecursive(_MIN_ const std::string& iTagSpecifier) const noexcept;
  
  /// @brief
  /// @param  index
  /// @return
  MDY_NODISCARD std::optional<CDyCamera*> GetFocusedCameraValidReference(_MIN_ const TI32 index) const noexcept;

  /// @brief  Ask it for opening level with levelName next frame.
  /// @param  levelName valid level meta information name
  /// @return If level is created successfully, return true or false.
  MDY_NODISCARD EDySuccess OpenLevel(const std::string& levelName);

  /// @brief Open first level. This function must be called in first-loading level.
  EDySuccess MDY_PRIVATE_SPECIFIER(OpenFirstLevel)();

  /// @brief Try Remove level. If level is not exist, just return DY_FAILURE.
  EDySuccess MDY_PRIVATE_SPECIFIER(RemoveLevel)();

  /// @brief Populate next level resource. If next level specifier is not exist,
  /// Do nothing and return DY_FAILURE.
  EDySuccess MDY_PRIVATE_SPECIFIER(PopulateNextLevelResources)();

  /// @brief 
  void MDY_PRIVATE_SPECIFIER(BuildNextLevel)();

  /// @brief
  EDySuccess MDY_PRIVATE_SPECIFIER(TransitionToNextLevel)();

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
  /// @brief Try draw debug ui if exist.
  void MDY_PRIVATE_SPECIFIER(TryRenderDebugUi)();

  /// @brief Try create loading ui layout. \n
  /// If Loading UI Widget meta information is not exist, just return DY_FAILURE doing nothing.
  MDY_NODISCARD EDySuccess  TryCreateLoadingUi();
  /// @brief Check loading ui is now on exist.
  MDY_NODISCARD bool        IsLoadingUiExist() const noexcept;
  /// @brief Try remove debug ui layout from screen. \n
  /// If already remove or not exist, return DY_FAILURE.
  MDY_NODISCARD EDySuccess  TryRemoveLoadingUi();
  /// @brief Try draw loading ui if exist.
  void MDY_PRIVATE_SPECIFIER(TryRenderLoadingUi)();

  /// @brief Try remove actor gc list anyway. \n
  /// If there is something to do actors in GC list, call something to do prior to this.
  void MDY_PRIVATE_SPECIFIER(TryRemoveActorGCList)() noexcept;

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

  /// Main Camera Ptr of present scene.
  CDyLegacyCamera*                mValidMainCameraPtr = nullptr;
  std::vector<CDyLegacyCamera*>   mValidSubCameraPtrs = {};

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
  std::vector<CDyCamera*>   mActivatedOnRenderingCameras  = {};
  /// Erasion (activated) model renderer list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32>         mErasionCamerasCandidateList  = {};

  /// Garbage collection actor instance list.
  std::vector<std::unique_ptr<FDyActor>> mActorGc = {};

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