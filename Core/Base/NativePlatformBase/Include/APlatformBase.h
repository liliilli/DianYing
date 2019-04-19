#pragma once
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

#include <cstdint>
#include <EPlatform.h>
#include <EBtResource.h>
#include <AHandlesBase.h>
#include <ABtResourceBase.h>
#include <ADebugBase.h>
#include <AProfilingBase.h>
#include <ALowInput.h>
#include <PWindowCreationDescriptor.h>
#include "DWindowHandle.h"

namespace dy
{

/// @class APlatformBase
/// @brief Platform base type. 
/// This manages platform dependent instance and do bootstrapping of DyEngine.
class APlatformBase
{
public:
  APlatformBase(EXPR_E(EPlatform) platform);
  virtual ~APlatformBase() = 0; 

  /// @brief Get debugging manager as reference.
  /// This function must be succeeded.
  ADebugBase& GetDebugManager() noexcept;

  /// @brief Get profiling manager as reference.
  /// This function must be succeeded.
  AProfilingBase& GetProfilingManager() noexcept;

  /// @brief Get input manager as reference.
  /// This function must be succeeded.
  base::ALowInput& GetInputManager() noexcept;

  virtual void SetWindowTitle(const std::string& newTitle) = 0;

  virtual std::string GetWindowTitle() const = 0;

  virtual uint32_t GetWindowHeight() const = 0;

  virtual uint32_t GetWindowWidth() const = 0;

  virtual void ResizeWindow(uint32_t width, uint32_t height) = 0;

  EXPR_E(EPlatform) GetPlatformType() const noexcept;

  /// @brief Create console window. 
  /// If console window is already created do nothing but just return false.
  virtual bool CreateConsoleWindow() = 0;

  /// @brief Check console window is created.
  [[nodiscard]] bool IsConsoleWindowCreated() noexcept;

  /// @brief Remove console window.
  /// If console window is not created, do nothing but just return false.
  virtual bool RemoveConsoleWindow() = 0;

  /// @brief Initialize platform dependent resources.
  /// Note that this function does not create main window.
  virtual bool InitPlatform() = 0;

  /// @brief Release platform dependent resources.
  /// Note that this function does not remove main window.
  virtual bool ReleasePlatform() = 0;

#ifdef _WIN32
  #ifdef CreateWindow
  #undef CreateWindow
  #endif
#endif

  /// @brief Create game window.
  /// Game window is initially visible.
  /// If failed, just return false.
  virtual std::optional<DWindowHandle> 
  CreateWindow(const PWindowCreationDescriptor& desc) = 0;

#ifdef _WIN32
  #ifndef CreateWindow
  #define CreateWindow CreateWindowW
  #endif
#endif

  /// @brief Remove game window.
  /// All related resource will be removed and released.
  /// If failed, just return false.
  virtual bool RemoveWindow(const DWindowHandle& handle) = 0;

#ifdef _WIN32
#undef FindResource
#endif

  /// @brief Find resource. If not found, returned value is nullptr.
  virtual std::unique_ptr<ABtResourceBase> 
  FindResource(int id, EXPR_E(EBtResource) type) = 0;

#ifdef _WIN32
#define FindResource FindResourceW
#endif
  
  /// @brief Processes only evetns that have already been received and that returns
  /// immediately.
  virtual void PollEvents() = 0;

protected:
  std::unique_ptr<AHandlesBase>     mHandle     = nullptr;
  std::unique_ptr<ADebugBase>       mDebug      = nullptr;
  std::unique_ptr<AProfilingBase>   mProfiling  = nullptr;
  std::unique_ptr<base::ALowInput>  mLowInput   = nullptr;
  bool mIsConsoleWindowCreated = false;

private:
  /// @brief Create background (helper) window.
  /// This window will get background message & HID and controller device detecting etc.
  virtual bool CreateBackgroundWindow() = 0;

  /// @brief Remove background (helper) window resource.
  /// If already removed or failed to remove window, just return false.
  virtual bool RemoveBackgroundWindow() = 0;

  EXPR_E(EPlatform) mPlatform;
};

} /// ::dy namespace