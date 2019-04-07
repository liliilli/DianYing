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

#include <Dy/Meta/Information/MetaInfoRenderPipeline.h>
#include <Dy/Component/CTransform.h>
#include <Math/Utility/XGraphicsMath.h>
#include <Dy/Include/GlInclude.h>

namespace dy
{

inline EDySuccess MRendering::Impl::Initialize()
{
  // Initialize framebuffer management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Initialize());
  MDY_CALL_ASSERT_SUCCESS(MUniformBufferObject::Initialize());
  MDY_CALL_ASSERT_SUCCESS(FDyModelHandlerManager::Initialize());

  const auto renderingApiType = MSetting::GetInstance().GetRenderingType();
  switch (renderingApiType)
  {
  case EDyRenderingApi::OpenGL: 
  {
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(kIndiceSeparator);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    XGLWrapper::SetupInitialGlobalStatus();
  } break;
  case EDyRenderingApi::Vulkan: 
  case EDyRenderingApi::DirectX11: 
  case EDyRenderingApi::DirectX12: { MDY_NOT_IMPLEMENTED_ASSERT(); } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  // Setup default render pipeline
  this->mEntryRenderPipelines.reserve(8);
  this->CreateRenderPipeline("dyBtDefault");
  this->CreateRenderPipeline("dyBtDefaultLoading");
  
  // Check Rendering API.
  switch (renderingApiType)
  {
  case EDyRenderingApi::Vulkan: 
  case EDyRenderingApi::DirectX11: 
  case EDyRenderingApi::DirectX12: 
    MDY_NOT_IMPLEMENTED_ASSERT();
    break;
  case EDyRenderingApi::OpenGL:
  { // IMGUI Setting
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(MWindow::GetInstance().GetGLMainWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }

  // 
  {
    // Get imgui IO module.
    ImGuiIO& io = ImGui::GetIO();
    const auto& windowManager = MWindow::GetInstance();

    {
      std::string japaneseFontName = "";
      if (windowManager.IsFontExistOnSystem(u8"Arial") == true)  { japaneseFontName = u8"Arial"; }
      
      // If font name is found, add font file to imgui. Otherwise, just pass it.
      if (japaneseFontName.empty() == false)
      {
        // Get system font path from manager.
        const auto optPath = windowManager.GetFontPathOnSystem(japaneseFontName);
        MDY_ASSERT_MSG_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
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
        MDY_ASSERT_MSG_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
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
        MDY_ASSERT_MSG_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
        // Create glyphs.
        io.Fonts->AddFontFromFileTTF(optPath.value().c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
      }
    }
#endif
  }

  //! Set common properties.
  auto& refSetting  = MSetting::GetInstance();
  const auto width  = refSetting.GetWindowSizeWidth();
  const auto height = refSetting.GetWindowSizeHeight();
  this->mUiGeneralProjectionMatrix = math::ProjectionMatrix<TReal>(
    math::EGraphics::OpenGL, math::EProjection::Orthogonal,
    0.f, TF32(width), 0.f, TF32(height), -1.f, 100.0f
  );

  return DY_SUCCESS;
}

inline EDySuccess MRendering::Impl::CreateRenderPipeline(const std::string& iPipelineSpecifier)
{
  if (ContainsIf(this->mEntryRenderPipelines, 
    [iPipelineSpecifier](const auto& pipeline)
    {
      return pipeline.GetName() == iPipelineSpecifier;
    }) == true)
  {

    return DY_FAILURE;
  }

  const auto& managerMeta = MIOMeta::GetInstance();
  // At first, check render pipeline is exist.
  if (managerMeta.IsRenderPipelineExist(iPipelineSpecifier) == false)
  {
    DyPushLogError(
      "Failed to create render pipeline, {}. Meta information is not exist.", 
      iPipelineSpecifier);
    return DY_FAILURE;
  }

  // Second, check child render pipeline and local render item is exist on Dy system.
  auto& entryRenderPipeline = managerMeta.GetRenderPipeline(iPipelineSpecifier);
  if (PDyRenderPipelineInstanceMetaInfo::HasValidChildItems(entryRenderPipeline) == false)
  {
    return DY_FAILURE;
  }

  // Create each render pipeline and render item into list, with wrapping to control handlers.
  // First, try to create render pipeline.
  auto optPipelineList = PDyRenderPipelineInstanceMetaInfo::GetAllChildPipelineNames(entryRenderPipeline);
  if (optPipelineList.has_value() == false) { return DY_FAILURE; }

  (*optPipelineList).emplace(iPipelineSpecifier);
  for (auto& renderPipelineName : (*optPipelineList))
  {
    if (Contains(this->mRenderPipelines, renderPipelineName) == false)
    {
      const auto& metaRenderPipeline = managerMeta.GetRenderPipeline(renderPipelineName);
      auto[_, isSucceeded] = this->mRenderPipelines.try_emplace(
        renderPipelineName,
        std::make_unique<FWrapperRenderPipeline>(metaRenderPipeline)
      );
      MDY_ASSERT_FORCE(isSucceeded == true);
    }
  }
  
  // Second, try to create render item.
  auto optItemList = PDyRenderPipelineInstanceMetaInfo::GetAllRenderItemNames(entryRenderPipeline);
  if (optItemList.has_value() == false) { return DY_FAILURE; }

  for (auto& renderItemName : (*optItemList))
  {
    if (Contains(this->mRenderItems, renderItemName) == false)
    {
      const auto& metaRenderItem = managerMeta.GetRenderItem(renderItemName);
      auto[_, isSucceeded] = this->mRenderItems.try_emplace(
        renderItemName,
        std::make_unique<FWrapperRenderItem>(metaRenderItem)
      );
      MDY_ASSERT_FORCE(isSucceeded == true);
    }
  }

  // And create actual render pipeline that just have handles of each render pipeline and items.
  this->CreateHandleRenderPipeline(entryRenderPipeline);
  return EDySuccess();
}

inline void MRendering::Impl::CreateHandleRenderPipeline(
  const PDyRenderPipelineInstanceMetaInfo& iEntryRenderPipeline)
{
  this->mEntryRenderPipelines.emplace_back(iEntryRenderPipeline);
  DyPushLogInfo("Create rendering pipeline, {} with uuid {}.", 
    iEntryRenderPipeline.mSpecifierName,
    iEntryRenderPipeline.mUuid.ToString());
}

inline void MRendering::Impl::Release()
{
}

inline MRendering::Impl::~Impl()
{
  switch (MSetting::GetInstance().GetRenderingType())
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

  // Clear all render pipelines.
  this->RemoveRenderPipeline("dyBtDefault");
  this->RemoveRenderPipeline("dyBtDefaultLoading");

  MDY_ASSERT(
    std::all_of(MDY_BIND_CBEGIN_CEND(this->mRenderItems), 
    [](const auto& renderItem) { return renderItem.second->IsBeingBinded() == false; }));
  MDY_ASSERT(
    std::all_of(MDY_BIND_CBEGIN_CEND(this->mRenderPipelines), 
    [](const auto& renderPipeline) { return renderPipeline.second->IsBeingBinded() == false; }));
  this->mRenderItems.clear();
  this->mRenderPipelines.clear();

  // Initialize internal management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(FDyModelHandlerManager::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Release());
  MDY_CALL_ASSERT_SUCCESS(MUniformBufferObject::Release());
}

inline EDySuccess MRendering::Impl::RemoveRenderPipeline(const std::string& iPipelineSpecifier)
{
  // Check there is a render pipeline.
  if (ContainsIf(this->mEntryRenderPipelines, 
    [iPipelineSpecifier](const auto& pipeline)
    {
      return pipeline.GetName() == iPipelineSpecifier;
    }) == false)
  {
    DyPushLogError(
      "Failed to remove render pipeline, {}. Matched specifier is not exist.",
      iPipelineSpecifier);
    return DY_FAILURE;
  }

  // Remove.
  EraseRemoveIf(this->mEntryRenderPipelines, 
    [iPipelineSpecifier](const auto& pipeline) { return pipeline.GetName() == iPipelineSpecifier; }
  );
  return DY_SUCCESS;
}

inline void MRendering::Impl::PreRender(TF32 dt)
{
  // Get skybox pointer from present level.
  auto& refWorld = MWorld::GetInstance();
  auto optSkybox = refWorld.GetPtrMainLevelSkybox();
  if (optSkybox.has_value() == true)
  { // Bind
    this->mPtrRequiredSkybox = *optSkybox;
  }

  // Do cpu frustum culling.
#ifdef false
  const auto* ptrCamera = MWorld::GetInstance().GetPtrMainLevelCamera();
  EraseRemoveIf(this->mOpaqueMeshDrawingList, [ptrCamera](TMeshDrawCallItem& iPtrOpaqueRenderer)
  {
    auto& [actorInfo, _, __] = iPtrOpaqueRenderer;
    const auto& worldPos = actorInfo->mPtrCompModelFilter
        ->GetBindedActor()->GetTransform()
        ->GetFinalWorldPosition();
    // Check
    return ptrCamera->IsSphereInFrustum(worldPos, 10) == false;
  });
#endif

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

inline void MRendering::Impl::SetupDrawModelTaskQueue()
{
  auto& handlerManager = FDyModelHandlerManager::GetInstance();

  const auto& modelHandleContainer = handlerManager.GetContainer();
  for (const auto& [specifier, smtInstance] : modelHandleContainer)
  {
    if (smtInstance == nullptr) { continue; }
    if (smtInstance->IsModelResourceValid() == false) { continue; }

    // Loop handler's actor information instance.
    auto& actorContainer = smtInstance->GetActorContainer();
    std::vector<NotNull<DDyModelHandler::DActorInfo*>> renderableActorList;

    // Insert satisfied render actor information instance.
    for (auto& [_, actorInfo] : actorContainer)
    {
      if (actorInfo.mPtrCompModelFilter == nullptr) { continue; }
      if (actorInfo.mPtrModelRenderer == nullptr) { continue; }
      renderableActorList.emplace_back(DyMakeNotNull(&actorInfo));
    }
  
    // Get 
    const auto& refBinder = smtInstance->GetModelBinderReference();
    const FResourceModel* ptrModelResc = refBinder.Get();

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

inline void MRendering::Impl::EnqueueDrawMesh(
  DDyModelHandler::DActorInfo& iRefModelRenderer,
  const FResourceMesh& iRefValidMesh, 
  const FResourceMaterial& iRefValidMat)
{
  switch (iRefValidMat.GetBlendMode())
  {
  case EMaterialBlendMode::Opaque: 
  {
    this->mOpaqueMeshDrawingList.emplace_back(&iRefModelRenderer, &iRefValidMesh, &iRefValidMat);
  } break;
  case EMaterialBlendMode::TranslucentOIT: 
  {
    this->mTranslucentMeshDrawingList.emplace_back(&iRefModelRenderer, &iRefValidMesh, &iRefValidMat);
  } break;
  default: MDY_UNEXPECTED_BRANCH(); break;
  }
}

inline void MRendering::Impl::EnqueueDebugDrawCollider(
  CBasePhysicsCollider& iRefCollider, 
  const DMat4& iTransformMatrix)
{
  this->mDebugColliderDrawingList.emplace_back(std::make_pair(&iRefCollider, iTransformMatrix));
}

inline void MRendering::Impl::RenderPipelines()
{
  //this->pClearRenderingFramebufferInstances();
  for (auto& renderPipeline : this->mEntryRenderPipelines)
  {
    // Need to check activation flag manually.
    if (renderPipeline.IsActivated() == false) { continue; }

    MDY_GRAPHIC_SET_CRITICALSECITON();
    renderPipeline.TryRender();
  }

  this->mOpaqueMeshDrawingList.clear();
  this->mDebugColliderDrawingList.clear();
  this->mUiObjectDrawingList.clear();
}

inline CLightDirectional* MRendering::Impl::GetPtrMainDirectionalLight() const noexcept
{
  return this->mMainDirectionalLight;
}

inline void MRendering::Impl::MDY_PRIVATE(BindMainDirectionalLight)(CLightDirectional& iRefLight)
{
  this->mMainDirectionalLight = &iRefLight;
}

inline EDySuccess MRendering::Impl::MDY_PRIVATE(UnbindMainDirectionalLight)(CLightDirectional& iRefLight)
{
  if (this->mMainDirectionalLight == &iRefLight) 
  {
    this->mMainDirectionalLight = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

inline CLightDirectional* MRendering::Impl::GetPtrMainDirectionalShadow() const noexcept
{
  return this->mMainDirectionalShadow;
}

inline void MRendering::Impl::MDY_PRIVATE(BindMainDirectionalShadow)(CLightDirectional& iRefLight)
{
  this->mMainDirectionalShadow = &iRefLight;
}

inline EDySuccess MRendering::Impl::MDY_PRIVATE(UnbindMainDirectionalShadow)(CLightDirectional& iRefLight)
{
  if (this->mMainDirectionalShadow == &iRefLight) 
  {
    this->mMainDirectionalShadow = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

inline const DMat4& MRendering::Impl::GetGeneralUiProjectionMatrix() const noexcept
{
  return this->mUiGeneralProjectionMatrix;
}

inline void MRendering::Impl::SwapBuffers()
{
  auto& managerWindow = MWindow::GetInstance();
  
  if (managerWindow.IsWindowShouldClose() == true) { return; }
  glfwSwapBuffers(managerWindow.GetGLMainWindow());
  glfwPollEvents();
}

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_RENDERINGMANAGER_IMPL_INL