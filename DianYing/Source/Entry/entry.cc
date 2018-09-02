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

#include <Dy/Helper/Type/Color.h>
#include <Dy/Core/Component/Resource/MaterialResource.h>

#include <Dy/Helper/ImmutableSetting.h>

#include <Dy/Management/TimeManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Core/Component/Internal/ModelType.h>
#include <Dy/Core/Component/Internal/EtcType.h>
#include <Dy/Core/Component/MeshRenderer.h>
#include <Dy/Management/SceneManager.h>
#include <Dy/Core/Component/Object/Camera.h>

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

//!
//! ~OpenGL (native) Codes
//!

void GLAPIENTRY DyGlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  std::fprintf(stderr, "DianYing OpenGL callback : %s type = 0x%x, severity = 0x%x, message = %s\n",
               (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
               type, severity, message);
}

bool gImguiShowDemoWindow = true;
bool gImguiShowAnotherWindow = false;

dy::DVector3                    gColor      {.2f, .2f, .2f};
dy::CDyMeshRenderer             gRenderer   = {};
dy::CDyShaderResource*          gShaderPtr  = nullptr;
std::unique_ptr<dy::CDyCamera>  gCameraPtr  = nullptr;

GLuint gVao = 0;
GLuint gVbo = 0;

void DyGlDrawGrid()
{
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);

  gShaderPtr->UseShader();
  glBindVertexArray(gVao);
  glDrawArrays(GL_LINES, 0, 6 * 2);
  glBindVertexArray(0);
  gShaderPtr->UnuseShader();

  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
}

