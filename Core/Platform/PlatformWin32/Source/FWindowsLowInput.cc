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

#include <FWindowsLowInput.h>
#include <PLowInputKeyboard.h>
#include <PLowInputMouseBtn.h>
#include <PLowInputMousePos.h>
#include <windowsx.h>

namespace 
{

std::array<int, 512> sKeyKeyCodes;
std::array<int, 512> sKeyScanCodes;

/// @brief 
void CreateKeyboardTables()
{
  using namespace dy::base;
  sKeyKeyCodes[0x00B] = Dy_Key_0;
  sKeyKeyCodes[0x002] = Dy_Key_1;
  sKeyKeyCodes[0x003] = Dy_Key_2;
  sKeyKeyCodes[0x004] = Dy_Key_3;
  sKeyKeyCodes[0x005] = Dy_Key_4;
  sKeyKeyCodes[0x006] = Dy_Key_5;
  sKeyKeyCodes[0x007] = Dy_Key_6;
  sKeyKeyCodes[0x008] = Dy_Key_7;
  sKeyKeyCodes[0x009] = Dy_Key_8;
  sKeyKeyCodes[0x00A] = Dy_Key_9;
  sKeyKeyCodes[0x01E] = Dy_Key_A;
  sKeyKeyCodes[0x030] = Dy_Key_B;
  sKeyKeyCodes[0x02E] = Dy_Key_C;
  sKeyKeyCodes[0x020] = Dy_Key_D;
  sKeyKeyCodes[0x012] = Dy_Key_E;
  sKeyKeyCodes[0x021] = Dy_Key_F;
  sKeyKeyCodes[0x022] = Dy_Key_G;
  sKeyKeyCodes[0x023] = Dy_Key_H;
  sKeyKeyCodes[0x017] = Dy_Key_I;
  sKeyKeyCodes[0x024] = Dy_Key_J;
  sKeyKeyCodes[0x025] = Dy_Key_K;
  sKeyKeyCodes[0x026] = Dy_Key_L;
  sKeyKeyCodes[0x032] = Dy_Key_M;
  sKeyKeyCodes[0x031] = Dy_Key_N;
  sKeyKeyCodes[0x018] = Dy_Key_O;
  sKeyKeyCodes[0x019] = Dy_Key_P;
  sKeyKeyCodes[0x010] = Dy_Key_Q;
  sKeyKeyCodes[0x013] = Dy_Key_R;
  sKeyKeyCodes[0x01F] = Dy_Key_S;
  sKeyKeyCodes[0x014] = Dy_Key_T;
  sKeyKeyCodes[0x016] = Dy_Key_U;
  sKeyKeyCodes[0x02F] = Dy_Key_V;
  sKeyKeyCodes[0x011] = Dy_Key_W;
  sKeyKeyCodes[0x02D] = Dy_Key_X;
  sKeyKeyCodes[0x015] = Dy_Key_Y;
  sKeyKeyCodes[0x02C] = Dy_Key_Z;

  sKeyKeyCodes[0x028] = Dy_Key_Apostrophe;
  sKeyKeyCodes[0x02B] = Dy_Key_Backslash;
  sKeyKeyCodes[0x033] = Dy_Key_Comma;
  sKeyKeyCodes[0x00D] = Dy_Key_Equal;
  sKeyKeyCodes[0x029] = Dy_Key_GraveAccent;
  sKeyKeyCodes[0x01A] = Dy_Key_LeftBracket;
  sKeyKeyCodes[0x00C] = Dy_Key_Minus;
  sKeyKeyCodes[0x034] = Dy_Key_Period;
  sKeyKeyCodes[0x01B] = Dy_Key_RightBracket;
  sKeyKeyCodes[0x027] = Dy_Key_Semicolon;
  sKeyKeyCodes[0x035] = Dy_Key_Slash;
  sKeyKeyCodes[0x056] = Dy_Key_World2;

  sKeyKeyCodes[0x00E] = Dy_Key_Backspace;
  sKeyKeyCodes[0x153] = Dy_Key_Delete;
  sKeyKeyCodes[0x14F] = Dy_Key_End;
  sKeyKeyCodes[0x01C] = Dy_Key_Enter;
  sKeyKeyCodes[0x001] = Dy_Key_Escape;
  sKeyKeyCodes[0x147] = Dy_Key_Home;
  sKeyKeyCodes[0x152] = Dy_Key_Insert;
  sKeyKeyCodes[0x15D] = Dy_Key_Menu;
  sKeyKeyCodes[0x151] = Dy_Key_PageDown;
  sKeyKeyCodes[0x149] = Dy_Key_PageUp;
  sKeyKeyCodes[0x045] = Dy_Key_Pause;
  sKeyKeyCodes[0x146] = Dy_Key_Pause;
  sKeyKeyCodes[0x039] = Dy_Key_Space;
  sKeyKeyCodes[0x00F] = Dy_Key_Tab;
  sKeyKeyCodes[0x03A] = Dy_Key_CapsLock;
  sKeyKeyCodes[0x145] = Dy_Key_NumLock;
  sKeyKeyCodes[0x046] = Dy_Key_ScrollLock;
  sKeyKeyCodes[0x03B] = Dy_Key_F1;
  sKeyKeyCodes[0x03C] = Dy_Key_F2;
  sKeyKeyCodes[0x03D] = Dy_Key_F3;
  sKeyKeyCodes[0x03E] = Dy_Key_F4;
  sKeyKeyCodes[0x03F] = Dy_Key_F5;
  sKeyKeyCodes[0x040] = Dy_Key_F6;
  sKeyKeyCodes[0x041] = Dy_Key_F7;
  sKeyKeyCodes[0x042] = Dy_Key_F8;
  sKeyKeyCodes[0x043] = Dy_Key_F9;
  sKeyKeyCodes[0x044] = Dy_Key_F10;
  sKeyKeyCodes[0x057] = Dy_Key_F11;
  sKeyKeyCodes[0x058] = Dy_Key_F12;
  sKeyKeyCodes[0x064] = Dy_Key_F13;
  sKeyKeyCodes[0x065] = Dy_Key_F14;
  sKeyKeyCodes[0x066] = Dy_Key_F15;
  sKeyKeyCodes[0x067] = Dy_Key_F16;
  sKeyKeyCodes[0x068] = Dy_Key_F17;
  sKeyKeyCodes[0x069] = Dy_Key_F18;
  sKeyKeyCodes[0x06A] = Dy_Key_F19;
  sKeyKeyCodes[0x06B] = Dy_Key_F20;
  sKeyKeyCodes[0x06C] = Dy_Key_F21;
  sKeyKeyCodes[0x06D] = Dy_Key_F22;
  sKeyKeyCodes[0x06E] = Dy_Key_F23;
  sKeyKeyCodes[0x076] = Dy_Key_F24;
  sKeyKeyCodes[0x038] = Dy_Key_LeftAlt;
  sKeyKeyCodes[0x01D] = Dy_Key_LeftControl;
  sKeyKeyCodes[0x02A] = Dy_Key_LeftShift;
  sKeyKeyCodes[0x15B] = Dy_Key_LeftSuper;
  sKeyKeyCodes[0x137] = Dy_Key_PrintScreen;

  sKeyKeyCodes[0x138] = Dy_Key_RightAlt;
  sKeyKeyCodes[0x11D] = Dy_Key_RightControl;
  sKeyKeyCodes[0x036] = Dy_Key_RightShift;
  sKeyKeyCodes[0x15C] = Dy_Key_RightSuper;
  sKeyKeyCodes[0x150] = Dy_Key_Down;
  sKeyKeyCodes[0x14B] = Dy_Key_Left;
  sKeyKeyCodes[0x14D] = Dy_Key_Right;
  sKeyKeyCodes[0x148] = Dy_Key_Up;

  sKeyKeyCodes[0x052] = Dy_Key_Kp0;
  sKeyKeyCodes[0x04F] = Dy_Key_Kp1;
  sKeyKeyCodes[0x050] = Dy_Key_Kp2;
  sKeyKeyCodes[0x051] = Dy_Key_Kp3;
  sKeyKeyCodes[0x04B] = Dy_Key_Kp4;
  sKeyKeyCodes[0x04C] = Dy_Key_Kp5;
  sKeyKeyCodes[0x04D] = Dy_Key_Kp6;
  sKeyKeyCodes[0x047] = Dy_Key_Kp7;
  sKeyKeyCodes[0x048] = Dy_Key_Kp8;
  sKeyKeyCodes[0x049] = Dy_Key_Kp9;
  sKeyKeyCodes[0x04E] = Dy_Key_KpAdd;
  sKeyKeyCodes[0x053] = Dy_Key_KpDecimal;
  sKeyKeyCodes[0x135] = Dy_Key_KpDivide;
  sKeyKeyCodes[0x11C] = Dy_Key_KpEnter;
  sKeyKeyCodes[0x059] = Dy_Key_KpEqual;
  sKeyKeyCodes[0x037] = Dy_Key_KpMultiply;
  sKeyKeyCodes[0x04A] = Dy_Key_KpSubtract;

  for (int scancode = 0; scancode < 512;  scancode++)
  {
    if (sKeyKeyCodes[scancode] > 0)
    {
      sKeyScanCodes[sKeyKeyCodes[scancode]] = scancode;
    }
  }
}

} /// ::dy namespace

