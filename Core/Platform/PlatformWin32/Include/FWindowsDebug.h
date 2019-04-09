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

#include <ADebugBase.h>

namespace dy
{

/// @class FWindowsDebug
/// @brief Windows debug type.
class FWindowsDebug : public ADebugBase
{
public:
  /// @brief Release function type.
  using TReleaseFuntion = void(*)(void);

  FWindowsDebug() = default;
  virtual ~FWindowsDebug() = default;

  FWindowsDebug(const FWindowsDebug&) = delete;
  FWindowsDebug& operator=(const FWindowsDebug&) = delete;
  FWindowsDebug(FWindowsDebug&&) noexcept = delete;
  FWindowsDebug& operator=(FWindowsDebug&&) noexcept = delete;

  /// @brief Function that called when assertion is failed.
  /// Every implementation must implement 
  void OnAssertionFailed(
    const char* failedMessage, 
    const char* function, 
    const char* file, 
    int line) override final;
};

} /// ::dy namespace