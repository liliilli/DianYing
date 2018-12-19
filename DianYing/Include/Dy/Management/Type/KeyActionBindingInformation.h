#ifndef GUARD_DY_MANAGEMENT_TYPE_KEYACTIONBINDINGINFORMATION_H
#define GUARD_DY_MANAGEMENT_TYPE_KEYACTIONBINDINGINFORMATION_H
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

#include <Dy/Management/Type/SettingContainer.h>

namespace dy
{

///
/// @struct DDyActionBindingInformation
/// @brief Stores action info to use in this game, and overall application.
/// You can bind arbitary (Supported on GLFW) action into axis container, and get values from
/// them to call input_event handler.
///
/// This type is not support repeat feature.
///
struct DDyActionBindingInformation final
{
  enum class EDyActionInputStatus
  {
    Pressed,
    Released,
    //
    Bottled,
  };

  EDyActionInputStatus  mKeyStatus    = EDyActionInputStatus::Released;

  /// container key name must be same as structure's name. (Camel)
  std::string		         mAxisSpecifierName  = "";
  std::vector<EDyButton> mActionId           = {};

  ///
  /// @brief Construct axis information from `DAction`.
  /// @param actionInfo Information from setting (MDySetting).
  ///
  DDyActionBindingInformation(_MIN_ const DDySettingInput::DAction& actionInfo);
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_KEYACTIONBINDINGINFORMATION_H