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
#include <array>

#include <FWindowsPlatform.h>
#include <PLowInputMousePos.h>
#include <cassert>

static wchar_t szWindowClass[] = L"win32app";
const wchar_t* windowName = L"Gainput basic sample";
bool doExit = false;

std::unique_ptr<dy::APlatformBase> platform = nullptr;
HWND gHwnd; // Window handle. 

int WINAPI WinMain(
  [[maybe_unused]] HINSTANCE hInstance, 
  [[maybe_unused]] HINSTANCE hPrevInstance, 
  [[maybe_unused]] LPSTR lpCmdLine, 
  [[maybe_unused]] int nCmdShow)
{
  platform = std::make_unique<dy::FWindowsPlatform>();
  platform->InitPlatform();
  platform->CreateConsoleWindow();

#ifdef _WIN32
  #ifdef CreateWindow
    #undef CreateWindow
  #endif
#endif

  dy::PWindowCreationDescriptor desc = {};
  desc.mIsWindowFullScreen = false;
  desc.mIsWindowResizable = true;
  desc.mIsWindowShouldFocus = true;
  desc.mIsWindowVisible = true;
  desc.mWindowName = "Win32App";
  desc.mWindowWidth = 800;
  desc.mWindowHeight = 600;

  auto optRes = platform->CreateWindow(desc);
  assert(optRes.has_value() == true);

#if 0
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
		MessageBox(NULL, L"Call to RegisterClassEx failed!", L"Gainput basic sample", NULL);
		return 1;
	}

	gHwnd = CreateWindow(
    szWindowClass,
    windowName,
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
    800, 600,
    NULL, NULL, hInstance, NULL);

	if (!gHwnd)
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", L"Gainput basic sample", NULL);
		return 1;
	}

	ShowWindow(gHwnd, nCmdShow);
	UpdateWindow(gHwnd);
#endif

  auto& input = platform->GetInputManager();
  input.SetMousePosFeatureState(dy::base::ELowMousePosState::Normal);

	while (platform->CanShutdown() == false)
	{
    platform->PollEvents();
	}

  platform->RemoveAllWindow();
  platform->RemoveConsoleWindow();
  platform->ReleasePlatform();
	return 0;
}
