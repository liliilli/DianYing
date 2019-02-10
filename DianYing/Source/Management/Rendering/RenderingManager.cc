#include <precompiled.h>
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

/// Header file
#include <Dy/Management/Rendering/RenderingManager.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Management/Editor/GuiSetting.h>

#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Management/Helper/SDyProfilingHelper.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Management/Internal/Render/FDyModelHandlerManager.h>
#include "Dy/Core/Resource/Resource/FDyModelResource.h"
#include "Dy/Management/WindowManager.h"

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{ 
  // Initialize framebuffer management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Initialize());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Initialize());
  MDY_CALL_ASSERT_SUCCESS(FDyModelHandlerManager::Initialize());

  this->mBasicOpaqueRenderer  = std::make_unique<decltype(this->mBasicOpaqueRenderer)::element_type>();
  this->mTranslucentOIT       = std::make_unique<decltype(this->mTranslucentOIT)::element_type>();
  this->mUiBasicRenderer      = std::make_unique<decltype(this->mUiBasicRenderer)::element_type>();
  this->mLevelFinalRenderer   = std::make_unique<decltype(this->mLevelFinalRenderer)::element_type>();
  this->mFinalDisplayRenderer = std::make_unique<decltype(this->mFinalDisplayRenderer)::element_type>();
  this->mCSMRenderer          = std::make_unique<decltype(this->mCSMRenderer)::element_type>();
  this->mSSAOPostEffect       = std::make_unique<decltype(mSSAOPostEffect)::element_type>();

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  case EDyRenderingApi::Vulkan: 
  case EDyRenderingApi::DirectX11: 
  case EDyRenderingApi::DirectX12: 
  { MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  case EDyRenderingApi::OpenGL: 
  {
    { // IMGUI Setting
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();

      ImGui_ImplGlfw_InitForOpenGL(MDyWindow::GetInstance().GetGLMainWindow(), true);
      ImGui_ImplOpenGL3_Init("#version 430");
    }
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

#if defined(MDY_FLAG_IN_EDITOR) == true
  //! Grid rendering setting.
  this->mGridEffect = std::make_unique<decltype(this->mGridEffect)::element_type>();
#endif /// MDY_FLAG_IN_EDITOR

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  switch (MDySetting::GetInstance().GetRenderingType())
  {
  case EDyRenderingApi::Vulkan: 
  case EDyRenderingApi::DirectX11: 
  case EDyRenderingApi::DirectX12: 
  { MDY_NOT_IMPLEMENTED_ASSERT();
  } break;
  case EDyRenderingApi::OpenGL:
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    MDY_LOG_INFO_D("Released ImGui Context.");
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  this->mLevelFinalRenderer   = MDY_INITIALIZE_NULL;
  this->mCSMRenderer          = MDY_INITIALIZE_NULL;
  this->mSSAOPostEffect       = MDY_INITIALIZE_NULL;
  this->mBasicOpaqueRenderer  = MDY_INITIALIZE_NULL;
  this->mUiBasicRenderer      = MDY_INITIALIZE_NULL;
  this->mFinalDisplayRenderer = MDY_INITIALIZE_NULL;
  this->mTranslucentOIT       = MDY_INITIALIZE_NULL;

  // Initialize internal management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(FDyModelHandlerManager::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Release());
  return DY_SUCCESS;
}

void MDyRendering::SetupDrawModelTaskQueue()
{
  auto& handlerManager = FDyModelHandlerManager::GetInstance();

  const auto& modelHandleContainer = handlerManager.GetContainer();
  for (const auto& [specifier, ptrsmtInstance] : modelHandleContainer)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtInstance)) { continue; }
    if (ptrsmtInstance->IsModelResourceValid() == false) { continue; }

    // Loop handler's actor information instance.
    auto& actorContainer = ptrsmtInstance->GetActorContainer();
    std::vector<NotNull<DDyModelHandler::DActorInfo*>> renderableActorList;

    // Insert satisfied render actor information instance.
    for (auto& [_, actorInfo] : actorContainer)
    {
      if (actorInfo.mPtrCompModelFilter == nullptr) { continue; }
      if (actorInfo.mPtrModelRenderer == nullptr) { continue; }
      renderableActorList.emplace_back(DyMakeNotNull(&actorInfo));
    }
  
    // Get 
    const auto& refBinder = ptrsmtInstance->GetModelBinderReference();
    const FDyModelResource* ptrModelResc = refBinder.Get();

    const auto& bindMeshList = ptrModelResc->GetMeshResourceList();
    const auto& bindMateList = ptrModelResc->GetMaterialResourceList();
    const auto  numMeshCount = static_cast<TU32>(bindMeshList.size());

    // Process
    for (TU32 i = 0; i < numMeshCount; ++i)
    {
      const auto& refMeshResc = *bindMeshList[i]->Get();
      const auto& refMatResc  = *bindMateList[i]->Get();

      // Insert static queue.
      for (auto& ptrActorInfo : renderableActorList)
      {
        this->EnqueueDrawMesh(*ptrActorInfo, refMeshResc, refMatResc);
      }
    }
  }
}

