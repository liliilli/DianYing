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
#include <Dy/Management/Rendering/RenderingManager.h>

#include <Dy/Management/LoggingManager.h>
#include <Dy/Management/WorldManager.h>
#include <Dy/Management/SettingManager.h>

#include <Dy/Management/Editor/GuiSetting.h>

#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/Rendering/FramebufferManager.h>
#include <Dy/Management/Rendering/UniformBufferObjectManager.h>
#include "Dy/Component/CDyCamera.h"
#include "Dy/Component/CDyModelRenderer.h"
#include "Dy/Management/Helper/SDyProfilingHelper.h"

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{ // Initialize framebuffer management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Initialize());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Initialize());

  this->mBasicOpaqueRenderer  = std::make_unique<decltype(this->mBasicOpaqueRenderer)::element_type>();
  this->mSceneFinalRenderer   = std::make_unique<decltype(this->mSceneFinalRenderer)::element_type>();
  this->mUiBasicRenderer      = std::make_unique<decltype(this->mUiBasicRenderer)::element_type>();
  this->mFinalDisplayRenderer = std::make_unique<decltype(this->mFinalDisplayRenderer)::element_type>();

  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == true)
  {
    this->mCSMRenderer = std::make_unique<decltype(this->mCSMRenderer)::element_type>();
  }
  if (information.mGraphics.mIsEnabledDefaultSsao == true)
  {
    this->mTempSsaoObject = std::make_unique<decltype(mTempSsaoObject)::element_type>();
  }

#if defined(MDY_FLAG_IN_EDITOR) == true
  //! Grid rendering setting.
  this->mGridEffect = std::make_unique<decltype(this->mGridEffect)::element_type>();
#endif /// MDY_FLAG_IN_EDITOR

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  this->mSceneFinalRenderer   = MDY_INITIALIZE_NULL;
  this->mCSMRenderer          = MDY_INITIALIZE_NULL;
  this->mTempSsaoObject       = MDY_INITIALIZE_NULL;
  this->mBasicOpaqueRenderer  = MDY_INITIALIZE_NULL;
  this->mUiBasicRenderer      = MDY_INITIALIZE_NULL;
  this->mFinalDisplayRenderer = MDY_INITIALIZE_NULL;

  // Initialize internal management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Release());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Release());
  return DY_SUCCESS;
}

void MDyRendering::PushDrawCallTask(_MIN_ CDyModelRenderer& rendererInstance)
{
  this->mOpaqueDrawCallList.emplace_back(DyMakeNotNull(&rendererInstance));
}

void MDyRendering::RenderDrawCallQueue()
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();

  // (0) Clear previous frame results of each framebuffers.
  this->pClearRenderingFramebufferInstances();

  {
    const auto* ptrCamera = MDyWorld::GetInstance().GetPtrMainLevelCamera();

    // If main camera is not exist, do not render level.
    if (MDY_CHECK_ISNOTNULL(ptrCamera))
    {
      DyEraseRemoveIf(this->mOpaqueDrawCallList, [ptrCamera](auto& iPtrOpaqueRenderer)
      {
        const auto& worldPos = iPtrOpaqueRenderer->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
        return ptrCamera->CheckIsPointInFrustum(worldPos) == false;
      });
      SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mOpaqueDrawCallList.size()));

      // Set viewport values to camera's properties.
      const auto viewportRect = ptrCamera->GetPixelizedViewportRectangle();
      glViewport(viewportRect[0], viewportRect[1], viewportRect[2], viewportRect[3]);

      // (1) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
      this->mBasicOpaqueRenderer->PreRender();
      this->mBasicOpaqueRenderer->RenderScreen(this->mOpaqueDrawCallList);

      // (2) Cascaded Shadow mapping to opaque call list.
      if (information.mGraphics.mIsEnabledDefaultShadow == true)
      { 
        // Pre-render update for update Segments.
        this->mCSMRenderer->PreRender();
        // Render
        // If passed culling test, do render.
        //this->mCSMRenderer->RenderScreen(*drawInstance);
      }
    }
  }

  // Clear opaque draw queue list
  this->mOpaqueDrawCallList.clear();

  // (3) Draw transparent call list with OIT.
  // @TODO IMPLEMENT THIS!

  //!
  //! Post processing effects
  //!

  //if (this->mIsEnabledSsaoRendering)
  // @TODO FIX THIS (SSAO)
  if (false) { this->mTempSsaoObject->RenderScreen(); }

  //!
  //! Only in editor effects
  //!

