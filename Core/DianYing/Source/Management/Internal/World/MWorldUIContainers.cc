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
#include <Dy/Management/Internal/World/MWorldUIContainers.h>
#include <Dy/Element/Widget/FWidget.h>
#include <Dy/Management/IO/MIOMeta.h>
#include <Dy/Management/MScript.h>
#include <Dy/Element/Type/DWidgetBinder.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Helper/Library/HelperContainer.h>
#include <Dy/Management/MLog.h>

namespace dy
{

EDySuccess MWorldUIContainers::TryCreateDebugUi()
{
  if (MDY_CHECK_ISNOTEMPTY(this->mDebugUi)) { return EDySuccess::DY_FAILURE; }

  this->mDebugUi = std::make_unique<FWidget>(MIOMeta::GetInstance().GetWidgetMetaInformation("DebugUi"));

  // CALL `Initiate()`
  MScript::GetInstance().UpdateWidgetScript(0.0f, EScriptState::CalledNothing);
  MScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

  this->mDebugUi->SetPropagateMode(true, EDySearchMode::Recursive);
  this->mDebugUi->TryPropagatePositionToChildren();
  this->mDebugUi->SetupFlagAsParent(true);
  return EDySuccess::DY_SUCCESS;
}

FWidget* MWorldUIContainers::GetPtrDebugUi() const noexcept
{
  return this->mDebugUi.get();
}

EDySuccess MWorldUIContainers::TryRemoveDebugUi()
{
  if (MDY_CHECK_ISEMPTY(this->mDebugUi)) { return EDySuccess::DY_FAILURE; }

  this->mDebugUi = nullptr;
  return EDySuccess::DY_SUCCESS;
}

EDySuccess MWorldUIContainers::TryCreateLoadingUi()
{
  auto& metaManager = MIOMeta::GetInstance();
  if (metaManager.IsLoadingWidgetMetaInfoExist() == false)
  {
    DyPushLogWarning("Loading widget is not exist, loading screen will not be presented.");
    return EDySuccess::DY_FAILURE;
  }

  // CALL `Initiate()`
  MScript::GetInstance().UpdateWidgetScript(0.0f, EScriptState::CalledNothing);
  MScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

  this->mLoadingUi = std::make_unique<FWidget>(*MIOMeta::GetInstance().MDY_PRIVATE(TryGetLoadingWidgetMetaLoading)());
  this->mLoadingUi->SetPropagateMode(true, EDySearchMode::Recursive);
  this->mLoadingUi->TryPropagatePositionToChildren();
  this->mLoadingUi->SetupFlagAsParent(true);
  return EDySuccess::DY_SUCCESS;
}

FWidget* MWorldUIContainers::GetPtrLoadingUi() const noexcept
{
  return this->mLoadingUi.get();
}

EDySuccess MWorldUIContainers::TryRemoveLoadingUi()
{
  if (MDY_CHECK_ISEMPTY(this->mLoadingUi)) { return EDySuccess::DY_FAILURE; }

  this->mLoadingUi = nullptr;
  return EDySuccess::DY_SUCCESS;
}

bool MWorldUIContainers::IsUiObjectExist(_MIN_ const std::string& iUiObjectName) const noexcept
{
  return Contains(this->mGeneralUiWidgetMap, iUiObjectName);
}

DWidgetBinder MWorldUIContainers::CreateUiObject(
    _MIN_ const std::string& iUiName, 
    _MIN_ const PDyMetaWidgetRootDescriptor& iRoot,
    _MIN_ TU32 ZOrder)
{
  auto [it, isSuccessful] = this->mGeneralUiWidgetMap.try_emplace(
      iUiName, std::make_unique<FWidget>(iRoot));
  MDY_ASSERT_MSG_FORCE(isSuccessful == true, "Unexpected error occurred.");

  auto& [key, object] = *it;
  object->SetPropagateMode(true, EDySearchMode::Recursive);
  object->TryPropagatePositionToChildren();

  object->MDY_PRIVATE(SetName)(key);
  object->mZOrder = ZOrder;
  object->SetupFlagAsParent(true);

  MScript::GetInstance().UpdateWidgetScript(0.0f, EScriptState::CalledNothing);
  MScript::GetInstance().TryMoveInsertWidgetScriptToMainContainer();

  return DWidgetBinder{*object};
}

std::optional<DWidgetBinder> MWorldUIContainers::GetUiObject(const std::string& iUiName)
{
  const auto it = this->mGeneralUiWidgetMap.find(iUiName);
  if (it == this->mGeneralUiWidgetMap.end())
  {
    DyPushLogError("Failed to get {} Ui Object. Could not found matched UI Object.", iUiName);
    return std::nullopt;
  }

  return DWidgetBinder{*it->second};
}

EDySuccess MWorldUIContainers::RemoveUiObject(_MIN_ const std::string& iUiName)
{
  const auto it = this->mGeneralUiWidgetMap.find(iUiName);
  if (it == this->mGeneralUiWidgetMap.end())
  {
    DyPushLogError("Failed to remove {} Ui Object. Could not found matched UI Object.", iUiName);
    return EDySuccess::DY_FAILURE;
  }

  this->mGeneralUiWidgetMap.erase(it);
  return EDySuccess::DY_SUCCESS;
}

void MWorldUIContainers::ClearGeneralUiObjectList()
{
  this->mGeneralUiWidgetMap.clear();
}

std::vector<NotNull<FWidget*>>& MWorldUIContainers::GetActivatedUiWidgetList() noexcept
{
  return this->mPtrActivatedGeneralUiWidgetList;
}

void MWorldUIContainers::BindActiveUiObject(_MIN_ FWidget& iRefWidget)
{
  this->mPtrActivatedGeneralUiWidgetList.emplace_back(DyMakeNotNull(&iRefWidget));
}

EDySuccess MWorldUIContainers::UnbindActiveUiObject(_MIN_ FWidget& iRefWidget)
{
  const auto it = std::find_if(
      MDY_BIND_BEGIN_END(this->mPtrActivatedGeneralUiWidgetList), 
      [ptr = &iRefWidget](const auto& ptrWidget) { return ptrWidget == ptr; });
  if (it == this->mPtrActivatedGeneralUiWidgetList.end()) { return EDySuccess::DY_FAILURE; }
  
  FaseErase(this->mPtrActivatedGeneralUiWidgetList, it);
  return EDySuccess::DY_SUCCESS;
}

bool MWorldUIContainers::IsDebugUiExist() const noexcept { return MDY_CHECK_ISNOTEMPTY(this->mDebugUi); }
bool MWorldUIContainers::IsLoadingUiExist() const noexcept { return MDY_CHECK_ISNOTEMPTY(this->mLoadingUi); }

} /// ::dy namespace