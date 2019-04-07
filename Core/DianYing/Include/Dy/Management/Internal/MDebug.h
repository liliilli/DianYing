#ifndef GUARD_DY_MANAGEMENT_INTERNAL_MDyDebug_H
#define GUARD_DY_MANAGEMENT_INTERNAL_MDyDebug_H
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

#include <Dy/Management/Interface/ISingletonCrtp.h>
#include <Dy/Management/Type/KeyActionBindingInformation.h>
#include <Dy/Editor/View/MainMenu.h>

namespace dy
{
  
/// @class MDebug
/// @brief Debug manager. Manages setting & rendering of debug ui & input etc.
/// This must be initiated only in debug mode `-d`.
///
/// @WARNING This management does not initiate imgui directly.
/// imgui is initiated in rendering manager, so user can use imgui in shipping mode.
class MDebug final : public ISingleton<MDebug>
{
public:
  MDY_SINGLETON_DERIVED(MDebug);
  MDY_SINGLETON_PROPERTIES(MDebug);

  /// @brief Update global (debug, outside world from in-game) input polling on present frame with delta time.
  /// This function must be called update phrase.
  /// If any key is pressed, return EDySuccess::DY_SUCCESS or EDySuccess::DY_FAILURE.
  EDySuccess CheckInput(_MIN_ TF32 dt) noexcept;

  /// @brief Update with dt (from MTime) and render ui object.
  void UpdateAndRender();
  /// @brief Check debug menu is opened.
  MDY_NODISCARD bool IsDebugMenuOpened() const noexcept { return this->mIsDebugMenuOpened; }

private:
  /// @brief Check action is pressed.
  MDY_NODISCARD bool IsActionPressed(_MIN_ const std::string& iSpecifier) const noexcept;
  /// @brief Check action is released.
  MDY_NODISCARD bool IsActionReleased(_MIN_ const std::string& iSpecifier) const noexcept;

  /// @brief Update input keys
  void pUpdateInputKeys();

  /// @brief Action map.
  using TActionMap  = std::unordered_map<std::string, DDyActionBindingInformation>;

  /// Global debug action map
  TActionMap mBindedActionMap = {};
  /// If true, do not check input-key of `mBinededActionMap`.
  bool mIsExternalInputActivated = false;

  //!
  //! Internal flag.
  //!

  /// @brief Check `Debug menu` opened.
  bool mIsDebugMenuOpened = false;
  std::unique_ptr<editor::FDyEditor_MainMenu> mMainMenu = nullptr;
};


} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_INTERNAL_MDyDebug_H