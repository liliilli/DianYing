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
#include <Dy/Element/Type/DDyActorBinder.h>
#include <Dy/Element/Type/DDyUiBinder.h>
#include <Dy/Element/Interface/IDyUpdatable.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DDyTransform;
class CDyModelRenderer;
class CDyModelAnimator;
class CDyLegacyCamera;
class CDyCamera;
class FDyLevel;
class CDySkybox;
class FDyUiWidget;
class FDyWorldUIContainer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class MDyWorld
/// @brief Manages scene and helping access each scene's unique properties.
class MDyWorld final : public IDySingleton<MDyWorld>, public IDyUpdatable
{
  MDY_SINGLETON_DERIVED(MDyWorld);
  MDY_SINGLETON_PROPERTIES(MDyWorld);
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
  MDY_NODISCARD FDyActor* GetActorWithObjectId(TU32 iObjectId) noexcept;

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
  MDY_NODISCARD std::optional<DDyUiBinder> GetUiObject(const std::string& iUiName);
  /// @brief Destroy UI Object.
  EDySuccess DestoryUiObject(DDyUiBinder& ioRefUi);
  /// @brief Destroy UI Object.
  EDySuccess DestroyUiObject(const std::string& iUiName);

  /// @brief Get `Focused` and `Main` camera of level.
  MDY_NODISCARD CDyCamera* GetPtrMainLevelCamera() const noexcept;
  /// @brief Get binded level camra counts.
  MDY_NODISCARD TI32 GetFocusedCameraCount() const noexcept;
  /// @brief
  /// @param  index
  /// @return
  MDY_NODISCARD std::optional<CDyCamera*> GetFocusedCameraValidReference(const TI32 index) const noexcept;

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
  EDySuccess MDY_PRIVATE(TryDetachActiveModelRenderer)(CDyModelRenderer* iPtrRenderer);

  /// @brief Bind (Enroll) active model animator component. This function must be called in `CDyModelAnimator`.
  void MDY_PRIVATE(BindActiveModelAnimator)(CDyModelAnimator& iRefComponent);
  /// @brief Unbind deactivated model animator component This function must be called in `CDyModelAnimator`.
  EDySuccess MDY_PRIVATE(UnbindActiveModelAnimator)(CDyModelAnimator& iRefComponent);

  /// @brief Try get valid pointer instance (not-null) of CDySkybox target to be rendered on renderer.
  MDY_NODISCARD std::optional<NotNull<CDySkybox*>> GetPtrMainLevelSkybox() const noexcept;
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

private:
  /// @brief  Move FDyActor instance to gc.
  /// @param  actorRawPtr Valid FDyActor pointer instance.
  void pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept;

  /// @brief
  /// @param  index
  /// @return
  void pfUnenrollActiveModelRenderer(TI32 index) noexcept;

  /// @brief
  /// @param  index
  /// @return
  void pfUnenrollActiveCamera(TI32& index) noexcept;

  /// @brief
  /// @param  validComponent
  /// @return
  MDY_NODISCARD TI32 pfEnrollActiveModelRenderer(CDyModelRenderer& validComponent) noexcept;

  /// @brief
  /// @param  validComponent
  /// @return
  MDY_NODISCARD TI32 pfEnrollActiveCamera(CDyCamera& validComponent) noexcept;

  class Impl; Impl* mInternal = nullptr;

  friend class FDyLevel;
  friend class CDyModelRenderer;
  friend class CDyCamera;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H