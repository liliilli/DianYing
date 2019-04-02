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

#include <Dy/Builtin/RenderItem/FBtLevelIntegeration.h>
#include <Dy/Management/Type/Render/DDyModelHandler.h>
#include <Dy/Management/MWorld.h>
#include <Dy/Core/Resource/Resource/AResourceFrameBufferBase.h>
#include <Dy/Core/Resource/Resource/FResourceShader.h>
#include <Dy/Core/Rendering/Type/EDrawType.h>
#include <Dy/Core/Rendering/Wrapper/XGLWrapper.h>
#include <Dy/Core/Resource/Resource/FResourceMesh.h>
#include <Dy/Management/Rendering/MRendering.h>
#include <Dy/Management/Rendering/MUniformBufferObject.h>
#include <Dy/Component/CLightDirectional.h>
#include <Dy/Element/FLevel.h>
#include <Dy/Core/Resource/Resource/FResourceModel.h>
#include <Dy/Core/Resource/Resource/AResourceAttachmentBase.h>
#include <Dy/Component/Internal/Lights/DUboPointLight.h>
#include <Dy/Component/CLightPoint.h>
#include "Dy/Core/Reflection/RReflection.h"
#include "Dy/Component/CCamera.h"

namespace dy
{

void FBtRenderItemLevelIntegeration::__ConstructionHelper
  ::ConstructBuffer(PDyRenderItemInstanceMetaInfo& oMeta)
{
  oMeta.mUuid = DUuid("1a8757c9-2406-41a7-a222-e8a726e65ec5", true);
}

EDySuccess FBtRenderItemLevelIntegeration::OnPreRenderCheckCondition()
{
  return 
      this->mBinderFrameBuffer.IsResourceExist() == true
  &&  this->mBinderOpaqueShader.IsResourceExist() == true
  &&  this->mBinderTriangle.IsResourceExist() == true ? DY_SUCCESS : DY_FAILURE;
}

void FBtRenderItemLevelIntegeration::OnSetupRenderingSetting()
{
  this->pSetupOpaqueCSMIntegration();
  this->pSetupTranslucentOITIntegration();
}

void FBtRenderItemLevelIntegeration::pSetupOpaqueCSMIntegration()
{
  // Update shader's uniform information.
  this->pUpdateUboShadowInfo();
  this->pUpdateUboDirectionalLightInfo();
  this->pUpdateUboPointLightsInfo();

  this->mBinderFrameBuffer->BindFrameBuffer();
  const auto& backgroundColor = MWorld::GetInstance().GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT);
}

void FBtRenderItemLevelIntegeration::pUpdateUboShadowInfo()
{
  auto& uboManager = MUniformBufferObject::GetInstance();
  if (const auto* ptr = MRendering::GetInstance().GetPtrMainDirectionalShadow();
      ptr != nullptr) 
  { 
    DDyUboDirShadow shadow = ptr->GetUboShadowInfo();
    uboManager.UpdateUboContainer("dyBtUboDirShadow", 0, sizeof(DDyUboDirShadow), &shadow);
  }
  else
  {
    DDyUboDirShadow shadow;
    uboManager.UpdateUboContainer("dyBtUboDirShadow", 0, sizeof(DDyUboDirShadow), &shadow);
  }
}

void FBtRenderItemLevelIntegeration::pUpdateUboDirectionalLightInfo()
{
  auto& uboManager = MUniformBufferObject::GetInstance();
  auto* ptrLight = MRendering::GetInstance().GetPtrMainDirectionalLight();
  if (this->mAddrMainLight != reinterpret_cast<ptrdiff_t>(ptrLight))
  {
    this->mAddrMainLight = reinterpret_cast<ptrdiff_t>(ptrLight);
    if (this->mAddrMainLight == 0)
    {
      DUboDirectionalLight light;
      uboManager.UpdateUboContainer("dyBtUboDirLight", 0, sizeof(DUboDirectionalLight), &light);
    }
    else
    {
      DUboDirectionalLight light = ptrLight->GetUboLightInfo();
      uboManager.UpdateUboContainer("dyBtUboDirLight", 0, sizeof(DUboDirectionalLight), &light);
    }
  }
}

