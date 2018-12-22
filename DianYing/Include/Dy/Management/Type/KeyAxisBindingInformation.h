#ifndef GUARD_DY_MANAGEMENT_TYPE_KEY_BINDING_INFORMATION_H
#define GUARD_DY_MANAGEMENT_TYPE_KEY_BINDING_INFORMATION_H
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
#include <Dy/Management/Type/SettingContainer.h>
#include <Dy/Meta/Type/Input/EDyInputButton.h>

namespace dy
{

///
/// @struct DDyAxisBindingInformation
/// @brief Stores axis info to use in this game, and overall application.
/// You can bind arbitary (Supported on GLFW) axis into axis container, and get values from
/// them to call input_event handler.
///
/// If the user presses negative key, key information instance bound to key container
/// set value to -1.0f. Inversely as positive key is.
/// And the user releases negative or positive key, key information instance let values
/// go to neurtal zone. (within range of mNeutralStatusThresholdValue)
///
struct DDyAxisBindingInformation final
{
  /// @enum EDyAxisInputStatus
  /// @brief Indicates key status of keys.
  enum class EDyAxisInputStatus : TU08
  {
    PositivePressed = 1,	    // If positive key is pressed, mKeyStatus will sustain PRESSED.
    NegativePressed = 2,	    // If negative key is pressed, mKeyStatus will sustain PRESSED.
    CommonReleased = 3,	      // Changes to CommonReleased when released. constraints of Update.
    CommonNeutral = 4,	      // If released, and within range of mNeutralStatusThresholdValue, NEURTAL.
  };

  EDyAxisInputStatus  mKeyStatus    = EDyAxisInputStatus::CommonNeutral;

  /// container key name must be same as structure's name. (Camel)
  std::string		          mAxisSpecifierName  = MDY_INITIALIZE_EMPTYSTR;
  std::vector<EDyButton>  mNegativeButtonId   = {};
  std::vector<EDyButton>  mPositiveButtonId   = {};

  /// Gravity to drag value down to neutral zone.
  TF32 mGravity                     = 1000.f;
  /// The range of neutral zone.
  TF32 mNeutralStatusThresholdValue = 0.001f;
  /// Value moves along with mNegativeButtonId/pos/gravity.
  TF32 mAxisValue                   = 0.f;

  ///
  /// @brief Construct axis information from `DAxis`.
  /// @param axisInfo Information from setting (MDySetting).
  ///
  DDyAxisBindingInformation(_MIN_ const DDySettingInput::DAxis& axisInfo);
};

using EDyInputAxisStatus = DDyAxisBindingInformation::EDyAxisInputStatus;

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_KEY_BINDING_INFORMATION_H
