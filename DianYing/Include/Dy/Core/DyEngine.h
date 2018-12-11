#ifndef GUARD_DY_CORE_MDYENGINE_H
#define GUARD_DY_CORE_MDYENGINE_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Core/Thread/TDyIO.h>
#include <Dy/Helper/Pointer.h>

//!
//! Forward declaration
//!

namespace dy
{
class MDyTime;
class MDyWindow;
class MDySynchronization;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

///
/// @class DyEngine
/// @brief `Dy`(DianYing) Core engine class.
///
class DyEngine final : public IDySingleton<DyEngine>
{
  MDY_SINGLETON_PROPERTIES(DyEngine);
  MDY_SINGLETON_DERIVED(DyEngine);
public:
  ///
  /// @brief Run engine.
  ///
  void operator()();

  /// @brief Get time manager reference.
  MDY_NODISCARD MDyTime&    GetTimeManager();
  /// @brief Get window manager reference.
  MDY_NODISCARD MDyWindow&  GetWindowManager();

private:
  /// @brief Get IO Thread Instance which not be nulled.
  NotNull<TDyIO*> pfGetIOThread();

  /// @brief Update entry function from engine.
  void pUpdate(_MIN_ TF32 dt);

  /// @brief Render entry function from engine.
  void pRender();

  void pfInitializeIndependentManager();
  void pfInitializeDependentManager();
  void pfReleaseDependentManager();
  void pfReleaseIndependentManager();

  MDySynchronization* mSynchronization = nullptr;

  friend class SDyIOConnectionHelper;
  friend class SDyIOWorkerConnHelper;
  friend class MDySynchronization;
};

extern DyEngine* gEngine;

} /// ::dy namespace

#endif /// GUARD_DY_CORE_MDYENGINE_H
