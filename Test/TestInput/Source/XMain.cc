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

#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <array>

#define LOG(...) {char buf[256]; sprintf(buf, __VA_ARGS__); OutputDebugStringA(buf); }

#include <ELowKeyboard.h>
#include "ELowMouse.h"

static char szWindowClass[] = "win32app";
const char* windowName = "Gainput basic sample";
bool doExit = false;

std::array<int, 512> sKeyKeyCodes;
std::array<int, 512> sKeyScanCodes;

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	char greeting[] = "Hello, World!";

  using namespace dy::base;

	switch (message)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			TextOut(hdc, 5, 5, greeting, int(strlen(greeting)));
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			doExit = true;
			break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
      // Get keyCode from table.
      const auto keyCode = sKeyKeyCodes[HIWORD(lParam) & 0x1ff];

      if (wParam == VK_SNAPSHOT)
      {
        // When key is pressed with `PrintScreen` key.
        sLowKeyboards[keyCode].Update(EInputState::Pressed);
        sLowKeyboards[keyCode].Update(EInputState::Released);
      }
      else
      {
        // General case
        const auto keyState = ((message == WM_KEYDOWN) || (message == WM_SYSKEYDOWN))
          ? EInputState::Pressed
          : EInputState::Released;

        // Insert
        sLowKeyboards[keyCode].Update(keyState);
      }
    } break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
      // Get mouse button code
      ELowMouseButton buttonCode = ELowMouseButton::DyMouse__Error;
      if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP)
      {
        buttonCode = ELowMouseButton::DyMouseButton1;
      }
      else if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
      {
        buttonCode = ELowMouseButton::DyMouseButton2;
      }
      else if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
      {
        buttonCode = ELowMouseButton::DyMouseButton3;
      }
      else 
      {
        switch (GET_XBUTTON_WPARAM(wParam))
        {
        case XBUTTON1: buttonCode = ELowMouseButton::DyMouseButton4; break;
        case XBUTTON2: buttonCode = ELowMouseButton::DyMouseButton5; break;
        default: break;
        }
      }

      // Get state of button input.
      EInputState mouseButtonState = EInputState::Released;
      if (message == WM_LBUTTONDOWN
      ||  message == WM_RBUTTONDOWN
      ||  message == WM_MBUTTONDOWN
      ||  message == WM_XBUTTONDOWN)
      {
        mouseButtonState = EInputState::Pressed;
      }

      // Insert
      sLowMouseButtons[buttonCode].Update(mouseButtonState);
    } break;

		default: { return DefWindowProc(hWnd, message, wParam, lParam); }
	}
	return 0;
}

int WINAPI WinMain(
  HINSTANCE hInstance, 
  [[maybe_unused]] HINSTANCE hPrevInstance, 
  [[maybe_unused]] LPSTR lpCmdLine, 
  int nCmdShow)
{
  CreateKeyboardTables();

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = 0;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", "Gainput basic sample", NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(
    szWindowClass,
    windowName,
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
    800, 600,
    NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", "Gainput basic sample", NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Setup Gainput
#ifdef false
	gainput::InputManager manager;
	manager.SetDisplaySize(width, height);
	gainput::DeviceId mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
	gainput::DeviceId keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
	gainput::DeviceId padId = manager.CreateDevice<gainput::InputDevicePad>();

	gainput::InputMap map(manager);
	map.MapBool(ButtonMenu, keyboardId, gainput::KeyEscape);
	map.MapBool(ButtonConfirm, mouseId, gainput::MouseButtonLeft);
	map.MapFloat(MouseX, mouseId, gainput::MouseAxisX);
	map.MapFloat(MouseY, mouseId, gainput::MouseAxisY);
	map.MapBool(ButtonConfirm, padId, gainput::PadButtonB);
#endif
  
  //map.GetManager().Get

	while (!doExit)
	{
		MSG msg;
		while (PeekMessage(&msg, hWnd,  0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

#ifdef false
		// Check button states
		if (map.GetBoolWasDown(ButtonConfirm))
		{
			gainput::InputDevicePad* pad = static_cast<gainput::InputDevicePad*>(manager.GetDevice(padId));
			pad->Vibrate(1.0f, 0.0f);
		}
		if (map.GetBoolWasDown(ButtonMenu))
		{
			gainput::InputDevicePad* pad = static_cast<gainput::InputDevicePad*>(manager.GetDevice(padId));
			pad->Vibrate(0.0f, 0.0f);
		}

		if (map.GetBool(ButtonMenu) && map.GetBoolPrevious(ButtonMenu) == false)
		{
			LOG("Open Menu!!\n");
		}
		if (map.GetBoolWasDown(ButtonConfirm))
		{
			LOG("Confirmed!!\n");
		}

		if (map.GetFloatDelta(MouseX) != 0.0f || map.GetFloatDelta(MouseY) != 0.0f)
		{
			LOG("Mouse: %f, %f\n", map.GetFloat(MouseX), map.GetFloat(MouseY));
		}
#endif
	}

	return 0;
}
