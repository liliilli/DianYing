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

#include <future>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Core/Component/Internal/EtcType.h>
#include <Dy/Core/Component/MeshRenderer.h>
#include <Dy/Core/Component/Object/Camera.h>
#include <Dy/Helper/Type/Vector3.h>

#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/Editor/GuiManager.h>
#include <Dy/Management/Rendering/RenderingManager.h>

#include <Dy/Builtin/Model/Box.h>
#include <Dy/Builtin/Model/Plain.h>
#include <Dy/Builtin/Model/Sphere.h>
#include <Dy/Builtin/Model/ScreenProjectionTriangle.h>
#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/Texture/ErrorBlue.h>
#include <Dy/Builtin/Material/OpaqueStaticPlain.h>
#include <Dy/Builtin/ShaderGl/RenderPass.h>
#include <Dy/Builtin/ShaderGl/RenderColorGeometry.h>
#include <Dy/Builtin/ShaderGl/RenderBasicShadow.h>
#include <Dy/Builtin/ShaderGl/RenderOpaqueStatic.h>
#include <Dy/Builtin/ShaderGl/RenderDefaultFont.h>
#include <Dy/Builtin/ShaderGl/RenderScreenOutput.h>

#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/PhysicsManager.h>
#include <Dy/Builtin/Widget/DebugUiMeta.h>
#include <Dy/Builtin/ShaderGl/RenderFontArraySDF.h>
#include <Dy/Builtin/ShaderGl/RenderUIBasicGaugeBar.h>
#include <Dy/Builtin/ShaderGl/RenderUIImage.h>

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

dy::DDyVector3 gColor {.2f, .3f, .2f};

void GLAPIENTRY DyGlMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  std::fprintf(stderr, "DianYing OpenGL callback : %s type = 0x%x, severity = 0x%x, message = %s\n",
               (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
               type, severity, message);
}

void DyInitializeBuiltinResource()
{
  auto& infoManager = dy::MDyIOData::GetInstance();
  auto& rescManager = dy::MDyIOResource::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderPass::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderPass::sName)));

  dy::builtin::FDyBuiltinModelBox();
  dy::builtin::FDyBuiltinModelPlain();
  dy::builtin::FDyBuiltinModelSphere();
  dy::builtin::FDyBuiltinModelScreenProjectionTriangle();

  dy::builtin::FDyBuiltinTextureChecker();
  dy::builtin::FDyBuiltinTextureErrorBlue();

  dy::builtin::FDyBuiltinShaderGLRenderColorGeometry();
  dy::builtin::FDyBuiltinShaderGLRenderOpaqueStatic();
  dy::builtin::FDyBuiltinShaderGLRenderDefaultFont();
  dy::builtin::FDyBuiltinShaderGLRenderFontArraySDF();
  dy::builtin::FDyBuiltinShaderGLRenderUiBasicGaugeBar();
  dy::builtin::FDyBuiltinShaderGLRenderUiImage();

  dy::builtin::FDyBuiltinMaterialOpaqueStaticPlain();
}

} /// unnamed namespace

//!
//! Platform depdendent anonymous namespace
//!

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

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
#elif defined(MDY_PLATFORM_FLAG_MACOS)
#endif

//!
//! Implementations
//!

