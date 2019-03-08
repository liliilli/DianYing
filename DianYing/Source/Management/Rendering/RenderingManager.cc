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
#include <Dy/Builtin/Constant/GeneralValue.h>
#include <Dy/Management/InputManager.h>

#include <Dy/Core/Rendering/Pipeline/BasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/FinalScreenDisplayRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSsao.h>
#include <Dy/Core/Rendering/Pipeline/UIBasicRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCascadeShadowRenderer.h>
#include <Dy/Core/Rendering/Pipeline/LevelCSMIntegration.h>
#include <Dy/Core/Rendering/Pipeline/LevelOITRenderer.h>
#include <Dy/Core/Rendering/Pipeline/DebugShapeRenderer.h>
#include <Dy/Core/Rendering/Pipeline/PostEffectSky.h>
#include <Dy/Core/Rendering/Pipeline/DebugAABBRenderer.h>
#include <Dy/Core/Rendering/Pipeline/Debug/PickingRenderer.h>
#include <Dy/Helper/Pointer.h>
#include <Dy/Helper/Internal/FDyCallStack.h>
#include <Dy/Element/Actor.h>
#include "Dy/Management/Internal/World/FDyWorldUIContainer.h"

#include <queue>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderPipeline.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperRenderItem.h>
#include <Dy/Core/Rendering/Wrapper/FWrapperHandleRenderPipeline.h>

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

class MDyRendering::Impl final : IDyInitializeHelper<void>
{
public:
  Impl();
  EDySuccess Initialize() override final;
  void Release() override final;
  ~Impl();

  /// @brief PreRender update functin.
  void PreRender(TF32 dt);

  /// @brief Create render pipeline with specifier.
  /// If not found, just return DY_FAILURE.
  EDySuccess CreateRenderPipeline(const std::string& iPipelineSpecifier);
  /// @brief Create entry render pipeline handle into list.
  /// Created render pipeline will be rendered following order of child and local render item.
  void CreateHandleRenderPipeline(const PDyRenderPipelineInstanceMetaInfo& iEntryRenderPipeline);

  /// @brief Remove render pipeline with specifier.
  /// If not found in list, just return DY_FAILURE.
  EDySuccess RemoveRenderPipeline(const std::string& iPipelineSpecifier);

  /// @brief 
  /// @TODO LOGIC IS TEMPORARY.
  void SetupDrawModelTaskQueue();

  /// @brief Render level information.
  void RenderPipelines();
  
