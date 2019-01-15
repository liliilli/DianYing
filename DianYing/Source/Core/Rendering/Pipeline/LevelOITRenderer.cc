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

namespace dy
{

bool FDyLevelOITRenderer::IsReady() const noexcept
{
  return this->mBinderFrameBuffer.IsResourceExist() == true;
}

EDySuccess FDyLevelOITRenderer::TrySetupRendering()
{
  if (this->IsReady() == false) { return DY_FAILURE; }

  this->mBinderFrameBuffer->BindFrameBuffer();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunci(0, GL_ONE,  GL_ONE);
  glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

  glClearBufferfv(GL_COLOR, 0, &DDyColorRGBA::Black.R);
  glClearBufferfv(GL_COLOR, 1, &DDyColorRGBA::White.R);
  return DY_SUCCESS;
}

void FDyLevelOITRenderer::RenderScreen(
    _MIN_ CDyModelRenderer& iRefRenderer,
    _MIN_ FDyMeshResource& iRefMesh, 
    _MIN_ FDyMaterialResource& iRefMaterial)
{
  if (this->mBinderFrameBuffer.IsResourceExist() == false) { return; }
  this->mBinderFrameBuffer->BindFrameBuffer();

  // General deferred rendering
  auto ptrModelTransform = iRefRenderer.GetBindedActor()->GetTransform();
  auto& shaderBinder = iRefMaterial.GetShaderResourceBinder();
  if (shaderBinder.IsResourceExist() == false) { return; }

  shaderBinder->UseShader();
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uModelMatrix", ptrModelTransform->GetTransform());
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Matrix4>("uRotationMatrix", ptrModelTransform->GetRotationMatrix());
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Float>("uAlphaOffset", 0.5f);
  shaderBinder.TryUpdateUniform<EDyUniformVariableType::Float>("uDepthScale",  0.1f);
  shaderBinder.TryUpdateUniformList();
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
  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

void FDyLevelOITRenderer::Clear()
{
  if (this->IsReady() == false) { return; }

  this->mBinderFrameBuffer->BindFrameBuffer();

#ifdef false
  const auto& backgroundColor = MDyWorld::GetInstance().GetValidLevelReference().GetBackgroundColor();
  glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

  this->mBinderFrameBuffer->UnbindFrameBuffer();
}

} /// ::dy namespace