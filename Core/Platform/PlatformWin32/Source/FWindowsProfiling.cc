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

/// Header file
#include <FWindowsProfiling.h>

#include <Windows.h>
#include <Psapi.h>
#include <strsafe.h>
#include <atlconv.h>

namespace dy
{

FWindowsProfiling::FWindowsProfiling(HANDLE& mainProcess) 
  : mSelf{mainProcess}
{
  // Get system information.
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  this->mNumProcessors = sysInfo.dwNumberOfProcessors;

  // Get current time of system.
  FILETIME ftime, fsys, fuser;
  GetSystemTimeAsFileTime(&ftime);
  memcpy(&this->mLastCpu, &ftime, sizeof(FILETIME));

  GetProcessTimes(this->mSelf, &ftime, &ftime, &fsys, &fuser);
  memcpy(&this->mLastSysCpu, &fsys, sizeof(FILETIME));
  memcpy(&this->mLastUserCpu, &fuser, sizeof(FILETIME));
}

float FWindowsProfiling::GetCpuUsage()
{
  FILETIME ftime, fsys, fuser;
  ULARGE_INTEGER now, sys, user;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&now, &ftime, sizeof(FILETIME));

  GetProcessTimes(this->mSelf, &ftime, &ftime, &fsys, &fuser);
  memcpy(&sys, &fsys, sizeof(FILETIME));
  memcpy(&user, &fuser, sizeof(FILETIME));

  float percent = float(sys.QuadPart - mLastSysCpu.QuadPart) + float(user.QuadPart - mLastUserCpu.QuadPart);
  percent /= (now.QuadPart - mLastCpu.QuadPart);
  percent /= mNumProcessors;

  this->mLastCpu     = now;
  this->mLastUserCpu = user;
  this->mLastSysCpu  = sys;

  return percent * 100;
}

uint64_t FWindowsProfiling::GetRamUsage()
{
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(
    GetCurrentProcess(), 
    reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), 
    sizeof(pmc));
  return pmc.WorkingSetSize;
}

} /// ::dy namespace