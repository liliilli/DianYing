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
#include <Dy/Core/Rendering/Pipeline/PostEffectSky.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyTextureResource.h>
#include <Dy/Core/Resource/Resource/FDyAttachmentResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Component/CDySkybox.h>
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

namespace dy
{

void FDyPostEffectSky::RenderScreen()
{
  if (this->IsReady() == false) { return; }

  // Render
  auto optSkybox = MDyWorld::GetInstance().GetPtrMainLevelSkybox();
  MDY_ASSERT(optSkybox.has_value() == true, "Unexpected error occurred.");

  auto ptrSkyboxTexture  = optSkybox.value();
  auto& refBinderTexture = ptrSkyboxTexture->MDY_PRIVATE(GetTextureBinderReference)();
  if (refBinderTexture.IsResourceExist() == false) { return; } // Failure check

  this->mBinderMeshSkybox->BindVertexArray();

  using EUniform = EDyUniformVariableType;
  this->mBinderShdSkybox->TryUpdateUniform<EUniform::Float>("uExposure",       ptrSkyboxTexture->GetExposure());
  this->mBinderShdSkybox->TryUpdateUniform<EUniform::Float>("uRotationDegree", ptrSkyboxTexture->GetRotationDegree());
  this->mBinderShdSkybox->TryUpdateUniform<EUniform::Vector3>("uTintColor",    static_cast<DDyVector3>(ptrSkyboxTexture->GetTintColor()));
  this->mBinderShdSkybox->TryInsertTextureRequisition(0, refBinderTexture->GetTextureId());
  //this->mBinderShdSkybox->TryInsertTextureRequisition(1, this->mBinderAttUnlit->GetAttachmentId());

  this->mBinderShdSkybox->UseShader();
  this->mBinderShdSkybox->TryUpdateUniformList();

  if (this->mBinderMeshSkybox->IsEnabledIndices() == true)
  {
    FDyGLWrapper::Draw(EDyDrawType::Triangle, true, this->mBinderMeshSkybox->GetIndicesCounts()); 
  }
  else
  {
    FDyGLWrapper::Draw(EDyDrawType::Triangle, false, this->mBinderMeshSkybox->GetVertexCounts()); 
  }

  this->mBinderShdSkybox->DisuseShader();
}

bool FDyPostEffectSky::IsReady() const noexcept
{
  return this->mBinderAttUnlit.IsResourceExist() == true
      && this->mBinderFbSkyRend.IsResourceExist() == true
      && this->mBinderShdSkybox.IsResourceExist() == true
      && this->mBinderMeshSkybox.IsResourceExist() == true;
}

EDySuccess FDyPostEffectSky::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFbSkyRend->BindFrameBuffer();
  const GLfloat one = 0.0f;
  glClearBufferfv(GL_COLOR, 0, &one);
  return DY_SUCCESS;
}

void FDyPostEffectSky::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFbSkyRend->BindFrameBuffer();
  const GLfloat one = 0.0f;
  glClearBufferfv(GL_COLOR, 0, &one);
  this->mBinderFbSkyRend->UnbindFrameBuffer();
}

EDySuccess FDyPostEffectSky::TryPopRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFbSkyRend->UnbindFrameBuffer();
  return DY_SUCCESS;
}

} /// ::dy namespace