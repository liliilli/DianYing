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
#include <Dy/Core/Rendering/Pipeline/LevelCSMIntegration.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Resource/Resource/FDyModelResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/Rendering/RenderingManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include <Dy/Component/CDyDirectionalLight.h>

namespace dy
{

FDyLevelCSMIntergration::FDyLevelCSMIntergration()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  PDyUboConstructionDescriptor desc = {};
  desc.mBindingIndex      = 1;
  desc.mUboSpecifierName  = "dyBtUboDirLight";
  desc.mBufferDrawType    = EDyBufferDrawType::DynamicDraw;
  desc.mUboElementSize    = sizeof(DDyUboDirectionalLight);
  desc.mUboArraySize      = 1;
  MDY_CALL_ASSERT_SUCCESS(uboManager.CreateUboContainer(desc))
}

FDyLevelCSMIntergration::~FDyLevelCSMIntergration()
{
  auto& uboManager = MDyUniformBufferObject::GetInstance();
  MDY_CALL_ASSERT_SUCCESS(uboManager.RemoveUboContainer("dyBtUboDirLight"))
}

bool FDyLevelCSMIntergration::IsReady() const noexcept
{
  return 
      this->mBinderFrameBuffer.IsResourceExist() == true
  &&  this->mBinderOpaqueShader.IsResourceExist() == true
  &&  this->mBinderTriangle.IsResourceExist() == true;
}

EDySuccess FDyLevelCSMIntergration::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }
  if (this->pSetupOpaqueCSMIntegration() == DY_FAILURE)       { return DY_FAILURE; }
  if (this->pSetupTranslucentOITIntegration() == DY_FAILURE)  { return DY_FAILURE; }

  return DY_SUCCESS;
}

void FDyLevelCSMIntergration::RenderScreen()
{
  if (this->IsReady() == false) { return; }
  
  this->mBinderFrameBuffer->BindFrameBuffer();
  this->mBinderOpaqueShader->UseShader();
  // Check Textures.
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttUnlit->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttNormal->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderAttSpecular->GetAttachmentId());
  glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, this->mBinderAttPosition->GetAttachmentId());
  glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D_ARRAY, this->mBinderAttShadow->GetAttachmentId());
  glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, this->mBinderAttZValue->GetAttachmentId());
  glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, this->mBinderAttSSAO->GetAttachmentId());

  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();
  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

  this->mBinderFbTranslucent->BindFrameBuffer();
  this->mBinderTransShader->UseShader();
  glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, this->mBinderAttOpaque->GetAttachmentId());
  glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, this->mBinderAttOITColor->GetAttachmentId());
  glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, this->mBinderAttOITWeigh->GetAttachmentId());
  FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

  this->mBinderTransShader->DisuseShader();
  this->mBinderFbTranslucent->UnbindFrameBuffer();
  FDyGLWrapper::UnbindVertexArrayObject();
}

EDySuccess FDyLevelCSMIntergration::pSetupOpaqueCSMIntegration()
{
  this->mBinderFrameBuffer->BindFrameBuffer();
  const auto& backgroundColor = MDyWorld::GetInstance().GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT);

  // Update shader's uniform information.
  this->mBinderOpaqueShader->UseShader();
  const auto* ptr = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
  if (MDY_CHECK_ISNOTNULL(ptr)) 
  { 
    // DyConvertToVector
    this->mBinderOpaqueShader.TryUpdateUniform<EDyUniformVariableType::Vector4>(
        "uNormalizedFarPlanes", 
        ptr->GetCSMNormalizedFarPlanes());
    this->mBinderOpaqueShader.TryUpdateUniform<EDyUniformVariableType::Matrix4Array>(
        "uLightVPSBMatrix[0]", 
        DyConvertToVector<DDyMatrix4x4>(ptr->GetCSMLightSegmentVPSBMatrix()));
    this->mBinderOpaqueShader.TryUpdateUniformList();
  }

  // Update directional light property.
  auto* ptrLight = MDyRendering::GetInstance().GetPtrMainDirectionalLight();
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

EDySuccess FDyLevelCSMIntergration::pSetupTranslucentOITIntegration()
{
  this->mBinderFbTranslucent->BindFrameBuffer();
  glClearColor(0, 0, 0, 0); 
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFbTranslucent->UnbindFrameBuffer(); 

  this->mBinderTransShader->UseShader();
  this->mBinderTransShader.TryUpdateUniformList();
  return DY_SUCCESS;
}

void FDyLevelCSMIntergration::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();

  const auto& backgroundColor = MDyWorld::GetInstance().GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT);

  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace