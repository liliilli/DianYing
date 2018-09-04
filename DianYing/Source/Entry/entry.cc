#include <precompiled.h>
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

#include <cassert>

#include <iostream>
#include <optional>

#ifdef false
#include <Dy/VkInterface.h>
#endif

#include <d3dx11effect.h>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>

//!
//! Forward declarations
//!

bool bDyWindowCloseSign = false;
bool bIsEnabledFeatureMsaa4x = true;

/// @todo Chapter 6
struct DVertex1
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT4 color;
};

struct DVectex2
{
  DirectX::XMFLOAT3 position;
  DirectX::XMFLOAT2 normal;
  DirectX::XMFLOAT2 texCoord0;
  DirectX::XMFLOAT2 texCoord1;
};

//!
//! DirectX12 API
//!

void                      DyD12RenderLoop();
void                      DyD12RenderFrame();

//!
//! OpenGL 4 API
//!

EDySuccess                DyGlCreateContentWgl();

//!
//! Implementation
//!

//!
//! ~DirectX11 Codes
//!

ID3D11Device* d11Device = nullptr;
ID3D11DeviceContext* d11DeviceContext = nullptr;

//!
//! ~OpenGL (native) Codes
//!

///
/// @brief
///
void DyInitiailzeAllManagers()
{
  { // Setting manager
    auto& settingManager = dy::MDySetting::GetInstance();
#if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
    const int32_t size = __argc;
    for (int32_t i = 0; i < size; ++i) {
      settingManager.ArgsPushback(__argv[i]);
    }
#elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)

#elif defined(MDY_PLATFORM_FLAG_MACOS)

#endif
    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());
  }

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyResource::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Initialize());

  // MDyWindow must be initialized at last.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());
}

///
/// @brief
///
void DyReleaseAllManagers()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());
  // Release other management instance.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
}

///
/// @brief Main entry function of WIN32 platforms.
///
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{
FILE* fp = nullptr;
} /// unname namespace

///
/// @brief Turn on memory leak detection feature and console window for logging.
///
EDySuccess __InitializeWin32Debug()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

  if (!AllocConsole()) {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  freopen_s(&fp, "CONOUT$", "w", stdout);
  std::cout << "Hello world!\n";
  return DY_SUCCESS;
}

///
/// @brief Turn off memory leak detection feature and console window for logging.
///
EDySuccess __ReleaseWin32Debug()
{
  fclose(fp);

  if (!FreeConsole()) {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  return DY_SUCCESS;
}

#if defined(_DEBUG) || !defined(NDEBUG)
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   MDY_CALL_ASSERT_SUCCESS(__InitializeWin32Debug())
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  MDY_CALL_ASSERT_SUCCESS(__ReleaseWin32Debug())
#else
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#endif

///
/// @brief Main function of win32 / win64 platform.
///
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
  MDY_WIN32_TRY_TURN_ON_DEBUG();
  DyInitiailzeAllManagers();
  dy::MDyWindow::GetInstance().Run();
  DyReleaseAllManagers();
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main()
{
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main()
{
  return 0;
}
#endif