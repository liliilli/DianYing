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

#ifndef NOMINMAX
  #define NOMINMAX
#endif
#include <Windows.h>
#include <AHandlesBase.h>

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

  HANDLE  mMainProcess = nullptr;
  HWND    mMainWindow = nullptr;
  HWND    mBackgroundWindow = nullptr;
  HDEVNOTIFY mDeviceNotiHandle = nullptr;

  GLFWwindow* mGlfwWindow = nullptr;
};

} /// ::dy namespace