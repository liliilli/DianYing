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

#include <optional>

#include <ELowKeyboard.h>
#include <ELowMouse.h>
#include <DInputButton.h>
#include <DInputMousePos.h>

namespace dy::base 
{

/// @class ALowInput
/// @brief Low-input management type.
class ALowInput
{
public:
  ALowInput() = default;
  virtual ~ALowInput() = 0;

  /// @brief Update keyboard key with derived platform descriptor instance.
  virtual void UpdateKeyboard(void* descriptor) = 0;

  /// @brief Update mouse button with derived platform descriptor instance.
  virtual void UpdateMouseButton(void* descriptor) = 0;

  /// @brief Get keyboard state. ELowKeyboard::Dy_Key__Error must not be used.
  EInputState GetKeyboard(ELowKeyboard id);

  /// @brief Get mouse state. ELowMouseButton::DyMouse__Error, __Sum must not be used.
  EInputState GetMouseButton(ELowMouseButton id);

  /// @brief Set mouse position binding feature state.
  ///
  /// If newState is... \n
  /// ELowMousePosState::Normal :: Mouse position binding range will be set with screen size. \n
  /// ELowMousePosState::Unlimited :: Mouse position binding range will be unlimited but cursor not move from center. \n
  /// ELowMousePosState::Off :: Mouse position binding feature will be disabled.
  void SetMousePosFeatureState(ELowMousePosState newState) noexcept;

  /// @brief Get mouse position binding feature state.
  ELowMousePosState GetMousePosState() const noexcept;

  /// @brief Try update mouse position with given (platform dependent OS) descriptor.
  /// If mouse position binding feature is disabled, this function do nothing.
  virtual void UpdateMousePos(void* descriptor) = 0;

  /// @brief Get mouse position.
  /// If mouse position checking feature is disabled, just return std::nullopt.
  /// If mouse position checking feature state is `Unlimited`, just return virtual position.
  std::optional<std::pair<int, int>> GetMousePos() const noexcept;

  /// @brief Get mouse previous frame position.
  /// If mouse position checking feature is disabled, just return std::nullopt.
  /// If mouse position checking feature state is `Unlimited`, just return virtual position.
  std::optional<std::pair<int, int>> GetMousePreviousPos() const noexcept;

  /// @brief Get moved mouse position amounts.
  /// If mouse position checking feature is disabled, just return std::nullopt.
  std::optional<std::pair<int, int>> GetMousePosMovement() const noexcept;

protected:
  std::array<DInputButtonItem, Dy_Key_Menu>   sLowKeyboards;
  std::array<DInputButtonItem, DyMouse__Sum>  sLowMouseButtons;
  DInputMousePos mLowMousePos;
};

inline ALowInput::~ALowInput() = default;

} /// ::dy::base namespace