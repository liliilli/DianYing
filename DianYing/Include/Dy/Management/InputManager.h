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
#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/KeyBindingInformation.h>
#include "Dy/Helper/Type/Vector2.h"

namespace dy
{

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
  [[nodiscard]] float GetKeyValue(const std::string& axisKeyName) noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] const DVector2& GetPresentMousePosition() const noexcept
  {
    return this->mMousePresentPosition;
  }

  ///
  /// @brief
  ///
  [[nodiscard]] const DVector2& GetPresentLastPosition() const noexcept
  {
    return this->mMouseLastPosition;
  }

  ///
  /// @brief Get whether or not specific key was pressed.
  /// @param[in] keyName The key name which key instance has.
  /// @return boolean value, if specific key was pressed, return true.
  ///
  [[nodiscard]] bool IsKeyPressed(const std::string& keyName) noexcept;

  ///
  /// @brief Get whether or not specific key was released.
  /// @param[in] keyName The key name which key instance has.
  /// @return boolean value, if specific key was released, return true.
  ///
  [[nodiscard]] bool IsKeyReleased(const std::string& keyName) noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] bool IsMouseMoved() const noexcept
  {
    return this->mIsMouseMoved;
  }

  ///
  /// @brief
  ///
  [[nodiscard]] bool pIsKeyExist(const std::string& keyName) const noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] EDySuccess pInsertKey(const PDyKeyBindingConstructionDescriptor& bindingKey) noexcept;

private:
  ///
  /// @brief
  ///
  void pfUpdate(float dt) noexcept;

  ///
  /// @brief
  ///
  [[nodiscard]] EDySuccess pReadInputFile(const std::string& file_path);

  using TKeyMap = std::unordered_map<std::string, DDyKeyBindingInformation>;

  // Window handle pointer (temporal)
  GLFWwindow*       mTempGlfwWindowPtr    = nullptr;;
  GLFWcursor*       mGlfwWindowCursorPtr  = nullptr;

  TKeyMap           mBindedKeyList;
  DVector2          mMouseLastPosition    = {};
  DVector2          mMousePresentPosition = {};

  bool              mIsEnabledKeyboard    = false;
  bool              mIsEnabledMouse       = false;
  bool              mIsEnabledJoystick    = false;

  bool              mIsMouseMoved         = false;

  std::vector<std::reference_wrapper<DDyKeyBindingInformation>> m_key_disposal;

  friend class MDyWindow;
};

}

#endif /// GUARD_DY_MANAGEMENT_INPUT_MANAGER_H
