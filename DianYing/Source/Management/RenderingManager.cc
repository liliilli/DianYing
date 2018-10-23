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
#include <Dy/Management/RenderingManager.h>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Management/DataInformationManager.h>
#include <Dy/Management/Editor/GuiSetting.h>

#include <Dy/Core/Component/Resource/MaterialResource.h>
#include <Dy/Core/Component/Resource/ModelResource.h>
#include <Dy/Core/Component/Resource/SubmeshResource.h>
#include <Dy/Core/Component/Resource/TextureResource.h>
#include <Dy/Core/Component/Object/Camera.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/Internal/FramebufferManager.h>

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{ // Initialize framebuffer management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Initialize());

  this->mBasicRenderer      = std::make_unique<decltype(mBasicRenderer)::element_type>();
  this->mFinalRenderingMesh = std::make_unique<decltype(mFinalRenderingMesh)::element_type>();

  if (this->mTempIsEnabledSsao)
  {
    mTempSsaoObject = std::make_unique<decltype(mTempSsaoObject)::element_type>();
  }

  if (this->mTempIsEnabledShadow)
  {
    this->mTempShadowObject = std::make_unique<decltype(this->mTempShadowObject)::element_type>();
  }

#if defined(MDY_FLAG_IN_EDITOR) == true
  //! Grid rendering setting.
  this->mGridEffect = std::make_unique<decltype(this->mGridEffect)::element_type>();
#endif /// MDY_FLAG_IN_EDITOR

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  this->mFinalRenderingMesh = MDY_INITIALIZE_NULL;
  this->mTempShadowObject   = MDY_INITIALIZE_NULL;
  this->mTempSsaoObject     = MDY_INITIALIZE_NULL;
  this->mBasicRenderer      = MDY_INITIALIZE_NULL;

  // Initialize framebuffer management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Release());
  return DY_SUCCESS;
}

void MDyRendering::PushDrawCallTask(_MIN_ CDyModelRenderer& rendererInstance)
{
  this->mOpaqueDrawCallList.emplace_back(DyMakeNotNull(&rendererInstance));
}

void MDyRendering::RenderDrawCallQueue()
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }

  // (0) Clear previous frame results of each framebuffers.
  this->pClearRenderingFramebufferInstances();

  // (1) Draw opaque call list.
  this->mBasicRenderer->RenderScreen(this->mOpaqueDrawCallList);

  // (2) Shadow mapping to opaque call list.
#ifdef false
  for (const auto& drawInstance : this->mOpaqueDrawCallList)
  { // General deferred rendering
    if (this->mTempIsEnabledShadow)
    { // Basic shadow (directional light etc)
      glViewport(0, 0, 512, 512);
      this->pRenderShadowFrameBufferWith(*drawInstance);
    }
  }
#endif

  // (3) Draw transparent call list with OIT.
  // @TODO IMPLEMENT THIS!

  // Clear opaque draw queue list
  this->mOpaqueDrawCallList.clear();

  //!
  //! Post processing effects
  //!

  //if (this->mTempIsEnabledSsao)
  // @TODO FIX THIS (SSAO)
  if (false) { this->mTempSsaoObject->RenderScreen(); }

  //!
  //! Only in editor effects
  //!

#if defined(MDY_FLAG_IN_EDITOR) == true
  glViewport(0, 0, setting.GetWindowSizeWidth(), setting.GetWindowSizeHeight());
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);
  if (editor::MDyEditorSetting::GetInstance().GetmIsEnabledViewportRenderGrid() && this->mGridEffect)
  {
    this->mGridEffect->RenderGrid();
  }
#endif /// MDY_FLAG_IN_EDITOR

  //!
  //! Final
  //!

#if defined(MDY_FLAG_IN_EDITOR) == false
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  this->mFinalRenderingMesh->RenderScreen();
#endif /// MDY_FLAG_IN_EDITOR == false
}

void MDyRendering::pClearRenderingFramebufferInstances() noexcept
{
  auto& worldManager = MDyWorld::GetInstance();
  if (worldManager.IsLevelPresentValid() == false) { return; }

  if (MDY_CHECK_ISNOTEMPTY(this->mBasicRenderer)) { this->mBasicRenderer->Clear(); }

  if (this->mTempIsEnabledSsao)
  {
    // @TODO DO NOTHING NOW.
  }

  // Reset all shadow framebuffer setting
  if (this->mTempIsEnabledShadow)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, this->mTempShadowObject->GetShadowFrameBufferId());
    glClear(GL_DEPTH_BUFFER_BIT);
  }

