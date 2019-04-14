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

#include <EPlatform.h>
#include <FWindowsHandles.h>
#include <FWindowsDebug.h>
#include <FBtResourceHandle.h>
#include <FWindowsProfiling.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#include <atlconv.h>
#include <cassert>

#define IS_32
#include <Dbt.h> // DEV_BROADCAST_DEVICEINTERFACE_W ... HID devices.

namespace 
{

LRESULT CALLBACK OnWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef false
  HANDLE window = GetPropW(nullptr, L"DyHelper");

  // If not found window handle,
  if (window == nullptr) 
  { 
    return DefWindowProcW(hWnd, uMsg, wParam, lParam); 
  }
#endif

  return DefWindowProcW(hWnd, uMsg, wParam, lParam); 
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
}

FWindowsPlatform::~FWindowsPlatform()
{

}

void FWindowsPlatform::SetWindowTitle(const std::string& newTitle)
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return; }

  glfwSetWindowTitle(handle.mGlfwWindow, newTitle.c_str());

#if 0
  // Widen string.
  std::wstring wideStr;
  const auto& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());

  for (const auto& currChar : newTitle)
  {
    wideStr.push_back(ctfacet.widen(currChar));
  }

  // Try set window text.
  const auto isSucceeded = SetWindowText(handle.mMainWindow, wideStr.c_str());
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
  }
#endif
}

std::string FWindowsPlatform::GetWindowTitle() const
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);

  // Get title name from main window handle.
  std::wstring titleName = {256, L'0'};
  GetWindowText(handle.mMainWindow, titleName.data(), static_cast<int>(titleName.size()));

  // If failed to get window text, just return empty string.
  if (titleName.length() == 0)
  {
    const auto errorFlag = GetLastError();
    return "";
  }

  // Narrow string into std::string (<char>)
  std::string resultTitleName;
  const auto& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());

  for (const auto& currWChar : titleName)
  {
    if (currWChar == L'0') { continue; }
    resultTitleName.push_back( ctfacet.narrow(currWChar, 0 ) );
  }
       
  // Return.
	return resultTitleName;
}

uint32_t FWindowsPlatform::GetWindowHeight() const
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return 0; }

  int width, height;
  glfwGetWindowSize(handle.mGlfwWindow, &width, &height);

  return height;

#if 0
  // Get RECT from main window.
  RECT rect;
  const auto isSucceeded = GetWindowRect(handle.mMainWindow, &rect);
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
    return 0;
  }

  // Return values.
  return rect.bottom - rect.top;
#endif
}

uint32_t FWindowsPlatform::GetWindowWidth() const
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return 0; }

  int width, height;
  glfwGetWindowSize(handle.mGlfwWindow, &width, &height);

  return width;

#if 0
  // Get RECT from main window.
  RECT rect;
  const auto isSucceeded = GetWindowRect(handle.mMainWindow, &rect);
  if (isSucceeded == FALSE)
  {
    const auto errorFlag = GetLastError();
    return 0;
  }

  // Return values.
  return rect.right - rect.left;
#endif
}

void FWindowsPlatform::ResizeWindow(uint32_t width, uint32_t height)
{
  auto& handle = static_cast<FWindowsHandles&>(*this->mHandle);
  if (handle.mGlfwWindow == nullptr) { return; }

  // Set size.
  glfwSetWindowSize(handle.mGlfwWindow, width, height);
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

bool FWindowsPlatform::CreateGameWindow()
{
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
  wndClass.lpszClassName= L"Dy";

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

bool FWindowsPlatform::RemoveGameWindow()
{
  if (this->RemoveBackgroundWindow() == false) { return false; }

  if (this->UnregisterWindowClassWin32() == false) { return false; }

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

} /// ::dy namespace

