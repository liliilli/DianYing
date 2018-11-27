#ifndef GUARD_DY_MANAGEMENT_PLATFORM_DDYWINDOWINFORMATIONWINDOWS_H
#define GUARD_DY_MANAGEMENT_PLATFORM_DDYWINDOWINFORMATIONWINDOWS_H
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

#include <atomic>
#include <Dy/Helper/System/Macroes.h>
#include <Dy/Core/Component/Internal/MaterialType.h>
#include <Dy/Management/Interface/IDyWindowDependent.h>

#if (defined(MDY_PLATFORM_FLAG_WINDOWS) == true)
namespace dy
{

///
/// @struct DDyWindowInformationWindows
/// @brief Windows information for windows platform.
///
struct DDyWindowInformationWindows : public IDyWindowDependent
{
public:
  void InitializeDep() override final;
  void ReleaseDep() override final;

  ///
  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE. \n
  ///
  MDY_NODISCARD EDySuccess CreateConsoleWindow() override final;

  ///
  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false.
  ///
  MDY_NODISCARD bool IsCreatedConsoleWindow() const noexcept override final;

  ///
  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return DY_SUCCESS or DY_FAILURE.
  ///
  MDY_NODISCARD EDySuccess RemoveConsoleWindow() override final;

  ///
  /// @brief Get cpu usage as 0 ~ 100 percent.
  ///
  MDY_NODISCARD TF32 GetCpuUsage() override final;

protected:
  Owner<FILE*> mFp                      = nullptr;
  HWND        mWindowHandle             = nullptr;
  HDC         mWindowDeviceContext      = nullptr;
  HGLRC       mWindowGlResourceContext  = nullptr;

  ULARGE_INTEGER  mLastCpu;
  ULARGE_INTEGER  mLastSysCpu;
  ULARGE_INTEGER  mLastUserCpu;
  TI32            mNumProcessors;
  HANDLE          mSelf;

  ///
  std::atomic<bool> mIsConsoleWindowInitialized = false;
};

}; /// ::dy namespace
#endif /// MDY_PLATFORM_FLAG_WINDOWS
#endif /// GUARD_DY_MANAGEMENT_PLATFORM_DDYWINDOWINFORMATIONWINDOWS_H