void MDyRendering::EnqueueDrawMesh(
    _MIN_ DDyModelHandler::DActorInfo& iRefModelRenderer,
    _MIN_ const FDyMeshResource& iRefValidMesh, 
    _MIN_ const FDyMaterialResource& iRefValidMat)
{
  switch (iRefValidMat.GetBlendMode())
  {
  case EDyMaterialBlendMode::Opaque: 
  {
    this->mOpaqueMeshDrawingList.emplace_back(&iRefModelRenderer, &iRefValidMesh, &iRefValidMat);
  } break;
  case EDyMaterialBlendMode::TranslucentOIT: 
  {
    this->mTranslucentMeshDrawingList.emplace_back(&iRefModelRenderer, &iRefValidMesh, &iRefValidMat);
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

void MDyRendering::RenderDrawCallQueue()
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();

  // (0) Clear previous frame results of each framebuffers.
  this->pClearRenderingFramebufferInstances();

  // @TODO TEMPORAL. MAKE STACK-BASED GL STATE MACHINE MANAGER.
  switch (MDySetting::GetInstance().GetRenderingMode())
  {
  case EDyModelRenderingMode::FillNormal: 
  {
    MDY_GRAPHIC_SET_CRITICALSECITON();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } break;
  case EDyModelRenderingMode::WireFrame: 
  {
    MDY_GRAPHIC_SET_CRITICALSECITON();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } break;
  }

  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  // DO CPU VIEW-FRUSTUM CULLING.
#ifdef false
  DyEraseRemoveIf(this->mOpaqueDrawCallList, [ptrCamera](auto& iPtrOpaqueRenderer)
  {
    const auto& worldPos = iPtrOpaqueRenderer->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
    return ptrCamera->CheckIsPointInFrustum(worldPos) == false;
  });
#endif

  // If main camera is not exist, do not render level.
  if (MDY_CHECK_ISNOTNULL(ptrCamera))
  {
    SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mOpaqueMeshDrawingList.size()));
    // (1) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
    this->mBasicOpaqueRenderer->PreRender();
    for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mOpaqueMeshDrawingList)
    { // Render
      this->mBasicOpaqueRenderer->RenderScreen(
          *iPtrModel,
          const_cast<FDyMeshResource&>(*iPtrValidMesh),
          const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }
  }

  // (2) Draw transparent call list with OIT.
  if (this->mTranslucentOIT->TrySetupRendering() == DY_SUCCESS)
  {
    for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mTranslucentMeshDrawingList)
    { // Render
      this->mTranslucentOIT->RenderScreen(
          *iPtrModel->mPtrModelRenderer, 
          const_cast<FDyMeshResource&>(*iPtrValidMesh),
          const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }
    SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mTranslucentMeshDrawingList.size()));
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunci(1, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  this->mTranslucentMeshDrawingList.clear();
  
  //!
  //! Effects ▽
  //!
  
  // @TODO TEMPORARY CODE
  {
    MDY_GRAPHIC_SET_CRITICALSECITON();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }


  // (3) Cascaded Shadow mapping to opaque call list. Pre-render update for update Segments.
  if (MDY_CHECK_ISNOTNULL(ptrCamera) && information.mGraphics.mIsEnabledDefaultShadow == true)
  {
    this->mCSMRenderer->PreRender();
    if (this->mCSMRenderer->TrySetupRendering() == DY_SUCCESS)
    {
      this->mCSMRenderer->SetupViewport();
      for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mOpaqueMeshDrawingList)
      { // Render
        this->mCSMRenderer->RenderScreen(
            *iPtrModel, 
            const_cast<FDyMeshResource&>(*iPtrValidMesh),
            const_cast<FDyMaterialResource&>(*iPtrValidMat)
        );
      }
    }
  }
  else { this->mCSMRenderer->Clear(); }
  this->mOpaqueMeshDrawingList.clear();

  // Set global viewport values to camera's properties.
  { MDY_GRAPHIC_SET_CRITICALSECITON();
    FDyGLWrapper::SetViewport(ptrCamera->GetPixelizedViewportRectangle());
  }

  //! Post processing effects
  if (information.mGraphics.mIsEnabledDefaultSsao == true)
  { 
    if (this->mSSAOPostEffect->TrySetupRendering() == DY_SUCCESS)
    { this->mSSAOPostEffect->RenderScreen(); }
  }
  else { this->mSSAOPostEffect->Clear(); }

  // Final
  if (MDY_CHECK_ISNOTEMPTY(this->mLevelFinalRenderer) 
  &&  this->mLevelFinalRenderer->IsReady() == true 
  &&  this->mLevelFinalRenderer->TrySetupRendering() == DY_SUCCESS)
  { this->mLevelFinalRenderer->RenderScreen(); }

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->RenderScreen(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->RenderScreen(); }
}

