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

namespace dy
{

/// @class AProfilingBase
/// @brief Application Profiling base type.
class AProfilingBase
{
public:
  AProfilingBase() = default;
  virtual ~AProfilingBase() = 0;

  /// @brief Get cpu usage as percentage.
  virtual float GetCpuUsage() = 0;

  /// @brief Get ram usage as byte unit.
  virtual uint64_t GetRamUsage() = 0;
};

inline AProfilingBase::~AProfilingBase() = default;

} /// ::dy namespace