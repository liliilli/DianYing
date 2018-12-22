#ifndef GUARD_DY_META_TYPE_EDYINPUT_H
#define GUARD_DY_META_TYPE_EDYINPUT_H
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

#include <Dy/Helper/GlobalType.h>
#include <nlohmann/json_fwd.hpp>

#if (defined(DELETE) == true)
#undef DELETE
#endif

namespace dy
{

///
/// @enum EDyKeyboard
/// @brief Keyboard key specifier type value.
///
enum class EDyKeyboard : TI32
{
  NoneError = 0x0FFFFFFF,

  Right   = 262,
  Left    = 263,
  Down    = 264,
  Up      = 265,

  A       = 65,
  B       = 66,
  C       = 67,
  D       = 68,
  E       = 69,
  F       = 70,
  G       = 71,
  H       = 72,
  I       = 73,
  J       = 74,
  K       = 75,
  L       = 76,
  M       = 77,
  N       = 78,
  O       = 79,
  P       = 80,
  Q       = 81,
  R       = 82,
  S       = 83,
  T       = 84,
  U       = 85,
  V       = 86,
  W       = 87,
  X       = 88,
  Y       = 89,
  Z       = 90,

  _0 = 48,
  _1 = 49,
  _2 = 50,
  _3 = 51,
  _4 = 52,
  _5 = 53,
  _6 = 54,
  _7 = 55,
  _8 = 56,
  _9 = 57,

  ESCAPE = 256,
  ENTER = 257,

  LSHIFT = 340,
  LCTRL = 341,
  LALT = 342,

  INSERT = 260,
  DELETE = 261,
  PAGEUP = 266,
  PAGEDOWN = 267,
  HOME = 268,
  END = 269,

  MINUS = 45,
  EQUAL = 61,
  LBRACKET = 91,
  RBRACKET = 93,

  SEMICOLON = 59,
  APOSTROPHE = 39,
  COMMA = 44,
  PERIOD = 46,
  SLASH = 47,

  F1 = 290,
  F2 = 291,
  F3 = 292,
  F4 = 293,
  F5 = 294,
  F6 = 295,
  F7 = 296,
  F8 = 297,
  F9 = 298,
  F10 = 299,
  F11 = 300,
  F12 = 301,

  CAPSLOCK = 280,
  TAB = 258,
  BACKSPACE = 259,
  BACLSLASH = 92,
  GBACCENT = 96
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const EDyKeyboard& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyKeyboard& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_EDYINPUT_H