namespace dy
{

void MDyWindow::Run()
{
  auto& timeManager     = MDyTime::GetInstance();
  auto& settingManager  = MDySetting::GetInstance();
  auto& sceneManager    = MDyWorld::GetInstance();

  sceneManager.OpenLevel(settingManager.GetInitialSceneInformationName());
  sceneManager.Update(-1);

  while (!glfwWindowShouldClose(this->mGlfwWindow))
  {
    timeManager.pUpdate();
    if (auto& instance = MDySound::GetInstance(); true) { instance.Update(MDY_INITIALIZE_DEFINT); }

    if (timeManager.IsGameFrameTicked() == DY_SUCCESS)
    {
      const auto dt = timeManager.GetGameScaledTickedDeltaTimeValue();

      this->pUpdate(dt);
      this->pRender();
    }
  }
}

///
/// @brief Update routine
///
void MDyWindow::pUpdate(float dt)
{
  #if defined(MDY_FLAG_IN_EDITOR)
    editor::MDyEditorGui::GetInstance().Update(dt);
  #endif // MDY_FLAG_IN_EDITOR

  //
  MDyPhysics::GetInstance().Update(dt);
  //
  MDyWorld::GetInstance().Update(dt);
  //
  MDyInput::GetInstance().pfUpdate(dt);
  MDyWorld::GetInstance().UpdateObjects(dt);
  //
  MDyWorld::GetInstance().RequestDrawCall(dt);
}

///
/// @brief Render routine.
///
void MDyWindow::pRender()
{
  glClearColor(gColor.X, gColor.Y, gColor.Z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef false
  gRenderer.CallDraw();
#endif

  glEnable(GL_DEPTH_TEST);
  MDyRendering::GetInstance().RenderDrawCallQueue();
  glDisable(GL_DEPTH_TEST);

  #if defined(MDY_FLAG_IN_EDITOR)
    editor::MDyEditorGui::GetInstance().DrawWindow(0);
  #endif // MDY_FLAG_IN_EDITOR

  if (glfwWindowShouldClose(this->mGlfwWindow)) { return; }
  glfwSwapBuffers(this->mGlfwWindow);
  glfwPollEvents();
}

#if defined(MDY_PLATFORM_FLAG_WINDOWS)
EDySuccess MDyWindow::pfInitialize()
{
  ///
  /// @brief This function is not implemented yet.
  ///
  static auto InitializeVulkan = [this]()
  {
    #ifdef false
    dy::DyVkInitialize(windowHandle, hInstance);

    ShowWindow(windowHandle, SW_SHOW);
    SetForegroundWindow(windowHandle);
    SetFocus(windowHandle);

    dy::DyVkRenderLoop();
    dy::DyVkCleanupResources();
    #endif
    return DY_FAILURE;
  };

  ///
  /// @brief This function is not implemented yet.
  ///
  static auto InitializeDirectX11 = [this]()
  {
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
    return DY_FAILURE;
  };

  ///
  /// @brief Initialize OpenGL Context.
  /// This function must be returned `DY_SUCCESS`.
  ///
  static auto InitializeOpenGL = [this]()
  {
    glfwInit();
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,    GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED,      GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const auto& settingManager = MDySetting::GetInstance();
    this->mGlfwWindow = glfwCreateWindow(
        settingManager.GetWindowSizeWidth(),
        settingManager.GetWindowSizeHeight(),
        "DianYing",
        nullptr,
        nullptr);

    if (this->mGlfwWindow == nullptr)
    {
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  MDY_LOG_INFO_D("{} | MDyWindow::pfInitialize().", "FunctionCall");
  MDY_INHERITENCE_WINDOW_INFORMATION_SUPER::InitializeDep();

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: MDY_UNEXPECTED_BRANCH(); break;
  case EDyRenderingApi::DirectX12:
    MDY_LOG_INFO_D("Initialize DirectX12 Context.");
    MDY_NOT_IMPLEMENTED_ASSERT();
    break;
  case EDyRenderingApi::Vulkan:
    MDY_LOG_INFO_D("Initialize Vulkan Context.");
    MDY_NOT_IMPLEMENTED_ASSERT();
    MDY_CALL_ASSERT_SUCCESS(InitializeVulkan());
    break;
  case EDyRenderingApi::DirectX11:
    MDY_LOG_INFO_D("Initialize DirectX11 Context.");
    MDY_NOT_IMPLEMENTED_ASSERT();
    MDY_CALL_ASSERT_SUCCESS(InitializeDirectX11());
    break;
  case EDyRenderingApi::OpenGL:
    MDY_LOG_INFO_D("Initialize OpenGL Context.");
    MDY_CALL_ASSERT_SUCCESS(InitializeOpenGL());

    { // IMGUI Setting
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui_ImplGlfw_InitForOpenGL(this->mGlfwWindow, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      ImGui::StyleColorsDark();
    }

    // Initialize builtin resources
    DyInitializeBuiltinResource();
    break;
  }

  return DY_SUCCESS;
}

EDySuccess MDyWindow::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyWindow::pfRelease().", "FunctionCall");

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: MDY_UNEXPECTED_BRANCH(); return DY_FAILURE;
  case EDyRenderingApi::DirectX11:  MDY_LOG_INFO_D("Release DirectX11 Context.");  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::DirectX12:  MDY_LOG_INFO_D("Release DirectX12 Context.");  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::Vulkan:     MDY_LOG_INFO_D("Release Vulkan Context.");     MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::OpenGL:
    MDY_LOG_INFO_D("Release OpenGL Context.");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    MDY_LOG_INFO_D("Released ImGui Context.");

    glfwTerminate();
    break;
  }

  MDY_INHERITENCE_WINDOW_INFORMATION_SUPER::ReleaseDep();
  return DY_SUCCESS;
}

#elif defined(MDY_PLATFORM_FLAG_LINUX)
#elif defined(MDY_PLATFORM_FLAG_MACOS)
#endif

} /// ::dy namespace