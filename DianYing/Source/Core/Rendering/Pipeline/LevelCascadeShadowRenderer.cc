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

dy::DDyMatrix4x4 sTestView;
dy::DDyMatrix4x4 slightProjMatrix;

dy::DDyVector3 kLevelUpDir = dy::DDyVector3{0, 1, 0};

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

  if (const auto& proj = ptrCamera->GetProjectionMatrix(); 
      this->mOldProjectionMatrix != proj) 
  { 
    this->mOldProjectionMatrix = proj;
    this->UpdateSegmentFarPlanes(*ptrCamera);
  }

  auto* ptrLight  = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (this->mAddrMainDirectionalShadow != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainDirectionalShadow = reinterpret_cast<ptrdiff_t>(ptrLight);

    if (this->mAddrMainDirectionalShadow == 0) 
    { sTestView = DDyMatrix4x4{}; }
    else 
    { 
      const auto& pos = ptrLight->GetBindedActor()->GetTransform()->GetFinalWorldPosition();

      auto fwd = ptrLight->GetLightDirection();
      if (fwd == kLevelUpDir) { fwd += DDyVector3{0.01f, 0.01f, 0.01f}; }
      fwd *= -1.0f;

      const auto eye = pos + fwd;
      sTestView = glm::lookAt(
        static_cast<glm::vec3>(pos), 
        static_cast<glm::vec3>(eye), 
        static_cast<glm::vec3>(kLevelUpDir)); 
    }
  }

  this->UpdateLightProjectionAndViewports(*ptrCamera);
}

void FDyLevelCascadeShadowRenderer::UpdateSegmentFarPlanes(_MIN_ const CDyCamera& iPtrCamera)
{
  const auto nearPlane  = iPtrCamera.GetNear();
  const auto farPlane   = iPtrCamera.GetFar();
  const auto diffPlane  = farPlane - nearPlane;
  
  static constexpr TF32 frustumSplitCorrection = 0.8f;

  for (TU32 i = 1; i <= kCSMSegment; ++i)
  {
    const TF32 distFactor = static_cast<TF32>(i) / kCSMSegment;
    const TF32 stdTerm    = nearPlane * pow(farPlane / nearPlane, distFactor);
    const TF32 corrTerm   = nearPlane + distFactor * diffPlane;
    const TF32 viewDepth  = frustumSplitCorrection * stdTerm + (1.0f - frustumSplitCorrection) * corrTerm;

    this->mFarPlanes[i - 1] = viewDepth;
    const auto projectedDepth = this->mOldProjectionMatrix.MultiplyVector({0, 0, -viewDepth, 1});
    this->mNormalizedFarPlanes[i - 1] = (projectedDepth.Z / projectedDepth.W) * 0.5f + 0.5f;
  }
}

