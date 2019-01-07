#ifndef GUARD_DY_META_TYPE_INPUT_EDYINPUTBUTTON_H
#define GUARD_DY_META_TYPE_INPUT_EDYINPUTBUTTON_H
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

#include <array>
#include <nlohmann/json_fwd.hpp>
#include <Dy/Helper/System/Macroes.h>
#include <Dy/Meta/Type/Input/EDyInputButton.h>

namespace dy
{

constexpr std::array<const char*, 114> kInputButtonSpecifierList =
{
  "KB_RIGHT", "KB_LEFT", "KB_DOWN", "KB_UP",

  "KB_A", "KB_B", "KB_C", "KB_D", "KB_E", "KB_F", "KB_G", "KB_H", "KB_I", "KB_J", "KB_K", "KB_L", "KB_M", 
  "KB_N", "KB_O", "KB_P", "KB_Q", "KB_R", "KB_S", "KB_T", "KB_U", "KB_V", "KB_W", "KB_X", "KB_Y", "KB_Z",

  "KB_0", "KB_1", "KB_2", "KB_3", "KB_4", "KB_5", "KB_6", "KB_7", "KB_8", "KB_9",
  "KB_KP0", "KB_KP1", "KB_KP2", "KB_KP3", "KB_KP4", "KB_KP5", "KB_KP6", "KB_KP7", "KB_KP8", "KB_KP9",

  "KB_ESCAPE", "KB_ENTER", "KB_SPACE",
  "KB_LSHIFT", "KB_LCTRL", "KB_LALT",

  "KB_INSERT", "KB_DELETE", "KB_PAGEUP", "KB_PAGEDOWN", "KB_HOME", "KB_END", 
  "KB_MINUS" , "KB_EQUAL" , "KB_LBRACKET", "KB_RBRACKET", 

  "KB_SEMICOLON", "KB_APOSTROPHE", "KB_COMMA", "KB_PERIOD", "KB_SLASH", 

  "KB_F1", "KB_F2", "KB_F3", "KB_F4", "KB_F5", "KB_F6", "KB_F7", "KB_F8", "KB_F9", "KB_F10", "KB_F11", "KB_F12",
  "KB_CAPSLOCK", "KB_TAB", "KB_BACKSPACE", "KB_BACLSLASH", "KB_GBACCENT",

  "MB_0", "MB_1", "MB_2", 
  "MB_3", "MB_4", "MB_5", "MB_6", "MB_7",

  "JS_0", "JS_1", "JS_2", "JS_3", 
  "JS_4", "JS_5", "JS_6", "JS_7", 
  "JS_8", "JS_9", "JS_10", "JS_11", 
  "JS_12", "JS_13", "JS_14", "JS_15", 
  "JS_16", "JS_17"
};

void to_json  (_MINOUT_ nlohmann::json& j,    _MIN_ const EDyInputButton& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ EDyInputButton& p);

} /// ::dy namespace

#endif /// GUARD_DY_META_TYPE_INPUT_EDYINPUTBUTTON_H