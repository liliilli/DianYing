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
#include <Dy/Core/DyEngine.h>

#include <Dy/Management/IO/IODataManager.h>
#include <Dy/Management/IO/IOResourceManager.h>
#include <Dy/Management/InputManager.h>
#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/PhysicsManager.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/SoundManager.h>
#include <Dy/Management/TimeManager.h>
#include <Dy/Management/WindowManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/FontManager.h>
#include <Dy/Management/ScriptManager.h>
#include <Dy/Management/Editor/GuiManager.h>
#include <Dy/Management/Internal/MDySynchronization.h>
#include <Dy/Core/Thread/SDyIOConnectionHelper.h>

#include <Dy/Builtin/Texture/Checker.h>
#include <Dy/Builtin/Texture/ErrorBlue.h>
#include <Dy/Builtin/Material/OpaqueStaticPlain.h>

#include <Dy/Builtin/Widget/DebugUiMeta.h>

//!
//! Local function & data (Local translation unit function & data)
//!

namespace
{

void DyInitializeBuiltinResource()
{
  auto& infoManager = dy::MDyIOData::GetInstance();
#ifdef false
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateModelInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinModelBox::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateModelInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinModelPlain::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateModelInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinModelScreenProjectionTriangle::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateModelInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinModelSphere::sName), dy::EDyScope::Global));

  //SDyIOConnectionHelper::PopulateResource(MSVSTR(FDyBuiltinTextureChecker::sName), EDyResourceType::Texture, EDyResourceStyle::Information, EDyScope::Global);
  //MDY_CALL_ASSERT_SUCCESS(infoManager.CreateTextureInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinTextureChecker::sName), dy::EDyScope::Global));
  //SDyIOConnectionHelper::PopulateResource(MSVSTR(FDyBuiltinTextureChecker::sName), EDyResourceType::Texture, EDyResourceStyle::Resource, EDyScope::Global);
  //MDY_CALL_ASSERT_SUCCESS(rescManager.CreateTextureResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinTextureChecker::sName), dy::EDyScope::Global));
  //MDY_CALL_ASSERT_SUCCESS(infoManager.CreateTextureInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinTextureErrorBlue::sName), dy::EDyScope::Global));
  //MDY_CALL_ASSERT_SUCCESS(rescManager.CreateTextureResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinTextureErrorBlue::sName), dy::EDyScope::Global));

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderPass::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderPass::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderColorGeometry::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderColorGeometry::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderOpaqueStatic::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderOpaqueStatic::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDefaultFont::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDefaultFont::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderFontArraySDF::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderFontArraySDF::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderUiBasicGaugeBar::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderUiImage::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderUiImage::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDefaultSSAO::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDefaultSSAO::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDefaultSSAOBlurring::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDefaultSSAOBlurring::sName)));

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderBasicShadow::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderBasicShadow::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDeferredRendering::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderDeferredRendering::sName)));
  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateShaderInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderScreenOutput::sName), dy::EDyScope::Global));
  MDY_CALL_ASSERT_SUCCESS(rescManager.CreateShaderResource_Deprecated(MSVSTR(dy::builtin::FDyBuiltinShaderGLRenderScreenOutput::sName)));
#endif

  MDY_CALL_ASSERT_SUCCESS(infoManager.CreateMaterialInformation_Deprecated(MSVSTR(dy::builtin::FDyBuiltinMaterialOpaqueStaticPlain::sName), dy::EDyScope::Global));
}

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

DyEngine* gEngine = nullptr;

EDySuccess DyEngine::pfInitialize()
{
  /// @brief Forward runtime arguments to setting manager.
  static auto InsertExecuteRuntimeArguments = []()
  {
    auto& settingManager = MDySetting::GetInstance();
    settingManager.pSetupExecutableArgumentSettings();
  };

  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //! FUNCTIONBODY ∨
  //! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  gEngine = this;
  InsertExecuteRuntimeArguments();
  this->pfInitializeIndependentManager();

   // Temporal
  DyInitializeBuiltinResource();

  this->mSynchronization = &MDySynchronization::GetInstance();
  const auto& metaInfo = MDyMetaInfo::GetInstance();
  const auto& bootResourceSpecifierList = metaInfo.GetBootResourceSpecifierList();
  SDyIOConnectionHelper::PopulateResources(bootResourceSpecifierList, true);

  return DY_SUCCESS;
}

EDySuccess DyEngine::pfRelease()
{
  this->pfReleaseDependentManager();
  this->mSynchronization = nullptr;
  this->pfReleaseIndependentManager();
  return DY_SUCCESS;
}

void DyEngine::operator()()
{
  static auto& window         = MDyWindow::GetInstance();
  static auto& timeManager    = MDyTime::GetInstance();
  static auto& soundManager   = MDySound::GetInstance();

  while (window.IsWindowShouldClose() == false)
  {
    timeManager.pUpdate();
    soundManager.Update(MDY_INITIALIZE_DEFINT);

    if (timeManager.IsGameFrameTicked() == DY_SUCCESS)
    {
      this->mSynchronization->RunFrame();

      if (this->mSynchronization->GetGlobalGameStatus() == EDyGlobalGameStatus::GameRuntime)
      {
        const auto dt = timeManager.GetGameScaledTickedDeltaTimeValue();

        this->pUpdate(dt);
        this->pRender();
      }
    }
  };
}

NotNull<TDyIO*> DyEngine::pfGetIOThread()
{
  MDY_ASSERT(MDY_CHECK_ISNOTNULL(this->mSynchronization), "Synchronization manager must be valid.");
  return this->mSynchronization->pfGetIOThread();
}

void DyEngine::pUpdate(_MIN_ TF32 dt)
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

void DyEngine::pRender()
{
  glClearColor(0.1f, 0.2f, 0.1f, 1.0f);
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

  this->GetWindowManager().TempSwapBuffers();
}

void DyEngine::pfInitializeIndependentManager()
{
  // `MDyLog` must be initialized first because of logging message from each managers.
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Initialize());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Initialize());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Initialize());
  MDY_CALL_ASSERT_SUCCESS(MDySynchronization::Initialize());
}

void DyEngine::pfInitializeDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Initialize());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Initialize());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Initialize());
}

void DyEngine::pfReleaseDependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWorld::Release());

  MDY_CALL_ASSERT_SUCCESS(dy::MDyScript::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyFont::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyPhysics::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySound::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyInput::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyRendering::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDySynchronization::Release());
}

  void DyEngine::pfReleaseIndependentManager()
{
  MDY_CALL_ASSERT_SUCCESS(dy::MDyWindow::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyTime::Release());

#if defined(MDY_FLAG_IN_EDITOR)
  MDY_CALL_ASSERT_SUCCESS(dy::editor::MDyEditorGui::Release());
#endif

  MDY_CALL_ASSERT_SUCCESS(dy::MDySetting::Release());
  MDY_CALL_ASSERT_SUCCESS(dy::MDyLog::Release());
}

MDyTime& DyEngine::GetTimeManager()     { return MDyTime::GetInstance(); }
MDyWindow& DyEngine::GetWindowManager() { return MDyWindow::GetInstance(); }

} /// ::dy namespace