#ifndef GUARD_DY_MANAGEMENT_WINDOWMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_WINDOWMANAGER_IMPL_INL
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

namespace dy
{

inline MWindow::Impl::Impl(MWindow& parent) : mImplParent{parent}
{
  /// @brief This function is not implemented yet.
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

  /// @brief This function is not implemented yet.
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

  /// @brief Initialize OpenGL Context.
  /// This function must be returned `DY_SUCCESS`.
  static auto InitializeOpenGL = [this]()
  {
    glfwInit();

    // Create descriptor.
    PDyGLWindowContextDescriptor descriptor{};
    descriptor.mWindowName          = "DianYing";
    descriptor.mIsWindowResizable   = false;
    descriptor.mIsWindowVisible     = true;
    descriptor.mIsWindowShouldFocus = true;
    descriptor.mIsUsingDefaultDoubleBuffer = true;

    const auto& settingManager = MSetting::GetInstance();
    descriptor.mWindowSize = DIVec2{
        settingManager.GetWindowSizeWidth(),
        settingManager.GetWindowSizeHeight()
    };

    this->mGlfwWindow = XGLWrapper::CreateGLWindow(descriptor);

    // Create window instance for giving context to I/O worker thread.
    descriptor.mIsWindowShouldFocus = false;
    descriptor.mIsWindowVisible     = false;
    descriptor.mSharingContext      = this->mGlfwWindow;
    for (auto& ptrWindow : this->mGlfwWorkerWnds) { ptrWindow = XGLWrapper::CreateGLWindow(descriptor); }

    // Check validity
    if (this->mGlfwWindow == nullptr) { glfwTerminate(); return DY_FAILURE; }
    XGLWrapper::CreateGLContext(this->mGlfwWindow);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSetInputMode(this->mGlfwWindow, GLFW_STICKY_KEYS, GL_FALSE);
    glfwSetFramebufferSizeCallback(this->mGlfwWindow, &DyGlCallbackFrameBufferSize);
    glfwSetWindowCloseCallback(this->mGlfwWindow, &DyGlCallbackWindowClose);

    // If in debug build environment, enable debug output logging.
    #if defined(_DEBUG) || !defined(_NDEBUG)
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(DyGlMessageCallback, nullptr);
      GLuint unusedIds = 0;
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
      glfwSetErrorCallback(&DyGLFWErrorFunction);
    #endif

    // Setting rendering.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    { // Make initialized screen to black for giving liability to users XD
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.f, 0.f, 0.f, 1.0f);
      glfwSwapBuffers(this->mGlfwWindow);
      glfwPollEvents();
    }
    return DY_SUCCESS;
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  DyPushLogDebugInfo("{} | MWindow::Impl::pfInitialize().", "FunctionCall");
  #if defined(_WIN32)
    this->mDependentWindowContext = new MPlatformInfoWindows();
  #else
    static_assert(false, "Other platform does not support yet.");
  #endif
  this->mDependentWindowContext->InitializeDep();

  // If we should create console window...
  if (MSetting::GetInstance().IsEnabledSubFeatureLoggingToConsole() == true)
  {
    MDY_CALL_ASSERT_SUCCESS(this->mDependentWindowContext->CreateConsoleWindow());
  }

  // @TODO TEMP
  switch (MSetting::GetInstance().GetRenderingType())
  {
  default: MDY_UNEXPECTED_BRANCH(); break;
  case EDyRenderingApi::DirectX12:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::Vulkan:     MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::DirectX11:  MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::OpenGL:
    DyPushLogDebugInfo("Initialize OpenGL Context.");
    MDY_CALL_ASSERT_SUCCESS(InitializeOpenGL());
    break;
  }
}

inline MWindow::Impl::~Impl()
{
  DyPushLogDebugInfo("{} | MWindow::Impl::pfRelease().", "FunctionCall");

  switch (MSetting::GetInstance().GetRenderingType())
  {
  default: MDY_UNEXPECTED_BRANCH(); 
  case EDyRenderingApi::DirectX11: DyPushLogDebugInfo("Release DirectX11 Context."); MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::DirectX12: DyPushLogDebugInfo("Release DirectX12 Context."); MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::Vulkan:    DyPushLogDebugInfo("Release Vulkan Context.");    MDY_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApi::OpenGL:
    glfwTerminate();
    break;
  }

  if (this->mDependentWindowContext->IsCreatedConsoleWindow() == true)
  {
    MDY_CALL_ASSERT_SUCCESS(this->mDependentWindowContext->RemoveConsoleWindow());
  }

  this->mDependentWindowContext->ReleaseDep();
  delete this->mDependentWindowContext;
  this->mDependentWindowContext = nullptr;
}

inline bool MWindow::Impl::IsWindowShouldClose() const noexcept
{
  return glfwWindowShouldClose(this->mGlfwWindow);
}

inline EDySuccess MWindow::Impl::MDY_PRIVATE(TerminateWindow)() noexcept
{
  glfwSetWindowShouldClose(this->GetGLMainWindow(), GLFW_TRUE);
  return DY_SUCCESS;
}

inline GLFWwindow* MWindow::Impl::GetGLMainWindow() const noexcept
{
  MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(this->mGlfwWindow), "GlfwWindow is not initiailized.");
  return this->mGlfwWindow;
}

inline const std::array<GLFWwindow*, 2>& MWindow::Impl::GetGLWorkerWindowList() const noexcept
{
  #if defined (NDEBUG) == false 
  for (const auto& ptrWindow : this->mGlfwWorkerWnds)
  { // Validation check.
    MaybeNotUsed(ptrWindow);
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrWindow), "GLFWwindow must be valid.");
  }
  #endif

  return this->mGlfwWorkerWnds;
}

inline EDySuccess MWindow::Impl::CreateConsoleWindow()
{
  return this->mDependentWindowContext->CreateConsoleWindow();
}

inline bool MWindow::Impl::IsCreatedConsoleWindow() const noexcept
{
  return this->mDependentWindowContext->IsCreatedConsoleWindow();
}

inline EDySuccess MWindow::Impl::RemoveConsoleWindow()
{
  return this->mDependentWindowContext->RemoveConsoleWindow();
}

inline TF32 MWindow::Impl::GetCpuUsage()
{
  return this->mDependentWindowContext->GetCpuUsage();
}

inline TU64 MWindow::Impl::GetRamUsage()
{
  return this->mDependentWindowContext->GetRamUsage();
}

inline bool MWindow::Impl::IsFontExistOnSystem(const std::string& iFontKey) const
{
  return this->mDependentWindowContext->IsFontExistOnSystem(iFontKey);
}

inline std::optional<std::string> MWindow::Impl::GetFontPathOnSystem(const std::string& iFontKey) const
{
  return this->mDependentWindowContext->GetFontPathOnSystem(iFontKey);
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_WINDOWMANAGER_IMPL_INL