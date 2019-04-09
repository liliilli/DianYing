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

#ifdef DELETE
#undef DELETE
#endif

namespace dy::base
{

/// @enum EInputButton
/// @brief Button specifier type value.
enum EInputButton : unsigned char 
{
  Right, Left, Down, Up,

  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
  NumKp0, NumKp1, NumKp2, NumKp3, NumKp4, NumKp5, NumKp6, NumKp7, NumKp8, NumKp9,

  ESCAPE, ENTER, SPACE,
  LSHIFT, LCTRL, LALT,

  INSERT, DELETE, PAGEUP, PAGEDOWN, HOME, END,
  MINUS, EQUAL, LBRACKET, RBRACKET,

  SEMICOLON, APOSTROPHE, COMMA, PERIOD, SLASH,

  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  CAPSLOCK, TAB, BACKSPACE, BACKSLASH, GBACCENT,

  Mouse0Lmb, Mouse1Rmb, Mouse2Mid, 
  Mouse3, Mouse4, Mouse5, Mouse6, Mouse7, 

  Joystick0, Joystick1, Joystick2, Joystick3, 
  Joystick4, Joystick5, Joystick6, Joystick7, 
  Joystick8, Joystick9, Joystick10, Joystick11, 
  Joystick12, Joystick13, Joystick14, Joystick15, 
  Joystick16, Joystick17, 

  __Error
};

} /// ::dy::base namespace
