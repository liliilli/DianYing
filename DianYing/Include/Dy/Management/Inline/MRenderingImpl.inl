#ifndef GUARD_DY_MANAGEMENT_RENDERINGMANAGER_IMPL_INL
#define GUARD_DY_MANAGEMENT_RENDERINGMANAGER_IMPL_INL
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

inline MDyRendering::Impl::Impl()
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
  this->mDebugShapeRenderer   = std::make_unique<decltype(this->mDebugShapeRenderer)::element_type>();
  this->mDebugAABBRenderer    = std::make_unique<decltype(mDebugAABBRenderer)::element_type>();
  this->mDebugPickingRenderer = std::make_unique<decltype(this->mDebugPickingRenderer)::element_type>();

  // Set callback function for global internal status stack.
  this->mInternal_FeatBlendStack.SetCallback(CbGlFeatBlendStack);
  this->mInternal_FeatDepthTestStack.SetCallback(CbGlFeatDepthTestStack);
  this->mInternal_FeatCullfaceStack.SetCallback(CbGlFeatCullfaceStack);
  this->mInternal_FeatScissorTestStack.SetCallback(CbGlFeatScissorTestStack);
  this->mInternal_PolygonModeStack.SetCallback(CbGlPolygonModeStack);
  this->mInternal_BlendModeStack.SetCallback(CbGlBlendModeStatus);
  this->mInternal_CullfaceModeStack.SetCallback(CbGlCullfaceModeStack);
  this->mInternal_ViewportStack.SetCallback(CbGlViewportStack);

  switch (MDySetting::GetInstance().GetRenderingType())
  {
  case EDyRenderingApi::OpenGL: 
  {
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(kIndiceSeparator);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    //! Push initial OpenGL global status.
    //! But we don't have to call callback function because it is alreay set on OpenGL system.
    DDyGlGlobalStatus initialStatus;
    {
      using EMode  = DDyGlGlobalStatus::DPolygonMode::EMode;
      using EValue = DDyGlGlobalStatus::DPolygonMode::EValue;
      using DPolygonMode  = DDyGlGlobalStatus::DPolygonMode;
      // Set value.
      initialStatus.mIsEnableBlend       = glIsEnabled(GL_BLEND); 
      initialStatus.mIsEnableCullface    = glIsEnabled(GL_CULL_FACE);
      initialStatus.mIsEnableDepthTest   = glIsEnabled(GL_DEPTH_TEST);
      initialStatus.mIsEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
      initialStatus.mPolygonMode         = DPolygonMode{EMode::FrontAndBack, EValue::Triangle}; 
      // Get blend mode.
      {
        using DBlendMode = DDyGlGlobalStatus::DBlendMode;
        using EEqut = DDyGlGlobalStatus::DBlendMode::EEqut;
        using EFunc = DDyGlGlobalStatus::DBlendMode::EFunc;
        DBlendMode mode{};
        mode.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::SrcAlpha, EFunc::OneMinusSrcAlpha);
        initialStatus.mBlendMode = mode;
      }
      // Get cullface mode.
      {
        using DCullfaceMode = DDyGlGlobalStatus::DCullfaceMode;
        DCullfaceMode cullface{DCullfaceMode::EValue::Back};
        initialStatus.mCullfaceMode = cullface;
      }
      // Get default viewport.
      {
        using DViewport = DDyGlGlobalStatus::DViewport;
        DViewport defaultViewport;
        // Get global size. 
        GLint defaultSize[4]; glGetIntegerv(GL_VIEWPORT, defaultSize); 
        defaultViewport.mViewportSettingList.emplace_back(
            -1, // Global 
            DDyArea2D{defaultSize[0], defaultSize[1], defaultSize[2], defaultSize[3]}
        );
        initialStatus.mViewportSettingList = defaultViewport;
      }
      // Insert
      this->InsertInternalGlobalStatus(initialStatus);
    }
      
    { // IMGUI Setting
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();

      ImGui_ImplGlfw_InitForOpenGL(MDyWindow::GetInstance().GetGLMainWindow(), true);
      ImGui_ImplOpenGL3_Init("#version 430");
    }
  } break;
  case EDyRenderingApi::Vulkan: 
  case EDyRenderingApi::DirectX11: 
  case EDyRenderingApi::DirectX12: { MDY_NOT_IMPLEMENTED_ASSERT(); } break;
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
}

inline MDyRendering::Impl::~Impl()
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
    DyPushLogDebugInfo("Released ImGui Context.");
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
  this->mDebugShapeRenderer   = MDY_INITIALIZE_NULL;
  this->mDebugAABBRenderer    = nullptr;
  this->mDebugPickingRenderer = nullptr;

  // Initialize internal management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(FDyModelHandlerManager::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Release());
}