void MDyRendering::MDY_PRIVATE_SPECIFIER(RenderLoading)()
{
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->Clear(); }

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->RenderScreen(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->RenderScreen(); }
}

CDyDirectionalLight* MDyRendering::GetPtrMainDirectionalLight() const noexcept
{
  return this->mMainDirectionalLight;
}

void MDyRendering::pClearRenderingFramebufferInstances() noexcept
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }

  if (MDY_CHECK_ISNOTEMPTY(this->mBasicOpaqueRenderer))   { this->mBasicOpaqueRenderer->Clear(); }

  // @TODO DO NOTHING NOW.
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultSsao == true) { }
  // Reset all shadow framebuffer setting.
  //if (information.mGraphics.mIsEnabledDefaultShadow == true) { this->mShadowRenderer->Clear(); }

#if defined(MDY_FLAG_IN_EDITOR) == false
  // Reset final rendering mesh setting.
  if (MDY_CHECK_ISNOTEMPTY(this->mLevelFinalRenderer))    { this->mLevelFinalRenderer->Clear(); }
#endif
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->Clear(); }
}

/// @brief Private function, bind directional light as main light.
void MDyRendering::MDY_PRIVATE_SPECIFIER(BindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalLight = &iRefLight;
}

/// @brief Private function, unbind directional light of main light.
EDySuccess MDyRendering::MDY_PRIVATE_SPECIFIER(UnbindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalLight == &iRefLight) 
  {
    this->mMainDirectionalLight = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

CDyDirectionalLight* MDyRendering::GetPtrMainDirectionalShadow() const noexcept
{
  return this->mMainDirectionalShadow;
}

/// @brief Private function, bind directional light as main light.
void MDyRendering::MDY_PRIVATE_SPECIFIER(BindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalShadow = &iRefLight;
}

/// @brief Private function, unbind directional light of main light.
EDySuccess MDyRendering::MDY_PRIVATE_SPECIFIER(UnbindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalShadow == &iRefLight) 
  {
    this->mMainDirectionalShadow = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

} /// ::dy namespace
