#ifndef GUARD_DY_MANAGEMENT_PLATFORM_MPlatformInfoWindows_H
#define GUARD_DY_MANAGEMENT_PLATFORM_MPlatformInfoWindows_H
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

#include <atomic>
#include <Dy/Helper/System/Macroes.h>
#include <Dy/Core/Resource/Internal/EMaterialBlendMode.h>
#include <Dy/Management/Interface/IPlatformDependent.h>
#include <Dy/Helper/System/Pointer.h>

#if (defined(_WIN32) == true)
namespace dy
{

/// @struct MPlatformInfoWindows
/// @brief Windows information for windows platform.
struct MPlatformInfoWindows : public IPlatformDependent
{
public:
  void InitializeDep() override final;
  void ReleaseDep() override final;

  /// @brief  Create console window if OS supports. Even though OS does not support console window,
  /// This function will return EDySuccess::DY_SUCCESS because of conformity with remove function.
  /// @return If succeeded, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE. \n
  MDY_NODISCARD EDySuccess CreateConsoleWindow() override final;

  /// @brief  Check if console window is created or not.
  /// @return If created anyway, return true or false.
  MDY_NODISCARD bool IsCreatedConsoleWindow() const noexcept override final;

  /// @brief  Remove console window when console window is initiailzed before.
  /// @return If succeeded, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  MDY_NODISCARD EDySuccess RemoveConsoleWindow() override final;

  /// @brief Get cpu usage as 0 ~ 100 percent.
  MDY_NODISCARD TF32 GetCpuUsage() override final;

  /// @brief Get ram usage as byte.
  MDY_NODISCARD TU64 GetRamUsage() override final;

  /// @brief Check font is installed and exist on given OS.
  MDY_NODISCARD bool IsFontExistOnSystem(_MIN_ const std::string& iFontKey) const override final;

  /// @brief Get system font path with iFontKey. If not found, just return null value.
  MDY_NODISCARD std::optional<std::string> GetFontPathOnSystem(_MIN_ const std::string& iFontKey) const override final;
  
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
#endif /// GUARD_DY_MANAGEMENT_PLATFORM_MPlatformInfoWindows_H