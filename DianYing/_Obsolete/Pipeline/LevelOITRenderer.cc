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
#include <Dy/Core/Rendering/Pipeline/LevelOITRenderer.h>
#include <Dy/Core/Resource/Resource/FDyFrameBufferResource.h>
#include <Dy/Core/Resource/Resource/FDyShaderResource.h>
#include <Dy/Core/Resource/Resource/FDyMaterialResource.h>
#include <Dy/Core/Resource/Resource/FDyMeshResource.h>
#include <Dy/Core/Rendering/Wrapper/FDyGLWrapper.h>
#include <Dy/Component/CDyModelRenderer.h>
#include <Dy/Element/Actor.h>
#include <Dy/Management/Type/Render/DDyGlGlobalStatus.h>
#include <Dy/Management/Rendering/RenderingManager.h>

namespace dy
{

bool FDyLevelOITRenderer::IsReady() const noexcept
{
  return this->mBinderFrameBuffer.IsResourceExist() == true;
}

EDySuccess FDyLevelOITRenderer::TryPushRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  DDyGlGlobalStatus status;
  using EMode   = DDyGlGlobalStatus::DPolygonMode::EMode;
  using EValue  = DDyGlGlobalStatus::DPolygonMode::EValue;
  using EEqut   = DDyGlGlobalStatus::DBlendMode::EEqut;
  using EFunc   = DDyGlGlobalStatus::DBlendMode::EFunc;
  using DBlendMode = DDyGlGlobalStatus::DBlendMode;

  status.mIsEnableBlend = true;
  status.mIsEnableDepthTest = false;
  status.mIsEnableCullface  = false;

  DBlendMode blendingList{};
  blendingList.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::One, EFunc::One);
  blendingList.mBlendingSettingList.emplace_back(EEqut::SrcAddDst, EFunc::Zero, EFunc::OneMinusSrcColor);
  status.mBlendMode = blendingList;

  auto& refRendering = MDyRendering::GetInstance();
  refRendering.InsertInternalGlobalStatus(status);

  glClearBufferfv(GL_COLOR, 0, &DDyColorRGBA::Black.R);
  glClearBufferfv(GL_COLOR, 1, &DDyColorRGBA::White.R);

  this->mBinderFrameBuffer->BindFrameBuffer();
  return DY_SUCCESS;
}

void FDyLevelOITRenderer::RenderScreen(
    _MIN_ CDyModelRenderer& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial)
{
  if (this->IsReady() == false) { return; }

  // General deferred rendering
  auto ptrModelTransform = iRefRenderer.GetBindedActor()->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Matrix4>("uModelMatrix", ptrModelTransform->GetTransform());
  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Matrix4>("uRotationMatrix", ptrModelTransform->GetRotationMatrix());
  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Float>("uAlphaOffset", 0.75f);
  shaderBinder->TryUpdateUniform<EDyUniformVariableType::Float>("uDepthScale",  0.1f);

  shaderBinder->UseShader();
  shaderBinder->TryUpdateUniformList();
  iRefMaterial.TryUpdateTextureList();
  iRefMesh.BindVertexArray();

  // Call function call drawing array or element. 
  if (iRefMesh.IsEnabledIndices() == true)
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, true, iRefMesh.GetIndicesCounts()); }
  else
  { FDyGLWrapper::Draw(EDyDrawType::Triangle, false, iRefMesh.GetVertexCounts()); }

  // Unbind present submesh vertex array object.
  // Unbind, unset, deactivate settings for this submesh and material.
  FDyGLWrapper::UnbindVertexArrayObject();
  shaderBinder->DisuseShader();
}

EDySuccess FDyLevelOITRenderer::TryPopRenderingSetting()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFrameBuffer->UnbindFrameBuffer();

  auto& refRendering = MDyRendering::GetInstance();
  refRendering.PopInternalGlobalStatus();

  return DY_SUCCESS;
}

void FDyLevelOITRenderer::Clear()
{
  if (this->IsReady() == false) { return; }

  { MDY_GRAPHIC_SET_CRITICALSECITON();
    this->mBinderFrameBuffer->BindFrameBuffer();
    glClearBufferfv(GL_COLOR, 0, &DDyColorRGBA::Black.R);
    glClearBufferfv(GL_COLOR, 1, &DDyColorRGBA::White.R);
    this->mBinderFrameBuffer->UnbindFrameBuffer();
  }
}

} /// ::dy namespace