  /// @brief Get ptr main directional light. If not exist, just return nullptr.
  CDyDirectionalLight* GetPtrMainDirectionalLight() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight);
    
  /// @brief Get ptr main directional shadow. If not exist, just return nullptr.
  CDyDirectionalLight* GetPtrMainDirectionalShadow() const noexcept;
  /// @brief Private function, bind directional light as main light.
  void MDY_PRIVATE(BindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight);
  /// @brief Private function, unbind directional light of main light.
  EDySuccess MDY_PRIVATE(UnbindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight);

  /// @brief Get General (Default) ui projection matrix.
  const DDyMatrix4x4& GetGeneralUiProjectionMatrix() const noexcept;
  /// @brief Insert GL global status.
  void InsertInternalGlobalStatus(_MIN_ const DDyGlGlobalStatus& iNewStatus); 
  /// @brief Pop GL global status.
  void PopInternalGlobalStatus();

  /// @brief Swap buffer.
  void SwapBuffers();

  /// @brief Enqueue static draw call to mesh with material.
  void EnqueueDrawMesh(
      _MIN_ DDyModelHandler::DActorInfo& iRefModelRenderer,
      _MIN_ const FDyMeshResource& iRefValidMesh, 
      _MIN_ const FDyMaterialResource& iRefValidMat);

  /// @brief Enqueue debug collider draw call.
  void EnqueueDebugDrawCollider(
      _MIN_ CDyPhysicsCollider& iRefCollider, 
      _MIN_ const DDyMatrix4x4& iTransformMatrix);
  
  /// @brief Reset all of rendering framebuffers related to rendering of scene for new frame rendering.
  void pClearRenderingFramebufferInstances() noexcept;

  /// @brief Check Entry RenderPipeline is exist on rendering system.
  MDY_NODISCARD bool HasEntryRenderPipeline(const std::string& iEntryPipelineName);
  /// @brief Set activation of entry renderpipeline.
  /// If Activated, this pipeline will be rendered with arbitary order.
  /// If Deactivated, this pipeline will not be rendered but leave render resources valid.
  ///
  /// If not found, just do nothing.
  EDySuccess ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated);

  std::unique_ptr<FDyBasicRenderer>               mBasicOpaqueRenderer  = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyLevelCascadeShadowRenderer>  mCSMRenderer          = MDY_INITIALIZE_NULL; 
  std::unique_ptr<FDyLevelCSMIntergration>        mLevelFinalRenderer   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyLevelOITRenderer>            mTranslucentOIT       = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyPostEffectSsao>              mSSAOPostEffect       = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyPostEffectSky>               mSkyPostEffect        = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyUIBasicRenderer>             mUiBasicRenderer      = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyFinalScreenDisplayRenderer>  mFinalDisplayRenderer = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDebugShapeRenderer>          mDebugShapeRenderer   = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDebugAABBRenderer>           mDebugAABBRenderer    = MDY_INITIALIZE_NULL;
  std::unique_ptr<FDyDebugPickingRenderer>        mDebugPickingRenderer = MDY_INITIALIZE_NULL;

  std::unordered_map<std::string, std::unique_ptr<FWrapperRenderPipeline>> 
    mRenderPipelines = {};

  std::unordered_map<std::string, std::unique_ptr<FWrapperRenderItem>> 
    mRenderItems = {};

  std::vector<FWrapperHandleRenderPipeline> mEntryRenderPipelines;

  std::vector<TMeshDrawCallItem> mOpaqueMeshDrawingList = {};
  std::vector<TMeshDrawCallItem> mTranslucentMeshDrawingList = {};
  std::vector<TDrawColliderItem> mDebugColliderDrawingList = {};
  std::vector<TUiDrawCallItem>   mUiObjectDrawingList = {};

  CDyDirectionalLight* mMainDirectionalLight   = nullptr;
  CDyDirectionalLight* mMainDirectionalShadow  = nullptr;

  /// @brief Required skybox pointer for rendering on present frame.
  /// If rendered, skybox pointer will be nulled again.
  CDySkybox* mPtrRequiredSkybox = nullptr;

  /// @brief Activated directional light list.
  std::queue<TI32>  mDirLightAvailableList     = {};
  /// @brief Default UI projection matrix. (Orthogonal)
  DDyMatrix4x4      mUiGeneralProjectionMatrix = {};

  /// @brief Global status stack for management. \n
  /// This container will be push & popped automatically by following rendering pipeline.
  /// This container must not be empty before termination of Dy application.
  FDyCallStack<DDyGlGlobalStatus> mInternalGlobalStatusStack;
  /// ▽ Actual state machine change logic will be operated in these stack.
  FDyCallStack<bool> mInternal_FeatBlendStack;
  FDyCallStack<bool> mInternal_FeatCullfaceStack;
  FDyCallStack<bool> mInternal_FeatDepthTestStack;
  FDyCallStack<bool> mInternal_FeatScissorTestStack;
  FDyCallStack<DDyGlGlobalStatus::DPolygonMode>   mInternal_PolygonModeStack;
  FDyCallStack<DDyGlGlobalStatus::DBlendMode>     mInternal_BlendModeStack;
  FDyCallStack<DDyGlGlobalStatus::DCullfaceMode>  mInternal_CullfaceModeStack;
  FDyCallStack<DDyGlGlobalStatus::DViewport>      mInternal_ViewportStack;

#if defined(MDY_FLAG_IN_EDITOR)
  std::unique_ptr<FDyGrid>                    mGridEffect           = nullptr;
#endif /// MDY_FLAG_IN_EDITOR
};

bool MDyRendering::Impl::HasEntryRenderPipeline(const std::string& iEntryPipelineName)
{
  if (ContainsIf(this->mEntryRenderPipelines,
    [iEntryPipelineName](const auto& pipeline) 
    { return pipeline.GetName() == iEntryPipelineName; }) == false)
  {
    return false;
  }

  return true;
}

EDySuccess MDyRendering::Impl::ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated)
{
  if (this->HasEntryRenderPipeline(iEntryPipelineName) == false)
  {
    DyPushLogDebugError("Failed to find entry render pipeline, {}.", iEntryPipelineName);
    return DY_FAILURE;
  }

  auto it = std::find_if(
    MDY_BIND_BEGIN_END(this->mEntryRenderPipelines),
    [iEntryPipelineName](const auto& pipeline) 
    { return pipeline.GetName() == iEntryPipelineName; });
  it->Activate(iIsActivated);
  return DY_SUCCESS;
}

} /// ::dy namespace
#include <Dy/Management/Inline/MRenderingImpl.inl>

//!
//! Proxy
//!

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{ 
  this->mInternal = new (std::nothrow) Impl();
  this->mInternal->Initialize();
  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  delete this->mInternal; this->mInternal = nullptr;
  return DY_SUCCESS;
}

void MDyRendering::PreRender(TF32 iDt) { this->mInternal->PreRender(iDt); }
void MDyRendering::SetupDrawModelTaskQueue() { this->mInternal->SetupDrawModelTaskQueue(); }

