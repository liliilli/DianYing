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
#include <Dy/Element/Widget/AWidgetObject.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Element/Widget/AWidgetUiObjectChildrenable.h>

namespace dy
{

void AWidgetObject::SetParentUiObject(AWidgetUiObjectChildrenable& parent) noexcept
{
  this->mPtrParentUiObject = &parent;
  this->UpdateFinalPosition();
  this->SetupFlagAsParent(this->mPtrParentUiObject->IsActivated());
}

bool AWidgetObject::CheckIsParentExist() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrParentUiObject);
}

const AWidgetUiObjectChildrenable* AWidgetObject::GetPtrParentUiObject() const noexcept
{
  return this->mPtrParentUiObject;  
}

void AWidgetObject::SetRelativePosition(const DVector2& position) noexcept
{
  this->mCentralRelativePosition = position;
  this->UpdateFinalPosition();
}

void AWidgetObject::SetFrameSize(const DVectorInt2& size) noexcept
{
  if (size.X > 0 && size.Y > 0) { this->mFrameSize = size; }
  this->UpdateFinalPosition();
}

const DVectorInt2& AWidgetObject::GetFrameSize() const noexcept
{
  return this->mFrameSize;
}

void AWidgetObject::SetOrigin(EDyOrigin iOrigin) noexcept
{
  this->mOrigin = iOrigin;
  this->UpdateFinalPosition();
}

EDyOrigin AWidgetObject::GetOrigin() const noexcept 
{
  return this->mOrigin; 
}

void AWidgetObject::SetFibot(EDyOrigin iFibot) noexcept
{
  this->mFibot = iFibot;
  this->UpdateFinalPosition();
}

EDyOrigin AWidgetObject::GetFibot() const noexcept  
{ 
  return this->mFibot; 
}

void AWidgetObject::UpdateFinalPosition()
{
  DVector2 mParentFinalPosition = {};
  if (this->CheckIsParentExist() == true)
  { // If parent is exist, retrieve values.
    const auto* ptrParent = this->GetPtrParentUiObject();
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrParent), "Parent must be valid.");
    mParentFinalPosition   = ptrParent->GetFinalPosition(this->GetOrigin());
  }

  this->mCentralFinalPosition = mParentFinalPosition + this->GetRelativePosition(this->GetFibot());
}

const std::string& AWidgetObject::GetUiObjectName() const noexcept
{
  MDY_ASSERT_MSG(this->pGetObjectName().empty() == false, "Unexpected error occurred.");
  return this->pGetObjectName();
}

DVector2 AWidgetObject::GetRelativePosition(const EDyOrigin& origin) const noexcept
{
  return DyGetPositionWithOrigin(this->mCentralRelativePosition, this->mFrameSize, origin);
}

DVector2 AWidgetObject::GetFinalPosition(const EDyOrigin& origin) const noexcept
{
  return DyGetPositionWithOrigin(this->mCentralFinalPosition, this->mFrameSize, origin);
}

} /// ::dy namespace