void DyGlTempInitializeResource()
{
  auto& manInfo = dy::MDyDataInformation::GetInstance();
  auto& manResc = dy::MDyResource       ::GetInstance();

  dy::PDyCameraConstructionDescriptor cameraDesc;
  {
    cameraDesc.mInitialFieldOfView  = 70.f;
    cameraDesc.mIsMoveable          = true;
    cameraDesc.mIsFocusInstantly    = true;
    cameraDesc.mIsOrthographic      = false;
    cameraDesc.mUseCustomViewport   = false;
  }
  gCameraPtr = std::make_unique<dy::CDyCamera>(cameraDesc);

  //!
  //! Grid rendering setting.
  //!

  dy::PDyShaderConstructionDescriptor gridShaderDesc;
  {
    gridShaderDesc.mShaderName = "dyBuiltinGrid";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./ShaderResource/Gl/grid.vert";
      gridShaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./ShaderResource/Gl/grid.frag";
      gridShaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation (gridShaderDesc)            );
  MDY_CALL_ASSERT_SUCCESS(manResc.CreateShaderResource    (gridShaderDesc.mShaderName));
  gShaderPtr = manResc.GetShaderResource(gridShaderDesc.mShaderName);

  glGenVertexArrays(1, &gVao);
  glGenBuffers(1, &gVbo);
  glBindVertexArray(gVao);

  std::vector<dy::DVector3> mPointers;
  {
    const float start     = -1.f;
    const float width     = 2.f;
    const int32_t step    = 6;
    const float interval  = width / step;

    for (int32_t i = 0; i <= step; ++i)
    {
      // Horizontal
      mPointers.emplace_back(start,         0.f, start + interval * i);
      mPointers.emplace_back(start + width, 0.f, start + interval * i);
      // Vertical
      mPointers.emplace_back(start + interval * i, 0.f,         start);
      mPointers.emplace_back(start + interval * i, 0.f, start + width);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, gVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(dy::DVector3) * mPointers.size(), mPointers.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dy::DVector3), nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  //!
  //! Other model and resource setting.
  //!

  dy::PDyShaderConstructionDescriptor shaderDesc;
  {
    shaderDesc.mShaderName = "TestShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVert.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glShader.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));

  // OpenGL (native) Texture binding DEMO
  dy::PDyTextureConstructionDescriptor textureDesc;
  {
    textureDesc.mTextureName                        = "TestTexture";
    textureDesc.mTextureFileAbsolutePath            = "./TestResource/S_7325920284368.jpg";
    textureDesc.mTextureType                        = dy::EDyTextureStyleType::D2;
    textureDesc.mIsEnabledCustomedTextureParameter  = false;
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateTextureInformation(textureDesc));

  dy::PDyMaterialConstructionDescriptor materialDesc;
  {
    materialDesc.mMaterialName  = "TestMaterial";
    materialDesc.mBlendMode     = dy::EDyMaterialBlendMode::Opaque;
    materialDesc.mShaderName    = "TestShader";
    materialDesc.mTextureName   = { "TestTexture" };
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateMaterialInformation(materialDesc));

#ifdef false
  dy::PDyModelConstructionDescriptor modelDesc;
  {
    modelDesc.mModelName = "TestModel";
    modelDesc.mModelPath = "./TestResource/nanosuit/nanosuit.obj";
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
#endif
#ifdef false
  std::vector<std::string> populatedMaterialNames;
  {
    const auto* model = manInfo.GetModelInformation(modelDesc.mModelName);
    const auto& materialNameList = model->GetBindedMaterialNameLists();

    dy::PDyMaterialPopulateDescriptor materialPopDesc;
    materialPopDesc.mIsEnabledShaderOverride  = true;
    materialPopDesc.mOverrideShaderName       = "TestShader";
    for (const auto& materialName : materialNameList)
    {
      const auto resultMatString = manInfo.PopulateMaterialInformation(materialName, materialPopDesc);
      if (!resultMatString.has_value()) { assert(false); }
      else
      {
        populatedMaterialNames.emplace_back(resultMatString.value());
      }
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manResc.CreateModelResource(modelDesc.mModelName));
#endif

  dy::PDyModelConstructionDescriptor bunnyModel;
  {
    bunnyModel.mModelName = "Bunny";
    bunnyModel.mModelPath = "./TestResource/bun_zipper.ply";
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(bunnyModel));

  dy::PDyModelConstructionDescriptor dragonModel;
  {
    bunnyModel.mModelName = "Dragon";
    bunnyModel.mModelPath = "./TestResource/dragon_vrip_res2.ply";
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(bunnyModel));

  dy::PDyRendererConsturctionDescriptor rendererDesc;
  {
    rendererDesc.mModelName     = "Dragon";
    rendererDesc.mMaterialNames = {"TestMaterial"};
  }
  MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
}

void DyImguiRenderFrame()
{
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
}

void GlRenderFrame()
{
  glClearColor(gColor.X, gColor.Y, gColor.Z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  DyGlDrawGrid();

  gRenderer.Render();

  DyImguiRenderFrame();
}

void GlRenderLoop()
{
  while (!glfwWindowShouldClose(gGlWindow))
  {
    GlRenderFrame();
    glfwSwapBuffers(gGlWindow);
    glfwPollEvents();
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
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyResource::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Initialize());
}

void DyReleaseAllManagers()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Release());
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
  windowRect.right   = dy::kScreenWidth;
  windowRect.bottom  = dy::kScreenHeight;
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
  const auto renderingType = dy::MDySetting::GetInstance().GetRenderingType();
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

      gGlWindow = glfwCreateWindow(dy::kScreenWidth, dy::kScreenHeight, "DianYing", nullptr, nullptr);
      if (!gGlWindow) {
        glfwTerminate();
        break;
      }

      glfwMakeContextCurrent(gGlWindow);
      gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

      glfwSetInputMode(gGlWindow, GLFW_STICKY_KEYS, GL_FALSE);
      glfwSetFramebufferSizeCallback(gGlWindow, &OnCallbackFrameBufferSize);

      glViewport(0, 0, dy::kScreenWidth, dy::kScreenHeight);
      glClearColor(0, 0, 0, 0);
      glClearDepth(1.0f);

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_BLEND);
      glBlendFunc(GL_BLEND_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if defined(_DEBUG) || !defined(_NDEBUG)
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(DyGlMessageCallback, nullptr);
#endif

      // IMGUI DEMO
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;

      ImGui_ImplGlfw_InitForOpenGL(gGlWindow, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      ImGui::StyleColorsDark();

      // Shader DEMO
      DyGlTempInitializeResource();
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