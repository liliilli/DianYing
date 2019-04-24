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

#include <FWindowsPlatform.h>
#include <sstream>
#include <queue>
#include <cassert>

#include <EPlatform.h>
#include <FWindowsHandles.h>
#include <FWindowsDebug.h>
#include <FBtResourceHandle.h>
#include <FWindowsProfiling.h>
#include <FWindowsLowInput.h>
#include <PLowInputKeyboard.h>
#include <PLowInputMouseBtn.h>
#include <PLowInputMousePos.h>
#include <DWin32PostMessage.h>
#include <StringUtil/XUtility.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#include <atlconv.h>

#define IS_32
#include <Dbt.h> // DEV_BROADCAST_DEVICEINTERFACE_W ... HID devices.

#define LOG(...) {char buf[256]; sprintf(buf, __VA_ARGS__); OutputDebugStringA(buf); }

namespace 
{

/// @brief
dy::APlatformBase* platform = nullptr;

/// @brief 
std::queue<dy::DWin32PostMessage> sPostMessages = {};

  /// @brief
  DWORD GetWin32WindowStyle(const dy::PWindowCreationDescriptor& desc)
{
  DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  if (desc.mIsWindowFullScreen == true)
  {
    // The windows is a pop-up window. This style cannot be used with the WS_CHILD style.
    style |= WS_POPUP;
    return style;
  }

  // The window has a window menu on its title bar. The WS_CAPTION style must also be specified.
  // The window has a title bar (includes the WS_BORDER style).
  style |= WS_SYSMENU | WS_MINIMIZEBOX;
  style |= WS_CAPTION;

  if (desc.mIsWindowResizable == true)
  {
    // The window has a sizing border. Same as the WS_SIZEBOX style.
    style |= WS_MAXIMIZEBOX | WS_THICKFRAME;
  }

  return style;
}

/// @brief
DWORD GetWin32WindowExStyle(const dy::PWindowCreationDescriptor& desc)
{
  DWORD style = WS_EX_APPWINDOW;

  if (desc.mIsWindowFullScreen == true)
  {
    style |= WS_EX_TOPMOST;
  }

  return style;
}

/// @brief
std::pair<int, int> GetWin32FullWindowSize(
  DWORD style, DWORD exStyle, 
  int width, int height, int dpi)
{
  RECT rect = { 0, 0, width, height };

  //if (_glfwIsWindows10AnniversaryUpdateOrGreaterWin32())
  AdjustWindowRectExForDpi(&rect, style, FALSE, exStyle, dpi);
  //else
    //AdjustWindowRectEx(&rect, style, FALSE, exStyle);

  return { rect.right - rect.left, rect.bottom - rect.top };
}

LRESULT CALLBACK OnWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	wchar_t greeting[] = L"Hello, World!";

  using namespace dy::base;

