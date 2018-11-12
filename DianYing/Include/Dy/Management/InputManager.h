#ifndef GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
#define GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
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

#include <string>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/KeyAxisBindingInformation.h>

namespace dy
{

///
/// @class MDyInput
/// @brief Manages input polling, mouse movement and joystick input signaling.
///
class MDyInput final : public ISingleton<MDyInput>
{
  MDY_SINGLETON_PROPERTIES(MDyInput);
  MDY_SINGLETON_DERIVED(MDyInput);
public:
  ///
  /// @brief Get Key value which is bound to key container.
  /// This returns [-1, 1] floating values of key information instance found.
  /// If the key name you find didn't find in container,
  /// output error flag in debug mode, and return always 0.0.
  ///
  /// @param[in] axisKeyName The key name which key instance has.
  /// @return float The key value which has [-1, 1] range floating value.
  ///
  MDY_NODISCARD TF32 GetAxisValue(_MIN_ const std::string& axisKeyName) noexcept;

  ///
  /// @brief Return present-frame mouse position.
  ///
  FORCEINLINE MDY_NODISCARD const DDyVector2& GetPresentMousePosition() const noexcept
  {
    return this->mMousePresentPosition;
  }

  ///
  /// @brief Return old-frame mouse position.
  ///
  FORCEINLINE MDY_NODISCARD const DDyVector2& GetPresentLastPosition() const noexcept
  {
    return this->mMouseLastPosition;
  }

  ///
  /// @brief Get whether or not specific key was pressed.
  /// @param[in] axisSpecifier The key name which key instance has.
  /// @return boolean value, if specific key was pressed, return true.
  ///
  MDY_NODISCARD bool IsAxisPressed(_MIN_ const std::string& axisSpecifier) noexcept;

  ///
  /// @brief Get whether or not specific key was released.
  /// @param[in] axisSpecifier The key name which key instance has.
  /// @return boolean value, if specific key was released, return true.
  ///
  MDY_NODISCARD bool IsAxisReleased(_MIN_ const std::string& axisSpecifier) noexcept;

  ///
  /// @brief check if mouse is moved on present frame, but false when mouse movement is not activated.
  ///
  FORCEINLINE MDY_NODISCARD bool IsMouseMoved() const noexcept
  {
    return this->mIsMouseMoved;
  }

private:
  ///
  /// @brief Update input polling on present frame with delta time.
  /// This function must be called update phrase.
  ///
  void pfUpdate(_MIN_ TF32 dt) noexcept;

  ///
  /// @brief Check if key (action, axis) exist.
  /// @param keySpecifierName key (action, axis) specifier name.
  ///
  MDY_NODISCARD bool pIsAxisExist(_MIN_ const std::string& keySpecifierName) const noexcept;

  using TAxisMap = std::unordered_map<std::string, DDyAxisBindingInformation>;

  // Window handle pointer (temporal)
  GLFWwindow*       mTempGlfwWindowPtr    = nullptr;;
  GLFWcursor*       mGlfwWindowCursorPtr  = nullptr;

  TAxisMap          mBindedAxisMap        = {};
  DDyVector2        mMouseLastPosition    = {};
  DDyVector2        mMousePresentPosition = {};

  bool              mIsMouseMoved         = false;

  friend class MDyWindow;
};

}

#endif /// GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
