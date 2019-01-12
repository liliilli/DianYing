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
#include <Dy/Core/Rendering/Pipeline/DeferredRenderingMesh.h>

#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>

#include <Dy/Builtin/ShaderGl/RenderDeferredRendering.h>
#include <Dy/Core/Rendering/Helper/FrameAttachmentString.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>

#include <Dy/Builtin/FrameBuffer/SceneIntegration/FDyBtFbSceneIntegration.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>

#include <glm/gtc/matrix_transform.inl>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

//!
//! Forward declaration
//!

namespace
{

dy::DDyMatrix4x4 sSamplePvMatrix =
    glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.02f, 100.0f) *
    glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

} /// ::unnamed namespace

namespace dy
{

FDyDeferredRenderingMesh::FDyDeferredRenderingMesh()
{
  this->pInitializeShaderSetting();
  this->pInitializeUboBuffers();

  MDY_ASSERT(this->mBinderFrameBuffer.IsResourceExist() == true, "Framebuffer must be bound.");
}

void FDyDeferredRenderingMesh::pInitializeShaderSetting()
{
  this->mBinderShader->UseShader();
  //ptr->SetupUniformVariableList();
  const auto id = this->mBinderShader->GetShaderProgramId();
  glUniform1i(glGetUniformLocation(id, "uUnlit"), 0);
  glUniform1i(glGetUniformLocation(id, "uNormal"), 1);
  glUniform1i(glGetUniformLocation(id, "uSpecular"), 2);
  glUniform1i(glGetUniformLocation(id, "uModelPosition"), 3);
  glUniform1i(glGetUniformLocation(id, "uShadow"), 4);
  this->mBinderShader->DisuseShader();
}

void FDyDeferredRenderingMesh::pInitializeUboBuffers()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  PDyUboConstructionDescriptor desc = {};
  desc.mBindingIndex      = 1;
  desc.mUboSpecifierName  = "dyBtUboDirLight";
  desc.mBufferDrawType    = EDyBufferDrawType::DynamicDraw;
  desc.mUboElementSize    = sizeof(DDyUboDirectionalLight);
  desc.mUboArraySize      = 1;
  MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc));
}

FDyDeferredRenderingMesh::~FDyDeferredRenderingMesh()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboDirLight"));
}

void FDyDeferredRenderingMesh::RenderScreen()
{
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderTriangle.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false) { return; }

  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "Unexpected error occurred.");

  this->mBinderFrameBuffer->BindFrameBuffer();
  this->mBinderShader->UseShader();
  submeshList[0]->Get()->BindVertexArray();

  // Bind g-buffers as textures.
  this->mBinderShader.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uShadowPv", sSamplePvMatrix);
  this->mBinderShader.TryUpdateUniformList();

  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttUnlit->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttNormal->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderAttSpecular->GetAttachmentId());
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, this->mBinderAttPosition->GetAttachmentId());
  glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, this->mBinderAttShadow->GetAttachmentId());
  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

  FDyGLWrapper::UnbindVertexArrayObject();
  this->mBinderShader->DisuseShader();
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

EDySuccess FDyDeferredRenderingMesh::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  auto* ptrLight  = MDyRendering::GetInstance().GetPtrMainDirectionalLight();
  if (this->mAddrMainLight != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainLight = reinterpret_cast<ptrdiff_t>(ptrLight);
    auto& uboManager = MDyUniformBufferObject::GetInstance();
    if (this->mAddrMainLight == 0)
    {
      DDyUboDirectionalLight light;
      MDY_CALL_ASSERT_SUCCESS(uboManager.UpdateUboContainer("dyBtUboDirLight", 0, sizeof(DDyUboDirectionalLight), &light));
    }
    else
    {
      DDyUboDirectionalLight light = ptrLight->GetUboLightInfo();
      MDY_CALL_ASSERT_SUCCESS(uboManager.UpdateUboContainer("dyBtUboDirLight", 0, sizeof(DDyUboDirectionalLight), &light));
    }
  }
  return DY_SUCCESS;
}

bool FDyDeferredRenderingMesh::IsReady() const noexcept
{
  return 
    !(this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderTriangle.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false);
}

void FDyDeferredRenderingMesh::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  auto& worldManager = MDyWorld::GetInstance();

  const auto& backgroundColor = worldManager.GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace