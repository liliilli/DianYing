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

#include <cstdint>
#include <string>

namespace dy
{

///
/// @enum EDyInputType
/// @brief The type of each key binding.
///
enum class EDyInputType
{
  NoneDoNothing,
  Keyboard,
  Mouse,
  Joystick
};

///
/// @struct PDyKeyBindingConstructionDescriptor
/// @brief Construction descriptor for DDyKeyBindingInfomation type.
///
struct PDyKeyBindingConstructionDescriptor final
{
  std::string   mKeyName              = "";
  int32_t       mNegativeButtonId     = MDY_NOT_INITIALIZED_M1;
  int32_t       mPositiveButtonId     = MDY_NOT_INITIALIZED_M1;
  float         mToNeutralGravity     = 1000.0f;
  float         mNeturalThreshold     = 0.1f;

  EDyInputType  mKeyType              = EDyInputType::NoneDoNothing;
  bool          mIsEnabledRepeatKey   = false;
};

///
/// @struct DDyKeyBindingInformation
/// @brief Stores key info to use in this game, and overall application.
/// You can bind arbitary (Supported on GLFW) key into key container, and get values from
/// them to call input_event handler.
///
/// If the user presses negative key, key information instance bound to key container
/// set value to -1.0f. Inversely as positive key is.
/// And the user releases negative or positive key, key information instance let values
/// go to neurtal zone. (within range of mNeutralStatusThresholdValue)
///
struct DDyKeyBindingInformation
{
  using TNegativeButton = int32_t;
  using TPositiveButton = int32_t;

  ///
  /// @enum EDyKeyInputStatus
  /// @brief Indicates key status of keys.
  ///
  enum class EDyKeyInputStatus : int
  {
    PositivePressed = 1,	    // If positive key is pressed, mKeyStatus will sustain PRESSED.
    NegativePressed = 2,	    // If negative key is pressed, mKeyStatus will sustain PRESSED.
    CommonReleased = 3,	      // Changes to CommonReleased when released. constraints of Update.
    CommonNeutral = 4,	      // If released, and within range of mNeutralStatusThresholdValue, NEURTAL.
    PositiveRepeated = 5,
    NegativeRepeated = 6
  };

  EDyInputType        mKeyType            = EDyInputType::NoneDoNothing;
  EDyKeyInputStatus   mKeyStatus          = EDyKeyInputStatus::CommonNeutral;
  bool			          mIsRepeatKey        = true;

  /// container key name must be same as structure's name. (Camel)
  std::string		      mKeyName            = "";
  TNegativeButton     mNegativeButtonId   = MDY_NOT_INITIALIZED_M1;
  TNegativeButton     mPositiveButtonId   = MDY_NOT_INITIALIZED_M1;

  /// Gravity to drag value down to neutral zone.
  float			mToNeutralGravity             = 1000.f;
  /// The range of neutral zone.
  float			mNeutralStatusThresholdValue  = 0.001f;
  /// Value moves along with mNegativeButtonId/pos/gravity.
  float			mAxisValue                    = 0.f;

  ///
  /// @brief constructor with descriptor.
  ///
  DDyKeyBindingInformation(const PDyKeyBindingConstructionDescriptor& descriptor) :
      mKeyType { descriptor.mKeyType },
      mIsRepeatKey( descriptor.mIsEnabledRepeatKey ),
      mKeyName ( descriptor.mKeyName ),
      mNegativeButtonId( descriptor.mNegativeButtonId ),
      mPositiveButtonId( descriptor.mPositiveButtonId ),
      mToNeutralGravity( descriptor.mToNeutralGravity ),
      mNeutralStatusThresholdValue( descriptor.mNeturalThreshold )
  {
    if (this->mKeyType == EDyInputType::NoneDoNothing ||
        this->mKeyName.empty() ||
        (this->mNegativeButtonId == MDY_NOT_INITIALIZED_M1 && this->mPositiveButtonId == MDY_NOT_INITIALIZED_M1) ||
        this->mToNeutralGravity == 0.f ||
        this->mNeutralStatusThresholdValue < 0.f)
    {
      throw std::runtime_error("Key constraint of key binding desciptor is not satisfied.");
    }
  }
};

inline EDyInputType GetKeyType(const std::string& token) noexcept
{
  if (token == "KB") return EDyInputType::Keyboard;
  if (token == "MS") return EDyInputType::Mouse;
  if (token == "JS") return EDyInputType::Joystick;
                     return EDyInputType::NoneDoNothing;
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_KEY_BINDING_INFORMATION_H
