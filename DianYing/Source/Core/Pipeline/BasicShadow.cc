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
#include <Dy/Core/Rendering/Pipeline/BasicShadow.h>
#include <Dy/Core/Resource/MeshRenderer.h>

#include <Dy/Core/Resource/Resource_Deprecated/SubmeshResource_Deprecated.h>
#include <Dy/Management/IO/IOResourceManager_Deprecated.h>

#include <Dy/Builtin/ShaderGl/RenderBasicShadow.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Type/FramebufferInformation.h>
#include <Dy/Element/Actor.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <glm/gtc/matrix_transform.inl>

//!
//! Forward declaration
//!

namespace
{

dy::DDyMatrix4x4 sSamplePvMatrix =
    glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.02f, 100.0f) *
    glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

} /// ::unnamed namespace

//!
//! Implementation
//!

namespace dy
{

FDyBasicShadow::FDyBasicShadow()
{
  auto& heapManager = MDyIOResource_Deprecated::GetInstance();

  this->mDirLightShaderResource = heapManager.GetShaderResource(MSVSTR(builtin::FDyBuiltinShaderGLRenderBasicShadow::sName));
  this->pCreateFramebufferComponents();

  // @TODO TEMPORAL
  this->mIsUsingShadowDirectionalLight = true;
}

FDyBasicShadow::~FDyBasicShadow()
{
  this->pReleaseFrameBufferComponents();
}

void FDyBasicShadow::RenderScreen(const CDyModelRenderer& renderer)
{ // Integrity test
  if (this->mIsUsingShadowDirectionalLight == false) { return; }

  // FunctionBody∨
  const auto materialListCount  = renderer.GetMaterialListCount();
  const auto opSubmeshListCount = renderer.GetModelSubmeshCount();

  //
  GLint previousViewport[4] = {};
  glGetIntegerv( GL_VIEWPORT, previousViewport );
  glViewport(0, 0, 512, 512);

  //
  glBindFramebuffer(GL_FRAMEBUFFER, this->mShadowFrameBuffer->GetFramebufferId());
  const auto shaderProgramId = mDirLightShaderResource->GetShaderProgramId();
  this->mDirLightShaderResource->UseShader();

  TI32 iterationCount = MDY_INITIALIZE_DEFINT;
  if (materialListCount < opSubmeshListCount.value()) { iterationCount = materialListCount; }
  else                                                { iterationCount = opSubmeshListCount.value(); }

  for (TI32 i = 0; i < iterationCount; ++i)
  {
    const auto& submesh = renderer.GetSubmeshResourcePtr(i);
    glBindVertexArray(submesh.GetVertexArrayId());

    // Bind shadow camera
    const auto uPvLightMatrix = glGetUniformLocation(shaderProgramId, "uPvLightMatrix");
    const auto uModelMatrix   = glGetUniformLocation(shaderProgramId, "uModelMatrix");
    const auto& model         = const_cast<FDyActor*>(renderer.GetBindedActor())->GetTransform()->GetTransform();
    glUniformMatrix4fv(uModelMatrix,    1, GL_FALSE, &model[0].X);
    glUniformMatrix4fv(uPvLightMatrix,  1, GL_FALSE, &sSamplePvMatrix[0].X);

#ifdef false
    // If skeleton animation is enabled, get bone transform and bind to shader.
    const auto boneTransform = glGetUniformLocation("boneTransform");
    if (renderer.mModelReferencePtr && renderer.mModelReferencePtr->IsEnabledModelAnimated())
    {
      const auto& matrixList = renderer.mModelReferencePtr->GetModelAnimationTransformMatrixList();
      const auto  matrixSize = static_cast<int32_t>(matrixList.size());
      for (int32_t i = 0; i < matrixSize; ++i)
      {
        glUniformMatrix4fv(boneTransform + i, 1, GL_FALSE, &matrixList[i].mFinalTransformation[0].X);
      }
    }
#endif

    // Call function call drawing array or element. (not support instancing yet)
    if (submesh.IsEnabledIndices()) { glDrawElements(GL_TRIANGLES, submesh.GetIndicesCounts(), GL_UNSIGNED_INT, nullptr); }
    else                            { glDrawArrays(GL_TRIANGLES, 0, submesh.GetVertexCounts()); }
    glBindVertexArray(0);
  }

  //
  this->mDirLightShaderResource->UnuseShader();
  glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyBasicShadow::Clear()
{
  if (this->mIsUsingShadowDirectionalLight == false)  { return; }
  if (MDY_CHECK_ISNULL(this->mShadowFrameBuffer))     { return; }

  glBindFramebuffer (GL_FRAMEBUFFER, this->mShadowFrameBuffer->GetFramebufferId());
  glClear           (GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

bool FDyBasicShadow::IsAvailableDirectionalLightShadow() const noexcept
{
  return this->mIsUsingShadowDirectionalLight;
}

EDySuccess FDyBasicShadow::UpdateDirectionalLightShadowToGpu(const CDyDirectionalLight& container)
{
  //const auto& pv = container.pfGetPvMatrix();
  return DY_SUCCESS;
}

EDySuccess FDyBasicShadow::UnbindDirectionalLightShadowToGpu(const CDyDirectionalLight& container)
{
  return DY_SUCCESS;
}

TU32 FDyBasicShadow::GetDirectionalLightDepthTextureId() const noexcept
{
  MDY_NOT_IMPLEMENTED_ASSERT();
  return MDY_INITIALIZE_DEFUINT;
}

void FDyBasicShadow::pCreateFramebufferComponents()
{
  // @TODO TEMPORAL!
  PDyGlFrameBufferInformation       framebufferInfo = {};
  PDyGlAttachmentInformation        attachmentInfo  = {};
  PDyGlAttachmentBinderInformation  binderInfo      = {};

  framebufferInfo.mFrameBufferName            = sFrameBuffer_Shadow;
  framebufferInfo.mIsUsingDefaultDepthBuffer  = false;
  framebufferInfo.mIsNotUsingPixelShader      = true;

  // Depth texture buffer
  attachmentInfo.mAttachmentName = sAttachment_DirectionalBasicShadow;
  attachmentInfo.mAttachmentSize = DDyVectorInt2{512, 512};
  attachmentInfo.mParameterList  = {
      PDyGlTexParameterInformation\
      {EDyGlParameterName::TextureMinFilter, EDyGlParameterValue::Nearest},
      {EDyGlParameterName::TextureMagFilter, EDyGlParameterValue::Nearest},
      {EDyGlParameterName::TextureWrappingS, EDyGlParameterValue::ClampToBorder},
      {EDyGlParameterName::TextureWrappingT, EDyGlParameterValue::ClampToBorder},
  };

  binderInfo.mAttachmentName = attachmentInfo.mAttachmentName;
  binderInfo.mAttachmentType = EDyGlAttachmentType::Depth;
  framebufferInfo.mAttachmentList.push_back(binderInfo);

  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.SetAttachmentInformation(attachmentInfo));

  // Create framebuffer.
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.InitializeNewFrameBuffer(framebufferInfo));
  MDY_LOG_INFO("{}::{} | Geometry buffer created.", "MDyRendering", "pCreateDeferredGeometryBuffers");
  this->mShadowFrameBuffer = framebufferManager.GetFrameBufferPointer(MSVSTR(sFrameBuffer_Shadow));
  MDY_ASSERT(this->mShadowFrameBuffer != nullptr, "Unexpected error.");
}

void FDyBasicShadow::pReleaseFrameBufferComponents()
{
  auto& framebufferManager  = MDyFramebuffer::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(framebufferManager.RemoveFrameBuffer(MSVSTR(sFrameBuffer_Shadow)));
  this->mShadowFrameBuffer = nullptr;
}

} /// ::dy namespace