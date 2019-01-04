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
#include <Dy/Management/Platform/DDyWindowInformationWindows.h>
#if (defined(MDY_PLATFORM_FLAG_WINDOWS) == true)
#include <Psapi.h>

namespace dy
{

void DDyWindowInformationWindows::InitializeDep()
{
  SYSTEM_INFO sysInfo;
  FILETIME ftime, fsys, fuser;

  GetSystemInfo(&sysInfo);
  mNumProcessors = sysInfo.dwNumberOfProcessors;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&mLastCpu, &ftime, sizeof(FILETIME));

  mSelf = GetCurrentProcess();
  GetProcessTimes(mSelf, &ftime, &ftime, &fsys, &fuser);
  memcpy(&mLastSysCpu, &fsys, sizeof(FILETIME));
  memcpy(&mLastUserCpu, &fuser, sizeof(FILETIME));
}

void DDyWindowInformationWindows::ReleaseDep()
{ 

}

EDySuccess DDyWindowInformationWindows::CreateConsoleWindow()
{
  MDY_ASSERT(this->mIsConsoleWindowInitialized.load() == false, "MDyWindow::mIsConsoleWindowInitialized must be falsed.");

  // Allocate console and forward stdout to console.
  if (AllocConsole() == false)
  {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }
  freopen_s(&this->mFp, "CONOUT$", "w", stdout);

  bool before = false;
  while (this->mIsConsoleWindowInitialized.compare_exchange_strong(before, true, std::memory_order_seq_cst) == false)
      ;
  return DY_SUCCESS;
}

bool DDyWindowInformationWindows::IsCreatedConsoleWindow() const noexcept
{
  return this->mIsConsoleWindowInitialized.load();
}

EDySuccess DDyWindowInformationWindows::RemoveConsoleWindow()
{
  MDY_ASSERT(this->mIsConsoleWindowInitialized.load() == true, "MDyWIndow::mIsConsoleWindowInitialized must be trued.");

  // Release console.
  fclose(this->mFp);
  if (FreeConsole() == false)
  {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  bool before = true;
  while (this->mIsConsoleWindowInitialized.compare_exchange_strong(before, false, std::memory_order_seq_cst) == false)
      ;
  return DY_SUCCESS;
}

TF32 DDyWindowInformationWindows::GetCpuUsage()
{
  FILETIME ftime, fsys, fuser;
  ULARGE_INTEGER now, sys, user;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&now, &ftime, sizeof(FILETIME));

  GetProcessTimes(mSelf, &ftime, &ftime, &fsys, &fuser);
  memcpy(&sys, &fsys, sizeof(FILETIME));
  memcpy(&user, &fuser, sizeof(FILETIME));
  TF32 percent = (sys.QuadPart - mLastSysCpu.QuadPart) + (user.QuadPart - mLastUserCpu.QuadPart);
  percent /= (now.QuadPart - mLastCpu.QuadPart);
  percent /= mNumProcessors;
  mLastCpu     = now;
  mLastUserCpu = user;
  mLastSysCpu  = sys;

  return percent * 100;
}

TU64 DDyWindowInformationWindows::GetRamUsage()
{
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
  return pmc.WorkingSetSize;
}

} /// ::dy namespace

#endif /// MDY_PLATFORM_FLAG_WINDOWS