inline void MDyRendering::Impl::PreRender(TF32 dt)
{
  { // Checking 
    static bool lock = false;
    auto& settingManager = MDySetting::GetInstance();
    if (const auto flag = settingManager.IsRenderPhysicsCollisionShape(); flag != lock)
    {
      this->mDebugShapeRenderer->Clear();
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

inline void MDyRendering::Impl::SetupDrawModelTaskQueue()
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

inline void MDyRendering::Impl::EnqueueDrawMesh(
  DDyModelHandler::DActorInfo& iRefModelRenderer,
  const FDyMeshResource& iRefValidMesh, 
  const FDyMaterialResource& iRefValidMat)
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

inline void MDyRendering::Impl::EnqueueDebugDrawCollider(
  CDyPhysicsCollider& iRefCollider, 
  const DDyMatrix4x4& iTransformMatrix)
{
  this->mDebugColliderDrawingList.emplace_back(std::make_pair(&iRefCollider, iTransformMatrix));
}

inline void MDyRendering::Impl::RenderLevelInformation()
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
        this->InsertInternalGlobalStatus(initialStatus);
      }
    } break;
    case EDyModelRenderingMode::WireFrame: 
    {
      initialStatus.mPolygonMode = DPolygonMode{EMode::FrontAndBack, EValue::Line}; 
      { MDY_GRAPHIC_SET_CRITICALSECITON();
        this->InsertInternalGlobalStatus(initialStatus);
      }
    } break;
    }
  }
  
  // (0) Clear previous frame results of each framebuffers.
  this->pClearRenderingFramebufferInstances();
  this->mBasicOpaqueRenderer->PreRender();

  // (1) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
  if (MDY_GRAPHIC_SET_CRITICALSECITON(); 
      this->mBasicOpaqueRenderer->TryPushRenderingSetting() == DY_SUCCESS)
  {
    for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mOpaqueMeshDrawingList)
    { // Render
      this->mBasicOpaqueRenderer->RenderScreen(
          *iPtrModel,
          const_cast<FDyMeshResource&>(*iPtrValidMesh),
          const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }

    // Pop setting.
    this->mBasicOpaqueRenderer->TryPopRenderingSetting();
  }
  SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mOpaqueMeshDrawingList.size()));

  // (2) Draw transparent call list with OIT.
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      this->mTranslucentMeshDrawingList.empty() == false
  &&  this->mTranslucentOIT->TryPushRenderingSetting() == DY_SUCCESS)
  {
    for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mTranslucentMeshDrawingList)
    { // Render
      this->mTranslucentOIT->RenderScreen(
          *iPtrModel->mPtrModelRenderer, 
          const_cast<FDyMeshResource&>(*iPtrValidMesh),
          const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }
    // Pop Setting
    this->mTranslucentOIT->TryPopRenderingSetting(); 
  }
  SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mTranslucentMeshDrawingList.size()));
  this->mTranslucentMeshDrawingList.clear();

  // Pop.
  this->PopInternalGlobalStatus();
  
  //!
  //! Effects ▽
  //!

  // (3) Cascaded Shadow mapping to opaque call list. Pre-render update for update Segments.
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      information.mGraphics.mIsEnabledDefaultShadow == true
  &&  this->mCSMRenderer->TryPushRenderingSetting() == DY_SUCCESS)
  {
    // Cascade shadow mapping use different and mutliple viewport.
    // Render only opaque mesh list.
    for (auto& [iPtrModel, iPtrValidMesh, iPtrValidMat] : this->mOpaqueMeshDrawingList)
    { // Render
      this->mCSMRenderer->RenderScreen(
          *iPtrModel, 
          const_cast<FDyMeshResource&>(*iPtrValidMesh),
          const_cast<FDyMaterialResource&>(*iPtrValidMat)
      );
    }
    // Pop Setting
    // Set global viewport values to camera's properties.
    this->mCSMRenderer->TryPopRenderingSetting();
  }
  else { this->mCSMRenderer->Clear(); }
  this->mOpaqueMeshDrawingList.clear();

  //! Default Post processing effects
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      information.mGraphics.mIsEnabledDefaultSsao == true
  &&  this->mSSAOPostEffect->TryPushRenderingSetting() == DY_SUCCESS)
  { 
    this->mSSAOPostEffect->RenderScreen();
    // Pop Setting.
    this->mSSAOPostEffect->TryPopRenderingSetting();
  }
  else { this->mSSAOPostEffect->Clear(); }

  // https://www.khronos.org/opengl/wiki/Cubemap_Texture
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      this->mPtrRequiredSkybox != nullptr
  &&  this->mSkyPostEffect->TryPushRenderingSetting() == DY_SUCCESS)
  {
    this->mSkyPostEffect->RenderScreen();
    this->mSkyPostEffect->TryPopRenderingSetting();
  }

  // Final. 
  // Level information without debug information is integrated in one renderbuffer.
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      this->mLevelFinalRenderer != nullptr 
  &&  this->mLevelFinalRenderer->TryPushRenderingSetting() == DY_SUCCESS)
  { 
    this->mLevelFinalRenderer->RenderScreen(); 
    this->mLevelFinalRenderer->TryPopRenderingSetting();
  }
}

