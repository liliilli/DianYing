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
#include <Dy/Core/Rendering/BasicShadow.h>
#include <Dy/Core/Component/MeshRenderer.h>

#include <Dy/Core/Component/Resource/ModelResource.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Management/HeapResourceManager.h>

#include <Dy/Builtin/ShaderGl/RenderBasicShadow.h>

namespace dy
{

FDyBasicShadow::FDyBasicShadow()
{
  dy::builtin::FDyBuiltinShaderGLRenderBasicShadow();
  this->mShaderResource = MDyHeapResource::GetInstance().GetShaderResource(builtin::FDyBuiltinShaderGLRenderBasicShadow::sName.data());

  this->pCreateFramebufferComponents();
}

FDyBasicShadow::~FDyBasicShadow()
{
  this->pReleaseFrameBufferComponents();
}

void FDyBasicShadow::RenderScreen(const CDyMeshRenderer& renderer)
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->GetShadowFrameBufferId());
  this->mShaderResource->UseShader();

  for (const auto& bindedMeshMatInfo : renderer.mMeshMaterialPtrBindingList)
  {
    const auto shaderResource = bindedMeshMatInfo.mMaterialResource->GetShaderResource();
    glBindVertexArray(bindedMeshMatInfo.mSubmeshResource->GetVertexArrayId());

    // Bind camera
#ifdef false
    if (auto* camera = MDyWorld::GetInstance().GetMainCameraPtr(); camera)
    {
      const auto viewMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "viewMatrix");
      const auto projMatirx = glGetUniformLocation(shaderResource->GetShaderProgramId(), "projectionMatrix");

      glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &camera->GetViewMatrix()[0].X);
      glUniformMatrix4fv(projMatirx, 1, GL_FALSE, &camera->GetProjectionMatrix()[0].X);
    }
#endif

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
    if (bindedMeshMatInfo.mSubmeshResource->IsEnabledIndices())
    {
      glDrawElements(GL_TRIANGLES, bindedMeshMatInfo.mSubmeshResource->GetIndicesCounts(), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
      glDrawArrays(GL_TRIANGLES, 0, bindedMeshMatInfo.mSubmeshResource->GetVertexCounts());
    }

    glBindVertexArray(0);
  }

  this->mShaderResource->UnuseShader();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TU32 FDyBasicShadow::GetDepthTextureId() const noexcept
{
  return this->mShadowDepthValueBuffer;
}

std::pair<TI32, TI32> FDyBasicShadow::GetShadowMapSize() const noexcept
{
  return std::make_pair(this->mMapWidth, this->mMapHeight);
}

void FDyBasicShadow::pCreateFramebufferComponents()
{
  glGenFramebuffers(1, &this->mShadowFramebufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, this->mShadowFramebufferId);

  // Unlit g-buffer
  glGenTextures   (1, &this->mShadowDepthValueBuffer);
  glBindTexture   (GL_TEXTURE_2D, this->mShadowDepthValueBuffer);
  glTexImage2D    (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->mShadowDepthValueBuffer, 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { PHITOS_UNEXPECTED_BRANCH(); }

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyBasicShadow::pReleaseFrameBufferComponents()
{
  glDeleteTextures(1, &this->mShadowDepthValueBuffer);
  glDeleteFramebuffers(1, &this->mShadowFramebufferId);
}

} /// ::dy namespace