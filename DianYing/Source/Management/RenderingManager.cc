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

#include <Dy/Core/Component/Object/Camera.h>
#include <Dy/Component/CDyDirectionalLight.h>
#include <Dy/Management/Internal/FramebufferManager.h>
#include <Dy/Management/Internal/UniformBufferObjectManager.h>

namespace dy
{

EDySuccess MDyRendering::pfInitialize()
{ // Initialize framebuffer management singleton instance.
  MDY_CALL_ASSERT_SUCCESS(MDyFramebuffer::Initialize());
  MDY_CALL_ASSERT_SUCCESS(MDyUniformBufferObject::Initialize());

  this->mBasicOpaqueRenderer  = std::make_unique<decltype(mBasicOpaqueRenderer)::element_type>();
  this->mSceneFinalRenderer   = std::make_unique<decltype(mSceneFinalRenderer)::element_type>();
  this->mShadowRenderer       = std::make_unique<decltype(this->mShadowRenderer)::element_type>();
  this->mUiBasicRenderer      = std::make_unique<decltype(this->mUiBasicRenderer)::element_type>();
  this->mFinalDisplayRenderer = std::make_unique<decltype(this->mFinalDisplayRenderer)::element_type>();

  if (this->mIsEnabledSsaoRendering)
  {
    mTempSsaoObject = std::make_unique<decltype(mTempSsaoObject)::element_type>();
  }

#if defined(MDY_FLAG_IN_EDITOR) == true
  //! Grid rendering setting.
  this->mGridEffect = std::make_unique<decltype(this->mGridEffect)::element_type>();
#endif /// MDY_FLAG_IN_EDITOR

  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfRelease()
{
  this->mSceneFinalRenderer = MDY_INITIALIZE_NULL;
  this->mShadowRenderer     = MDY_INITIALIZE_NULL;
  this->mTempSsaoObject     = MDY_INITIALIZE_NULL;
  this->mBasicOpaqueRenderer      = MDY_INITIALIZE_NULL;

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

  // (0) Clear previous frame results of each framebuffers.
  this->pClearRenderingFramebufferInstances();
  // (1) Draw opaque call list.
  this->mBasicOpaqueRenderer->RenderScreen(this->mOpaqueDrawCallList);
  // (2) Shadow mapping to opaque call list.
  if (this->mIsEnabledShadowRendering == true)
  { // Basic shadow (directional light etc)
    for (const auto& drawInstance : this->mOpaqueDrawCallList)
    {
      this->mShadowRenderer->RenderScreen(*drawInstance);
    }
  }
  // (3) Draw transparent call list with OIT.
  // @TODO IMPLEMENT THIS!

  // Clear opaque draw queue list
  this->mOpaqueDrawCallList.clear();

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

  //!
  //! Final
  //!

#if defined(MDY_FLAG_IN_EDITOR) == false
  //glViewport(0, 0, setting.GetWindowSizeWidth(), setting.GetWindowSizeHeight());
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  this->mSceneFinalRenderer->RenderScreen();
#endif /// MDY_FLAG_IN_EDITOR == false

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))
  {
    this->mUiBasicRenderer->RenderScreen();
  }
}

void MDyRendering::pClearRenderingFramebufferInstances() noexcept
{
  auto& worldManager = MDyWorld::GetInstance();
  if (worldManager.IsLevelPresentValid() == false) { return; }

  if (MDY_CHECK_ISNOTEMPTY(this->mBasicOpaqueRenderer)) { this->mBasicOpaqueRenderer->Clear(); }

  if (this->mIsEnabledSsaoRendering == true)
  { // @TODO DO NOTHING NOW.

  }

  if (this->mIsEnabledShadowRendering == true)
  { // Reset all shadow framebuffer setting.
    this->mShadowRenderer->Clear();
  }

#if defined(MDY_FLAG_IN_EDITOR) == false
  if (MDY_CHECK_ISNOTEMPTY(this->mSceneFinalRenderer))
  { // Reset final rendering mesh setting.
    this->mSceneFinalRenderer->Clear();
  }
#endif

  if (MDY_CHECK_ISNOTEMPTY(this->mUiBasicRenderer))
  {
    this->mUiBasicRenderer->Clear();
  }
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
  if (this->mIsEnabledShadowRendering == false)
  {
    MDY_LOG_WARNING("MDyRendering::pfIsAvailableDirectionalLightShadow | Shadow feature is disabled now.");
    return false;
  }

  MDY_ASSERT(MDY_CHECK_ISNOTEMPTY(this->mShadowRenderer), "Shadow object must be valid if shadow feature is enabled.");
  return this->mShadowRenderer->IsAvailableDirectionalLightShadow();
}

EDySuccess MDyRendering::pfUpdateDirectionalLightShadowToGpu(const CDyDirectionalLight& component)
{ // Integrity test
  if (this->mIsEnabledShadowRendering == false)          { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mShadowRenderer))   { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mSceneFinalRenderer)) { return DY_FAILURE; }

  // Update values
#ifdef false
  MDY_NOTUSED
  const auto flag1 = this->mShadowRenderer->UpdateDirectionalLightShadowToGpu(component);
  MDY_NOT_IMPLEMENTED_ASSERT();
#endif
  return DY_SUCCESS;
}

EDySuccess MDyRendering::pfUnbindDirectionalLightShadowToGpu(const CDyDirectionalLight& component)
{ // Integrity test
  if (this->mIsEnabledShadowRendering == false)          { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mShadowRenderer))   { return DY_FAILURE; }
  if (MDY_CHECK_ISEMPTY(this->mSceneFinalRenderer)) { return DY_FAILURE; }

  // Update values
  MDY_NOTUSED
  const auto flag1 = this->mShadowRenderer->UnbindDirectionalLightShadowToGpu(component);
  MDY_NOT_IMPLEMENTED_ASSERT();
  return DY_SUCCESS;
}

} /// ::dy namespace