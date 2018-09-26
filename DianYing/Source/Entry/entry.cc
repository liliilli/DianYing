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

#include <sol2/sol.hpp>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/HeapResourceManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/RenderingManager.h>
#include <Dy/Management/SceneManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/SynchronizationManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>

#include <Dy/Management/Editor/GuiManager.h>
#include <Dy/Helper/Pointer.h>

#include <PxPhysicsAPI.h>
#include "Dy/Management/Helper/PhysXErrorCallback.h"

namespace
{

///
/// @brief Initialize all managers related to DianYing rendering application.
/// Initialzation order must be ended with MDyWindow and started with MDySetting by getting argv
/// as option.
///
void DyInitiailzeAllManagers()
{
  { // Setting manager
    auto& settingManager = dy::MDySetting::GetInstance();
    #if defined(MDY_PLATFORM_FLAG_WINDOWS) && defined(_WIN32)
      const int32_t size = __argc;
      for (int32_t i = 0; i < size; ++i)
      {
        settingManager.pArgsPushback(__argv[i]);
      }
    #elif defined(MDY_PLATFORM_FLAG_LINUX) && defined(__linux__)
      static_assert(false, "Linux does not support now.");
    #elif defined(MDY_PLATFORM_FLAG_MACOS)
      static_assert(false, "Macos does not support now.");
    #endif
    MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());
  }

  {
    auto& logManager = dy::MDySetting::GetInstance();
    logManager.SetSubFeatureLoggingToConsole(true);
    logManager.SetSubFeatureLoggingToFile(true);
  }
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Initialize());
#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyHeapResource::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Initialize());

  // MDyWindow must be initialized at last.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDySync::Initialize());
  MDY_LOG_WARNING_D("========== DIANYING MANAGER INITIALIZED ==========");
}

///
/// @brief Release all managers related to DianYing rendering application.
/// Order must be revered with DyInitializeAllManagers().
///
void DyReleaseAllManagers()
{
  MDY_LOG_WARNING_D("========== DIANYING MANAGER RELEASED ==========");
  MDY_CALL_ASSERT_SUCCESS(dy::MDySync::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());

  // Release other management instance.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScene::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyHeapResource::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyDataInformation::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());
#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
}

} /// unnamed namespace

///
/// @brief Main entry function of WIN32 platforms.
///
#if defined(MDY_PLATFORM_FLAG_WINDOWS)
namespace
{

dy::Owner<FILE*> gFp      = nullptr;
HINSTANCE ghInstance      = nullptr;
HINSTANCE ghPrevInstance  = nullptr;
LPSTR     gpCmdLine;
int       gnCmdShow;

///
/// @brief Turn on memory leak detection feature and console window for logging.
///
EDySuccess __DyInitializeWin32Debug()
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

  if (!AllocConsole()) {
    MessageBox(nullptr, L"The console window was not created.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }

  freopen_s(&gFp, "CONOUT$", "w", stdout);
  return DY_SUCCESS;
}

///
/// @brief Turn off memory leak detection feature and console window for logging.
///
EDySuccess __DyReleaseWin32Debug()
{
  fclose(gFp);

  if (!FreeConsole()) {
    MessageBox(nullptr, L"Failed to free console resource.", nullptr, MB_ICONEXCLAMATION);
    return DY_FAILURE;
  }
  return DY_SUCCESS;
}

} /// unname namespace

#if defined(_DEBUG) || !defined(NDEBUG)
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   MDY_CALL_ASSERT_SUCCESS(__DyInitializeWin32Debug())
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  MDY_CALL_ASSERT_SUCCESS(__DyReleaseWin32Debug())
#else
#define MDY_WIN32_TRY_TURN_ON_DEBUG()   (void)0
#define MDY_WIN32_TRY_TURN_OFF_DEBUG()  (void)0
#endif

physx::PxDefaultAllocator     gAllocator;
dy::FDyPhysXErrorCallback     gCallback;
physx::PxPvd*                 gPvd;
physx::PxFoundation*          gFoundation;
physx::PxPhysics*             gPhysicx;
physx::PxCpuDispatcher*       gDispatcher;
physx::PxScene*               gScene;
physx::PxMaterial*            gMaterial;
physx::PxReal                 stackZ = 10.0f;

///
/// @brief Main function of win32 / win64 platform.
///
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
  ghInstance      = hInstance;
  ghPrevInstance  = hPrevInstance;
  gpCmdLine       = pCmdLine;
  gnCmdShow       = nCmdShow;

#ifdef false
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);

  int value = lua.script("return 54");
  MDY_LOG_CRITICAL_D("Hello world Lua! : {}", value);
