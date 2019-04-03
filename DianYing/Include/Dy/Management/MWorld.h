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
#include <Dy/Element/Type/DActorBinder.h>
#include <Dy/Element/Type/DWidgetBinder.h>
#include <Dy/Element/Interface/IUpdatable.h>
#include <Dy/Helper/System/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
struct DTransform;
class CModelRenderer;
class CModelAnimator;
class CDyLegacyCamera;
class CCamera;
class FLevel;
class CSkybox;
class FWidget;
class MWorldUIContainers;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class MWorld
/// @brief Manages level and helping access each level's unique properties.
class MWorld final : public ISingleton<MWorld>, public IUpdatable
{
  DY_PIMPL_SINGELTON_DERIVED(MWorld);
  MDY_SINGLETON_PROPERTIES(MWorld);
public:
  /// @brief Update level structures prior to dive in level objects tree hierarchy.
  /// Level transition will be executed maybe.
  void Update(TF32 dt) override final;

  /// @brief Update valid objects. this function must be called after this->Update().
  /// @param dt Delta time
  void UpdateObjects(TF32 dt);
  /// @brief Update animator component (pre-render phase).
  void UpdateAnimator(TF32 dt);

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithTag(const std::string& iTagSpecifier) const noexcept;

  /// @brief Get all actors with tag. Tag must be valid. \n
  /// If iTagSpecifier is empty, this function get all actors which is not specified any tag. \n
  /// and this function search all actor of object tree from root to leaf, so might take some time.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithTagRecursive(const std::string& iTagSpecifier) const noexcept;

  /// @brief Get all actors with matched name within only one depth of level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithName(const std::string& iNameSpecifier) const noexcept; 

  /// @brief Get all actors with matched name within overall level object tree. \n
  /// If iNameSpecifier is empty, just return empty list.
  MDY_NODISCARD std::vector<NotNull<FActor*>>
  GetAllActorsWithNameRecursive(const std::string& iNameSpecifier) const noexcept; 

  /// @brief Get pointer of actor with object id.
  /// If not found, just return nullptr.
  MDY_NODISCARD FActor* GetActorWithObjectId(TU32 iObjectId) noexcept;

  /// @brief Create actor.
  DActorBinder CreateActor(
    const std::string& iActorName, 
    const std::string& iPrefabName, 
    const DTransform& iSpawnTransform,
    FActor* iPtrParent = MDY_INITIALIZE_NULL,
    const std::string& iObjectTag = MDY_INITIALIZE_EMPTYSTR,
    bool iDoSweep = false);

  /// @brief Destroy Actor
  void DestroyActor(FActor& ioRefActor);

  /// @brief Create UI Object. 
  /// If ui object wraps over exist Ui Object, but flag is true, create anyway with auto-generated UI name.
  std::optional<DWidgetBinder> CreateUiObject(
    const std::string& iUiName,
    const std::string& iNewCustomizedName = "",
    bool isForcedZOrder = false,
    TU32 ZOrder = 0);
  /// @brief Get UI Object as binder.
  MDY_NODISCARD std::optional<DWidgetBinder> GetUiObject(const std::string& iUiName);
  /// @brief Destroy UI Object.
  EDySuccess DestoryUiObject(DWidgetBinder& ioRefUi);
  /// @brief Destroy UI Object.
  EDySuccess DestroyUiObject(const std::string& iUiName);

  /// @brief Get `Focused` and `Main` camera of level.
  MDY_NODISCARD CCamera* GetPtrMainLevelCamera() const noexcept;
  /// @brief Get binded level camra counts.
  MDY_NODISCARD TI32 GetFocusedCameraCount() const noexcept;
  /// @brief
  /// @param  index
  /// @return
  MDY_NODISCARD std::optional<CCamera*> GetFocusedCameraValidReference(const TI32 index) const noexcept;

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

  /// @brief  Check level is initialized and valid.
  /// @return
  MDY_NODISCARD bool IsLevelPresentValid() const noexcept;

  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  MDY_NODISCARD FLevel& GetValidLevelReference() noexcept;

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
  MDY_NODISCARD bool IsActorCreationExist() const noexcept;
  /// @brief
  void TryCreateActorsFromCreationList() noexcept;
  /// @brief
  void CleanActorCreationList() noexcept;
  /// @brief Check Gc-candidate actor list is not empty and exist something.
  MDY_NODISCARD bool IsActorGCCandidateExist() const noexcept;
  /// @brief Try remove actor gc list anyway. \n
  /// If there is something to do actors in GC list, call something to do prior to this.
  void MDY_PRIVATE(TryRemoveActorGCList)() noexcept;

  /// @brief Try detach active model renderer.
  EDySuccess MDY_PRIVATE(TryDetachActiveModelRenderer)(CModelRenderer* iPtrRenderer);

  /// @brief Bind (Enroll) active model animator component. This function must be called in `CModelAnimator`.
  void MDY_PRIVATE(BindActiveModelAnimator)(CModelAnimator& iRefComponent);
  /// @brief Unbind deactivated model animator component This function must be called in `CModelAnimator`.
  EDySuccess MDY_PRIVATE(UnbindActiveModelAnimator)(CModelAnimator& iRefComponent);

  /// @brief Try get valid pointer instance (not-null) of CSkybox target to be rendered on renderer.
  MDY_NODISCARD std::optional<NotNull<CSkybox*>> GetPtrMainLevelSkybox() const noexcept;
  /// @brief Bind (Enroll) active skybox component. This function must be called in `CSkybox`.
  void MDY_PRIVATE(BindActiveSkybox)(CSkybox& iRefComponent);
  /// @brief Unbind deactivated skybox component. This function must be called in `CSkybox`.
  EDySuccess MDY_PRIVATE(UnbindActiveSkybox)(CSkybox& iRefComponent);

  /// @brief Get Internal World UI container list.
  MWorldUIContainers& MDY_PRIVATE(GetUiContainer)() noexcept;
  /// @brief Bind (Enroll) active Ui object (widget) into internal container.
  /// This function must be called in `FWidget`.
  void MDY_PRIVATE(BindActiveUiObject)(FWidget& iRefWidget);
  /// @brief Unbind deactivated ui object component. This function must be called in `FWidget`.
  EDySuccess MDY_PRIVATE(UnbindActiveUiObject)(FWidget& iRefWidget);

private:
  /// @brief  Move FActor instance to gc.
  /// @param  actorRawPtr Valid FActor pointer instance.
  void pfMoveActorToGc(NotNull<FActor*> actorRawPtr) noexcept;

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
  MDY_NODISCARD TI32 pfEnrollActiveModelRenderer(CModelRenderer& validComponent) noexcept;

  /// @brief
  /// @param  validComponent
  /// @return
  MDY_NODISCARD TI32 pfEnrollActiveCamera(CCamera& validComponent) noexcept;

  friend class FLevel;
  friend class CModelRenderer;
  friend class CCamera;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H