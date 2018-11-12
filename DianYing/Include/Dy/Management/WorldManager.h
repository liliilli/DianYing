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
  /// Scene transition will be executed maybe.
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
  /// @deprecated This function is deprecated since v0.0.1. Use
  /// @brief  Return main camera ptr.
  /// @return
  ///
  MDY_DEPRECATED("v0.0.1", GetMainCameraPtr, pfEnrollActiveCamera)
  MDY_NODISCARD CDyLegacyCamera* GetMainCameraPtr() const noexcept;

  ///
  /// @deprecated This function is deprecated since v0.0.1. Use
  /// @brief  Check if main camera is binded so be able to focused by scene.
  /// @return
  ///
  MDY_DEPRECATED("v0.0.1", IsMainCameraFocused, None)
  MDY_NODISCARD FORCEINLINE bool IsMainCameraFocused() const noexcept
  {
    return MDY_CHECK_ISNOTNULL(this->mValidMainCameraPtr);
  }

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD FORCEINLINE TI32 GetFocusedCameraCount() const noexcept
  {
    return static_cast<TI32>(this->mActivatedOnRenderingCameras.size());
  }

  ///
  /// @brief
  /// @param  index
  /// @return
  ///
  MDY_NODISCARD std::optional<CDyCamera*> GetFocusedCameraValidReference(_MIN_ const TI32 index) const noexcept;

  ///
  /// @brief  Ask it for opening level with levelName next frame.
  /// @param  levelName valid level meta information name
  /// @return If level is created successfully, return true or false.
  ///
  EDySuccess OpenLevel(const std::string& levelName);

  ///
  /// @brief  Check scene is initialized and valid.
  /// @return
  ///
  MDY_NODISCARD bool IsLevelPresentValid() const noexcept;

  ///
  /// @brief  Get valid level reference.
  /// @return Valid level reference. when level is not specified, unexpected behaviour.
  ///
  MDY_NODISCARD FDyLevel& GetValidLevelReference() noexcept;

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
  ///
  /// @brief This function must be called in MDyWorld::Update() function.
  /// Wipe out deactivated components from activated component lists.
  ///
  void pGcAcitvatedComponents();

  /// Bind valid camera to main camera and let object have focused.
  MDY_DEPRECATED("v0.0.1", pfBindFocusCamera, pfEnrollActiveCamera)
  void pfBindFocusCamera(_MIN_ CDyLegacyCamera& validCameraPtr) noexcept;

  /// Unbind main camera. this function must not be called manually, but using camera's mechanism.
  MDY_DEPRECATED("v0.0.1", pfUnbindCameraFocus, pfUnenrollActiveCamera)
  void pfUnbindCameraFocus();

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

  ///
  /// @brief  Enroll activated FDyPawn raw pointer instance to list to update.
  /// @param  pawnRawPtr FDyPawn instance to insert into activated list.
  /// @return index of pawn raw ptr. Always success.
  ///
  MDY_NODISCARD TI32 pfEnrollActiveScript(_MIN_ const NotNull<CDyScript*>& pawnRawPtr) noexcept;

  ///
  /// @brief
  /// @param  validComponent
  /// @return
  /// @TODO SCRIPT THIS!
  ///
  MDY_NODISCARD TI32 pfEnrollActiveModelRenderer(_MIN_ CDyModelRenderer& validComponent) noexcept;

  ///
  /// @brief
  /// @param  validComponent
  /// @return
  /// @TODO SCRIPT THIS!
  ///
  MDY_NODISCARD TI32 pfEnrollActiveCamera(_MIN_ CDyCamera& validComponent) noexcept;

  /// Main Camera Ptr of present scene.
  CDyLegacyCamera*                mValidMainCameraPtr = nullptr;
  std::vector<CDyLegacyCamera*>   mValidSubCameraPtrs = {};

  std::string               mNextLevelName      = MDY_INITILAIZE_EMPTYSTR;
  std::string               mPresentLevelName   = MDY_INITILAIZE_EMPTYSTR;
  std::string               mPreviousLevelName  = MDY_INITILAIZE_EMPTYSTR;

  /// Present valid level instance.
  std::unique_ptr<FDyLevel> mLevel              = nullptr;

  /// Activated CDyScript component list.
  /// this list must not be invalidated when iterating list, but except for unenrolling.
  std::vector<CDyScript*>   mActivatedScripts           = {};
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

  friend class CDyLegacyCamera;
  friend class FDyLevel;
  friend class CDyScript;
  friend class CDyModelRenderer;
  friend class CDyCamera;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H