void FBtRenderItemLevelIntegeration::pUpdateUboPointLightsInfo()
{
  auto& activateLightPtrList  = MRendering::GetInstance().__GetActivatedPointLights();
  std::vector<DUboPointLight> pointLightChunk; 
  pointLightChunk.reserve(16);

  // Do cpu frustum culling.
  const auto* ptrCamera = MWorld::GetInstance().GetPtrMainLevelCamera();
  for (auto& ptrLight : activateLightPtrList)
  {
    const auto& lightInfo = ptrLight->GetUboLightInfo();
    if (ptrCamera->IsSphereInFrustum(lightInfo.mPosition, lightInfo.mRange) == true)
    {
      pointLightChunk.emplace_back(lightInfo);
    }
  }

  for (size_t i = 0, size = pointLightChunk.size(); i < size; ++i)
  {
    this->mBinderOpaqueShader->TryUpdateUniformStruct(i, pointLightChunk[i]);
  }

  for (size_t i = pointLightChunk.size(); i < 16; ++i)
  {
    static const DUboPointLight blank{};
    this->mBinderOpaqueShader->TryUpdateUniformStruct(i, blank);
  }
}

void FBtRenderItemLevelIntegeration::pSetupTranslucentOITIntegration()
{
  this->mBinderFbTranslucent->BindFrameBuffer();
  glClearColor(0, 0, 0, 0); 
  glClear(GL_COLOR_BUFFER_BIT);
  this->mBinderFbTranslucent->UnbindFrameBuffer(); 
}

void FBtRenderItemLevelIntegeration::OnRender()
{
  const auto& submeshList = this->mBinderTriangle->GetMeshResourceList();
  MDY_ASSERT_MSG(submeshList.size() == 1, "Unexpected error occurred.");
  submeshList[0]->Get()->BindVertexArray();

  {
    this->mBinderFrameBuffer->BindFrameBuffer();
    // Check Textures.
    this->mBinderOpaqueShader->TryInsertTextureRequisition(0, this->mBinderAttUnlit->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(1, this->mBinderAttNormal->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(2, this->mBinderAttSpecular->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(3, this->mBinderAttPosition->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(4, this->mBinderAttShadow->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(5, this->mBinderAttZValue->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(6, this->mBinderAttSSAO->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(7, this->mBinderAttSky->GetSourceAttachmentId());
    this->mBinderOpaqueShader->TryInsertTextureRequisition(8, this->mBinderAttEmissive->GetSourceAttachmentId());
    this->mBinderOpaqueShader->UseShader();
    this->mBinderOpaqueShader->TryUpdateUniformList();

    XGLWrapper::Draw(EDrawType::Triangle, true, 3);
  }

  {
    this->mBinderFbTranslucent->BindFrameBuffer();
    // 
    this->mBinderTransShader->TryInsertTextureRequisition(0, this->mBinderAttOpaque->GetSourceAttachmentId());
    this->mBinderTransShader->TryInsertTextureRequisition(1, this->mBinderAttOITColor->GetSourceAttachmentId());
    this->mBinderTransShader->TryInsertTextureRequisition(2, this->mBinderAttOITWeigh->GetSourceAttachmentId());
    this->mBinderTransShader->UseShader();
    this->mBinderTransShader->TryUpdateUniformList();

    XGLWrapper::Draw(EDrawType::Triangle, true, 3);

    this->mBinderTransShader->DisuseShader();
    this->mBinderFbTranslucent->UnbindFrameBuffer();
  }

  XGLWrapper::UnbindVertexArrayObject();
}

void FBtRenderItemLevelIntegeration::OnReleaseRenderingSetting()
{
  /* Do nothing */
}

} /// ::dy namespace
