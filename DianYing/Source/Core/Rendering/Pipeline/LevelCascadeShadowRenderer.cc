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
//! Forward declaration
//!

namespace
{

dy::DDyMatrix4x4 mViewMatrix;
dy::DDyMatrix4x4 mProjMatrix;

}

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

  const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();
  if (MDY_CHECK_ISNULL(ptrCamera)) { return; }

  auto* ptrLight  = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (this->mAddrMainDirectionalShadow != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainDirectionalShadow = reinterpret_cast<ptrdiff_t>(ptrLight);
    if (this->mAddrMainDirectionalShadow == 0) 
    {
      mViewMatrix = DDyMatrix4x4{};
    }
    else
    { 
      ptrLight->UpdateLightViewMatrix(); 
      mViewMatrix = ptrLight->GetLightViewMatrix(); 
    }
  }

  if (ptrLight != nullptr)
  {
    ptrLight->UpdateSegmentFarPlanes(*ptrCamera);
    ptrLight->UpdateCSMFrustum(*ptrCamera);
    ptrLight->UpdateProjectionMatrix();
    ptrLight->UpdateLightProjectionAndViewports(*ptrCamera, ptrLight->GetCSMFarPlanes(), ptrLight->GetCSMNormalizedFarPlanes());

    mProjMatrix = ptrLight->GetProjectionMatrix();
  }
}

void FDyLevelCascadeShadowRenderer::SetupViewport()
{
  auto* ptrLight  = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  MDY_ASSERT(this->mAddrMainDirectionalShadow == reinterpret_cast<ptrdiff_t>(ptrLight), 
      "CSM Renderer light handle is not matched to given light.");

  const auto& lightViewports = ptrLight->GetCSMIndexedViewports();
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    FDyGLWrapper::SetViewportIndexed(i, lightViewports[i]);
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

EDySuccess FDyLevelCascadeShadowRenderer::TrySetupRendering()
{
  if (this->mDirLightShaderResource.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false) { return DY_FAILURE; }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  
  this->mBinderFrameBuffer->BindFrameBuffer();
  const GLfloat one = 1.0f;
  glClearBufferfv(GL_DEPTH, 0, &one);

  this->mDirLightShaderResource->UseShader();
  this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Integer>("uFrustumSegmentCount", static_cast<TI32>(kCSMSegment));
  this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Matrix4>("mProjMatrix", mProjMatrix);
  this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Matrix4>("mViewMatrix", mViewMatrix);
  this->mDirLightShaderResource.TryUpdateUniformList();
  return DY_SUCCESS;
}

void FDyLevelCascadeShadowRenderer::RenderScreen(
    _MIN_ DDyModelHandler::DActorInfo& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial)
{
  // Validation test
  const auto* ptrModelBinder = iRefRenderer.mPtrModelRenderer->GetModelResourceBinder();
  if (MDY_CHECK_ISNULL(ptrModelBinder)) { return; }

  const auto& meshList = (*ptrModelBinder)->GetMeshResourceList();
  this->mDirLightShaderResource->UseShader();

  const auto& refModelMatrix = iRefRenderer.mPtrModelRenderer->GetBindedActor()->GetTransform()->GetTransform();
  this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uModelMatrix", refModelMatrix);
  this->mDirLightShaderResource.TryUpdateUniformList();
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