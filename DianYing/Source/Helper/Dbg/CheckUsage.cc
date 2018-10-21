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
/// @reference https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
///

/// Header file
#include <Dy/Helper/Dbg/CheckUsage.h>

#if defined(_WIN32)
#include <Pdh.h>
#elif defined(__linux__)
#include <sys/types.h>
#include <sys/sysinfo.h>
#endif

namespace
{
#if defined(_WIN32)
PDH_HQUERY    sCpuQuery;
PDH_HCOUNTER  sCpuTotal;
#elif defined(__linux__)

#endif
}

namespace dy
{

EDySuccess MDyUsageChecker::pfInitialize()
{
#ifdef false
  PdhOpenQuery(NULL, NULL, &sCpuQuery);
  PdhAddEnglishCounter(sCpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &sCpuTotal);
  PdhCollectQueryData(sCpuQuery);
#endif

  return DY_SUCCESS;
}

EDySuccess MDyUsageChecker::pfRelease()
{
#ifdef false
  PdhRemoveCounter(sCpuQuery);
  PdhCloseQuery(sCpuQuery);
#endif

  return DY_SUCCESS;
}

void MDyUsageChecker::UpdateCpuUsageGraph(float dt)
{
#ifdef false
#if defined(_WIN32)
  PDH_FMT_COUNTERVALUE counterVal;
  PdhCollectQueryData(sCpuQuery);
  PdhGetFormattedCounterValue(sCpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

  // @TODO IMPLEMENT THIS
#elif defined(__linux__)

#endif
#endif
}

void MDyUsageChecker::UpdateRanUsageGraph(float dt)
{

}

void MDyUsageChecker::ResetCpuUsageGraph()
{

}

void MDyUsageChecker::ResetRamUsageGraph()
{

}

} /// ::dy namespace