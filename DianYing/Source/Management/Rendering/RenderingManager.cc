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
#include "Dy/Management/PhysicsManager.h"
#include "Dy/Component/CDyModelFilter.h"

//!
//! Forward declaration & Local translation unit function and data.
//!

namespace 
{

void CbGlGlobalStatus(const dy::DDyGlGlobalStatus& iTopStatus)
{
  if (iTopStatus.mPolygonMode.has_value() == true)
  {
    // Get value from structure.
    const auto& polygonMode = iTopStatus.mPolygonMode.value();
    GLenum mode   = GL_NONE;
    GLenum value  = GL_NONE;

    // Set mode
    switch (polygonMode.mMode)
    {
    case dy::DDyGlGlobalStatus::DPolygonMode::EMode::Front: mode = GL_FRONT;  break;
    case dy::DDyGlGlobalStatus::DPolygonMode::EMode::Back:  mode = GL_BACK;   break;
    case dy::DDyGlGlobalStatus::DPolygonMode::EMode::FrontAndBack: mode = GL_FRONT_AND_BACK; break;
    }
    // Set value
    switch (polygonMode.mValue)
    {
    case dy::DDyGlGlobalStatus::DPolygonMode::EValue::Triangle: value = GL_FILL; break;
    case dy::DDyGlGlobalStatus::DPolygonMode::EValue::Line:     value = GL_LINE; break;
    case dy::DDyGlGlobalStatus::DPolygonMode::EValue::Point:    value = GL_POINT; break;
    }

    // Issue into OpenGL system.
    glPolygonMode(mode, value);
  }
}

} /// ::unnamed namespace

