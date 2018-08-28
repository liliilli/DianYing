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

#include <d3dx11effect.h>

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
//! Forward declarations
//!

bool bDyWindowCloseSign = false;
bool bIsEnabledFeatureMsaa4x = true;

std::optional<std::vector<char>> DyReadBinaryFile(const std::string& fileName);

//!
//! DirectX11 API
//!

EDySuccess                DyD11InitializeDirect3D();
void                      DyD11RenderLoop();
void                      DyD11RenderFrame();

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

void                      DyD11BindVertexLayout();

ID3D11Buffer* sDyVertexBuffer = nullptr;
ID3D11Buffer* sDyIndexBuffer  = nullptr;
ID3D11RenderTargetView* sDyRenderTargetView = nullptr;
ID3D11DepthStencilView* sDyDepthStencilView = nullptr;
ID3D11InputLayout*      sDyInputLayout = nullptr;
IDXGISwapChain*         sDySwapChain = nullptr;

//!
//! DirectX12 API
//!

void                      DyD12RenderLoop();
void                      DyD12RenderFrame();

//!
//! OpenGL 4 API
//!

EDySuccess                DyGlCreateContentWgl();
EDySuccess                DyGlRenderLoop();
EDySuccess                DyGlRenderFrame();

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
[[nodiscard]] EDySuccess DyD11InitializeDirect3D()
{
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
  if ( result != S_OK )
  {
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

  if (bIsEnabledFeatureMsaa4x)
  {
    sd.SampleDesc.Count   = 4;
    sd.SampleDesc.Quality = msaa4xQuality - 1;
  }
  else
  {
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
  sDySwapChain = dxgiSwapChain;

  DyD11ReleaseComInstance(dxgiFactory);
  DyD11ReleaseComInstance(dxgiAdapter);
  DyD11ReleaseComInstance(dxgiDevice);

  // Create render target view.
  ID3D11RenderTargetView* renderTargetView  = nullptr;
  ID3D11Texture2D*        backBuffer        = nullptr;
  dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
  d11Device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
  sDyRenderTargetView = renderTargetView;

  DyD11ReleaseComInstance(backBuffer);

  // Create Depth/Stencil Buffer.
  /// Needs to fille out D3D11_TEXTURE2D_DESC.
  D3D11_TEXTURE2D_DESC depthStencilDesc = {};
  depthStencilDesc.Width          = area.right;
  depthStencilDesc.Height         = area.bottom;
  depthStencilDesc.MipLevels      = 1;
  depthStencilDesc.ArraySize      = 1;
  depthStencilDesc.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;
  if (bIsEnabledFeatureMsaa4x)
  {
    depthStencilDesc.SampleDesc.Count    = 4;
    depthStencilDesc.SampleDesc.Quality  = msaa4xQuality - 1;
  }
  else
  {
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
  sDyDepthStencilView = depthStencilView;

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

///
/// @brief
///
void DyD11CreateEffectFx()
{

}

///
/// @brief
///
void DyD11BindVertexLayout()
{
  std::array<D3D11_INPUT_ELEMENT_DESC, 2> inputElementDescs =
  {
    D3D11_INPUT_ELEMENT_DESC\
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0,},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0,},
  };

  assert(d11Device);
  //ID3DX11Effect* fx = nullptr;
  ID3DX11EffectTechnique* effectTechnique = nullptr;
  ID3D11InputLayout* inputLayout = nullptr;

  assert(effectTechnique);
  D3DX11_PASS_DESC passDesc = {};
  effectTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
  HR(d11Device->CreateInputLayout(
      inputElementDescs.data(),
      2,
      passDesc.pIAInputSignature,
      passDesc.IAInputSignatureSize,
      &inputLayout));
  sDyInputLayout = inputLayout;
}

void DyD11CreateVertexBuffer()
{
  using DirectX::XMFLOAT3;
  using DirectX::XMFLOAT4;
  std::array<DVertex1, 8> vertices =
  {
    DVertex1\
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
    { XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
    { XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
    { XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
    { XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
    { XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)}
  };

  D3D11_BUFFER_DESC vertexBufferDesc = {};
  vertexBufferDesc.Usage      = D3D11_USAGE_IMMUTABLE;
  vertexBufferDesc.BindFlags  = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.ByteWidth  = sizeof(DVertex1) * 8;

  D3D11_SUBRESOURCE_DATA vertexBufferInitialData = {};
  vertexBufferInitialData.pSysMem = vertices.data();

  ID3D11Buffer* vertexBuffer = nullptr;
  HR(d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferInitialData, &vertexBuffer));
  sDyVertexBuffer = vertexBuffer;

  std::array<UINT, 36> indices = {
    // front face
    0, 1, 2, 0, 2, 3,
    // back face
    4, 6, 5, 4, 7, 6,
    // left face
    4, 5, 1, 4, 1, 0,
    // right face
    3, 2, 6, 3, 6, 7,
    // top face
    1, 5, 6, 1, 6, 2,
    // bottom face
    4, 0, 3, 4, 3, 7
  };

  D3D11_BUFFER_DESC indicesBufferDesc = {};
  indicesBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indicesBufferDesc.ByteWidth = sizeof(UINT) * static_cast<UINT>(indices.size());
  indicesBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;

  D3D11_SUBRESOURCE_DATA indicesBufferInitialData = {};
  indicesBufferInitialData.pSysMem = indices.data();

  ID3D11Buffer* indexBuffer = nullptr;
  HR(d11Device->CreateBuffer(&indicesBufferDesc, &indicesBufferInitialData, &indexBuffer));
  sDyIndexBuffer = indexBuffer;
}

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
      DyD11RenderFrame();
    }
  }
#elif defined(__linux__)

#endif
}

///
/// @brief
///
void DyD11RenderFrame()
{
#ifdef false
  const auto backColor = DirectX::XMFLOAT4{.1f, .1f, .1f, 1.f};
  d11DeviceContext->ClearRenderTargetView(
      sDyRenderTargetView,
      reinterpret_cast<const float*>(&backColor)
  );
  d11DeviceContext->ClearDepthStencilView(
      sDyDepthStencilView,
      D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
      1.0f,
      0
  );

  d11DeviceContext->IASetInputLayout(sDyInputLayout);
  d11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  UINT stride = sizeof(DVertex1);
  UINT offset = 0;
  d11DeviceContext->IASetVertexBuffers(0, 1, &sDyVertexBuffer, &stride, &offset);
  d11DeviceContext->IASetIndexBuffer(sDyIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  d11DeviceContext->DrawIndexed(36, 0, 0);
#endif

  HR(sDySwapChain->Present(0, 0));
}

//!
//! Platform dependent codes
//!

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
  // Time manager
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

    DyD11CreateEffectFx();
    DyD11BindVertexLayout();
    DyD11CreateVertexBuffer();
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