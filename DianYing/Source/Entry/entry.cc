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
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Core/Component/Shader.h>
#include <Dy/Helper/Type/Color.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/SettingManager.h>
#include "Dy/Core/Component/Texture.h"

#define MDY_RESOLUTION_WIDTH 1280
#define MDY_RESOLUTION_HEIGHT 720

///
/// Undefined proprocessor WIN32 macro "max, min" for preventing misuse.
///
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

//!
//! Forward declarations
//!

bool bDyWindowCloseSign = false;
bool bIsEnabledFeatureMsaa4x = true;

std::optional<std::vector<char>> DyReadBinaryFile(const std::string& fileName);

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
EDySuccess                DyGlRenderLoop();
EDySuccess                DyGlRenderFrame();

GLFWwindow*               gGlWindow = nullptr;

//!
//! Global DianYing API
//!

#if defined(_WIN32)
HWND windowHandle = nullptr;
HDC  windowDeviceContext = nullptr;
HGLRC windowGlResourceContext = nullptr;
#endif

//!
//! Implementation
//!

//!
//! ~DirectX11 Codes
//!

ID3D11Device* d11Device = nullptr;
ID3D11DeviceContext* d11DeviceContext = nullptr;

bool gImguiShowDemoWindow = true;
bool gImguiShowAnotherWindow = false;

dy::DVector3 gColor {1.f, 0.f, 0.5f};
dy::CDyShaderComponent gShader;
dy::CDyTextureComponent gTexture;

void GlRenderFrame()
{
  glClearColor(gColor.X, gColor.Y, gColor.Z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (gImguiShowDemoWindow)
  {
    ImGui::ShowDemoWindow(&gImguiShowDemoWindow);
  }

  {
    static float f = 0.0f;
    static int32_t counter = 0;

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Demo Window", &gImguiShowDemoWindow);
    ImGui::Checkbox("Another Window", &gImguiShowAnotherWindow);

    ImGui::SliderFloat("Float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("Clear color", gColor.Data().data());

    if (ImGui::Button("Button"))
    {
      ++counter;
    }
    ImGui::SameLine();
    ImGui::Text("Counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    if (gImguiShowAnotherWindow)
    {
      ImGui::Begin("Another Window", &gImguiShowAnotherWindow);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close me"))
      {
        gImguiShowAnotherWindow = false;
      }
      ImGui::End();
    }
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  gShader.BindShader();
  gShader.TempRender();
  gShader.UnbindShader();
}

void GlRenderLoop()
{
  while (!glfwWindowShouldClose(gGlWindow))
  {
    glfwPollEvents();
    GlRenderFrame();
    glfwSwapBuffers(gGlWindow);
  }
#ifdef false
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
      SwapBuffers(windowDeviceContext);
    }
  }
#elif (__linux__)
  assert(false);
#endif
#endif
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
    if (LOWORD(wParam) == WA_INACTIVE)
         { std::cout << "Inactivated\n"; }
    else { std::cout << "Activated\n";   }
    break;
  case WM_CLOSE:
    DestroyWindow(windowHandle);
    PostQuitMessage(0);
    break;
  case WM_PAINT:
    ValidateRect(windowHandle, nullptr);
    break;
  default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return S_OK;
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
/// @brief
/// Callback method for size check and resizing.
///
void OnCallbackFrameBufferSize(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
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
  auto& setting = dy::MDySetting::GetInstance();
  const auto renderingType = setting.GetRenderingType();
  switch (renderingType)
  {
  default: assert(false); break;
  case dy::ERenderingType::DirectX11:
    {
      assert(false);
#ifdef false
      MDY_CALL_ASSERT_SUCCESS(DyWin32InitializeWindow(hInstance));
      MDY_CALL_ASSERT_SUCCESS(DyD11InitializeDirect3D());

      ShowWindow(windowHandle, SW_SHOW);
      SetForegroundWindow(windowHandle);
      SetFocus(windowHandle);

      DyD11CreateEffectFx();
      DyD11BindVertexLayout();
      DyD11CreateVertexBuffer();
      DyD11RenderLoop();
#endif
    }
    break;
  case dy::ERenderingType::OpenGL:
    {
      glfwInit();
      // OpenGL Setting
      glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      gGlWindow = glfwCreateWindow(MDY_RESOLUTION_WIDTH, MDY_RESOLUTION_HEIGHT, "DianYing", nullptr, nullptr);
      if (!gGlWindow) {
        glfwTerminate();
        break;
      }

      glfwMakeContextCurrent(gGlWindow);
      gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

      glfwSetInputMode(gGlWindow, GLFW_STICKY_KEYS, GL_FALSE);
      glfwSetFramebufferSizeCallback(gGlWindow, &OnCallbackFrameBufferSize);

      glViewport(0, 0, MDY_RESOLUTION_WIDTH, MDY_RESOLUTION_HEIGHT);
      glClearColor(0, 0, 0, 0);
      glClearDepth(1.0f);
      glDepthFunc(GL_LESS);
      glEnable(GL_DEPTH_TEST);

      // IMGUI DEMO
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;

      ImGui_ImplGlfw_InitForOpenGL(gGlWindow, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      ImGui::StyleColorsDark();

      // Shader DEMO
      dy::PDyShaderConstructionDescriptor shaderDesc;
      {
        dy::PDyShaderFragmentInformation fragmentInfo;
        fragmentInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
        fragmentInfo.mShaderPath = "./glShader.vert";
        shaderDesc.mShaderFragments.emplace_back(fragmentInfo);
      }
      {
        dy::PDyShaderFragmentInformation fragmentInfo;
        fragmentInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
        fragmentInfo.mShaderPath = "./glShader.frag";
        shaderDesc.mShaderFragments.emplace_back(fragmentInfo);
      }
      MDY_CALL_ASSERT_SUCCESS(gShader.pInitializeShaderProgram(shaderDesc));

      // OpenGL (native) Texture binding DEMO
      dy::PDyTextureConstructionDescriptor textureDesc;
      {
        textureDesc.mTexturePath = "./TestResource/S_7325920284368.jpg";
        textureDesc.mTextureType = GL_TEXTURE_2D;
        textureDesc.mIsTextureParameterCustomed = false;
      }
      MDY_CALL_ASSERT_SUCCESS(gTexture.pInitializeTextureResource(textureDesc));

      GlRenderLoop();

      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();

      glfwDestroyWindow(gGlWindow);
      glfwTerminate();
    }
    break;
  case dy::ERenderingType::DirectX12: assert(false); break;
  case dy::ERenderingType::Vulkan:
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