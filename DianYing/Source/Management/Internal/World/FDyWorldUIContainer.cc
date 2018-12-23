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
#include <Dy/Management/Internal/World/FDyWorldUIContainer.h>
#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/ScriptManager.h>

namespace dy
{

EDySuccess FDyWorldUIContainer::TryCreateDebugUi()
{
  if (MDY_CHECK_ISNOTEMPTY(this->mDebugUi)) { return DY_FAILURE; }

  this->mDebugUi = std::make_unique<FDyUiWidget>(MDyMetaInfo::GetInstance().GetWidgetMetaInformation("DebugUi"));

  // CALL `Initiate()`
  MDyScript::GetInstance().UpdateWidgetScript(0.0f, EDyScriptState::CalledNothing);
  MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

  this->mDebugUi->SetPropagateMode(true, EDySearchMode::Recursive);
  this->mDebugUi->TryPropagatePositionToChildren();
  return DY_SUCCESS;
}

EDySuccess FDyWorldUIContainer::TryRemoveDebugUi()
{
  if (MDY_CHECK_ISEMPTY(this->mDebugUi)) { return DY_FAILURE; }

  this->mDebugUi = nullptr;
  return DY_SUCCESS;
}

EDySuccess FDyWorldUIContainer::TryCreateLoadingUi()
{
  auto& metaManager = MDyMetaInfo::GetInstance();
  if (metaManager.IsLoadingWidgetMetaInfoExist() == false)
  {
    MDY_LOG_WARNING("Loading widget is not exist, loading screen will not be presented.");
    return DY_FAILURE;
  }

  // CALL `Initiate()`
  MDyScript::GetInstance().UpdateWidgetScript(0.0f, EDyScriptState::CalledNothing);
  MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

  this->mLoadingUi = std::make_unique<FDyUiWidget>(*MDyMetaInfo::GetInstance().MDY_PRIVATE_SPECIFIER(TryGetLoadingWidgetMetaLoading)());
  this->mLoadingUi->SetPropagateMode(true, EDySearchMode::Recursive);
  this->mLoadingUi->TryPropagatePositionToChildren();
  return DY_SUCCESS;
}

EDySuccess FDyWorldUIContainer::TryRemoveLoadingUi()
{
  if (MDY_CHECK_ISEMPTY(this->mLoadingUi)) { return DY_FAILURE; }

  this->mLoadingUi = nullptr;
  return DY_SUCCESS;
}

void FDyWorldUIContainer::TryRenderDebugUi()
{
  if (MDY_CHECK_ISEMPTY(this->mDebugUi)) { return; }
  this->mDebugUi->Render();
}

void FDyWorldUIContainer::TryRenderLoadingUi()
{
  if (MDY_CHECK_ISEMPTY(this->mLoadingUi)) { return; }
  this->mLoadingUi->Render();
}

bool FDyWorldUIContainer::IsDebugUiExist() const noexcept { return MDY_CHECK_ISNOTEMPTY(this->mDebugUi); }
bool FDyWorldUIContainer::IsLoadingUiExist() const noexcept { return MDY_CHECK_ISNOTEMPTY(this->mLoadingUi); }

} /// ::dy namespace