	switch (message)
	{
		case WM_PAINT:
    {
			hdc = BeginPaint(hWnd, &ps);
			TextOut(hdc, 5, 5, greeting, int(lstrlen(greeting)));
			EndPaint(hWnd, &ps);
    } break;
    // @reference http://ssb777.blogspot.com/2009/07/mfc-wmclose-wmdestroy-wmquit.html
    case WM_CLOSE:
    {
      dy::DWin32PostMessage deleteMessage;
      deleteMessage.mType = decltype(deleteMessage.mType)::DeleteWindow;
      deleteMessage.mData = hWnd;
      // Assign deletion message of given hWnd.
      sPostMessages.push(deleteMessage);
    } break;
		case WM_DESTROY: { } break;
    case WM_KEYDOWN: case WM_KEYUP:
    case WM_SYSKEYDOWN: case WM_SYSKEYUP:
    {
      dy::PLowInputKeyboard desc;
      desc.mLparam  = lParam;
      desc.mWparam  = wParam;
      desc.mMessage = message;

      auto& input = platform->GetInputManager();
      input.UpdateKeyboard(&desc);
    } break;
    case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN: case WM_XBUTTONDOWN:
    case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: case WM_XBUTTONUP:
    {
      dy::PLowInputMouseBtn desc;
      desc.mMessage = message;
      desc.mWparam  = wParam;

      auto& input = platform->GetInputManager();
      input.UpdateMouseButton(&desc);
    } break;
    case WM_MOUSEMOVE:
    {
      dy::PLowInputMousePos desc;
      desc.mFocusedWindow = hWnd;
      desc.mLparam = lParam;

      platform->GetInputManager().UpdateMousePos(&desc);

      // Debug
      if (auto optPos = platform->GetInputManager().GetMousePos(); 
          optPos.has_value() == true)
      {
        LOG("Mouse Position : (%3d, %3d)\n", (*optPos).first, (*optPos).second);
      }
      if (auto optAmnt = platform->GetInputManager().GetMousePosMovement();
          optAmnt.has_value() == true)
      {
        LOG("Mouse Movement Amount : (%3d, %3d)\n", (*optAmnt).first, (*optAmnt).second);
      }
      // Dispensable tracing event.
      {
        TRACKMOUSEEVENT mouseEvent;
        mouseEvent.cbSize     = sizeof(mouseEvent);
        mouseEvent.dwFlags    = TME_LEAVE;
        mouseEvent.hwndTrack  = hWnd;
        TrackMouseEvent(&mouseEvent);
      }
    } break;
    case WM_MOUSELEAVE:
    case WM_MOUSEWHEEL: // Vertical scroll.
    case WM_MOUSEHWHEEL: // Horizontal scroll.
    {

    } break;
    default: return DefWindowProcW(hWnd, message, wParam, lParam); 
	}

  return 0;
}

} /// anonymous namespace

namespace dy
{

FWindowsPlatform::FWindowsPlatform() 
  : APlatformBase{EPlatform::Windows}
{
  this->mHandle = std::make_unique<FWindowsHandles>(GetCurrentProcess());
  this->mDebug  = std::make_unique<FWindowsDebug>();
  this->mProfiling = std::make_unique<FWindowsProfiling>(
    static_cast<FWindowsHandles&>(*this->mHandle).mMainProcess);
  this->mLowInput = std::make_unique<FWindowsLowInput>();
}

FWindowsPlatform::~FWindowsPlatform() = default;

void FWindowsPlatform::SetWindowTitle(const DWindowHandle& handle, const std::string& newTitle)
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  
  // If handle not find, just return.
  auto it = handleContainer.mWindowHandles.find(handle.mHandleUuid);
  if (it == handleContainer.mWindowHandles.end())
  {
    return;
  }

  auto& [uuid, hwnd] = *it;
  const auto flag = SetWindowText(hwnd, ConvertStringToWString(newTitle).c_str());
  if (flag == 0)
  {
    LOG("Failed to set window title, %s.\n", newTitle.c_str());
    return;
  }

#if 0
  // Temporary
  if (handleContainer.mGlfwWindow == nullptr) { return; }
  glfwSetWindowTitle(handleContainer.mGlfwWindow, newTitle.c_str());

#endif
}

std::string FWindowsPlatform::GetWindowTitle(const DWindowHandle& handle) const
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);

  // If handle not find, just return.
  const auto it = handleContainer.mWindowHandles.find(handle.mHandleUuid);
  if (it == handleContainer.mWindowHandles.end())
  {
    return "";
  }

  // Get title name from main window handle.
  //std::wstring titleName = {256, 0};
  wchar_t titleName[256] = {0};
  auto& [uuid, hwnd] = *it;
  const auto length = GetWindowText(hwnd, titleName, 255);

  // If failed to get window text, just return empty string.
  if (length == 0)
  {
    const auto errorFlag = GetLastError();
    return "";
  }

  // Narrow string into std::string (<char>) and return.
  return ConvertWStringToString(titleName);
}

uint32_t FWindowsPlatform::GetWindowHeight(const DWindowHandle& handle) const
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  const auto it = handleContainer.mWindowHandles.find(handle.mHandleUuid);
  if (it == handleContainer.mWindowHandles.end())
  {
    return 0;
  }

#if 0
  // Temporary code
  if (handleContainer.mGlfwWindow == nullptr) { return 0; }

  int width, height;
  glfwGetWindowSize(handleContainer.mGlfwWindow, &width, &height);

  return height;
