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

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/Editor/GuiManager.h>
#include <Dy/Management/RenderingManager.h>

#include <Dy/Builtin/Model/Box.h>
#include <Dy/Builtin/Model/Plain.h>
#include <Dy/Builtin/Model/Sphere.h>
#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/ShaderGl/RenderPass.h>
#include <Dy/Builtin/ShaderGl/RenderColorGeometry.h>
#include <Dy/Builtin/ShaderGl/RenderBasicShadow.h>
#include <Dy/Builtin/ShaderGl/RenderOpaqueStatic.h>
#include <Dy/Builtin/Material/OpaqueStaticPlain.h>

#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/PhysicsManager.h>

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

dy::DDyVector3                  gColor      {.2f, .3f, .2f};
dy::CDyMeshRenderer             gRenderer   = {};
std::unique_ptr<dy::CDyLegacyCamera>  gCameraPtr  = nullptr;

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
  //auto& manInfo = dy::MDyDataInformation::GetInstance();

#ifdef false
  dy::PDyCameraConstructionDescriptor cameraDesc;
  {
    cameraDesc.mInitialFieldOfView  = 70.f;
    cameraDesc.mIsMoveable          = true;
    cameraDesc.mIsFocusInstantly    = true;
    cameraDesc.mIsOrthographic      = true;
    cameraDesc.mUseCustomViewport   = false;
  }
  gCameraPtr = std::make_unique<dy::CDyLegacyCamera>(cameraDesc);
#endif

  //!
  //! Shader
  //!

  dy::builtin::FDyBuiltinModelBox();
  dy::builtin::FDyBuiltinModelPlain();
  dy::builtin::FDyBuiltinModelSphere();

  dy::builtin::FDyBuiltinTextureChecker();

  dy::builtin::FDyBuiltinShaderGLRenderPass();
  dy::builtin::FDyBuiltinShaderGLRenderColorGeometry();
  dy::builtin::FDyBuiltinShaderGLRenderOpaqueStatic();

  dy::builtin::FDyBuiltinMaterialOpaqueStaticPlain();

#ifdef false
  {
    auto animAsyncTask = std::async(std::launch::async, [&manInfo] {
      dy::PDyModelConstructionDescriptor modelDesc;
      {
        modelDesc.mModelName = "TestModel";
        modelDesc.mModelPath = "./TestResource/bun_zipper.ply";
      }
      MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
      return true;
    });

    if (animAsyncTask.get()) { MDY_LOG_DEBUG_D("OK"); };
  }
#endif

#ifdef false
  {
    dy::PDyShaderConstructionDescriptor desc;
    desc.mShaderName = "TestSkeletalAnimShader";
    {
      dy::PDyShaderFragmentInformation vertexShaderInfo;
      vertexShaderInfo.mShaderType = dy::EDyShaderFragmentType::Vertex;
      vertexShaderInfo.mShaderPath = "./glMeshVertSAnim.vert";
      desc.mShaderFragments.emplace_back(vertexShaderInfo);
    }
    {
      dy::PDyShaderFragmentInformation fragmentShaderInfo;
      fragmentShaderInfo.mShaderType = dy::EDyShaderFragmentType::Pixel;
      fragmentShaderInfo.mShaderPath = "./glShader.frag";
      desc.mShaderFragments.emplace_back(fragmentShaderInfo);
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateShaderInformation(desc));
  }

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
      modelDesc.mModelPath = "./TestResource/nanosuit/nanosuit.obj";
    }
    MDY_CALL_ASSERT_SUCCESS(manInfo.CreateModelInformation(modelDesc));
    return true;
  });
