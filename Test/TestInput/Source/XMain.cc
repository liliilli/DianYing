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

#include <windowsx.h>
#include <FWindowsPlatform.h>
#include <PLowInputKeyboard.h>
#include <PLowInputMouseBtn.h>

static char szWindowClass[] = "win32app";
const char* windowName = "Gainput basic sample";
bool doExit = false;

std::unique_ptr<dy::APlatformBase> platform = nullptr;

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
      dy::PLowInputKeyboard desc;
      desc.mLparam  = lParam;
      desc.mWparam  = wParam;
      desc.mMessage = message;

      auto& input = platform->GetInputManager();
      input.UpdateKeyboard(&desc);
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
      dy::PLowInputMouseBtn desc;
      desc.mMessage = message;
      desc.mWparam  = wParam;

      auto& input = platform->GetInputManager();
      input.UpdateMouseButton(&desc);
    } break;
    case WM_MOUSEMOVE:
    {
      const int x = GET_X_LPARAM(lParam);
      const int y = GET_Y_LPARAM(lParam);



    } break;
    case WM_MOUSELEAVE:
    {

    } break;
    case WM_MOUSEWHEEL:
    {

    } break;
    case WM_MOUSEHWHEEL:
    {

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
  //CreateKeyboardTables();
  platform = std::make_unique<dy::FWindowsPlatform>();

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

	while (!doExit)
	{
		MSG msg;
		while (PeekMessage(&msg, hWnd,  0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
