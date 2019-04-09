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

#include <EInputState.h>

namespace dy::base
{

/// @class DInputButtonItem
/// @brief Button status binder. Axis and Action must be retrieve values from this.
class DInputButtonItem final
{
public:
  /// @brief Update status with from library.
  void Update(EInputState newState) noexcept;

  /// @brief Get status value on present frame.
  EInputState Get() const noexcept;

private:
  EInputState mState = EInputState::Released;
};

} /// ::dy::base namespace