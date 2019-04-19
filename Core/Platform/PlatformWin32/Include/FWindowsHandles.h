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

#include <unordered_map>

#ifndef NOMINMAX
  #define NOMINMAX
#endif
#include <Windows.h>
#include <AHandlesBase.h>
#include <Math/Type/Micellanous/DUuid.h>

//!
//! Forward declaration
//!

struct GLFWwindow;

//!
//! Implementation
//!

namespace dy
{

struct FWindowsHandles final : public AHandlesBase
{
  FWindowsHandles(HANDLE mainProcess);

  /// @brief Main process handle. (Not changed)
  HANDLE mMainProcess = nullptr; 
  /// @brief Background window handle. (Not changed)
  HWND mBackgroundWindow = nullptr;
  /// @brief Device notification handle for HID etc.
  HDEVNOTIFY mDeviceNotiHandle = nullptr;

  /// @brief User-created window handle container.
  /// All values must be valid.
  std::unordered_map<math::DUuid, HWND> mWindowHandles;

  /// @brief Temporary
  GLFWwindow* mGlfwWindow = nullptr;
};

} /// ::dy namespace