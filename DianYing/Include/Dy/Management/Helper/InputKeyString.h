#ifndef GUARD_DY_MANAGEMENT_HELPER_INPUTKEYSTRING_H
#define GUARD_DY_MANAGEMENT_HELPER_INPUTKEYSTRING_H
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

#include <array>
#include <optional>

#include <Dy/Helper/Macroes.h>
#include <Dy/Meta/Type/EDyInput.h>

namespace dy
{

using TInputKeyPair = std::pair<std::string_view, TI32>;

constexpr std::array<TInputKeyPair, 77> sKeyInputString =
{
  TInputKeyPair\
  {"KB_RIGHT", 262},
  {"KB_LEFT", 263},
  {"KB_DOWN", 264},
  {"KB_UP", 265},

  {"KB_A", 65},
  {"KB_B", 66},
  {"KB_C", 67},
  {"KB_D", 68},
  {"KB_E", 69},
  {"KB_F", 70},
  {"KB_G", 71},
  {"KB_H", 72},
  {"KB_I", 73},
  {"KB_J", 74},
  {"KB_K", 75},
  {"KB_L", 76},
  {"KB_M", 77},
  {"KB_N", 78},
  {"KB_O", 79},
  {"KB_P", 80},
  {"KB_Q", 81},
  {"KB_R", 82},
  {"KB_S", 83},
  {"KB_T", 84},
  {"KB_U", 85},
  {"KB_V", 86},
  {"KB_W", 87},
  {"KB_X", 88},
  {"KB_Y", 89},
  {"KB_Z", 90},

  {"KB_0", 48},
  {"KB_1", 49},
  {"KB_2", 50},
  {"KB_3", 51},
  {"KB_4", 52},
  {"KB_5", 53},
  {"KB_6", 54},
  {"KB_7", 55},
  {"KB_8", 56},
  {"KB_9", 57},

  {"KB_ESCAPE", 256},
  {"KB_ENTER", 257},

  {"KB_LSHIFT", 340},
  {"KB_LCTRL", 341},
  {"KB_LALT", 342},

  {"KB_INSERT", 260},
  {"KB_DELETE", 261},
  {"KB_PAGEUP", 266},
  {"KB_PAGEDOWN", 267},
  {"KB_HOME", 268},
  {"KB_END", 269},

  {"KB_MINUS", 45},
  {"KB_EQUAL", 61},
  {"KB_LBRACKET", 91},
  {"KB_RBRACKET", 93},

  {"KB_SEMICOLON", 59},
  {"KB_APOSTROPHE", 39},
  {"KB_COMMA", 44},
  {"KB_PERIOD", 46},
  {"KB_SLASH", 47},

  {"KB_F1", 290},
  {"KB_F2", 291},
  {"KB_F3", 292},
  {"KB_F4", 293},
  {"KB_F5", 294},
  {"KB_F6", 295},
  {"KB_F7", 296},
  {"KB_F8", 297},
  {"KB_F9", 298},
  {"KB_F10", 299},
  {"KB_F11", 300},
  {"KB_F12", 301},

  {"KB_CAPSLOCK", 280},
  {"KB_TAB", 258},
  {"KB_BACKSPACE", 259},
  {"KB_BACLSLASH", 92},
  {"KB_GBACCENT", 96}
};

///
/// @brief  Get key uid string for glfw input access.
/// @param  keyString Key string from json information string.
/// @return If exists, return value.
///
inline std::optional<TI32> DyGetKeyUidValue(const std::string& keyString)
{
#if (defined(_WIN32) == true) && (defined(MDY_PLATFORM_FLAG_WINDOWS) == true)
  for (auto& [string, uid] : sKeyInputString)
  {
    if (keyString == string) { return uid; }
  }

  return std::nullopt;
#elif defined(__linux__) && defined(MDY_PLATFORM_FLAG_LINUX)
  /// @todo LINUX GET KEY UID VALUE BODY
#elif defined(MDY_PLATFORM_FLAG_MACOS)
  /// @todo MACOS GET KEY UID VALUE BODY
#else
  static_assert(false, "Need to specify platform macro.");
#endif
}

///
/// @brief  Convert `keyString` to appropriate `EDyKeyboard` value. If found nothing, just return Error value.
/// @param  keyString String value.
/// @return Proper `EDyKeyboard` value or `EDyKeyboard::NoneError`.
///
inline MDY_NODISCARD EDyKeyboard DyConvertToEDyKeyboard(_MIN_ const std::string& keyString) noexcept
{
  std::string key = keyString;
  std::transform(key.begin(), key.end(), key.begin(), ::toupper);

  for (auto& [string, uid] : sKeyInputString)
  {
    if (key == string) { return static_cast<EDyKeyboard>(uid); }
  }

  return EDyKeyboard::NoneError;
}

///
/// @brief  Convert `EDyKeyboard` to plain string. This function is reverse version of `DyConvertToEDyKeyboard`,
/// @param  keyEnum Enumeration value.
/// @return Plain string or empty value when input is NoneError value.
///
inline MDY_NODISCARD std::string DyConvertToPlainString(_MIN_ const EDyKeyboard keyEnum) noexcept
{
  const TI32 keyUid = static_cast<std::underlying_type_t<EDyKeyboard>>(keyEnum);
  for (auto& [string, uid] : sKeyInputString)
  {
    if (keyUid == uid) { return MSVSTR(string); }
  }

  MDY_UNEXPECTED_BRANCH();
  return "";
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_HELPER_INPUTKEYSTRING_H