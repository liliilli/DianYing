#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Internal/MDySynchronization.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>
#include <Dy/Core/DyEngine.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>

namespace dy
{

EDySuccess MDySynchronization::pfInitialize()
{
  ///
  /// @brief Initialize threads.
  ///
  static auto InitializeThread = [this]
  {
    this->mIOThreadInstance = std::make_unique<TDyIO>();
    this->mIOThreadThread   = std::thread(&TDyIO::operator(), std::ref(*this->mIOThreadInstance));
  };

  InitializeThread();
  return DY_SUCCESS;
}

EDySuccess MDySynchronization::pfRelease()
{
  static auto ReleaseThread = [this]
  {
    SDyIOConnectionHelper::TryStop();

    this->mIOThreadThread.join();
    this->mIOThreadInstance = nullptr;
  };

  return DY_SUCCESS;
}

NotNull<TDyIO*> MDySynchronization::pfGetIOThread()
{
  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mIOThreadInstance), "IOThread Instance must not be null except for initialization and destruction.");
  return DyMakeNotNull(this->mIOThreadInstance.get());
}

void MDySynchronization::TrySynchronization()
{
  switch (gEngine->GetGlobalGameStatus())
  {
  case EDyGlobalGameStatus::Booted:       { this->pRunFrameBooted(); }        break;
  case EDyGlobalGameStatus::FirstLoading: { this->pRunFrameFirstLoading(); }  break; // Syncrhonization
  case EDyGlobalGameStatus::Loading:      { this->pRunFrameLoading(); }       break;
  case EDyGlobalGameStatus::GameRuntime:  { this->pRunFrameGameRuntime(); }   break;
  case EDyGlobalGameStatus::Shutdown: break;
  default: /* Do nothing */ break;
  }
}

void MDySynchronization::pRunFrameBooted()
{
  if (SDyIOConnectionHelper::IsMainTaskListIsEmpty() == false)
  {
    SDyIOConnectionHelper::ForceProcessDeferredMainTaskList();
  }

  if (SDyIOConnectionHelper::CheckIOResultInCondition() == true)
  {
    SDyIOConnectionHelper::ForceProcessIOInsertPhase();
  }

  SDyIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MDySynchronization::pRunFrameFirstLoading()
{
  using TSyncHelper = SDyIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListIsEmpty() == false)    { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::CheckIOResultInCondition() == true)  { TSyncHelper::ForceProcessIOInsertPhase(); }

  // Check whether IO thread working is done, if so change status to `Loading`. 
  SDyIOConnectionHelper::TryCallSleptCallbackFunction();
}

void MDySynchronization::pRunFrameLoading()
{
  gEngine->SetNextGameStatus(EDyGlobalGameStatus::GameRuntime);
}

void MDySynchronization::pRunFrameGameRuntime()
{
  using TSyncHelper = SDyIOConnectionHelper;
  if (TSyncHelper::IsMainTaskListIsEmpty() == false)  { TSyncHelper::ForceProcessDeferredMainTaskList(); }
  if (TSyncHelper::CheckIOResultInCondition() == true){ TSyncHelper::ForceProcessIOInsertPhase(); }
}

} /// ::dy namespace