inline void MDyRendering::Impl::RenderDebugInformation()
{
  //!
  //! Debug rendering.
  //! https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/DebugVisualization.html#debugvisualization
  //!

  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  if (ptrCamera == nullptr) { return; }

  const auto& setting   = MDySetting::GetInstance();
  
  // Set status
  DDyGlGlobalStatus statusSetting;
  statusSetting.mIsEnableDepthTest = false;
  this->InsertInternalGlobalStatus(statusSetting);
 
  if (setting.IsRenderPhysicsCollisionShape() == true)
  { // Draw collider shapes. (NOT AABB!) If main camera is not exist, do not render level.
    // (1) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
    if (MDY_GRAPHIC_SET_CRITICALSECITON();
        this->mDebugShapeRenderer != nullptr
    &&  this->mDebugShapeRenderer->TryPushRenderingSetting() == DY_SUCCESS)
    {
      for (auto& [ptrCollider, transformMatrix] : this->mDebugColliderDrawingList)
      {
        this->mDebugShapeRenderer->RenderScreen(*ptrCollider, transformMatrix);
      }
      // Pop setting.
      this->mDebugShapeRenderer->TryPopRenderingSetting();
    }
  }

  if (setting.IsRenderPhysicsCollisionAABB() == true) 
  { // Draw collider AABB.
    // (2) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
    if (MDY_GRAPHIC_SET_CRITICALSECITON();
        this->mDebugAABBRenderer != nullptr
    &&  this->mDebugAABBRenderer->TryPushRenderingSetting() == DY_SUCCESS)
    {
      for (auto& [ptrCollider, transformMatrix] : this->mDebugColliderDrawingList)
      {
        this->mDebugAABBRenderer->RenderScreen(*ptrCollider, transformMatrix);
      }
      // Pop setting.
      this->mDebugAABBRenderer->TryPopRenderingSetting();
    }
  }
  this->mDebugColliderDrawingList.clear();

  // If picking actor is exist, render it only edge using kernel & ping-pong.
  if (auto& refInput = MDyInput::GetInstance();
      refInput.IsActorPicked() == true)
  {
    auto& refActor = *(*refInput.__GetPPtrPickingTarget());
    if (MDY_GRAPHIC_SET_CRITICALSECITON();
        this->mDebugPickingRenderer != nullptr
    &&  this->mDebugPickingRenderer->TryPushRenderingSetting() == DY_SUCCESS)
    {
      this->mDebugPickingRenderer->RenderScreen(refActor);
    }
    // Pop setting.
    this->mDebugPickingRenderer->TryPopRenderingSetting();
  }

  this->PopInternalGlobalStatus();
}

inline void MDyRendering::Impl::RenderUIInformation()
{
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      this->mUiBasicRenderer != nullptr
  &&  this->mUiBasicRenderer->TryPushRenderingSetting() == DY_SUCCESS) 
  { 
    this->mUiBasicRenderer->RenderScreen(this->mUiObjectDrawingList); 
    this->mUiBasicRenderer->TryPopRenderingSetting();
  }
  this->mUiObjectDrawingList.clear();
}

inline void MDyRendering::Impl::Integrate()
{
  //! Level & Ui & Debug integration section.
  //! ImGUI rendering will be held outside and after this function call.
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      this->mFinalDisplayRenderer != nullptr
  &&  this->mFinalDisplayRenderer->TryPushRenderingSetting() == DY_SUCCESS) 
  { 
    this->mFinalDisplayRenderer->RenderScreen(); 
    this->mFinalDisplayRenderer->TryPopRenderingSetting();
  }
}

inline void MDyRendering::Impl::MDY_PRIVATE(RenderLoading)()
{
  if (MDY_GRAPHIC_SET_CRITICALSECITON();
      this->mUiBasicRenderer != nullptr
  &&  this->mUiBasicRenderer->TryPushRenderingSetting() == DY_SUCCESS) 
  { 
    this->mUiBasicRenderer->RenderScreen(this->mUiObjectDrawingList); 
    this->mUiBasicRenderer->TryPopRenderingSetting();
  }
  this->mUiObjectDrawingList.clear();
  this->Integrate();
}

inline CDyDirectionalLight* MDyRendering::Impl::GetPtrMainDirectionalLight() const noexcept
{
  return this->mMainDirectionalLight;
}

