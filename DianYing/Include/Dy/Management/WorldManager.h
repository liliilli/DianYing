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
class CDyCamera;
class FDyPawn;
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
class MDyWorld final : public ISingleton<MDyWorld>, public IDyUpdatable
{
  MDY_SINGLETON_DERIVED(MDyWorld);
  MDY_SINGLETON_PROPERTIES(MDyWorld);
public:
  ///
  /// @brief Update scene structures prior to dive in scene objects tree hierarchy.
  /// Scene transition will be executed maybe.
  ///
  void Update(float dt) override final;

  ///
  /// @brief Update valid objects. this function must be called after this->Update().
  ///
  void UpdateObjects(float dt);

  ///
  /// @brief Return main camera ptr.
  ///
  [[nodiscard]] CDyCamera* GetMainCameraPtr() const noexcept;

  ///
  /// @brief Check if main camera is binded so be able to focused by scene.
  ///
  [[nodiscard]] FORCEINLINE bool IsMainCameraFocused() const noexcept
  {
    return this->mValidMainCameraPtr != nullptr;
  }

  ///
  /// @brief  Ask it for opening level with levelName next frame.
  /// @param  levelName valid level meta information name
  /// @return If level is created successfully, return true or false.
  ///
  EDySuccess OpenLevel(const std::string& levelName);

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
  /// Bind valid camera to main camera and let object have focused.
  void __pfBindFocusCamera(CDyCamera* validCameraPtr);
  /// Unbind main camera. this function must not be called manually, but using camera's mechanism.
  void __pfUnbindCameraFocus();

  ///
  /// @brief  Move FDyActor instance to gc.
  /// @param  actorRawPtr Valid FDyActor pointer instance.
  ///
  void pfMoveActorToGc(NotNull<FDyActor*> actorRawPtr) noexcept;

  ///
  /// @brief  Enroll activated FDyPawn raw pointer instance to list to update.
  /// @param  pawnRawPtr FDyPawn instance to insert into activated list.
  /// @return index of pawn raw ptr. Always success.
  ///
  [[nodiscard]] TI32 pfEnrollActivePawn(const NotNull<FDyPawn*>& pawnRawPtr) noexcept;

  ///
  /// @brief  Unenroll activated FDyPawn raw pointer from list.
  /// raw pointer value will be nullptr at first, and erase actual memory space at next frame.
  /// erasion pawn candidate index list will be cleared when next frame starts.
  ///
  /// @param  index Index to erase.
  ///
  void pfUnenrollActivePawn(TI32 index) noexcept;

  /// @brief
  CDyCamera*                mValidMainCameraPtr = nullptr;
  std::vector<CDyCamera*>   mValidSubCameraPtrs = {};

  std::string               mNextLevelName      = MDY_NOT_INITILAIZED_STR;
  std::string               mPresentLevelName   = MDY_NOT_INITILAIZED_STR;
  std::string               mPreviousLevelName  = MDY_NOT_INITILAIZED_STR;

  // Present valid level instance.
  std::unique_ptr<FDyLevel> mLevel              = nullptr;

  // Activated pawn list. this list must not be invalidated when iterating list, but except for unenrolling.
  std::vector<FDyPawn*>     mActivatedPawn = {};
  // Erasion (activated) pawn candidate list. this list must be sorted descendently not to invalidate order.
  std::vector<TI32>         mErasionPawnCandidateList = {};

  // Garbage collection actor instance list.
  std::vector<std::unique_ptr<FDyActor>> mActorGc = {};

  friend class CDyCamera;
  friend class FDyLevel;
  friend class FDyPawn;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_COMPONENT_RESOURCE_SCENE_MANAGER_H