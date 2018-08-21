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
#include <sstream>

#ifdef false
#include <Dy/VkInterface.h>
#endif

#include <Dy/Management/TimeManager.h>
#include <Dy/Management/SettingManager.h>

#define MDY_RESOLUTION_WIDTH 1280
#define MDY_RESOLUTION_HEIGHT 720

///
/// Undefined proprocessor WIN32 macro "max, min" for preventing misuse.
///
#undef max
#undef min

//!
//! Forward declaration
//!

template <typename TType> using TOptional = std::optional<TType>;
template <typename TType> using TVector   = std::vector<TType>;

bool bDyWindowCloseSign = false;
bool bIsEnabledFeatureMsaa4x = true;

TOptional<std::vector<char>> DyReadBinaryFile(const std::string& fileName);

//!
//! DirectX11 API
//!

EDySuccess                DyD11InitializeDirect3D();
void                      DyD11RenderLoop();

//!
//! OpenGL 4 API
//!

EDySuccess                DyGlCreateContentWgl();

//!
//! Global DianYing API
//!

#if defined(_WIN32)

HWND windowHandle = nullptr;

#endif

//!
//! Implementation
//!

///
/// @brief
///
std::optional<std::vector<char>> DyReadBinaryFile(const std::string& fileName) {
  std::ifstream fileStream { fileName, std::ios::ate | std::ios::binary };
  if (!fileStream.is_open()) return std::nullopt;

  const size_t fileSize = fileStream.tellg();
  std::vector<char> fileBuffer(fileSize);

  fileStream.seekg(0);
  fileStream.read(fileBuffer.data(), fileSize);

  fileStream.close();
  return fileBuffer;
}


//!
//! ~DirectX11 Codes
//!

ID3D11Device* d11Device = nullptr;
ID3D11DeviceContext* d11DeviceContext = nullptr;

///
/// @brief
///
template <typename TType>
void DyD11ReleaseComInstance(TType*& instance) {
  instance->Release();
  instance = nullptr;
}