void FDyLevelCascadeShadowRenderer::UpdateLightProjectionAndViewports(_MIN_ const CDyCamera& iRefCamera)
{
  // Find a bounding box of whole camera frustum in light view space.
  DDyVector4 minFrustum {NumericalMax<TF32>};
  DDyVector4 maxFrustum {NumericalMin<TF32>};
  this->FrustumBoundingBoxLightViewSpace(iRefCamera.GetNear(), iRefCamera.GetFar(), iRefCamera, minFrustum, maxFrustum);
  
  // Update light projection matrix to only cover the area viewable by the camera.
  slightProjMatrix = glm::ortho(minFrustum.X, maxFrustum.X, minFrustum.Y, maxFrustum.Y, -maxFrustum.Z, -minFrustum.Z);
  
  // Find a bounding box of segment in light view space.
  TF32 nearSegmentPlane = 0.0f;
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    DDyVector4 minSegment {NumericalMax<TF32>};
    DDyVector4 maxSegment {NumericalMin<TF32>};
    this->FrustumBoundingBoxLightViewSpace(nearSegmentPlane, mFarPlanes[i], iRefCamera, minSegment, maxSegment);

    // Update viewports.
    const DDyVector2 frustumSize {maxFrustum.X - minFrustum.X, maxFrustum.Y - minFrustum.Y};
    const TF32 segmentSizeX = maxSegment.X - minSegment.X;
    const TF32 segmentSizeY = maxSegment.Y - minSegment.Y;
    const TF32 segmentSize  = std::max(segmentSizeX, segmentSizeY);

    const DDyVector2 offsetBottomLeft {minSegment.X - minFrustum.X, minSegment.Y - minFrustum.Y};
    const DDyVector2 offsetSegmentSizeRatio   {offsetBottomLeft.X / segmentSize, offsetBottomLeft.Y / segmentSize};
    const DDyVector2 frustumSegmentSizeRatio  {frustumSize.X / segmentSize, frustumSize.Y / segmentSize};

    DDyVector2 pixelOffsetTopLeft = offsetSegmentSizeRatio * kCSMAttachmentTextureSize;
    DDyVector2 pixelFrustumSize   = frustumSegmentSizeRatio * kCSMAttachmentTextureSize;

    // Scale factor that helps if frustum size is supposed to be bigger than maximum viewport size.
    const DDyVector2 scaleFactor{
        sViewportDims[0] < pixelFrustumSize.X ? sViewportDims[0] / pixelFrustumSize.X : 1.0f,
        sViewportDims[1] < pixelFrustumSize.Y ? sViewportDims[1] / pixelFrustumSize.Y : 1.0f
    };
    pixelOffsetTopLeft  *= scaleFactor;
    pixelFrustumSize    *= scaleFactor;

    mLightViewports[i].mLeftDown  = pixelOffsetTopLeft * -1;
    mLightViewports[i].mRightUp   = pixelFrustumSize;

    // Update light view-projection matrices per segments.
    DDyMatrix4x4 lightProjMatrix = glm::ortho(
        minSegment.X, 
        minSegment.X + segmentSize, 
        minSegment.Y, 
        minSegment.Y + segmentSize,
        -maxFrustum.Z,
        -minFrustum.Z);
    DDyMatrix4x4 lightScale = DDyMatrix4x4{
        0.5f * scaleFactor.X, 0, 0, 0,
        0, 0.5f * scaleFactor.Y, 0, 0,
        0, 0, 0.5f, 0, 
        0, 0, 0, 1};
    DDyMatrix4x4 lightBias = DDyMatrix4x4{
        0, 0, 0, 0.5f * scaleFactor.X,
        0, 0, 0, 0.5f * scaleFactor.Y,
        0, 0, 0, 0.5f,
        0, 0, 0, 1};

    this->mLightSegmentVPSBMatrices[i] = sTestView.Multiply(lightProjMatrix).Multiply(lightScale).Multiply(lightBias);
    nearSegmentPlane = this->mNormalizedFarPlanes[i];
  }
}

