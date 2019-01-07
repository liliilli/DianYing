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
  for (TI32 i = 0; i < sDirectionalLightCount; ++i) { this->mDirLightAvailableList.push(i); }
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
  desc.mUboArraySize      = FDyDeferredRenderingMesh::sDirectionalLightCount;
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
  // Bind vertex array
  const FDyMeshResource& mesh = *submeshList[0]->Get();

  // Set
  glBindFramebuffer(GL_FRAMEBUFFER, this->mBinderFrameBuffer->GetFrameBufferId());
  this->mBinderShader->UseShader();
  glBindVertexArray(mesh.GetVertexArrayId());

  // Bind g-buffers as textures.
  const auto uShadowPv_Id = glGetUniformLocation(this->mBinderShader->GetShaderProgramId(), "uShadowPv");
  glUniformMatrix4fv(uShadowPv_Id, 1, GL_FALSE, &sSamplePvMatrix[0].X);

  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttUnlit->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttNormal->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderAttSpecular->GetAttachmentId());
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, this->mBinderAttPosition->GetAttachmentId());
  glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, this->mBinderAttShadow->GetAttachmentId());
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

  // Rewind
  glBindVertexArray(0);
  this->mBinderShader->DisuseShader();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FDyDeferredRenderingMesh::Clear()
{
  if (this->mBinderShader.IsResourceExist() == false
  ||  this->mBinderTriangle.IsResourceExist() == false
  ||  this->mBinderFrameBuffer.IsResourceExist() == false) { return; }

  glBindFramebuffer(GL_FRAMEBUFFER, this->mBinderFrameBuffer->GetFrameBufferId());
  auto& worldManager = MDyWorld::GetInstance();

  const auto& backgroundColor = worldManager.GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::optional<TI32> FDyDeferredRenderingMesh::GetAvailableDirectionalLightIndex() noexcept
{
  if (this->mDirLightAvailableList.empty() == true) { return std::nullopt; }
  else
  {
    const auto returnId = this->mDirLightAvailableList.front();
    this->mDirLightAvailableList.pop();

    return returnId;
  }
}

EDySuccess FDyDeferredRenderingMesh::UpdateDirectionalLightValueToGpu(
    _MIN_ const TI32 index,
    _MIN_ const DDyUboDirectionalLight& container)
{ // Integrity check
  constexpr TI32 UboElementSize = sizeof(DDyUboDirectionalLight);

  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.UpdateUboContainer("dyBtUboDirLight", UboElementSize * index, UboElementSize, &container));
  return DY_SUCCESS;
}

EDySuccess FDyDeferredRenderingMesh::UnbindDirectionalLight(_MIN_ const TI32 index)
{ // Integrity check
  constexpr TI32 UboElementSize = sizeof(DDyUboDirectionalLight);

  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.ClearUboContainer("dyBtUboDirLight", UboElementSize * index, UboElementSize));
  return DY_SUCCESS;
}

} /// ::dy namespace