///
/// @brief
///
[[nodiscard]] EDySuccess DyD11InitializeDirect3D() {
  UINT createDeviceFlags = 0;
#if !defined(NDEBUG)
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  std::vector<D3D_FEATURE_LEVEL> featureLevel = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

  D3D_FEATURE_LEVEL resultFeatureLevel;
  const auto result = D3D11CreateDevice(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      createDeviceFlags,
      featureLevel.data(),
      static_cast<UINT>(featureLevel.size()),
      D3D11_SDK_VERSION,
      &d11Device,
      &resultFeatureLevel,
      &d11DeviceContext);
  if ( result != S_OK ) {
    MessageBox(nullptr, L"D3D11CreateDevice Failed.", nullptr, 0);
    return DY_FAILURE;
  }

  // Check MSAA x4 supported.
  UINT msaa4xQuality;
  HR(d11Device->CheckMultisampleQualityLevels(
      DXGI_FORMAT_R8G8B8A8_UNORM,
      4,
      &msaa4xQuality));
  assert(msaa4xQuality > 0);

  // Initialize D3D11 Swap chain and describe.
  /// At first, fill out information into DXGI_SWAP_CHAIN_DESC.
  DXGI_SWAP_CHAIN_DESC sd = {};
  RECT area;
  GetClientRect(windowHandle, &area);
  sd.BufferDesc.Width                 = area.right;
  sd.BufferDesc.Height                = area.bottom;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.ScanlineOrdering      = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

  if (bIsEnabledFeatureMsaa4x) {
    sd.SampleDesc.Count   = 4;
    sd.SampleDesc.Quality = msaa4xQuality - 1;
  }
  else {
    sd.SampleDesc.Count   = 1;
    sd.SampleDesc.Quality = 0;
  }
  sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount  = 1;
  sd.OutputWindow = windowHandle;
  sd.Windowed     = true;
  sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
  sd.Flags        = 0;

  /// Second, create the swap chain.
  IDXGIDevice*  dxgiDevice  = nullptr;
  IDXGIAdapter* dxgiAdapter = nullptr;
  IDXGIFactory* dxgiFactory = nullptr;
  HR(d11Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)));
  HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter)));
  HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)));

  IDXGISwapChain* dxgiSwapChain = nullptr;
  HR(dxgiFactory->CreateSwapChain(d11Device, &sd, &dxgiSwapChain));

  DyD11ReleaseComInstance(dxgiFactory);
  DyD11ReleaseComInstance(dxgiAdapter);
  DyD11ReleaseComInstance(dxgiDevice);

  // Create render target view.
  ID3D11RenderTargetView* renderTargetView  = nullptr;
  ID3D11Texture2D*        backBuffer        = nullptr;
  dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
  d11Device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
  DyD11ReleaseComInstance(backBuffer);

  // Create Depth/Stencil Buffer.
  /// Needs to fille out D3D11_TEXTURE2D_DESC.
  D3D11_TEXTURE2D_DESC depthStencilDesc = {};
  depthStencilDesc.Width          = area.right;
  depthStencilDesc.Height         = area.bottom;
  depthStencilDesc.MipLevels      = 1;
  depthStencilDesc.ArraySize      = 1;
  depthStencilDesc.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;
  if (bIsEnabledFeatureMsaa4x) {
    depthStencilDesc.SampleDesc.Count    = 4;
    depthStencilDesc.SampleDesc.Quality  = msaa4xQuality - 1;
  }
  else {
    depthStencilDesc.SampleDesc.Count    = 1;
    depthStencilDesc.SampleDesc.Quality  = 0;
  }
  depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
  depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags      = 0;

  /// Create actual COM texture2d for depth/stencil buffer and view to see this.
  ID3D11Texture2D* depthStencilBuffer;
  ID3D11DepthStencilView* depthStencilView;
  HR(d11Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer));
  HR(d11Device->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView));

  // Bind the RTV and DSV to the pipeline.
  d11DeviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

  // Set the viewport transform.
  D3D11_VIEWPORT screenViewport;
  screenViewport.TopLeftX = 0;
  screenViewport.TopLeftY = 0;
  screenViewport.Width    = static_cast<FLOAT>(area.right);
  screenViewport.Height   = static_cast<FLOAT>(area.bottom);
  screenViewport.MinDepth = 0.0f;
  screenViewport.MaxDepth = 1.0f;

  d11DeviceContext->RSSetViewports(1, &screenViewport);

  return DY_SUCCESS;
}

//!
//! Platform dependent codes
//!

///
/// @brief
///
void DyD11RenderLoop()
{
#if defined(_WIN32)
  MSG   win32Message;
  bool  win32QuitMessageReceived = false;
  while (!win32QuitMessageReceived) {
    while (PeekMessage(&win32Message, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&win32Message);
      DispatchMessage(&win32Message);

      if (win32Message.message == WM_QUIT) {
        win32QuitMessageReceived = true;
        bDyWindowCloseSign = true;
        break;
      }
    }

    if (!IsIconic(windowHandle)) {
      // Do something.
    }
  }
#elif defined(__linux__)

#endif
}

///
/// @brief
///
void DyD12RenderLoop()
{

}

///
/// @brief
///
void DyGlRenderLoop()
{

}

void DyInitiailzeAllManagers()
{
  { // Setting manager
    auto& settingManager = dy::MDySetting::GetInstance();
#if defined(_WIN32)
    const int32_t size = __argc;
    for (int32_t i = 0; i < size; ++i) {
      settingManager.ArgsPushback(__argv[i]);
    }
#elif defined(__linux__)

#endif
    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());
  }
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
}

void DyReleaseAllManagers()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
}

//!
//! WIN32 platform.
//!

#if defined(_WIN32)

///
/// @brief Callback function of WIN32 instance handle.
///
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_ACTIVATE:
    if (LOWORD(wParam) == WA_INACTIVE) {
      std::cout << "Inactivated\n";
    }
    else {
      std::cout << "Activated\n";
    }
    break;
  case WM_CLOSE:
    DestroyWindow(windowHandle);
    PostQuitMessage(0);
    break;
  case WM_PAINT:
    ValidateRect(windowHandle, nullptr);
    break;
  default: break;
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