namespace dy
{

FWindowsLowInput::FWindowsLowInput()
{
  CreateKeyboardTables();
}

void FWindowsLowInput::UpdateKeyboard(void* descriptor)
{
  using namespace base;
  auto& desc = *static_cast<PLowInputKeyboard*>(descriptor);

  // Get keyCode from table.
  const auto keyCode = sKeyKeyCodes[HIWORD(desc.mLparam) & 0x1ff];

  if (desc.mWparam == VK_SNAPSHOT)
  {
    // When key is pressed with `PrintScreen` key.
    this->sLowKeyboards[keyCode].Update(EInputState::Pressed);
    this->sLowKeyboards[keyCode].Update(EInputState::Released);
  }
  else
  {
    // General case
    const auto keyState = ((desc.mMessage == WM_KEYDOWN) || (desc.mMessage == WM_SYSKEYDOWN))
      ? EInputState::Pressed
      : EInputState::Released;

    // Insert
    this->sLowKeyboards[keyCode].Update(keyState);
  }
}

void FWindowsLowInput::UpdateMouseButton(void* descriptor)
{
  using namespace base;
  auto& desc = *static_cast<PLowInputMouseBtn*>(descriptor);

  // Get mouse button code
  ELowMouseButton buttonCode = ELowMouseButton::DyMouse__Error;
  if (desc.mMessage == WM_LBUTTONDOWN || desc.mMessage == WM_LBUTTONUP)
  {
    buttonCode = ELowMouseButton::DyMouseButton1;
  }
  else if (desc.mMessage == WM_RBUTTONDOWN || desc.mMessage == WM_RBUTTONUP)
  {
    buttonCode = ELowMouseButton::DyMouseButton2;
  }
  else if (desc.mMessage == WM_MBUTTONDOWN || desc.mMessage == WM_MBUTTONUP)
  {
    buttonCode = ELowMouseButton::DyMouseButton3;
  }
  else
  {
    switch (GET_XBUTTON_WPARAM(desc.mWparam))
    {
    case XBUTTON1: buttonCode = ELowMouseButton::DyMouseButton4; break;
    case XBUTTON2: buttonCode = ELowMouseButton::DyMouseButton5; break;
    default: break;
    }
  }

  // Get state of button input.
  EInputState mouseButtonState = EInputState::Released;
  if ( desc.mMessage == WM_LBUTTONDOWN
    || desc.mMessage == WM_RBUTTONDOWN
    || desc.mMessage == WM_MBUTTONDOWN
    || desc.mMessage == WM_XBUTTONDOWN)
  {
    mouseButtonState = EInputState::Pressed;
  }

  // Insert
  this->sLowMouseButtons[buttonCode].Update(mouseButtonState);
}

void FWindowsLowInput::UpdateMousePos(void* descriptor)
{
  using namespace base;
  auto& desc = *static_cast<PLowInputMousePos*>(descriptor);

  // Get position.
  auto x = GET_X_LPARAM(desc.mLparam);
  auto y = GET_Y_LPARAM(desc.mLparam);

  // Branch
  switch (this->GetMousePosState())
  {
  case ELowMousePosState::Normal: 
  {
    // Try update position. When state is Off, just pass it.
    this->mLowMousePos.UpdatePosition(x, y);
  } break;
  case ELowMousePosState::Unlimited: 
  {
    // Get screen size.
    RECT rect;
    GetWindowRect(desc.mFocusedWindow, &rect);
   
    if (this->mLowMousePos.HasInitiallyMoved() == false)
    {
      // Initial virtual position starts (0, 0).
      this->mLowMousePos.UpdatePosition(0, 0);
    }
    else
    {
      // Get screen (width, height) from overall screen RECT.
      const auto screenWidth  = rect.right - rect.left;
      const auto screenHeight = rect.bottom - rect.top;
      const auto amountX = x - (screenWidth >> 1);
      const auto amountY = y - (screenHeight >> 1);

      // Add relatvie moving amount.
      auto [relativeX, relativeY] = this->mLowMousePos.GetPresentPosition();
      x = relativeX + amountX + 8; // MAGIC NUMBER 
      y = relativeY + amountY + 31;// MAGIC NUMBER

      // Try update position. When state is Off, just pass it.
      this->mLowMousePos.UpdatePosition(x, y);
    }

    // Restrictly align position into centeral position.
    const auto entireScreenHalfX = (rect.right + rect.left) >> 1;
    const auto entireScreenHalfY = (rect.top + rect.bottom) >> 1;

    POINT pos = { entireScreenHalfX, entireScreenHalfY };
    ClientToScreen(desc.mFocusedWindow, &pos);
    SetCursorPos(entireScreenHalfX, entireScreenHalfY);
  } break;
  default: break;
  }
}

} /// ::dy namespace