#if defined(MDY_FLAG_IN_EDITOR) == true
  glBindFramebuffer(GL_FRAMEBUFFER, this->mDeferredFrameBufferId);
  if (editor::MDyEditorSetting::GetInstance().GetmIsEnabledViewportRenderGrid() && this->mGridEffect)
  {
    this->mGridEffect->RenderGrid();
  }
#endif /// MDY_FLAG_IN_EDITOR

  // Final
#if defined(MDY_FLAG_IN_EDITOR) == false
  this->mSceneFinalRenderer->RenderScreen();
#endif /// MDY_FLAG_IN_EDITOR == false

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->RenderScreen(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->RenderScreen(); }
}

void MDyRendering::MDY_PRIVATE_SPECIFIER(RenderLoading)()
{
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->Clear(); }

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->RenderScreen(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->RenderScreen(); }
}

void MDyRendering::pClearRenderingFramebufferInstances() noexcept
{
  if (MDyWorld::GetInstance().IsLevelPresentValid() == false) { return; }

  if (MDY_CHECK_ISNOTEMPTY(this->mBasicOpaqueRenderer))   { this->mBasicOpaqueRenderer->Clear(); }

  // @TODO DO NOTHING NOW.
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultSsao == true) { }
  // Reset all shadow framebuffer setting.
  //if (information.mGraphics.mIsEnabledDefaultShadow == true) { this->mShadowRenderer->Clear(); }

#if defined(MDY_FLAG_IN_EDITOR) == false
  // Reset final rendering mesh setting.
  if (MDY_CHECK_ISNOTEMPTY(this->mSceneFinalRenderer))    { this->mSceneFinalRenderer->Clear(); }
#endif
  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))       { this->mUiBasicRenderer->Clear(); }
  if (MDY_CHECK_ISNOTEMPTY(this->mFinalDisplayRenderer))  { this->mFinalDisplayRenderer->Clear(); }
}

std::optional<TI32> MDyRendering::pGetAvailableDirectionalLightIndex(_MIN_ const CDyDirectionalLight&)
{
  return this->mSceneFinalRenderer->GetAvailableDirectionalLightIndex();
}

EDySuccess MDyRendering::pUnbindDirectionalLight(const CDyDirectionalLight& component)
{
  if (component.IsBindedToLightingSystem() == false) { return DY_FAILURE; }
  return this->mSceneFinalRenderer->UnbindDirectionalLight(component.TryGetBindedIndexValue().value());
}

EDySuccess MDyRendering::pUpdateDirectionalLightValueToGpu(
    _MIN_ const TI32 index,
    _MIN_ const DDyUboDirectionalLight& container)
{
  return this->mSceneFinalRenderer->UpdateDirectionalLightValueToGpu(index, container);
}

bool MDyRendering::pfIsAvailableDirectionalLightShadow(const CDyDirectionalLight&)
{ // Integrity test
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == false)
  {
    MDY_LOG_WARNING("MDyRendering::pfIsAvailableDirectionalLightShadow | Shadow feature is disabled now.");
    return false;
  }

  //MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mShadowRenderer), "Shadow object must be valid if shadow feature is enabled.");
  return true;
}

EDySuccess MDyRendering::pfUpdateDirectionalLightShadowToGpu(const CDyDirectionalLight& component)
{ // Integrity test
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == false) { return DY_FAILURE; }
  //if (MDY_CHECK_ISEMPTY(this->mShadowRenderer))   { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mSceneFinalRenderer)) { return DY_FAILURE; }

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfUnbindDirectionalLightShadowToGpu(const CDyDirectionalLight& component)
{ // Integrity test
  const auto& information = MDySetting::GetInstance().GetGameplaySettingInformation();
  if (information.mGraphics.mIsEnabledDefaultShadow == false) { return DY_FAILURE; }
  //if (MDY_CHECK_ISEMPTY(this->mShadowRenderer))   { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mSceneFinalRenderer)) { return DY_FAILURE; }

  // Update values
  //MDY_NOTUSED
  //const auto flag1 = this->mShadowRenderer->UnbindDirectionalLightShadowToGpu(component);
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_SUCCESS;
}

} /// ::dy namespace