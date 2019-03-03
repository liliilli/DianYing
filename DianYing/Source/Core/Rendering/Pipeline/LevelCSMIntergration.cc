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
#include <Dy/Element/Level.h>

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

EDySuccess FDyLevelCSMIntergration::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }
  if (this->pSetupOpaqueCSMIntegration() == DY_FAILURE)       { return DY_FAILURE; }
  if (this->pSetupTranslucentOITIntegration() == DY_FAILURE)  { return DY_FAILURE; }

  return DY_SUCCESS;
}

EDySuccess FDyLevelCSMIntergration::pSetupOpaqueCSMIntegration()
{
  // Update shader's uniform information.
  if (const auto* ptr = MDyRendering::GetInstance().GetPtrMainDirectionalShadow();
      ptr != nullptr) 
  { 
    using EUniform = EDyUniformVariableType;
    // DyConvertToVector
    this->mBinderOpaqueShader->TryUpdateUniform<EUniform::Vector4>(
        "uNormalizedFarPlanes", 
        ptr->GetCSMNormalizedFarPlanes());
    this->mBinderOpaqueShader->TryUpdateUniform<EUniform::Matrix4Array>(
        "uLightVPSBMatrix[0]", 
        DyConvertToVector<DDyMatrix4x4>(ptr->GetCSMLightSegmentVPSBMatrix()));
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
      uboManager.UpdateUboContainer("dyBtUboDirLight", 0, sizeof(DDyUboDirectionalLight), &light);
    }
    else
    {
      DDyUboDirectionalLight light = ptrLight->GetUboLightInfo();
      uboManager.UpdateUboContainer("dyBtUboDirLight", 0, sizeof(DDyUboDirectionalLight), &light);
    }
  }

  this->mBinderFrameBuffer->BindFrameBuffer();
  const auto& backgroundColor = MDyWorld::GetInstance().GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT);

  return DY_SUCCESS;
}

EDySuccess FDyLevelCSMIntergration::pSetupTranslucentOITIntegration()
{
  this->mBinderFbTranslucent->BindFrameBuffer();
  glClearColor(0, 0, 0, 0); 
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFbTranslucent->UnbindFrameBuffer(); 

  return DY_SUCCESS;
}


void FDyLevelCSMIntergration::RenderScreen()
{
  if (this->IsReady() == false) { return; }
  
  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();

  {
    this->mBinderFrameBuffer->BindFrameBuffer();
    // Check Textures.
    this->mBinderOpaqueShader->TryInsertTextureRequisition(0, this->mBinderAttUnlit->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(1, this->mBinderAttNormal->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(2, this->mBinderAttSpecular->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(3, this->mBinderAttPosition->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(4, this->mBinderAttShadow->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(5, this->mBinderAttZValue->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(6, this->mBinderAttSSAO->GetAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(7, this->mBinderAttSky->GetAttachmentId());
    this->mBinderOpaqueShader->UseShader();
    this->mBinderOpaqueShader->TryUpdateUniformList();

    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);
  }

  {
    this->mBinderFbTranslucent->BindFrameBuffer();
    // 
    this->mBinderTransShader->TryInsertTextureRequisition(0, this->mBinderAttOpaque->GetAttachmentId());
    this->mBinderTransShader->TryInsertTextureRequisition(1, this->mBinderAttOITColor->GetAttachmentId());
    this->mBinderTransShader->TryInsertTextureRequisition(2, this->mBinderAttOITWeigh->GetAttachmentId());
    this->mBinderTransShader->UseShader();
    this->mBinderTransShader->TryUpdateUniformList();

    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, 3);

    this->mBinderTransShader->DisuseShader();
    this->mBinderFbTranslucent->UnbindFrameBuffer();
  }

  FDyGLWrapper::UnbindVertexArrayObject();
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

EDySuccess FDyLevelCSMIntergration::TryPopRenderingSetting()
{
  return DY_SUCCESS;
}

} /// ::dy namespace