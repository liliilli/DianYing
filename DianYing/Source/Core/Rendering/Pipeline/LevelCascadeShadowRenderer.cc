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
#include <Dy/Core/Rendering/Pipeline/LevelCascadeShadowRenderer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Dy/Component/CDyCamera.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include "Dy/Component/CDyDirectionalLight.h"
#include "Dy/Core/Resource/Resource/FDyMaterialResource.h"

#ifdef near
#undef near
#endif

#ifdef far
#undef far 
#endif

//!
//! Implementation
//!

namespace dy
{

std::array<TF32, 2> FDyLevelCascadeShadowRenderer::sViewportDims;

FDyLevelCascadeShadowRenderer::FDyLevelCascadeShadowRenderer() = default;

void FDyLevelCascadeShadowRenderer::PreRender()
{
  static bool isFirst = true;
  if (isFirst == true)
  {
    FDyGLWrapper::QueryFloatVector(GL_MAX_VIEWPORT_DIMS, sViewportDims.data());
    isFirst = false;
  }

  auto* ptrLight  = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (this->mAddrMainDirectionalShadow != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainDirectionalShadow = reinterpret_cast<ptrdiff_t>(ptrLight);
    if (this->mAddrMainDirectionalShadow == 0) 
    {
      this->mViewMatrix = DDyMatrix4x4{};
    }
    else
    { 
      ptrLight->UpdateLightViewMatrix(); 
      this->mViewMatrix = ptrLight->GetLightViewMatrix(); 
    }
  }

  if (const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
      ptrLight != nullptr)
  {
    //
    ptrLight->UpdateSegmentFarPlanes(*ptrCamera);
    ptrLight->UpdateCSMFrustum(*ptrCamera);
    ptrLight->UpdateProjectionMatrix();
    ptrLight->UpdateLightProjectionAndViewports(
        *ptrCamera, 
        ptrLight->GetCSMFarPlanes(), 
        ptrLight->GetCSMNormalizedFarPlanes());
    //
    this->mProjMatrix = ptrLight->GetProjectionMatrix();
  }
}

void FDyLevelCascadeShadowRenderer::Clear()
{
  if (this->mDirLightShaderResource.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

bool FDyLevelCascadeShadowRenderer::IsReady() const noexcept
{
  return this->mBinderFrameBuffer.IsResourceExist() == true
      && this->mDirLightShaderResource.IsResourceExist() == true;
}

EDySuccess FDyLevelCascadeShadowRenderer::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  auto* ptrLight = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (ptrLight == nullptr) { return DY_FAILURE; }

  // Update view frustum for shadow mapping.
  // Do not move the order of `PreRender` and checking assert statement.
  this->PreRender();
  MDY_ASSERT(this->mAddrMainDirectionalShadow == reinterpret_cast<ptrdiff_t>(ptrLight), 
      "CSM Renderer light handle is not matched to given light.");

  // Try update uniform value.
  using EUniform = EDyUniformVariableType;
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Integer>("uFrustumSegmentCount", static_cast<TI32>(kCSMSegment));
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Matrix4>("mProjMatrix", this->mProjMatrix);
  this->mDirLightShaderResource->TryUpdateUniform<EUniform::Matrix4>("mViewMatrix", this->mViewMatrix);

  // Set and push global internal setting.
  DDyGlGlobalStatus status;
  using EValue = DDyGlGlobalStatus::DCullfaceMode::EValue;
  status.mIsEnableDepthTest = true;
  status.mIsEnableCullface  = true;
  status.mCullfaceMode = EValue::Back;

  using DViewport = DDyGlGlobalStatus::DViewport;
  DViewport viewport;

  /// @brief Setup indexed viewports of light shadow map segments for writing.
  const auto& lightViewports = ptrLight->GetCSMIndexedViewports();
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    viewport.mViewportSettingList.emplace_back(i, lightViewports[i]);
  }
  status.mViewportSettingList = viewport;

  auto& refRendering = MDyRendering::GetInstance();
  refRendering.InsertInternalGlobalStatus(status);

  this->mBinderFrameBuffer->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);
  return DY_SUCCESS;
}

EDySuccess FDyLevelCascadeShadowRenderer::TryPopRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFrameBuffer->UnbindFrameBuffer();
  auto& refRendering = MDyRendering::GetInstance();
  refRendering.PopInternalGlobalStatus();

  return DY_SUCCESS;
}

void FDyLevelCascadeShadowRenderer::RenderScreen(
    _MIN_ DDyModelHandler::DActorInfo& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial)
{
  // Validation test
  const auto* ptrModelBinder = iRefRenderer.mPtrModelRenderer->GetModelResourceBinder();
  if (ptrModelBinder == nullptr) { return; }

  const auto& refModelMatrix = iRefRenderer.mPtrModelRenderer->GetBindedActor()->GetTransform()->GetTransform();
  this->mDirLightShaderResource->TryUpdateUniform<EDyUniformVariableType::Matrix4>("uModelMatrix", refModelMatrix);

  this->mDirLightShaderResource->UseShader();
  this->mDirLightShaderResource->TryUpdateUniformList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
  if (iRefMesh.IsEnabledIndices() == true)
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  FDyGLWrapper::UnbindVertexArrayObject();
  this->mDirLightShaderResource->DisuseShader();
}

} /// ::dy namespace