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

namespace dy
{

/// @class ADebugBase
/// @brief Base Debug Type.
class ADebugBase
{
public:
  /// @brief Release function type.
  using TReleaseFuntion = void(*)(void);

  ADebugBase() = default;
  virtual ~ADebugBase() = 0;

  ADebugBase(const ADebugBase&) = delete;
  ADebugBase& operator=(const ADebugBase&) = delete;
  ADebugBase(ADebugBase&&) noexcept = delete;
  ADebugBase& operator=(ADebugBase&&) noexcept = delete;

  /// @brief Function that called when assertion is failed.
  /// Every implementation must implement 
  virtual void OnAssertionFailed(
    const char* failedMessage, 
    const char* function, 
    const char* file, 
    int line) = 0;

  /// @brief Set release function.
  void SetReleaseFunction(TReleaseFuntion function);

protected:
  /// @brief Try call release function if only release function is set.
  void TryCallReleaseFunction();

private:
  /// @brief Release function pointer.
  TReleaseFuntion mReleaseFunction = nullptr;
};

} /// ::dy namespace