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
#include <Dy/Element/Widget/AWidgetUiObjectChildrenable.h>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

void AWidgetUiObjectChildrenable::Render()
{
  if (this->IsActivated() == false) { return; }

  for (auto& uiObject : this->mWidgetObjectList)
  {
    if (uiObject == nullptr) { continue; }
    if (uiObject->IsActivated() == false) { continue; }

    uiObject->Render();
  }

  for (auto& uiGroupObject : this->mUiChildrenableObjectList)
  {
    if (uiGroupObject == nullptr) { continue; }
    if (uiGroupObject->IsActivated() == false) { continue; }

    uiGroupObject->Render();
  }
}

void AWidgetUiObjectChildrenable::PropagateActivationFlag(bool iParentFlag)
{
  for (auto& uiObject : this->mWidgetObjectList)
  {
    if (uiObject == nullptr) { continue; }
    uiObject->SetupFlagAsParent(iParentFlag);
  }

  for (auto& uiGroupObject : this->mUiChildrenableObjectList)
  {
    if (uiGroupObject == nullptr) { continue; }
    uiGroupObject->PropagateActivationFlag(iParentFlag);
  }
}

void AWidgetUiObjectChildrenable::TryActivateInstance()
{
  this->PropagateActivationFlag(true);
}

void AWidgetUiObjectChildrenable::TryDeactivateInstance()
{
  this->PropagateActivationFlag(false);
}

void AWidgetUiObjectChildrenable::SetRelativePosition(const DVec2& position) noexcept
{
  AWidgetObject::SetRelativePosition(position);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

void AWidgetUiObjectChildrenable::SetFrameSize(const DIVec2& size) noexcept
{
  AWidgetObject::SetFrameSize(size);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

void AWidgetUiObjectChildrenable::SetOrigin(EDyOrigin iOrigin) noexcept
{
  AWidgetObject::SetOrigin(iOrigin);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

void AWidgetUiObjectChildrenable::SetFibot(EDyOrigin iOrigin) noexcept
{
  AWidgetObject::SetFibot(iOrigin);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

bool AWidgetUiObjectChildrenable::CheckIsPropagable() const noexcept
{
  return this->mIsCanPropagatePosition;
}

void AWidgetUiObjectChildrenable::SetPropagateMode(bool isEnabled, EDySearchMode mode)
{
  this->mIsCanPropagatePosition = isEnabled;
  if (mode == EDySearchMode::Recursive)
  {
    for (auto& ptrsmtChildrenableObject : this->mUiChildrenableObjectList)
    {
      if (MDY_CHECK_ISEMPTY(ptrsmtChildrenableObject)) { continue; }
      ptrsmtChildrenableObject->SetPropagateMode(isEnabled, mode);
    }
  }
}

void AWidgetUiObjectChildrenable::TryPropagatePositionToChildren()
{
  if (this->CheckIsPropagable() == false)   { return; }
  for (auto& ptrsmtUiReafObject : this->mWidgetObjectList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtUiReafObject)) { continue; }
    ptrsmtUiReafObject->UpdateFinalPosition();
  }

  for (auto& ptrsmtUiSpawnableObject : this->mUiChildrenableObjectList)
  {
    if (MDY_CHECK_ISEMPTY(ptrsmtUiSpawnableObject)) { continue; }
    ptrsmtUiSpawnableObject->TryPropagatePositionToChildren();
  }
}

AWidgetObject* AWidgetUiObjectChildrenable::GetWidget(const std::string& objectName, EDySearchMode searchMode)
{
  if (searchMode == EDySearchMode::Default)
  {
    for (std::unique_ptr<AWidgetObject>& objectPtr : this->mWidgetObjectList)
    {
      if (MDY_CHECK_ISEMPTY(objectPtr)) { continue; }
      if (objectPtr->GetUiObjectName() == objectName) { return objectPtr.get(); }
    }

    // FAILED
    return nullptr;
  }
  else
  {
    MDY_NOT_IMPLEMENTED_ASSERT();
    return nullptr;
  }
}

} /// ::dy namespace