///
/// @brief Setup windows instance.
///
[[nodiscard]] EDySuccess DyWin32InitializeWindow(HINSTANCE hInstance) {
  WNDCLASSEX wndClass;
  wndClass.cbSize       = sizeof(WNDCLASSEX);
  wndClass.style        = CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc  = WndProc;
  wndClass.cbClsExtra   = 0;
  wndClass.cbWndExtra   = 0;
  wndClass.hInstance    = hInstance;
  wndClass.hIcon        = LoadIcon(nullptr, IDI_APPLICATION);
  wndClass.hCursor      = LoadIcon(nullptr, IDC_ARROW);
  wndClass.hbrBackground= static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
  wndClass.lpszMenuName = nullptr;
  wndClass.lpszClassName= L"DianYing";
  wndClass.hIconSm      = LoadIcon(nullptr, IDI_WINLOGO);

  if (!RegisterClassEx(&wndClass)) {
    std::cout << "Faeild to register window class.\n";
    fflush(stdout);
    exit(1);
  }

#ifdef false
  dy::TInt32 screenWidth  = GetSystemMetrics(SM_CXSCREEN);
  dy::TInt32 screenHeight = GetSystemMetrics(SM_CYSCREEN);
#endif

  const DWORD dwordExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  const DWORD dwordStyle   = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  RECT  windowRect;
  windowRect.left    = 0L;
  windowRect.top     = 0L;
  windowRect.right   = MDY_RESOLUTION_WIDTH;
  windowRect.bottom  = MDY_RESOLUTION_HEIGHT;
  AdjustWindowRectEx(&windowRect, dwordStyle, FALSE, dwordExStyle);

  windowHandle = CreateWindowEx(0,
      L"DianYing",
      L"DianYing v0.0.1",
      dwordStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      64L,
      64L,
      windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top,
      nullptr,
      nullptr,
      hInstance,
      nullptr);

  if (!windowHandle) {
    const DWORD errorCode = GetLastError();
    printf("Failed to create window handle with %lx\n", errorCode);
    fflush(stdout);
    exit(1);
  }

  return DY_SUCCESS;
}

///
/// @brief Main entry function of WIN32 platforms.
///
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
#if !defined(NDEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

  if (!AllocConsole()) {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
  }

  FILE* fp;
  freopen_s(&fp, "CONOUT$", "w", stdout);
  std::cout << "Hello world!\n";
#endif

  DyInitiailzeAllManagers();
  MDY_CALL_ASSERT_SUCCESS(DyWin32InitializeWindow(hInstance));

  auto& setting = dy::MDySetting::GetInstance();
  const auto renderingType = setting.GetRenderingType();
  switch (renderingType)
  {
  default: assert(false); break;
  case dy::ERenderingType::DirectX11:
    MDY_CALL_ASSERT_SUCCESS(DyD11InitializeDirect3D());

    ShowWindow(windowHandle, SW_SHOW);
    SetForegroundWindow(windowHandle);
    SetFocus(windowHandle);

    DyD11RenderLoop();
    break;
  case dy::ERenderingType::DirectX12:
    assert(false);
    break;
  case dy::ERenderingType::OpenGL:
    assert(false);
    break;
  case dy::ERenderingType::Vulkan:
    // Create vulkan instance.
    assert(false);
#ifdef false
    dy::DyVkInitialize(windowHandle, hInstance);

    ShowWindow(windowHandle, SW_SHOW);
    SetForegroundWindow(windowHandle);
    SetFocus(windowHandle);

    dy::DyVkRenderLoop();
    dy::DyVkCleanupResources();
#endif
    break;
  }

#if !defined(NDEBUG)
  fclose(fp);

  if (!FreeConsole()) {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
  }
#endif

  DyReleaseAllManagers();
  return 0;
}

#else /// Other platforms... or other libraries.

#endif