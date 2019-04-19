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

#include <array>
#include <DInputButton.h>

namespace dy::base
{

/// @enum ELowKeyboard
/// @brief Low-level keyboard enum value. (keycode)
/// This enum is inspired from GLFW3 input system.
enum ELowKeyboard
{
  Dy___Error = -1

  ,Dy_Key_Space         = 32
  ,Dy_Key_Apostrophe    = 39  /* ' */
  ,Dy_Key_Comma         = 44  /* , */
  ,Dy_Key_Minus         = 45  /* - */
  ,Dy_Key_Period        = 46  /* . */
  ,Dy_Key_Slash         = 47  /* / */
  ,Dy_Key_0             = 48
  ,Dy_Key_1             = 49
  ,Dy_Key_2             = 50
  ,Dy_Key_3             = 51
  ,Dy_Key_4             = 52
  ,Dy_Key_5             = 53
  ,Dy_Key_6             = 54
  ,Dy_Key_7             = 55
  ,Dy_Key_8             = 56
  ,Dy_Key_9             = 57
  ,Dy_Key_Semicolon     = 59  /* ; */
  ,Dy_Key_Equal         = 61  /* = */
  ,Dy_Key_A             = 65
  ,Dy_Key_B             = 66
  ,Dy_Key_C             = 67
  ,Dy_Key_D             = 68
  ,Dy_Key_E             = 69
  ,Dy_Key_F             = 70
  ,Dy_Key_G             = 71
  ,Dy_Key_H             = 72
  ,Dy_Key_I             = 73
  ,Dy_Key_J             = 74
  ,Dy_Key_K             = 75
  ,Dy_Key_L             = 76
  ,Dy_Key_M             = 77
  ,Dy_Key_N             = 78
  ,Dy_Key_O             = 79
  ,Dy_Key_P             = 80
  ,Dy_Key_Q             = 81
  ,Dy_Key_R             = 82
  ,Dy_Key_S             = 83
  ,Dy_Key_T             = 84
  ,Dy_Key_U             = 85
  ,Dy_Key_V             = 86
  ,Dy_Key_W             = 87
  ,Dy_Key_X             = 88
  ,Dy_Key_Y             = 89
  ,Dy_Key_Z             = 90
  ,Dy_Key_LeftBracket   = 91  /* [ */
  ,Dy_Key_Backslash     = 92  /* \ */
  ,Dy_Key_RightBracket  = 93  /* ] */
  ,Dy_Key_GraveAccent   = 96  /* ` */
  ,Dy_Key_World1        = 161 /* Non-Us #1 */
  ,Dy_Key_World2        = 162 /* Non-Us #2 */

  /* Function Keys */
  ,Dy_Key_Escape        = 256
  ,Dy_Key_Enter         = 257
  ,Dy_Key_Tab           = 258
  ,Dy_Key_Backspace     = 259
  ,Dy_Key_Insert        = 260
  ,Dy_Key_Delete        = 261
  ,Dy_Key_Right         = 262
  ,Dy_Key_Left          = 263
  ,Dy_Key_Down          = 264
  ,Dy_Key_Up            = 265
  ,Dy_Key_PageUp        = 266
  ,Dy_Key_PageDown      = 267
  ,Dy_Key_Home          = 268
  ,Dy_Key_End           = 269
  ,Dy_Key_CapsLock      = 280
  ,Dy_Key_ScrollLock    = 281
  ,Dy_Key_NumLock       = 282
  ,Dy_Key_PrintScreen   = 283
  ,Dy_Key_Pause         = 284
  ,Dy_Key_F1            = 290
  ,Dy_Key_F2            = 291
  ,Dy_Key_F3            = 292
  ,Dy_Key_F4            = 293
  ,Dy_Key_F5            = 294
  ,Dy_Key_F6            = 295
  ,Dy_Key_F7            = 296
  ,Dy_Key_F8            = 297
  ,Dy_Key_F9            = 298
  ,Dy_Key_F10           = 299
  ,Dy_Key_F11           = 300
  ,Dy_Key_F12           = 301
  ,Dy_Key_F13           = 302
  ,Dy_Key_F14           = 303
  ,Dy_Key_F15           = 304
  ,Dy_Key_F16           = 305
  ,Dy_Key_F17           = 306
  ,Dy_Key_F18           = 307
  ,Dy_Key_F19           = 308
  ,Dy_Key_F20           = 309
  ,Dy_Key_F21           = 310
  ,Dy_Key_F22           = 311
  ,Dy_Key_F23           = 312
  ,Dy_Key_F24           = 313
  ,Dy_Key_F25           = 314
  ,Dy_Key_Kp0           = 320
  ,Dy_Key_Kp1           = 321
  ,Dy_Key_Kp2           = 322
  ,Dy_Key_Kp3           = 323
  ,Dy_Key_Kp4           = 324
  ,Dy_Key_Kp5           = 325
  ,Dy_Key_Kp6           = 326
  ,Dy_Key_Kp7           = 327
  ,Dy_Key_Kp8           = 328
  ,Dy_Key_Kp9           = 329
  ,Dy_Key_KpDecimal     = 330
  ,Dy_Key_KpDivide      = 331
  ,Dy_Key_KpMultiply    = 332
  ,Dy_Key_KpSubtract    = 333
  ,Dy_Key_KpAdd         = 334
  ,Dy_Key_KpEnter       = 335
  ,Dy_Key_KpEqual       = 336
  ,Dy_Key_LeftShift     = 340
  ,Dy_Key_LeftControl   = 341
  ,Dy_Key_LeftAlt       = 342
  ,Dy_Key_LeftSuper     = 343
  ,Dy_Key_RightShift    = 344
  ,Dy_Key_RightControl  = 345
  ,Dy_Key_RightAlt      = 346
  ,Dy_Key_RightSuper    = 347
  ,Dy_Key_Menu          = 348
};

} /// ::dy::base namespace