//!
//! Implementation
//!

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
  this->mSkyPostEffect        = std::make_unique<decltype(this->mSkyPostEffect)::element_type>();
  this->mDebugRenderer        = std::make_unique<decltype(this->mDebugRenderer)::element_type>();

  // Set callback function for altering global render status.
  this->mInternalGlobalStatusStack.SetCallback(CbGlGlobalStatus);

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  case EDyRenderingApi::Vulkan: 
  case EDyRenderingApi::DirectX11: 
  case EDyRenderingApi::DirectX12: { MDY_NOT_IMPLEMENTED_ASSERT(); } break;
  case EDyRenderingApi::OpenGL: 
  {
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xFFFFFFFF);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    //! Push initial OpenGL global status.
    //! But we don't have to call callback function because it is alreay set on OpenGL system.
    DDyGlGlobalStatus initialStatus;
    {
      using EMode  = DDyGlGlobalStatus::DPolygonMode::EMode;
      using EValue = DDyGlGlobalStatus::DPolygonMode::EValue;
      // Set value.
      initialStatus.mIsEnableBlend       = glIsEnabled(GL_BLEND); 
      initialStatus.mIsEnableCullface    = glIsEnabled(GL_CULL_FACE);
      initialStatus.mIsEnableDepthTest   = glIsEnabled(GL_DEPTH_TEST);
      initialStatus.mIsEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
      initialStatus.mPolygonMode         = DDyGlGlobalStatus::DPolygonMode{EMode::FrontAndBack, EValue::Triangle}; 
      this->mInternalGlobalStatusStack.Push(initialStatus, false);
    }
      
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

  // 
  {
    // Get imgui IO module.
    ImGuiIO& io = ImGui::GetIO();
    const auto& windowManager = MDyWindow::GetInstance();

    {
      std::string japaneseFontName = "";
      if (windowManager.IsFontExistOnSystem(u8"Arial") == true)  { japaneseFontName = u8"Arial"; }
      
      // If font name is found, add font file to imgui. Otherwise, just pass it.
      if (japaneseFontName.empty() == false)
      {
        // Get system font path from manager.
        const auto optPath = windowManager.GetFontPathOnSystem(japaneseFontName);
        MDY_ASSERT_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
        // Create glyphs.
        io.Fonts->AddFontFromFileTTF(optPath.value().c_str(), 14.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
      }
    }

#ifdef false
    // Check hangul (NanumGothic) font is exist on system.
    {
      std::string koreanFontName = MDY_INITIALIZE_EMPTYSTR;
      if (windowManager.IsFontExistOnSystem(u8"나눔고딕") == true)  { koreanFontName = u8"나눔고딕"; }
      
      // If korean font name is found, add font file to imgui. Otherwise, just pass it.
      if (koreanFontName.empty() == false)
      {
        // Get system font path from manager.
        const auto optPath = windowManager.GetFontPathOnSystem(koreanFontName);
        MDY_ASSERT_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
        // Create korean glyphs.
        io.Fonts->AddFontFromFileTTF(optPath.value().c_str(), 14.0f, nullptr, io.Fonts->GetGlyphRangesKorean());
      }
    }

    // Check japanese (meiryo UI) font is exist on system. 
    {
      std::string japaneseFontName = "";
      if (windowManager.IsFontExistOnSystem(u8"Meiryo UI") == true)  { japaneseFontName = u8"Meiryo UI"; }
      
      // If font name is found, add font file to imgui. Otherwise, just pass it.
      if (japaneseFontName.empty() == false)
      {
        // Get system font path from manager.
        const auto optPath = windowManager.GetFontPathOnSystem(japaneseFontName);
        MDY_ASSERT_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
        // Create glyphs.
        io.Fonts->AddFontFromFileTTF(optPath.value().c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
      }
    }
#endif
  }

  //! Set common properties.
  auto& refSetting  = MDySetting::GetInstance();
  const auto width  = refSetting.GetWindowSizeWidth();
  const auto height = refSetting.GetWindowSizeHeight();
  this->mUiGeneralProjectionMatrix = DDyMatrix4x4::OrthoProjection(0.f, TF32(width), 0.f, TF32(height), -1.f, 100.0f);

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
  this->mSkyPostEffect        = MDY_INITIALIZE_NULL;
  this->mSSAOPostEffect       = MDY_INITIALIZE_NULL;
  this->mBasicOpaqueRenderer  = MDY_INITIALIZE_NULL;
  this->mUiBasicRenderer      = MDY_INITIALIZE_NULL;
  this->mFinalDisplayRenderer = MDY_INITIALIZE_NULL;
  this->mTranslucentOIT       = MDY_INITIALIZE_NULL;
  this->mDebugRenderer        = MDY_INITIALIZE_NULL;

  // Initialize internal management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(FDyModelHandlerManager::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Release());
  return DY_SUCCESS;
}

void MDyRendering::PreRender(_MIN_ TF32 dt)
{
  { // Checking 
    static bool lock = false;
    auto& settingManager = MDySetting::GetInstance();
    if (const auto flag = settingManager.IsRenderPhysicsCollisionShape(); flag != lock)
    {
      this->mDebugRenderer->Clear();
      lock = flag;
    }
  }

  // Get skybox pointer from present level.
  auto& refWorld = MDyWorld::GetInstance();
  auto optSkybox = refWorld.GetPtrMainLevelSkybox();
  if (optSkybox.has_value() == true)
  { // Bind
    this->mPtrRequiredSkybox = *optSkybox;
  }

  // Do cpu frustum culling.
  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  DyEraseRemoveIf(this->mOpaqueMeshDrawingList, [ptrCamera](TMeshDrawCallItem& iPtrOpaqueRenderer)
  {
    auto& [actorInfo, _, __] = iPtrOpaqueRenderer;
    const auto& worldPos = actorInfo->mPtrCompModelFilter
        ->GetBindedActor()->GetTransform()
        ->GetFinalWorldPosition();
    // Check
    return ptrCamera->CheckIsPointInFrustum(worldPos) == false;
  });

  // Set ordering of UI (If Debug and Loading UI is exist, also include them but as highest order. 
  // (so rendered as final.)
  auto& uiContainer = refWorld.MDY_PRIVATE(GetUiContainer)();

  // Order UI.
  auto& candidateUIList = uiContainer.GetActivatedUiWidgetList();
  std::sort(
      MDY_BIND_BEGIN_END(candidateUIList), 
      [](const auto& lhs, const auto& rhs) { return lhs->mZOrder > rhs->mZOrder; });
  this->mUiObjectDrawingList.insert(this->mUiObjectDrawingList.end(), MDY_BIND_BEGIN_END(candidateUIList));
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

void MDyRendering::EnqueueDebugDrawCollider(
    _MIN_ CDyPhysicsCollider& iRefCollider, 
    _MIN_ const DDyMatrix4x4& iTransformMatrix)
{
  this->mDebugColliderDrawingList.emplace_back(std::make_pair(&iRefCollider, iTransformMatrix));
}

void MDyRendering::RenderLevelInformation()
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }
  
  // If main camera is not exist, do not render level.
  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  if (ptrCamera == nullptr) { return; }

  { // Set overall rendering mode.
    DDyGlGlobalStatus initialStatus{};
    using DPolygonMode = DDyGlGlobalStatus::DPolygonMode;
    using EMode  = DDyGlGlobalStatus::DPolygonMode::EMode;
    using EValue = DDyGlGlobalStatus::DPolygonMode::EValue;
    //
    switch (MDySetting::GetInstance().GetRenderingMode())
    {
    case EDyModelRenderingMode::FillNormal: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Triangle}; 
      { MDY_GRAPHIC_SET_CRITICALSECITON();
        this->mInternalGlobalStatusStack.Push(initialStatus);
      }
    } break;
    case EDyModelRenderingMode::WireFrame: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Line}; 
      { MDY_GRAPHIC_SET_CRITICALSECITON();
        this->mInternalGlobalStatusStack.Push(initialStatus);
      }
    } break;
    }
  }
  
  // (0) Clear previous frame results of each framebuffers.
  this->pClearRenderingFramebufferInstances();
  this->mBasicOpaqueRenderer->PreRender();

  // (1) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
  if (MDY_GRAPHIC_SET_CRITICALSECITON(); 
      this->mBasicOpaqueRenderer->TrySetupRendering() == DY_SUCCESS)
  {
    glDisable(GL_CULL_FACE);
    for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mOpaqueMeshDrawingList)
    { // Render
      this->mBasicOpaqueRenderer->RenderScreen(
          *iPtrModel,
          const_cast<FDyMeshResource&>(*iPtrValidMesh),
          const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }

    // Pop setting.
    this->mBasicOpaqueRenderer->PopRenderingSetting();
  }
  SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mOpaqueMeshDrawingList.size()));

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
    SDyProfilingHelper::AddScreenRenderedActorCount(
        static_cast<TI32>(this->mTranslucentMeshDrawingList.size()));
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunci(0, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunci(1, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  this->mTranslucentMeshDrawingList.clear();
  this->mInternalGlobalStatusStack.Pop();
  
  //!
  //! Effects ▽
  //!

  // (3) Cascaded Shadow mapping to opaque call list. Pre-render update for update Segments.
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == true)
  {
    // pre-render update.
    this->mCSMRenderer->PreRender();
    if (this->mCSMRenderer->TrySetupRendering() == DY_SUCCESS)
    {
      // Cascade shadow mapping use different and mutliple viewport.
      this->mCSMRenderer->SetupViewport();
      // Render only opaque mesh list.
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

  //! Default Post processing effects
  if (information.mGraphics.mIsEnabledDefaultSsao == true)
  { 
    if (this->mSSAOPostEffect->TrySetupRendering() == DY_SUCCESS) { this->mSSAOPostEffect->RenderScreen(); }
  }
  else { this->mSSAOPostEffect->Clear(); }

  // https://www.khronos.org/opengl/wiki/Cubemap_Texture
  if (MDY_CHECK_ISNOTNULL(this->mPtrRequiredSkybox)
  &&  this->mSkyPostEffect->TrySetupRendering() == DY_SUCCESS)
  {
    this->mSkyPostEffect->RenderScreen();
  }

  // Final. 
  // Level information without debug information is integrated in one renderbuffer.
  if (MDY_CHECK_ISNOTEMPTY(this->mLevelFinalRenderer) 
  &&  this->mLevelFinalRenderer->IsReady() == true 
  &&  this->mLevelFinalRenderer->TrySetupRendering() == DY_SUCCESS)
  { 
    this->mLevelFinalRenderer->RenderScreen(); 
  }
}

void MDyRendering::RenderDebugInformation()
{
  //!
  //! Debug rendering.
  //! https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  //!

  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  const auto& setting   = MDySetting::GetInstance();

  // Draw collider shapes. (NOT AABB!)
  // If main camera is not exist, do not render level.
  if (MDY_CHECK_ISNOTNULL(ptrCamera) && setting.IsRenderPhysicsCollisionShape() == true)
  {
    SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mOpaqueMeshDrawingList.size()));
    // (1) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
    if (this->mDebugRenderer->IsReady() == true
    &&  this->mDebugRenderer->TrySetupRendering() == DY_SUCCESS)
    {
      for (auto& [ptrCollider, transformMatrix] : this->mDebugColliderDrawingList)
      {
        this->mDebugRenderer->RenderScreen(*ptrCollider, transformMatrix);
      }

      glEnable(GL_DEPTH_TEST);
    }
  }

  this->mDebugColliderDrawingList.clear();

  // Draw collider AABB.


}