#endif

  auto& [uuid, hwnd] = *it;
  // Get RECT from window handle.
  // Do not use GetWindowRect (global)
  RECT rect;
  const auto isSucceeded = GetClientRect(hwnd, &rect);
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
    return 0;
  }

  // Return values.
  return rect.bottom - rect.top;
}

uint32_t FWindowsPlatform::GetWindowWidth(const DWindowHandle& handle) const
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  const auto it = handleContainer.mWindowHandles.find(handle.mHandleUuid);
  if (it == handleContainer.mWindowHandles.end())
  {
    return 0;
  }

#if 0
  // Temporary code
  if (handleContainer.mGlfwWindow == nullptr) { return 0; }

  int width, height;
  glfwGetWindowSize(handleContainer.mGlfwWindow, &width, &height);

  return width;
#endif

  auto& [uuid, hwnd] = *it;
  // Get RECT from main window.
  RECT rect;
  const auto isSucceeded = GetClientRect(hwnd, &rect);
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
    return 0;
  }

  // Return values.
  return rect.right - rect.left;
}

void FWindowsPlatform::ResizeWindow(const DWindowHandle& handle, uint32_t width, uint32_t height)
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  const auto it = handleContainer.mWindowHandles.find(handle.mHandleUuid);
  if (it == handleContainer.mWindowHandles.end())
  {
    return;
  }

#ifdef false
  // Temporary code
  if (handleContainer.mGlfwWindow == nullptr) { return; }
  // Set size.
  glfwSetWindowSize(handleContainer.mGlfwWindow, width, height);
#endif

  auto& [uuid, hwnd] = *it;

  // Set RECT as original (width, height).
  RECT rect = {};
  rect.bottom = height;
  rect.right = width;
  
  // Get style of given hwnd.
  const DWORD style = GetWindowLong(hwnd, GWL_STYLE);
  const DWORD exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  const HMENU menu = GetMenu(hwnd);

  // Get adjusted window rect.
  const auto flag = AdjustWindowRectEx(&rect, style, menu ? TRUE : FALSE, exStyle);
  if (flag == 0)
  {
    LOG("Failed to resize window. Failed to adjust window rect in WIN32.\n");
    return;
  }
  
  // Resize window without moving position and zorder etc.
  const auto isSucceeded = SetWindowPos(
    hwnd, nullptr,
    0, 0, 
    rect.right - rect.left, rect.bottom - rect.top, 
    SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOMOVE);
  if (isSucceeded == 0)
  {
    LOG("Failed to resize window. Failed to resize with win32 api, SetWindowPos.\n");
  }
}

bool FWindowsPlatform::CreateConsoleWindow()
{
  if (this->mIsConsoleWindowCreated == true) { return false; } 

  // Allocate console and forward stdout to console.
  if (AllocConsole() == false)
  {
    MessageBox(
      nullptr, 
      L"The console window was not created.", 
      nullptr, MB_ICONEXCLAMATION);
    return false;
  }

  // Open console.
  freopen_s(&this->mFdConsole, "CONOUT$", "w", stdout);
  this->mIsConsoleWindowCreated = true;
  return true;
}

bool FWindowsPlatform::RemoveConsoleWindow()
{
  if (this->mIsConsoleWindowCreated == false) { return false; } 

  // Release console.
  if (fclose(this->mFdConsole) != 0)
  {
     MessageBox(
      nullptr, 
      L"Unexpected error occurred when closing console.", 
      nullptr, MB_ICONEXCLAMATION);   
     return false;
  }

  if (FreeConsole() == false)
  {
    MessageBox(
      nullptr, 
      L"Failed to free console resource.", 
      nullptr, MB_ICONEXCLAMATION);
    return false;
  }

  this->mIsConsoleWindowCreated = false;
  return true;
}

#undef FindResource

std::unique_ptr<ABtResourceBase> 
FWindowsPlatform::FindResource(int id, EXPR_E(EBtResource) type)
{
#define FindResource FindResourceW
  USES_CONVERSION;
  HRSRC hResource = FindResource(
    nullptr, 
    MAKEINTRESOURCE(id), 
    A2W(EBtResource::ToString(type))
  );
  if (hResource == nullptr) { return nullptr; }

  return std::make_unique<FBtResourceHandle>(hResource);
}

