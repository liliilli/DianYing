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

#include <ELowMouse.h>
#include <utility>

namespace dy::base
{

/// @class DInputMousePos
/// @brief Mouse position state binder type.
class DInputMousePos final
{
public:
  /// @brief Set mouse position binding feature state.
  ///
  /// If newState is... \n
  /// ELowMousePosState::Normal :: Mouse position binding range will be set with screen size. \n
  /// ELowMousePosState::Unlimited :: Mouse position binding range will be unlimited but cursor not move from center. \n
  /// ELowMousePosState::Off :: Mouse position binding feature will be disabled.
  void SetFeatureState(ELowMousePosState newState) noexcept;

  /// @brief Get mouse position binding feature state.
  ELowMousePosState GetFeatureState() const noexcept;

  /// @brief Update position with (x, y) and state value.
  void UpdatePosition(int x, int y);

  /// @brief Get present position (x, y) as integer.
  /// This function does not check availability of mouse position checking feature,
  /// just return `mPosX` and `mPosY` as a pair.
  /// @return Position (first, second) as (x, y).
  std::pair<int, int> GetPresentPosition() const noexcept;

  /// @brief Get previous position (x, y) as integer.
  /// This function does not check availability of mouse position checking feature,
  /// just return `mPrePosX` and `mPrePosY` as a pair.
  /// @return Position (first, second) as (x, y).
  std::pair<int, int> GetPreviousPosition() const noexcept;

  /// @brief Get movement amount of position - previous position.
  /// This function does not check availability of mouse position checking feature.
  /// @return Position amount (first, second) as (x, y).
  std::pair<int, int> GetMovementPosition() const noexcept;

  /// @brief Check mouse initially moved.
  [[nodiscard]] bool HasInitiallyMoved() const noexcept;

private:
  /// @brief Position value.
  int mPosX = -1, mPosY = -1;
  /// @brief Previous Position value.
  int mPrePosX = -1, mPrePosY = -1;
  /// @brief Check mouse is moved initially to calculate previous position and position.
  bool mIsMovedFirst = false;

  /// @brief Position feature state.
  ELowMousePosState mState = ELowMousePosState::Off;
  /// @brief

};

} /// ::dy::base namespace