void FDyLevelCascadeShadowRenderer::FrustumBoundingBoxLightViewSpace(
    _MIN_ TF32 iNear, 
    _MIN_ TF32 iFar,
    _MIN_ const CDyCamera& iRefCamera, 
    _MOUT_ DDyVector4& iMin, 
    _MOUT_ DDyVector4& iMax) const
{
  DDyVector4 minResult {NumericalMax<TF32>};
  DDyVector4 maxResult {NumericalMin<TF32>};

  const TF32 fov  = math::DegToRadVal<TF32> * iRefCamera.GetFieldOfView();
  const auto xywh = iRefCamera.GetPixelizedViewportRectangle();
  const auto pos  = iRefCamera.GetPosition();
  const TF32 nearHeight = 2.0f * tan(fov) * iNear;
  const TF32 nearWidth  = nearHeight * xywh[2] / xywh[3];
  const TF32 farHeight  = 2.0f * tan(fov) * iFar;
  const TF32 farWidth   = farHeight * xywh[2] / xywh[3];
  
  const auto& camViewMatrix = iRefCamera.GetViewMatrix();
  const auto rightDir = DDyVector3{camViewMatrix[0]};
  const auto upDir    = DDyVector3{camViewMatrix[1]};
  const auto forDir   = DDyVector3{camViewMatrix[2]};

  const auto nc = pos + forDir * iNear;
  const auto fc = pos + forDir * iFar;

  // Vertices in a world space.
  std::array<DDyVector4, 8> boundingBoxVertices =
  { //         z    y                             x
    DDyVector4{nc - (upDir * nearHeight * 0.5f) - (rightDir * nearWidth * 0.5f), 1.0f}, // NBL
    DDyVector4{nc - (upDir * nearHeight * 0.5f) + (rightDir * nearWidth * 0.5f), 1.0f}, // NBR
    DDyVector4{nc + (upDir * nearHeight * 0.5f) + (rightDir * nearWidth * 0.5f), 1.0f}, // NTR
    DDyVector4{nc + (upDir * nearHeight * 0.5f) - (rightDir * nearWidth * 0.5f), 1.0f}, // NTL

    DDyVector4{fc - (upDir * farHeight * 0.5f) - (rightDir * farWidth * 0.5f), 1.0f}, // FBL
    DDyVector4{fc - (upDir * farHeight * 0.5f) + (rightDir * farWidth * 0.5f), 1.0f}, // FBR
    DDyVector4{fc + (upDir * farHeight * 0.5f) + (rightDir * farWidth * 0.5f), 1.0f}, // FTR
    DDyVector4{fc + (upDir * farHeight * 0.5f) - (rightDir * farWidth * 0.5f), 1.0f}, // FTL
  };

  for (TU32 vertId = 0; vertId < 8; ++vertId)
  { // Light view space.
    boundingBoxVertices[vertId] = sTestView.MultiplyVector(boundingBoxVertices[vertId]);
    // Update bounding box. (at least small point and at most biggest point)
    for (TU32 i = 0; i < 4; ++i)
    {
      minResult[i] = std::min(minResult[i], boundingBoxVertices[vertId][i]);
      maxResult[i] = std::max(maxResult[i], boundingBoxVertices[vertId][i]);
    }
  }

  iMin = minResult;
  iMax = maxResult;
}

void FDyLevelCascadeShadowRenderer::SetupViewport()
{
  for (TU32 i = 0; i < kCSMSegment; ++i)
  {
    FDyGLWrapper::SetViewportIndexed(i, this->mLightViewports[i]);
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
  this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Matrix4>("mProjMatrix", slightProjMatrix);
  this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Matrix4>("mViewMatrix", sTestView);
  this->mDirLightShaderResource.TryUpdateUniformList();
  return DY_SUCCESS;
}

void FDyLevelCascadeShadowRenderer::RenderScreen(_MIN_ CDyModelRenderer& renderer)
{
  // Validation test
  const auto* ptrModelBinder = renderer.GetModelResourceBinder();
  if (MDY_CHECK_ISNULL(ptrModelBinder)) { return; }

  const auto& meshList = (*ptrModelBinder)->GetMeshResourceList();
  const auto  opSubmeshListCount = renderer.GetModelSubmeshCount();
  if (opSubmeshListCount.has_value() == false) { return; }

  const auto  meshCount = opSubmeshListCount.value();
  this->mDirLightShaderResource->UseShader();

  for (TI32 i = 0; i < meshCount; ++i)
  {
    const auto& ptrMesh = *meshList[i];
    const auto& refModelMatrix = renderer.GetBindedActor()->GetTransform()->GetTransform();
    this->mDirLightShaderResource.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uModelMatrix", refModelMatrix);
    this->mDirLightShaderResource.TryUpdateUniformList();
    ptrMesh->BindVertexArray();

    // Call function call drawing array or element. (not support instancing yet) TODO IMPLEMENT BATCHING SYSTEM.
    if (ptrMesh->IsEnabledIndices() == true)
    { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, ptrMesh->GetIndicesCounts()); }
    else
    { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, ptrMesh->GetVertexCounts()); }
  }

  // Unbind present submesh vertex array object.
  FDyGLWrapper::UnbindVertexArrayObject();
  this->mDirLightShaderResource->DisuseShader();
}

} /// ::dy namespace