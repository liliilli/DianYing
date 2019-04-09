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

#include <AProfilingBase.h>
#include <Windows.h>

namespace dy
{

class FWindowsProfiling final : public AProfilingBase
{
public:
  FWindowsProfiling(HANDLE& mainProcess);
  virtual ~FWindowsProfiling() = default;

  /// @brief Get cpu usage as percentage.
  float GetCpuUsage() override final;

  /// @brief Get ram usage as byte unit.
  uint64_t GetRamUsage() override final;

private:
  ULARGE_INTEGER mLastCpu;
  ULARGE_INTEGER mLastSysCpu;
  ULARGE_INTEGER mLastUserCpu;
  int            mNumProcessors = 0;
  HANDLE         mSelf = nullptr;
};

} /// ::dy namespace