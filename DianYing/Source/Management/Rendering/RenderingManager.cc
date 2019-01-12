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
#include "Dy/Core/Rendering/Wrapper/FDyGLWrapper.h"

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
      // DO CPU VIEW-FRUSTUM CULLING.
#ifdef false
      DyEraseRemoveIf(this->mOpaqueDrawCallList, [ptrCamera](auto& iPtrOpaqueRenderer)
      {
        const auto& worldPos = iPtrOpaqueRenderer->GetBindedActor()->GetTransform()->GetFinalWorldPosition();
        return ptrCamera->CheckIsPointInFrustum(worldPos) == false;
      });
#endif
      SDyProfilingHelper::AddScreenRenderedActorCount(static_cast<TI32>(this->mOpaqueDrawCallList.size()));

      // (1) Cascaded Shadow mapping to opaque call list.
      if (information.mGraphics.mIsEnabledDefaultShadow == true)
      { 
        // Pre-render update for update Segments.
        this->mCSMRenderer->PreRender();
        if (this->mCSMRenderer->TrySetupRendering() == DY_SUCCESS)
        {
          this->mCSMRenderer->SetupViewport();
          for (auto& drawInstance : this->mOpaqueDrawCallList)
          { // Render
            this->mCSMRenderer->RenderScreen(*drawInstance);
          }
        }
      }

      // Set global viewport values to camera's properties.
      FDyGLWrapper::SetViewport(ptrCamera->GetPixelizedViewportRectangle());
      // (2) Draw opaque call list. Get valid Main CDyCamera instance pointer address.
      this->mBasicOpaqueRenderer->PreRender();
      this->mBasicOpaqueRenderer->RenderScreen(this->mOpaqueDrawCallList);
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
  if (this->mSceneFinalRenderer->TrySetupRendering() == DY_SUCCESS)
  {
    this->mSceneFinalRenderer->RenderScreen();
  }

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

CDyDirectionalLight* MDyRendering::GetPtrMainDirectionalLight() const noexcept
{
  return this->mMainDirectionalLight;
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

/// @brief Private function, bind directional light as main light.
void MDyRendering::MDY_PRIVATE_SPECIFIER(BindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalLight = &iRefLight;
}

/// @brief Private function, unbind directional light of main light.
EDySuccess MDyRendering::MDY_PRIVATE_SPECIFIER(UnbindMainDirectionalLight)(_MIN_ CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalLight == &iRefLight) 
  {
    this->mMainDirectionalLight = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}
  
/// @brief Private function, bind directional light as main light.
void MDyRendering::MDY_PRIVATE_SPECIFIER(BindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight)
{
  this->mMainDirectionalShadow = &iRefLight;
}

/// @brief Private function, unbind directional light of main light.
EDySuccess MDyRendering::MDY_PRIVATE_SPECIFIER(UnbindMainDirectionalShadow)(_MIN_ CDyDirectionalLight& iRefLight)
{
  if (this->mMainDirectionalShadow == &iRefLight) 
  {
    this->mMainDirectionalShadow = nullptr;
    return DY_SUCCESS;
  }
  else { return DY_FAILURE; }
}

} /// ::dy namespace