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

#include <memory>
#ifndef NOMINMAX
  #define NOMINMAX
#endif

#include <APlatformBase.h>

#include <Windows.h>
#include <XDisableWinMacro.h>

namespace dy
{

/// @class FWindowsPlatform
/// @brief Platform base type. 
/// This manages platform dependent instance and do bootstrapping of DyEngine.
class FWindowsPlatform : public APlatformBase
{
public:
  FWindowsPlatform();
  virtual ~FWindowsPlatform(); 

  void SetWindowTitle(const std::string& newTitle) override final;

  std::string GetWindowTitle() const override final;

  uint32_t GetWindowHeight() const override final;

  uint32_t GetWindowWidth() const override final;

  void ResizeWindow(uint32_t width, uint32_t height) override final;

  /// @brief Create console window. 
  /// If console window is already created do nothing but just return false.
  bool CreateConsoleWindow() override final;

  /// @brief Remove console window.
  /// If console window is not created, do nothing but just return false.
  bool RemoveConsoleWindow() override final;

#undef FindResource // This sucks

  std::unique_ptr<ABtResourceBase> 
  FindResource(int id, EXPR_E(EBtResource) type) override final;

#define FindResource FindResourceW // Resume

  /// @brief Create game window.
  /// Game window is initially visible.
  /// If failed, just return false.
  bool CreateGameWindow() override final;
  
  /// @brief Remove game window.
  /// All related resource will be removed and released.
  /// If failed, just return false.
  bool RemoveGameWindow() override final;

private:
  /// @brief Register window class into Win32 internal system.
  bool RegisterWindowClassWin32();
  /// @brief Create background (helper) window.
  /// This window will get background message & HID and controller device detecting etc.
  bool CreateBackgroundWindow() override final;

  /// @brief Remove background (helper) window resource.
  /// If already removed or failed to remove window, just return false.
  bool RemoveBackgroundWindow() override final;
  /// @brief Unregister window class from Win32 internal system.
  bool UnregisterWindowClassWin32();

  FILE* mFdConsole = nullptr;
};

}