void MDyRendering::EnqueueDrawMesh(
    DDyModelHandler::DActorInfo& iRefModelRenderer,
    const FDyMeshResource& iRefValidMesh, 
    const FDyMaterialResource& iRefValidMat)
{
  this->mInternal->EnqueueDrawMesh(iRefModelRenderer, iRefValidMesh, iRefValidMat);
}

void MDyRendering::EnqueueDebugDrawCollider(
    CDyPhysicsCollider& iRefCollider, 
    const DDyMatrix4x4& iTransformMatrix)
{
  this->mInternal->EnqueueDebugDrawCollider(iRefCollider, iTransformMatrix);
}

void MDyRendering::RenderPipelines() 
{ 
  this->mInternal->RenderPipelines(); 
}

void MDyRendering::MDY_PRIVATE(BindMainDirectionalLight)(CDyDirectionalLight& iRefLight)
{
  return this->mInternal->MDY_PRIVATE(BindMainDirectionalLight)(iRefLight);
}
CDyDirectionalLight* MDyRendering::GetPtrMainDirectionalLight() const noexcept
{
  return this->mInternal->GetPtrMainDirectionalLight();
}
EDySuccess MDyRendering::MDY_PRIVATE(UnbindMainDirectionalLight)(CDyDirectionalLight& iRefLight)
{
  return this->mInternal->MDY_PRIVATE(UnbindMainDirectionalLight)(iRefLight);
}

void MDyRendering::MDY_PRIVATE(BindMainDirectionalShadow)(CDyDirectionalLight& iRefLight)
{
  this->mInternal->MDY_PRIVATE(BindMainDirectionalShadow)(iRefLight);
}
CDyDirectionalLight* MDyRendering::GetPtrMainDirectionalShadow() const noexcept
{
  return this->mInternal->GetPtrMainDirectionalShadow();
}
EDySuccess MDyRendering::MDY_PRIVATE(UnbindMainDirectionalShadow)(CDyDirectionalLight& iRefLight)
{
  return this->mInternal->MDY_PRIVATE(UnbindMainDirectionalShadow)(iRefLight);
}

const DDyMatrix4x4& MDyRendering::GetGeneralUiProjectionMatrix() const noexcept
{
  return this->mInternal->GetGeneralUiProjectionMatrix();
}

void MDyRendering::InsertInternalGlobalStatus(const DDyGlGlobalStatus& iNewStatus)
{
  this->mInternal->InsertInternalGlobalStatus(iNewStatus);
}
void MDyRendering::PopInternalGlobalStatus() { this->mInternal->PopInternalGlobalStatus(); }

void MDyRendering::SwapBuffers() { this->mInternal->SwapBuffers(); }

std::vector<MDyRendering::TMeshDrawCallItem>& MDyRendering::GetOpaqueMeshQueueList()
{
  return this->mInternal->mOpaqueMeshDrawingList;
}

std::vector<MDyRendering::TMeshDrawCallItem>& MDyRendering::GetTranclucentOitMeshQueueList()
{
  return this->mInternal->mTranslucentMeshDrawingList;
}

std::vector<MDyRendering::TDrawColliderItem>& MDyRendering::GetColliderMeshQueueList()
{
  return this->mInternal->mDebugColliderDrawingList;
}

std::vector<MDyRendering::TUiDrawCallItem>& MDyRendering::GetUiObjectQueuelist()
{
  return this->mInternal->mUiObjectDrawingList;
}

bool MDyRendering::HasRenderItem(const std::string& iRenderItemName)
{
  return DyIsMapContains(this->mInternal->mRenderItems, iRenderItemName);
}

FWrapperRenderItem* MDyRendering::GetRenderItem(const std::string& iRenderItemName)
{
  if (this->HasRenderItem(iRenderItemName) == false) 
  { 
    return nullptr; 
  }

  return this->mInternal->mRenderItems.at(iRenderItemName).get();
}

bool MDyRendering::HasRenderPipeline(const std::string& iRenderPipelineName)
{
  return DyIsMapContains(this->mInternal->mRenderPipelines, iRenderPipelineName);
}

FWrapperRenderPipeline* MDyRendering::GetRenderPipeline(const std::string& iRenderPipelineName)
{
  if (this->HasRenderPipeline(iRenderPipelineName) == false)
  {
    return nullptr;
  }

  return this->mInternal->mRenderPipelines.at(iRenderPipelineName).get();
}

bool MDyRendering::HasEntryRenderPipeline(const std::string& iEntryPipelineName)
{
  return this->mInternal->HasEntryRenderPipeline(iEntryPipelineName);
}

EDySuccess MDyRendering::ActivateEntryRenderPipeline(const std::string& iEntryPipelineName, bool iIsActivated)
{
  return this->mInternal->ActivateEntryRenderPipeline(iEntryPipelineName, iIsActivated);
}

} /// ::dy namespace