inline void MDyRendering::Impl::pClearRenderingFramebufferInstances() noexcept
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }

  // Reset final rendering mesh setting.
  if (MDY_CHECK_ISNOTEMPTY(this->mLevelFinalRenderer))    { this->mLevelFinalRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->Clear(); }
}

inline void MDyRendering::Impl::MDY_PRIVATE(BindMainDirectionalLight)(CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalLight = &iRefLight;
}

inline EDySuccess MDyRendering::Impl::MDY_PRIVATE(UnbindMainDirectionalLight)(CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalLight == &iRefLight) 
  {
    this->mMainDirectionalLight = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

inline CDyDirectionalLight* MDyRendering::Impl::GetPtrMainDirectionalShadow() const noexcept
{
  return this->mMainDirectionalShadow;
}

inline void MDyRendering::Impl::MDY_PRIVATE(BindMainDirectionalShadow)(CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalShadow = &iRefLight;
}

inline EDySuccess MDyRendering::Impl::MDY_PRIVATE(UnbindMainDirectionalShadow)(CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalShadow == &iRefLight) 
  {
    this->mMainDirectionalShadow = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

inline const DDyMatrix4x4& MDyRendering::Impl::GetGeneralUiProjectionMatrix() const noexcept
{
  return this->mUiGeneralProjectionMatrix;
}

inline void MDyRendering::Impl::InsertInternalGlobalStatus(_MIN_ const DDyGlGlobalStatus& iNewStatus)
{
  //
  this->mInternalGlobalStatusStack.Push(iNewStatus, false);

  const auto& topStatusChunk = this->mInternalGlobalStatusStack.Top();
  // Set
  if (topStatusChunk.mIsEnableBlend.has_value() == true)
  { this->mInternal_FeatBlendStack.Push(*topStatusChunk.mIsEnableBlend); }
  if (topStatusChunk.mIsEnableCullface.has_value() == true)
  { this->mInternal_FeatCullfaceStack.Push(*topStatusChunk.mIsEnableCullface); }
  if (topStatusChunk.mIsEnableDepthTest.has_value() == true)
  { this->mInternal_FeatDepthTestStack.Push(*topStatusChunk.mIsEnableDepthTest); }
  if (topStatusChunk.mIsEnableScissorTest.has_value() == true)
  { this->mInternal_FeatScissorTestStack.Push(*topStatusChunk.mIsEnableScissorTest); }

  if (topStatusChunk.mBlendMode.has_value() == true)
  { this->mInternal_BlendModeStack.Push(*topStatusChunk.mBlendMode); }
  if (topStatusChunk.mPolygonMode.has_value() == true)
  { this->mInternal_PolygonModeStack.Push(*topStatusChunk.mPolygonMode); }
  if (topStatusChunk.mCullfaceMode.has_value() == true)
  { this->mInternal_CullfaceModeStack.Push(*topStatusChunk.mCullfaceMode); }
  if (topStatusChunk.mViewportSettingList.has_value() == true)
  { this->mInternal_ViewportStack.Push(*topStatusChunk.mViewportSettingList); }
}

inline void MDyRendering::Impl::PopInternalGlobalStatus()
{
  if (this->mInternalGlobalStatusStack.IsEmpty() == true) { return; }

  auto extracted = this->mInternalGlobalStatusStack.ExtractTop(false);

  if (extracted.mIsEnableBlend.has_value() == true)       { this->mInternal_FeatBlendStack.Pop(); }
  if (extracted.mIsEnableCullface.has_value() == true)    { this->mInternal_FeatCullfaceStack.Pop(); }
  if (extracted.mIsEnableDepthTest.has_value() == true)   { this->mInternal_FeatDepthTestStack.Pop(); }
  if (extracted.mIsEnableScissorTest.has_value() == true) { this->mInternal_FeatScissorTestStack.Pop(); }

  if (extracted.mBlendMode.has_value() == true)     { this->mInternal_BlendModeStack.Pop(); }
  if (extracted.mPolygonMode.has_value() == true)   { this->mInternal_PolygonModeStack.Pop(); }
  if (extracted.mCullfaceMode.has_value() == true)  { this->mInternal_CullfaceModeStack.Pop(); }
  if (extracted.mViewportSettingList.has_value() == true)  { this->mInternal_ViewportStack.Pop(); }
}

inline void MDyRendering::Impl::SwapBuffers()
{
  auto& managerWindow = MDyWindow::GetInstance();
  
  if (managerWindow.IsWindowShouldClose() == true) { return; }
  glfwSwapBuffers(managerWindow.GetGLMainWindow());
  glfwPollEvents();
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_RENDERINGMANAGER_IMPL_INL