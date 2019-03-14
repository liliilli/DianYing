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
#include <Dy/Element/Canvas/UiObjectChildrenable.h>

namespace dy
{

void FDyUiObjectChildrenable::Render()
{
  for (auto& uiObject : this->mUiObjectList)
  {
    if (MDY_CHECK_ISEMPTY(uiObject) == true) { continue; }
    /// @TODO IMPLEMENT uiObject Is enabled or disabled.

    uiObject->Render();
  }
}

void FDyUiObjectChildrenable::SetRelativePosition(_MIN_ const DVector2& position) noexcept
{
  FDyUiObject::SetRelativePosition(position);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

void FDyUiObjectChildrenable::SetFrameSize(_MIN_ const DVectorInt2& size) noexcept
{
  FDyUiObject::SetFrameSize(size);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

void FDyUiObjectChildrenable::SetOrigin(_MIN_ EDyOrigin iOrigin) noexcept
{
  FDyUiObject::SetOrigin(iOrigin);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

void FDyUiObjectChildrenable::SetFibot(EDyOrigin iOrigin) noexcept
{
  FDyUiObject::SetFibot(iOrigin);
  if (this->CheckIsPropagable() == true) { this->TryPropagatePositionToChildren(); }
}

bool FDyUiObjectChildrenable::CheckIsPropagable() const noexcept
{
  return this->mIsCanPropagatePosition;
}

void FDyUiObjectChildrenable::SetPropagateMode(_MIN_ bool isEnabled, _MIN_ EDySearchMode mode)
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

void FDyUiObjectChildrenable::TryPropagatePositionToChildren()
{
  if (this->CheckIsPropagable() == false)   { return; }
  for (auto& ptrsmtUiReafObject : this->mUiObjectList)
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

FDyUiObject* FDyUiObjectChildrenable::GetUiObject(_MIN_ const std::string& objectName, _MIN_ EDySearchMode searchMode)
{
  if (searchMode == EDySearchMode::Default)
  {
    for (std::unique_ptr<FDyUiObject>& objectPtr : this->mUiObjectList)
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