#ifdef CreateWindow
#undef CreateWindow
#endif

std::optional<DWindowHandle>
FWindowsPlatform::CreateWindow(const PWindowCreationDescriptor& desc)
{
  // Get style and exstyle.
  DWORD style = GetWin32WindowStyle(desc);
  DWORD exStyle = GetWin32WindowExStyle(desc);
  int posX = CW_USEDEFAULT, posY = CW_USEDEFAULT;

  // Get window size.
  auto [width, height] = GetWin32FullWindowSize(
    style, exStyle, 
    desc.mWindowWidth, desc.mWindowHeight, 96);

  // Widen string.
  auto wideStr = ConvertStringToWString(desc.mWindowName);

  // Create window actually.
  // Created window will inherits L"Dy" window class.
  auto handle = CreateWindowExW(
    exStyle, L"Dy", wideStr.c_str(), style, 
    posX, posY, width, height, 
    nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
  // Check
  assert(handle != nullptr && "Failed to create window.");
  if (handle == nullptr)
  {
    return std::nullopt;
  }

  DragAcceptFiles(handle, TRUE);
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  
  // Set properties.
  if (desc.mIsWindowVisible == true)
  {
    ShowWindow(handle, SW_SHOW);
  }

  if (desc.mIsWindowShouldFocus == true)
  {
    BringWindowToTop(handle);
    SetForegroundWindow(handle);
    SetFocus(handle);
  }
	UpdateWindow(handle);

  // Insert handle with generated uuid.
  DWindowHandle uuidHandle = {};
  auto [it, isSucceeded] = handleContainer.mWindowHandles.try_emplace(
      uuidHandle.mHandleUuid
    , handle);
  assert(isSucceeded == true);
 
  // Return
  return uuidHandle;
}

#ifndef CreateWindow
#define CreateWindow CreateWindowW
#endif
 
bool FWindowsPlatform::RemoveWindow(const DWindowHandle& handle)
{
  // Check handle is already not exist (already deleted) in handle container.
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  const auto it = handleContainer.mWindowHandles.find(handle.mHandleUuid);
  if (it == handleContainer.mWindowHandles.end())
  {
    return true;
  }

  // Destory window.
  auto& [uuid, hwnd] = *it;
  const auto flag = DestroyWindow(hwnd);
  if (flag == FALSE)
  {
    return false;
  }

  handleContainer.mWindowHandles.erase(it);
  return true;
}

bool FWindowsPlatform::RemoveAllWindow()
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  for (auto& [uuid, hwnd] : handleContainer.mWindowHandles)
  {
    const auto flag = DestroyWindow(hwnd);
    if (flag == FALSE)
    {
      return false;
    }
  }
  handleContainer.mWindowHandles.clear();

  return true;
}

void FWindowsPlatform::PollEvents()
{
  MSG msg;
  while (PeekMessage(&msg, nullptr,  0, 0, PM_REMOVE)) 
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  
  // Do post Dy platform(WIN32) message task.
  while (sPostMessages.empty() == false)
  {
    auto& task = sPostMessages.front();
    switch (task.mType)
    {
    case EWin32PostMesasge::DeleteWindow:
    {
      // Get handle container and try to match target hwnd and hwnds.
      auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
      auto& targetHwnd = std::any_cast<HWND&>(task.mData);

      for (auto& [uuid, hwnd] : handleContainer.mWindowHandles)
      {
        // If found, remove window and remove item pair of handle container.
        // and break.
        if (hwnd == targetHwnd)
        {
          const auto flag = DestroyWindow(hwnd);
          assert(flag == TRUE);
          handleContainer.mWindowHandles.erase(uuid);
          break;
        }
      }
    } break;
    }

    // POP.
    sPostMessages.pop();
  }
}

bool FWindowsPlatform::InitPlatform()
{
  platform = this;

  if (this->RegisterWindowClassWin32() == false) { return false; }

  if (this->CreateBackgroundWindow() == false) { return false; }

  return true;
}