#if defined(MDY_FLAG_IN_EDITOR) == false
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalRenderingMesh))
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    const auto& backgroundColor = worldManager.GetValidLevelReference().GetBackgroundColor();
    glClearColor(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
#endif
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MDyRendering::pRenderDeferredFrameBufferWith(_MIN_ const CDyModelRenderer& renderer, _MIN_ const CDyCamera& validCamera) noexcept
{
  const auto materialListCount  = renderer.GetMaterialListCount();
  const auto opSubmeshListCount = renderer.GetModelSubmeshCount();

  // Integrity test
  if (opSubmeshListCount.has_value() == false) { return; }

  TI32 iterationCount = MDY_INITIALIZE_DEFINT;
  if (materialListCount < opSubmeshListCount.value()) { iterationCount = materialListCount; }
  else                                                { iterationCount = opSubmeshListCount.value(); }

  for (TI32 i = 0; i < iterationCount; ++i)
  {
    auto& material      = const_cast<CDyMaterialResource&>(renderer.GetMaterialResourcePtr(i));
    auto shaderResource = material.GetShaderResource();
    if (shaderResource == nullptr)
    {
      MDY_LOG_CRITICAL("{} | Shader resource of {} is not binded, Can not render mesh.", "CDyMeshRenderer::Render", material.GetMaterialName());
      continue;
    }

    shaderResource->UseShader();

    const CDySubmeshResource& submesh = renderer.GetSubmeshResourcePtr(i);
    glBindVertexArray(submesh.GetVertexArrayId());

    const auto modelMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "modelMatrix");
    const auto viewMatrix = glGetUniformLocation(shaderResource->GetShaderProgramId(), "viewMatrix");
    const auto projMatirx = glGetUniformLocation(shaderResource->GetShaderProgramId(), "projectionMatrix");

    const auto& model = const_cast<FDyActor*>(renderer.GetBindedActor())->GetTransform()->GetTransform();

    glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &model[0].X);
    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &validCamera.GetViewMatrix()[0].X);
    glUniformMatrix4fv(projMatirx, 1, GL_FALSE, &validCamera.GetProjectionMatrix()[0].X);

    // If skeleton animation is enabled, get bone transform and bind to shader.
#ifdef false
    const auto boneTransform = glGetUniformLocation(shaderResource->GetShaderProgramId(), "boneTransform");
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

    // Bind textures of one material.
    const auto& textureResources        = material.GetBindedTextureResources();
    const auto  textureResourceListSize = static_cast<int32_t>(textureResources.size());
    for (int32_t j = 0; j < textureResourceListSize; ++j)
    {
      glUniform1i(glGetUniformLocation(shaderResource->GetShaderProgramId(), (std::string("uTexture") + std::to_string(j)).c_str()), j);

      const auto texturePointer = textureResources[j].mValidTexturePointer;
      glActiveTexture(GL_TEXTURE0 + j);
      switch (texturePointer->GetTextureType())
      {
      case EDyTextureStyleType::D1: glBindTexture(GL_TEXTURE_1D, texturePointer->GetTextureId()); break;
      case EDyTextureStyleType::D2: glBindTexture(GL_TEXTURE_2D, texturePointer->GetTextureId()); break;
      default: MDY_UNEXPECTED_BRANCH(); break;
      }
    }

    // Call function call drawing array or element. (not support instancing yet)
    if (submesh.IsEnabledIndices()) { glDrawElements(GL_TRIANGLES, submesh.GetIndicesCounts(), GL_UNSIGNED_INT, nullptr); }
    else                            { glDrawArrays(GL_TRIANGLES, 0, submesh.GetVertexCounts()); }

    // Unbind, unset, deactivate settings for this submesh and material.
    for (TI32 j = 0; j < textureResourceListSize; ++j)
    {
      glActiveTexture(GL_TEXTURE0 + j);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Unbind present submesh vertex array object.
    glBindVertexArray(0);
    shaderResource->UnuseShader();
  }
}

void MDyRendering::pRenderShadowFrameBufferWith(_MIN_ const CDyModelRenderer& renderer) noexcept
{
  this->mTempShadowObject->RenderScreen(renderer);
}

std::optional<TI32> MDyRendering::pGetAvailableDirectionalLightIndex(_MIN_ const CDyDirectionalLight&)
{
  return this->mFinalRenderingMesh->GetAvailableDirectionalLightIndex();
}

EDySuccess MDyRendering::pUnbindDirectionalLight(const CDyDirectionalLight& component)
{
  if (component.IsBindedToLightingSystem() == false) { return DY_FAILURE; }
  return this->mFinalRenderingMesh->UnbindDirectionalLight(component.TryGetBindedIndexValue().value());
}

EDySuccess MDyRendering::pUpdateDirectionalLightValueToGpu(
    _MIN_ const TI32 index,
    _MIN_ const DDyUboDirectionalLight& container)
{
  return this->mFinalRenderingMesh->UpdateDirectionalLightValueToGpu(index, container);
}

bool MDyRendering::pfIsAvailableDirectionalLightShadow(const CDyDirectionalLight&)
{ // Integrity test
  if (this->mTempIsEnabledShadow == false)
  {
    MDY_LOG_WARNING("MDyRendering::pfIsAvailableDirectionalLightShadow | Shadow feature is disabled now.");
    return false;
  }

  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mTempShadowObject), "Shadow object must be valid if shadow feature is enabled.");
  return this->mTempShadowObject->IsAvailableDirectionalLightShadow();
}

EDySuccess MDyRendering::pfUpdateDirectionalLightShadowToGpu(const CDyDirectionalLight& component)
{ // Integrity test
  if (this->mTempIsEnabledShadow == false)          { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mTempShadowObject))   { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mFinalRenderingMesh)) { return DY_FAILURE; }

  // Update values
  MDY_NOTUSED
  const auto flag1 = this->mTempShadowObject->UpdateDirectionalLightShadowToGpu(component);
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfUnbindDirectionalLightShadowToGpu(const CDyDirectionalLight& component)
{ // Integrity test
  if (this->mTempIsEnabledShadow == false)          { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mTempShadowObject))   { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mFinalRenderingMesh)) { return DY_FAILURE; }

  // Update values
  MDY_NOTUSED
  const auto flag1 = this->mTempShadowObject->UnbindDirectionalLightShadowToGpu(component);
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_SUCCESS;
}

} /// ::dy namespace