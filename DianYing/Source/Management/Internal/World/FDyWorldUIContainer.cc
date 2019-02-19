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
#include <Dy/Management/Internal/World/FDyWorldUIContainer.h>
#include <Dy/Element/Canvas/Widget.h>
#include <Dy/Management/IO/MetaInfoManager.h>
#include <Dy/Management/ScriptManager.h>
#include <Dy/Element/Type/DDyUiBinder.h>

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

FDyUiWidget* FDyWorldUIContainer::GetPtrDebugUi() const noexcept
{
  return this->mDebugUi.get();
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

  this->mLoadingUi = std::make_unique<FDyUiWidget>(*MDyMetaInfo::GetInstance().MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)());
  this->mLoadingUi->SetPropagateMode(true, EDySearchMode::Recursive);
  this->mLoadingUi->TryPropagatePositionToChildren();
  return DY_SUCCESS;
}

FDyUiWidget* FDyWorldUIContainer::GetPtrLoadingUi() const noexcept
{
  return this->mLoadingUi.get();
}

EDySuccess FDyWorldUIContainer::TryRemoveLoadingUi()
{
  if (MDY_CHECK_ISEMPTY(this->mLoadingUi)) { return DY_FAILURE; }

  this->mLoadingUi = nullptr;
  return DY_SUCCESS;
}

bool FDyWorldUIContainer::IsUiObjectExist(_MIN_ const std::string& iUiObjectName) const noexcept
{
  return DyIsMapContains(this->mGeneralUiWidgetMap, iUiObjectName);
}

DDyUiBinder FDyWorldUIContainer::CreateUiObject(
    _MIN_ const std::string& iUiName, 
    _MIN_ const PDyMetaWidgetRootDescriptor& iRoot,
    _MIN_ TU32 ZOrder)
{
  auto [it, isSuccessful] = this->mGeneralUiWidgetMap.try_emplace(
      iUiName, std::make_unique<FDyUiWidget>(iRoot));
  MDY_ASSERT_FORCE(isSuccessful == true, "Unexpected error occurred.");

  auto& [key, object] = *it;
  object->SetPropagateMode(true, EDySearchMode::Recursive);
  object->TryPropagatePositionToChildren();

  object->MDY_PRIVATE(SetName)(key);
  object->mZOrder = ZOrder;

  MDyScript::GetInstance().UpdateWidgetScript(0.0f, EDyScriptState::CalledNothing);
  MDyScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

  return DDyUiBinder{*object};
}

EDySuccess FDyWorldUIContainer::RemoveUiObject(_MIN_ const std::string& iUiName)
{
  const auto it = this->mGeneralUiWidgetMap.find(iUiName);
  if (it == this->mGeneralUiWidgetMap.end())
  {
    MDY_LOG_ERROR("Failed to remove {} Ui Object. Could not found matched UI Object.", iUiName);
    return DY_FAILURE;
  }

  this->mGeneralUiWidgetMap.erase(it);
  return DY_SUCCESS;
}

std::vector<NotNull<FDyUiWidget*>>& FDyWorldUIContainer::GetActivatedUiWidgetList() noexcept
{
  return this->mPtrActivatedGeneralUiWidgetList;
}

bool FDyWorldUIContainer::IsDebugUiExist() const noexcept { return MDY_CHECK_ISNOTEMPTY(this->mDebugUi); }
bool FDyWorldUIContainer::IsLoadingUiExist() const noexcept { return MDY_CHECK_ISNOTEMPTY(this->mLoadingUi); }

} /// ::dy namespace