bool FWindowsPlatform::RegisterWindowClassWin32()
{
  WNDCLASSEXW wndClass;
  ZeroMemory(&wndClass, sizeof(wndClass));

  wndClass.cbSize = sizeof(wndClass);
  // CS_H/VREDRAW : Horizontally, Vertically redraw window.
  // CS_OWNDC : Allocates a unique device context for each window in the class. 
  wndClass.style  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wndClass.lpfnWndProc  = (WNDPROC) OnWindowProc;
  wndClass.hInstance    = GetModuleHandleW(nullptr);
  wndClass.hCursor      = LoadCursorW(nullptr, IDC_ARROW);
  wndClass.lpszClassName= L"Dy"; // This name will be used when creating user defined window.

  // Check user-provided icon is exist.
  // But in this case, just load default icon temporary.
  wndClass.hIcon = static_cast<HICON>(LoadImageW(
    nullptr,
    IDI_APPLICATION, IMAGE_ICON,
    0, 0,
    LR_DEFAULTSIZE | LR_SHARED));
  
  // Check Class registration is successful.
  if (const auto atom = RegisterClassExW(&wndClass); atom == 0)
  {
    assert((atom != 0) && "Failed to regsiter window class into WIN32 internal system.");
    return false;
  }

  return true;
}

bool FWindowsPlatform::CreateBackgroundWindow()
{
  MSG msg;
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);

  // Size does not matter because this window handle is used as background.
  handle.mBackgroundWindow = CreateWindowEx(
    WS_EX_OVERLAPPEDWINDOW,
    L"Dy",
    L"Back message window",
    WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    0, 0, 1, 1, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);

  if (handle.mBackgroundWindow == nullptr)
  {
    this->GetDebugManager().OnAssertionFailed(
      "Failed to create background helper window.", 
      __FUNCTION__, __FILE__, __LINE__);
    return false;
  }

  // HACK: The command to the first ShowWindow call is ignored if the parent
  //       process passed along a STARTUPINFO, so clear that with a no-op call
  ShowWindow(handle.mBackgroundWindow, SW_HIDE);

  // Register for HID device (controllers...) notifications.
  // https://docs.microsoft.com/en-us/windows/desktop/api/dbt/ns-dbt-_dev_broadcast_deviceinterface_w
  DEV_BROADCAST_DEVICEINTERFACE_W dbi;
  ZeroMemory(&dbi, sizeof(dbi));
  dbi.dbcc_size = sizeof(dbi);
  dbi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
  dbi.dbcc_classguid  = {0x4d1e55b2,0xf16f,0x11cf,{0x88,0xcb,0x00,0x11,0x11,0x00,0x00,0x30}};

  handle.mDeviceNotiHandle = RegisterDeviceNotificationW(
    handle.mBackgroundWindow,
    reinterpret_cast<DEV_BROADCAST_HDR*>(&dbi),
    DEVICE_NOTIFY_WINDOW_HANDLE);

  // Dispatches incoming sent messages, 
  // checks the thread message queue for a posted message, and retrieves the message (if any exist).
  // Messages are removed from the queue after processing by PeekMessage. 
  while (PeekMessageW(&msg, handle.mBackgroundWindow, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  return true;
}

bool FWindowsPlatform::ReleasePlatform()
{
  if (this->RemoveBackgroundWindow() == false) { return false; }
  if (this->UnregisterWindowClassWin32() == false) { return false; }

  platform = nullptr;
  PostQuitMessage(0);

  return true;
}

bool FWindowsPlatform::RemoveBackgroundWindow()
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mDeviceNotiHandle != nullptr)
  {
    UnregisterDeviceNotification(handle.mDeviceNotiHandle);
    handle.mDeviceNotiHandle = nullptr;
  }

  if (handle.mBackgroundWindow != nullptr)
  {
    DestroyWindow(handle.mBackgroundWindow);
    handle.mBackgroundWindow = nullptr;
  }

  return true;
}

bool FWindowsPlatform::UnregisterWindowClassWin32()
{
  // Unregister.
  const auto flag = UnregisterClassW(L"Dy", GetModuleHandleW(nullptr));
  return flag != 0;
}

bool FWindowsPlatform::CanShutdown()
{
  auto& handleContainer = static_cast<FWindowsHandles&>(*this->mHandle);
  return handleContainer.mWindowHandles.empty() == true;
}

} /// ::dy namespace