#endif

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
#endif
#ifdef false
  {
    dy::PDyMaterialPopulateDescriptor popDesc;
    popDesc.mIsEnabledShaderOverride  = true;
    popDesc.mOverrideShaderName       = "TestDeferredShader";
    const auto spawnedMatStr = manInfo.PopulateMaterialInformation("asdf1:Beta_HighLimbsGeoSG2", popDesc);

    dy::PDyRendererConsturctionDescriptor rendererDesc;
    {
      rendererDesc.mModelName     = "Boxing";
      rendererDesc.mMaterialNames = { spawnedMatStr.value(), spawnedMatStr.value() };
    }
    MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
  }

  dy::PDyRendererConsturctionDescriptor rendererDesc;
  {
    rendererDesc.mModelName     = "TestModel";
    rendererDesc.mMaterialNames = std::vector<std::string>(394, "TestMat");
  }
  MDY_CALL_ASSERT_SUCCESS(gRenderer.pfInitialize(rendererDesc));
#endif
}

///
/// @brief
///
void DyTestSoundFmod()
{
  dy::PDySoundConstructionDescriptor desc;
  desc.mSoundName = "1";
  desc.mSoundPath = "./TestResource/_02Effect.mp3";

  auto& manInfo = dy::MDyDataInformation::GetInstance();
  auto& resInfo = dy::MDyHeapResource::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(manInfo.CreateSoundInformation(desc));
  MDY_CALL_ASSERT_SUCCESS(resInfo.CreateSoundResource(desc.mSoundName));
}

} /// unnamed namespace

void DyTempInitializeTestResources()
{
  DyGlTempInitializeResource();
  DyTestSoundFmod();
}

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

  auto* cam = MDyWorld::GetInstance().GetMainCameraPtr();
  if (cam)
  {
    cam->Update(dt);
  }

#ifdef false
  gRenderer.Update(dt);
#endif
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
  MDY_LOG_INFO_D("{} | MDyWindow::pfInitialize().", "FunctionCall");

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: PHITOS_UNEXPECTED_BRANCH(); break;
  case EDyRenderingApiType::DirectX12:  MDY_LOG_INFO_D("Initialize DirectX12 Context.");  PHITOS_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApiType::Vulkan:     MDY_LOG_INFO_D("Initialize Vulkan Context.");     PHITOS_NOT_IMPLEMENTED_ASSERT();
#ifdef false
    dy::DyVkInitialize(windowHandle, hInstance);

    ShowWindow(windowHandle, SW_SHOW);
    SetForegroundWindow(windowHandle);
    SetFocus(windowHandle);

    dy::DyVkRenderLoop();
    dy::DyVkCleanupResources();
#endif
    break;
  case EDyRenderingApiType::DirectX11: MDY_LOG_INFO_D("Initialize DirectX11 Context."); PHITOS_NOT_IMPLEMENTED_ASSERT();
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
    break;
  case EDyRenderingApiType::OpenGL:
    MDY_LOG_INFO_D("Initialize OpenGL Context.");
    {
      glfwInit();
      glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      const auto& settingManager = MDySetting::GetInstance();
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

      glEnable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      // IMGUI Setting
      {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(this->mGlfwWindow, true);
        ImGui_ImplOpenGL3_Init("#version 430");
        ImGui::StyleColorsDark();
      }
    }
    break;
  }

  return DY_SUCCESS;
}

EDySuccess MDyWindow::pfRelease()
{
  MDY_LOG_INFO_D("{} | MDyWindow::pfRelease().", "FunctionCall");

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  default: PHITOS_UNEXPECTED_BRANCH(); return DY_FAILURE;
  case EDyRenderingApiType::DirectX11:  MDY_LOG_INFO_D("Release DirectX11 Context.");  PHITOS_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApiType::DirectX12:  MDY_LOG_INFO_D("Release DirectX12 Context.");  PHITOS_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApiType::Vulkan:     MDY_LOG_INFO_D("Release Vulkan Context.");     PHITOS_NOT_IMPLEMENTED_ASSERT(); break;
  case EDyRenderingApiType::OpenGL:
    MDY_LOG_INFO_D("Release OpenGL Context.");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    MDY_LOG_INFO_D("Released ImGui Context.");

    glfwTerminate();
    break;
  }

  return DY_SUCCESS;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
#elif defined(MDY_PLATFORM_FLAG_MACOS)
#endif

} /// ::dy namespace