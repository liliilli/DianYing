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

/// Header file
#include <Dy/Management/WindowManager.h>

#include <iostream>
#include <future>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Core/Component/Internal/EtcType.h>
#include <Dy/Core/Component/MeshRenderer.h>
#include <Dy/Core/Component/Resource/ShaderResource.h>
#include <Dy/Core/Component/Object/Camera.h>
#include <Dy/Core/Component/Object/Grid.h>
#include <Dy/Helper/Type/Vector3.h>
#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/Editor/GuiManager.h>

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
//! Independent anonymous namespace
//!

namespace
{

bool gImguiShowDemoWindow = false;
bool gImguiShowAnotherWindow = false;

dy::DDyVector3                  gColor      {.2f, .3f, .2f};
dy::CDyMeshRenderer             gRenderer   = {};
std::unique_ptr<dy::CDyCamera>  gCameraPtr  = nullptr;
std::unique_ptr<dy::FDyGrid>    gGrid       = nullptr;

void GLAPIENTRY DyGlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  std::fprintf(stderr, "DianYing OpenGL callback : %s type = 0x%x, severity = 0x%x, message = %s\n",
               (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
               type, severity, message);
}

///
/// @brief
///
void DyGlTempInitializeResource()
{
  auto& manInfo = dy::MDyDataInformation::GetInstance();

  dy::PDyCameraConstructionDescriptor cameraDesc;
  {
    cameraDesc.mInitialFieldOfView  = 70.f;
    cameraDesc.mIsMoveable          = true;
    cameraDesc.mIsFocusInstantly    = true;
    cameraDesc.mIsOrthographic      = true;
    cameraDesc.mUseCustomViewport   = false;
  }
  gCameraPtr = std::make_unique<dy::CDyCamera>(cameraDesc);

  //!
  //! Grid rendering setting.
  //!

  gGrid = std::make_unique<dy::FDyGrid>();

  //!
  //! Shader
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

  {
    shaderDesc.mShaderFragments.clear();
    shaderDesc.mShaderName = "MeshNanosuitShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVert.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glMeshDNSFrag.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));

  //!
  //! Model
  //!

  auto tempAsyncTask = std::async(std::launch::async, [&manInfo] {
    dy::PDyModelConstructionDescriptor bunnyModel;
    {
      bunnyModel.mModelName = "Bunny";
      bunnyModel.mModelPath = "./TestResource/bun_zipper.ply";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(bunnyModel));
    return true;
  });

  auto modelAsyncTask = std::async(std::launch::async, [&manInfo] {
    dy::PDyModelConstructionDescriptor modelDesc;
    {
      modelDesc.mModelName = "TestModel";
      //modelDesc.mModelPath = "./TestResource/Maximilian/max.obj";
      modelDesc.mModelPath = "./TestResource/nanosuit/nanosuit.obj";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
    return true;
  });

  if (tempAsyncTask.get() && modelAsyncTask.get()) { MDY_LOG_DEBUG_D("OK"); };

#ifdef false
  {
    dy::PDyModelConstructionDescriptor modelDesc;
    {
      modelDesc.mModelName = "Sponza";
      modelDesc.mModelPath = "./TestResource/crytek-sponza/sponza.obj";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
  };

  std::unordered_map<std::string, std::string> populatedMaterialList = {};
  dy::PDyMaterialPopulateDescriptor popDesc;
  {
    popDesc.mOverrideShaderName       = "MeshNanosuitShader";
    popDesc.mTextureOverrideNames     = {"glass_dif", "glass_ddn"};
    popDesc.mIsEnabledTextureOverride = true;
    popDesc.mIsEnabledShaderOverride  = true;
    if (auto matPtr = manInfo.PopulateMaterialInformation("Glass", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Glass", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"leg_dif", "leg_showroom_ddn", "leg_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Leg", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Leg", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"hand_dif", "hand_showroom_ddn", "hand_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Hand", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Hand", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"arm_dif", "arm_showroom_ddn", "arm_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Arm", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Arm", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"helmet_dif", "helmet_showroom_ddn", "helmet_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Helmet", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Helmet", matPtr.value()); }
  }
  {
    popDesc.mTextureOverrideNames     = {"body_dif", "body_showroom_ddn", "body_showroom_spec"};
    if (auto matPtr = manInfo.PopulateMaterialInformation("Body", popDesc); !matPtr.has_value()) { return; }
    else { populatedMaterialList.try_emplace("Body", matPtr.value()); }
  }

  dy::PDyRendererConsturctionDescriptor rendererDesc;
  {
    rendererDesc.mModelName     = "TestModel";
    rendererDesc.mMaterialNames = {
      populatedMaterialList["Glass"],
      populatedMaterialList["Leg"],
      populatedMaterialList["Hand"],
      populatedMaterialList["Glass"],
      populatedMaterialList["Arm"],
      populatedMaterialList["Helmet"],
      populatedMaterialList["Body"]
    };
  }
  MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
#endif

#ifdef false
  {
    shaderDesc.mShaderFragments.clear();
    shaderDesc.mShaderName = "MeshShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVert.vert";
      shaderDesc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glMeshFrag.frag";
      shaderDesc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
  }
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(shaderDesc));

  std::vector<std::string> materialNameList = {};
  dy::PDyMaterialPopulateDescriptor popDesc;
  {
    popDesc.mOverrideShaderName       = "MeshShader";
    popDesc.mIsEnabledShaderOverride  = true;
    popDesc.mIsEnabledMaterialCustomNameOverride = true;

    popDesc.mMaterialOverrideName     = "MaterialDennum1";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2385", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialCap";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2412", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialFace";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2386", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialDennum3";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2410", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialDennum2";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2387", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }
  {
    popDesc.mMaterialOverrideName     = "MaterialShoes";
    if (auto matPtr = manInfo.PopulateMaterialInformation("Material__2413", popDesc); !matPtr.has_value()) { return; }
    else { materialNameList.emplace_back(matPtr.value()); }
  }

  dy::PDyRendererConsturctionDescriptor rendererDesc;
  {
    rendererDesc.mModelName     = "TestModel";
    rendererDesc.mMaterialNames = materialNameList;
  }
  MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
#endif

  // OpenGL (native) Texture binding DEMO
#ifdef false
  { //
    const auto cubeModelPtr = manInfo.GetModelInformation("Cube");
    const std::vector<dy::DDySubmeshInformation>& cubeMeshInfoList = cubeModelPtr->GetMeshInformation();
    // Iterate all mesh information list for createing populated mesh instance with shader and aligned textures.
    for (const auto& info : cubeMeshInfoList)
    {
      const dy::PDySubmeshInformationDescriptor& i = info.GetInformation();
      if (!i.mMaterialName.empty())
      {
        auto* materialPtr = manInfo.GetMaterialInformation(i.mMaterialName);
        for (const auto& textureName : materialPtr->GetInformation().mTextureNames)
        {

        };
      }
    }
  }
#endif

}

#ifdef MDY_FLAG_IN_EDITOR
#ifdef false
void DyImguiFeatGuiMainMenuRenderFrame()
{
  static bool sHelpLicenseWindow  = false;
  static bool sHelpAboutLicenseWindow = false;
  static bool sHelpAboutWindow    = false;
  static bool sViewCpuUsage       = false;
  static bool sViewLogWindow      = false;

  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("View"))
    {
      ImGui::MenuItem("Cpu Usage", nullptr, &sViewCpuUsage);
      ImGui::MenuItem("Console", nullptr, &sViewLogWindow);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
      ImGui::MenuItem("Licenses", nullptr, &sHelpLicenseWindow);
      ImGui::MenuItem("About", nullptr, &sHelpAboutWindow);
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  if (sViewCpuUsage)
  {
    ImGui::Begin("Cpu Usage", &sViewCpuUsage, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::End();
  }

  if (sViewLogWindow)
  {
    ImGui::Begin("Log Window", &sViewLogWindow, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::End();
  }

  if (sHelpLicenseWindow)
  {
    ImGui::Begin("Lincense and third party library using", &sHelpLicenseWindow);

    for (const auto& libraryInfo : dy::editor::kLibraryLicenseInfo)
    {
      ImGui::BeginGroup();
      ImGui::BeginChild(libraryInfo.GetLibraryName(), ImVec2(0, 100), true);
      ImGui::Text("Library"); ImGui::SameLine(); ImGui::Text(libraryInfo.GetLibraryName());
      ImGui::Text("Version"); ImGui::SameLine(); ImGui::Text(libraryInfo.GetVersionName());

      bool isHp = false;
      if (!libraryInfo.IsHomepageUrlEmpty())
      {
        if (ImGui::Button("Homepage"))
        {
        #if defined(_WIN32)
          ShellExecute(nullptr, nullptr, libraryInfo.GetHomepageUrl().c_str(), nullptr, nullptr, SW_SHOW);
        #elif defined(__linux__)
          // @todo EXECUTE WEB BROWSER WITH URL.
        #endif
        }
        isHp = true;
      }

      if (!libraryInfo.IsGithubUrlEmpty())
      {
        if (isHp) ImGui::SameLine();
        if (ImGui::Button("Github"))
        {
        #if defined(_WIN32)
          ShellExecute(nullptr, nullptr, libraryInfo.GetGithubUrl().c_str(), nullptr, nullptr, SW_SHOW);
        #elif defined(__linux__)
          // @todo EXECUTE WEB BROWSER WITH URL.
        #endif
        }
      }

      if (ImGui::Button("View license"))
      {
        //ImGui::TextWrapped(libraryInfo.GetLicenseStatement());
      };
      ImGui::EndChild();
      ImGui::EndGroup();
    }

    ImGui::End();
  }
}
#endif
#endif

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
#if defined(MDY_FLAG_IN_EDITOR)
  dy::editor::MDyEditorGui::GetInstance().DrawWindow(0);
#endif

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


} /// unnamed namespace

//!
//! Platform depdendent anonymous namespace
//!

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

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
#ifdef false
    DestroyWindow(windowHandle);
#endif
    PostQuitMessage(0);
    break;
  case WM_PAINT:
#ifdef false
    ValidateRect(windowHandle, nullptr);
#endif
    break;
  default: return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return S_OK;
}

///
/// @brief Setup windows instance.
///
[[nodiscard]] EDySuccess DyWin32InitializeWindow(HINSTANCE hInstance, HWND* windowHandle) {
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

  const auto& settingManager = dy::MDySetting::GetInstance();

  const DWORD dwordExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  const DWORD dwordStyle   = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  RECT  windowRect;
  windowRect.left    = 0L;
  windowRect.top     = 0L;
  windowRect.right   = settingManager.GetWindowSizeWidth();
  windowRect.bottom  = settingManager.GetWindowSizeHeight();
  AdjustWindowRectEx(&windowRect, dwordStyle, FALSE, dwordExStyle);

  *windowHandle = CreateWindowEx(0,
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

  if (!*windowHandle) {
    const DWORD errorCode = GetLastError();
    printf("Failed to create window handle with %lx\n", errorCode);
    fflush(stdout);
    exit(1);
  }

  return DY_SUCCESS;
}

///
/// @brief Callback method for size check and resizing.
///
void DyGlCallbackFrameBufferSize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

///
/// @brief Callback method for closing arbitary glfw window window handle.
///
void DyGlCallbackWindowClose(GLFWwindow* window)
{
  glfwSetKeyCallback(window, nullptr);
  glfwSetCursorPosCallback(window, nullptr);
  glfwDestroyWindow(window);
}

} /// unnamed namespace
#elif defined(MDY_PLATFORM_FLAG_LINUX)
namespace
{

} /// unnamed namespace
#elif defined(MDY_PLATFORM_FLAG_MACOS)
namespace {

} /// unnamed namespace
#endif

//!
//! Implementations
//!

namespace dy
{

void MDyWindow::Run()
{
  auto& timeManager  = MDyTime::GetInstance();

  while (!glfwWindowShouldClose(this->mGlfwWindow))
  {
    timeManager.pUpdate();
    if (timeManager.IsGameFrameTicked() == DY_SUCCESS)
    {
      const auto dt = timeManager.GetGameScaledTickedDeltaTimeValue();

      this->pUpdate(dt);
      this->pRender();
    }
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

///
/// @brief
///
void MDyWindow::pUpdate(float dt)
{
#if defined(MDY_FLAG_IN_EDITOR)
  editor::MDyEditorGui::GetInstance().Update(dt);
#endif /// MDY_FLAG_IN_EDITOR
  MDyInput::GetInstance().pfUpdate(dt);

  auto& sceneManager = MDyScene::GetInstance();
  auto* cam = sceneManager.GetMainCameraPtr();
  if (cam)
  {
    cam->Update(dt);
  }
}

///
/// @brief
///
void MDyWindow::pRender()
{
  glClearColor(gColor.X, gColor.Y, gColor.Z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  if (gGrid)
  {
    gGrid->RenderGrid();
  };

#ifdef false
  gRenderer.Render();
#endif

  glDisable(GL_DEPTH_TEST);

  DyImguiRenderFrame();
  if (glfwWindowShouldClose(this->mGlfwWindow)) return;

  glfwSwapBuffers(this->mGlfwWindow);
  glfwPollEvents();
}

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
EDySuccess MDyWindow::pfInitialize()
{
  MDY_LOG_INFO_D("{} | MDyWindow::pfInitialize().", "FunctionCall");

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: assert(false); break;
  case dy::EDyRenderingApiType::DirectX11:
    MDY_LOG_INFO_D("Initialize DirectX11 Context.");
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
  case dy::EDyRenderingApiType::OpenGL:
    MDY_LOG_INFO_D("Initialize OpenGL Context.");
    {
      glfwInit();

      // OpenGL Setting
      glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      const auto& settingManager = dy::MDySetting::GetInstance();
      this->mGlfwWindow = glfwCreateWindow(settingManager.GetWindowSizeWidth(), settingManager.GetWindowSizeHeight(), "DianYing", nullptr, nullptr);
      if (!this->mGlfwWindow) {
        glfwTerminate();
        return DY_FAILURE;
      }

      {
        glfwMakeContextCurrent(this->mGlfwWindow);
        gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        glfwSetInputMode(this->mGlfwWindow, GLFW_STICKY_KEYS, GL_FALSE);

        glfwSetFramebufferSizeCallback(this->mGlfwWindow, &DyGlCallbackFrameBufferSize);
        glfwSetWindowCloseCallback(this->mGlfwWindow, &DyGlCallbackWindowClose);
      }

      // If in debug build environment, enable debug output logging.
      #if defined(_DEBUG) || !defined(_NDEBUG)
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DyGlMessageCallback, nullptr);
      #endif

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_DEPTH_TEST);

      // IMGUI DEMO
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;

      ImGui_ImplGlfw_InitForOpenGL(this->mGlfwWindow, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      ImGui::StyleColorsDark();

      // Shader DEMO
      DyGlTempInitializeResource();
    }
    break;
  case dy::EDyRenderingApiType::DirectX12:
    MDY_LOG_INFO_D("Initialize DirectX12 Context.");
    assert(false);
    break;
  case dy::EDyRenderingApiType::Vulkan:
    MDY_LOG_INFO_D("Initialize Vulkan Context.");
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

  return DY_SUCCESS;
}

EDySuccess MDyWindow::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyWindow::pfRelease().", "FunctionCall");

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  case EDyRenderingApiType::DirectX11:
    MDY_LOG_INFO_D("Initialize DirectX11 Context.");
    break;
  case EDyRenderingApiType::DirectX12:
    MDY_LOG_INFO_D("Initialize DirectX12 Context.");
    break;
  case EDyRenderingApiType::OpenGL:
    MDY_LOG_INFO_D("Initialize OpenGL Context.");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    MDY_LOG_INFO_D("Released ImGui Context.");

    glfwTerminate();
    break;
  case EDyRenderingApiType::Vulkan:
    MDY_LOG_INFO_D("Initialize Vulkan Context.");
    break;
  default: assert(false); return DY_FAILURE;
  }

  return DY_SUCCESS;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
EDySuccess MDyWindow::pfInitialize()
{

}

EDySuccess MDyWindow::pfRelease()
{

}
#elif defined(MDY_PLATFORM_FLAG_MACOS)
EDySuccess MDyWindow::pfInitialize()
{

}

EDySuccess MDyWindow::pfRelease()
{

}
#endif

} /// ::dy namespace