#endif

  MDY_WIN32_TRY_TURN_ON_DEBUG();
  DyInitiailzeAllManagers();

  DyTempInitializeTestResources();

  {
    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gCallback);
    gPhysicx = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd);

    physx::PxSceneDesc sceneDescriptor{gPhysicx->getTolerancesScale()};
    sceneDescriptor.gravity = physx::PxVec3{0, -9.81f, 0};

    gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDescriptor.cpuDispatcher = gDispatcher;
    sceneDescriptor.filterShader  = physx::PxDefaultSimulationFilterShader;

    gScene = gPhysicx->createScene(sceneDescriptor);

    physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if (pvdClient)
    {
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    gMaterial = gPhysicx->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxRigidStatic *groundPlane = PxCreatePlane(*gPhysicx, physx::PxPlane(0, 1, 0, 0), *gMaterial);

    gScene->addActor(*groundPlane);

    ///
    /// @function
    /// @brief
    ///
    static auto CreateStack = [](const physx::PxTransform &t, physx::PxU32 size, physx::PxReal halfExtent)
    {
      using namespace physx;

      PxShape *shape = gPhysicx->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
      for (PxU32 i = 0; i < size; i++)
      {
        for (PxU32 j = 0; j < size - i; j++)
        {
          const PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);

          PxRigidDynamic *body = gPhysicx->createRigidDynamic(t.transform(localTm));
          body->attachShape(*shape);

          PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
          gScene->addActor(*body);
        }
      }
      shape->release();
    };

    ///
    /// @function
    /// @brief
    ///
    static auto CreateDynamic = [](const physx::PxTransform &t, const physx::PxGeometry &geometry, const physx::PxVec3 &velocity = physx::PxVec3(0)) -> physx::PxRigidDynamic*
    {
      using namespace physx;

      PxRigidDynamic *dynamic = PxCreateDynamic(*gPhysicx, t, geometry, *gMaterial, 10.0f);
      dynamic->setAngularDamping(0.5f);
      dynamic->setLinearVelocity(velocity);
      gScene->addActor(*dynamic);

      return dynamic;
    };

    for (physx::PxU32 i = 0; i < 5; i++)
    {
      CreateStack(physx::PxTransform(physx::PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
    }

    CreateDynamic(physx::PxTransform(physx::PxVec3(0, 40, 100)), physx::PxSphereGeometry(10), physx::PxVec3(0, -50, -100));
  }

  // PhysX step physics
  for (TI32 i = 0; i < 100; ++i)
  {
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);
  }

  // PhysX test release
  {
    gScene->release();
    gPhysicx->release();
    gFoundation->release();

    printf("SnippetHelloWorld done.\n");
  }

  MDY_LOG_INFO_D("Running application routine.");
  dy::MDyWindow::GetInstance().Run();

  MDY_LOG_INFO_D("Release all managers and resources.");
  DyReleaseAllManagers();
  MDY_WIN32_TRY_TURN_OFF_DEBUG();
  return 0;
}
#elif defined(MDY_PLATFORM_FLAG_LINUX)
int main() { return 0; }
#elif defined(MDY_PLATFORM_FLAG_MACOS)
int main() { return 0; }
#endif