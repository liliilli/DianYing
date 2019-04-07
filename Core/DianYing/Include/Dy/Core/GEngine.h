#ifndef GUARD_DY_CORE_MDYENGINE_H
#define GUARD_DY_CORE_MDYENGINE_H
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

#include <Dy/Core/Thread/TRescIO.h>
#include <Dy/Helper/System/Pointer.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/EGlobalGameState.h>

#include <AEngineBase.h>

//!
//! Forward declaration
//!

namespace dy
{
class MTime;
class MWindow;
class MSynchronization;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

#define TEMP_CAST(Engine) static_cast<::dy::GEngine*>(Engine.get())

/// @class GEngine
/// @brief `Dy`(DianYing) Core engine class.
class GEngine final : public AEngineBase
{
  MDY_SINGLETON_PROPERTIES(GEngine);
public:
  GEngine() = default;               
  [[nodiscard]] EDySuccess pfInitialize();
  [[nodiscard]] EDySuccess pfRelease();
  friend class ISingleton<GEngine>; 
  virtual ~GEngine() = default;

  /// @brief Run engine.
  void operator()();

  /// @brief Get time manager reference.
  MDY_NODISCARD MTime&    GetTimeManager();
  /// @brief Get window manager reference.
  MDY_NODISCARD MWindow&  GetWindowManager();

  /// @brief Get game's global status.
  MDY_NODISCARD EGlobalGameState GetGlobalGameStatus() const noexcept;

  /// @brief Set next status.
  void SetNextGameStatus(EGlobalGameState iNextStatus) noexcept;
  /// @brief Set in-game update pause activation flag.
  /// @warning DO NOT CALL THIS with `true` IN IN-GAME SCRIPT WITH NO GLOBAL RETRIVATION LOGIC.
  /// all script & animation & physics will be paused when true, so could not resume in in-game world.
  void SetInGameUpdatePause(bool iActivated) noexcept;
  /// @brief Check in-game update pause flag is true or false.
  MDY_NODISCARD bool IsInGameUpdatePaused() const noexcept;

  /// @brief Try end game. Duplicated function call is neglected to once.
  MDY_NOTUSED EDySuccess TryEndGame() noexcept;
  /// @brief Check game should be ended.
  MDY_NODISCARD bool MDY_PRIVATE(IsGameEndCalled)() const noexcept;
  /// @brief Check game level is discarded and loading new level.
  MDY_NODISCARD bool MDY_PRIVATE(IsGameNeedTransitLevel)() const noexcept;
  /// @brief Check game status need to be transited. This function returns true when
  /// 1. IsGameEndCalled() returns true.
  /// 2. IsGameNeedTransitLevel() returns true.
  MDY_NODISCARD bool MDY_PRIVATE(IsGameNeedToBeTransitted)() const noexcept;

private:
  /// @brief Get IO Thread Instance which not be nulled.
  NotNull<TRescIO*> pfGetIOThread();

  /// @brief Reflect game status transition.
  void MDY_PRIVATE(ReflectGameStatusTransition)();

  /// @brief Render every update from engine.
  void MDY_PRIVATE(Render)(EGlobalGameState iEngineStatus);
  /// @brief Update entry function from engine.
  void MDY_PRIVATE(Update)(EGlobalGameState iEngineStatus, TF32 dt);
  /// @brief Update sequence before render phase.
  void MDY_PRIVATE(PreRender)(EGlobalGameState iEngineStatus, TF32 dt);
  /// @brief Update sequence afterward render phase.
  void MDY_PRIVATE(PostRender)(EGlobalGameState iEngineStatus, TF32 dt);

  /// @brief Try update status. if changed, `mIsStatusTransitionDone` be falsed.
  /// This function must be called at final.
  void TryUpdateStatus();

  void pfInitializeIndependentManager();
  void pfInitializeDependentManager();
  void pfReleaseDependentManager();
  void pfReleaseIndependentManager();

  MSynchronization* mSynchronization = nullptr;

  EGlobalGameState mNextStatus = EGlobalGameState::Booted;
  EGlobalGameState mStatus     = EGlobalGameState::None;
  EGlobalGameState mPrevStatus = EGlobalGameState::None;
  bool mIsStatusTransitionDone = false;
  bool mIsGameEndCalled        = false;
  /// @brief If true, update (in-game input, physics, skinned animation etc) will be neglected,
  /// and in-game resource gc also will be passed.
  /// If resource manager has been changed, you need to call GC manually.
  bool mIsInGameUpdatePaused   = false;

  friend class SIOConnectionHelper;
  friend class SIOWorkerConnHelper;
  friend class SIOBindingHelper;
  friend class MSynchronization;
};

} /// ::dy namespace

#endif /// GUARD_DY_CORE_MDYENGINE_H
