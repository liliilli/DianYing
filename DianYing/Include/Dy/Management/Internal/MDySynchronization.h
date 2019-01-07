#ifndef GUARD_DY_MANAGER_INTERNAL_MDYSYNCHRONIZATION_H
#define GUARD_DY_MANAGER_INTERNAL_MDYSYNCHRONIZATION_H
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
#include <Dy/Core/Thread/TDyIO.h>
#include <Dy/Management/Type/EDyGlobalGameStatus.h>

namespace dy
{

///
/// @class MDySynchronization
/// @brief Synchronization manager for ticking IOThread / LogicThread / etc.
///
class MDySynchronization final : public IDySingleton<MDySynchronization>
{
  MDY_SINGLETON_DERIVED(MDySynchronization);
  MDY_SINGLETON_PROPERTIES(MDySynchronization);
public:
  /// @brief Get IO Thread Instance which not be nulled.
  NotNull<TDyIO*> pfGetIOThread();

  /// @brief Run synchronization phase or operating system.
  void TrySynchronization();

private:
  /// @brief Run booted phase.
  void pRunFrameBooted();
  /// @brief Run first loading phase.
  void pRunFrameFirstLoading();
  /// @brief Run loading phase.
  void pRunFrameLoading();
  /// @brief Run runtime phase.
  void pRunFrameGameRuntime();
  /// @brief Run shutdown phase.
  void PRunFrameShutdown();

  std::unique_ptr<TDyIO>  mIOThreadInstance = nullptr;
  std::thread             mIOThreadThread;
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGER_INTERNAL_MDYSYNCHRONIZATION_H