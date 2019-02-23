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

/// @brief 
void CbGlFeatBlendStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_BLEND); } 
  else                    { glDisable(GL_BLEND); }
}

/// @brief
void CbGlFeatCullfaceStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_CULL_FACE); } 
  else                    { glDisable(GL_CULL_FACE); }
}

/// @brief
void CbGlFeatDepthTestStack(const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_DEPTH_TEST); } 
  else                    { glDisable(GL_DEPTH_TEST); }
}

/// @brief
void CbGlFeatScissorTestStack(_MIN_ const bool& iTopStatus)
{
  if (iTopStatus == true) { glEnable(GL_SCISSOR_TEST); } 
  else                    { glDisable(GL_SCISSOR_TEST); }
}

/// @brief
void CbGlPolygonModeStack(_MIN_ const dy::DDyGlGlobalStatus::DPolygonMode& iTopStatus)
{
  // Get value from structure.
  const auto& polygonMode = iTopStatus;
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

void CbGlBlendModeStatus(_MIN_ const dy::DDyGlGlobalStatus::DBlendMode& iTopStatus)
{
  const auto& blendMode = iTopStatus;

  for (TU32 i = 0, size = static_cast<TU32>(blendMode.mBlendingSettingList.size());
       i < size; ++i)
  {
    const auto& [equation, src, dst] = blendMode.mBlendingSettingList[i];
    switch (equation)
    {
    case dy::DDyGlGlobalStatus::DBlendMode::EEqut::SrcAddDst: glBlendEquationi(i, GL_FUNC_ADD); break;
    case dy::DDyGlGlobalStatus::DBlendMode::EEqut::SrcSubDst: glBlendEquationi(i, GL_FUNC_SUBTRACT); break;
    case dy::DDyGlGlobalStatus::DBlendMode::EEqut::DstSubSrc: glBlendEquationi(i, GL_FUNC_REVERSE_SUBTRACT); break;
    case dy::DDyGlGlobalStatus::DBlendMode::EEqut::CompareMin: glBlendEquationi(i, GL_MIN); break;
    case dy::DDyGlGlobalStatus::DBlendMode::EEqut::CompareMax: glBlendEquationi(i, GL_MAX); break;
    }

    GLenum srcEnum = GL_NONE, dstEnum = GL_NONE;
    switch (src)
    {
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::Zero: srcEnum = GL_ZERO; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::One:  srcEnum = GL_ONE; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::SrcColor: srcEnum = GL_SRC_COLOR; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::OneMinusSrcColor: srcEnum = GL_ONE_MINUS_SRC_COLOR; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::SrcAlpha: srcEnum = GL_SRC_ALPHA; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::OneMinusSrcAlpha: srcEnum = GL_ONE_MINUS_SRC_ALPHA; break;
    }
    switch (dst)
    {
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::Zero: dstEnum = GL_ZERO; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::One:  dstEnum = GL_ONE; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::SrcColor: dstEnum = GL_SRC_COLOR; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::OneMinusSrcColor: dstEnum = GL_ONE_MINUS_SRC_COLOR; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::SrcAlpha: dstEnum = GL_SRC_ALPHA; break;
    case dy::DDyGlGlobalStatus::DBlendMode::EFunc::OneMinusSrcAlpha: dstEnum = GL_ONE_MINUS_SRC_ALPHA; break;
    }

    glBlendFunci(i, srcEnum, dstEnum);
  }
}

void CbGlCullfaceModeStack(_MIN_ const dy::DDyGlGlobalStatus::DCullfaceMode& iTopStatus)
{
  using EValue = dy::DDyGlGlobalStatus::DCullfaceMode::EValue;
  switch (iTopStatus.mValue)
  {
  case EValue::Front:         glCullFace(GL_FRONT);           break;
  case EValue::Back:          glCullFace(GL_BACK);            break;
  case EValue::FrontAndBack:  glCullFace(GL_FRONT_AND_BACK);  break;
  }
}

void CbGlViewportStack(_MIN_ const dy::DDyGlGlobalStatus::DViewport& iTopStatus)
{
  for (auto& [index, area] : iTopStatus.mViewportSettingList)
  {
    if (index <= -1)  { dy::FDyGLWrapper::SetViewport(area); }
    else              { dy::FDyGLWrapper::SetViewportIndexed(static_cast<TU32>(index), area); }
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
    glPrimitiveRestartIndex(0xFFFFFFFF);
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
    &&  this->mDebugRenderer->TryPushRenderingSetting() == DY_SUCCESS)
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

  // Reset final rendering mesh setting.
  if (MDY_CHECK_ISNOTEMPTY(this->mLevelFinalRenderer))    { this->mLevelFinalRenderer->Clear(); }
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

void MDyRendering::InsertInternalGlobalStatus(_MIN_ const DDyGlGlobalStatus& iNewStatus)
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

void MDyRendering::PopInternalGlobalStatus()
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

} /// ::dy namespace