void MDyRendering::RenderUIInformation()
{
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer)) 
  { 
    this->mUiBasicRenderer->RenderScreen(this->mUiObjectDrawingList); 
  }
  this->mUiObjectDrawingList.clear();
}

void MDyRendering::Integrate()
{
  //!
  //! Level & Ui integration section.
  //! ImGUI rendering will be held outside and after this function call.
  //!
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer)) { this->mFinalDisplayRenderer->RenderScreen(); }
}

void MDyRendering::MDY_PRIVATE(RenderLoading)()
{
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->Clear(); }

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->RenderScreen(this->mUiObjectDrawingList); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->RenderScreen(); }

  this->mUiObjectDrawingList.clear();
}

CDyDirectionalLight* MDyRendering::GetPtrMainDirectionalLight() const noexcept
{
  return this->mMainDirectionalLight;
}

void MDyRendering::pClearRenderingFramebufferInstances() noexcept
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }

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
void MDyRendering::MDY_PRIVATE(BindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalLight = &iRefLight;
}

/// @brief Private function, unbind directional light of main light.
EDySuccess MDyRendering::MDY_PRIVATE(UnbindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight)
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
void MDyRendering::MDY_PRIVATE(BindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalShadow = &iRefLight;
}

/// @brief Private function, unbind directional light of main light.
EDySuccess MDyRendering::MDY_PRIVATE(UnbindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalShadow == &iRefLight) 
  {
    this->mMainDirectionalShadow = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

const DDyMatrix4x4& MDyRendering::GetGeneralUiProjectionMatrix() const noexcept
{
  return this->mUiGeneralProjectionMatrix;
}

} /// ::dy namespace
