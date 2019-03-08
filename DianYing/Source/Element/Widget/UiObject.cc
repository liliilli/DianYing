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
#include <Dy/Element/Canvas/UiObject.h>
#include <Dy/Meta/Type/EDyWidgetTypes.h>
#include <Dy/Element/Canvas/UiObjectChildrenable.h>

namespace dy
{

void FDyUiObject::SetParentUiObject(_MIN_ FDyUiObjectChildrenable& parent) noexcept
{
  this->mPtrParentUiObject = &parent;
  this->UpdateFinalPosition();
}

bool FDyUiObject::CheckIsParentExist() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrParentUiObject);
}

const FDyUiObjectChildrenable* FDyUiObject::GetPtrParentUiObject() const noexcept
{
  return this->mPtrParentUiObject;  
}

void FDyUiObject::SetRelativePosition(_MIN_ const DDyVector2& position) noexcept
{
  this->mCentralRelativePosition = position;
  this->UpdateFinalPosition();
}

void FDyUiObject::SetFrameSize(_MIN_ const DDyVectorInt2& size) noexcept
{
  if (size.X > 0 && size.Y > 0) { this->mFrameSize = size; }
  this->UpdateFinalPosition();
}

const DDyVectorInt2& FDyUiObject::GetFrameSize() const noexcept
{
  return this->mFrameSize;
}

void FDyUiObject::SetOrigin(_MIN_ EDyOrigin iOrigin) noexcept
{
  this->mOrigin = iOrigin;
  this->UpdateFinalPosition();
}

EDyOrigin FDyUiObject::GetOrigin() const noexcept 
{
  return this->mOrigin; 
}

void FDyUiObject::SetFibot(_MIN_ EDyOrigin iFibot) noexcept
{
  this->mFibot = iFibot;
  this->UpdateFinalPosition();
}

EDyOrigin FDyUiObject::GetFibot() const noexcept  
{ 
  return this->mFibot; 
}

void FDyUiObject::UpdateFinalPosition()
{
  DDyVector2 mParentFinalPosition = {};
  if (this->CheckIsParentExist() == true)
  { // If parent is exist, retrieve values.
    const auto* ptrParent = this->GetPtrParentUiObject();
    MDY_ASSERT_MSG(MDY_CHECK_ISNOTNULL(ptrParent), "Parent must be valid.");
    mParentFinalPosition   = ptrParent->GetFinalPosition(this->GetOrigin());
  }

  this->mCentralFinalPosition = mParentFinalPosition + this->GetRelativePosition(this->GetFibot());
}

const std::string& FDyUiObject::GetUiObjectName() const noexcept
{
  MDY_ASSERT_MSG(this->pGetObjectName().empty() == false, "Unexpected error occurred.");
  return this->pGetObjectName();
}

DDyVector2 FDyUiObject::GetRelativePosition(const EDyOrigin& origin) const noexcept
{
  return DyGetPositionWithOrigin(this->mCentralRelativePosition, this->mFrameSize, origin);
}

DDyVector2 FDyUiObject::GetFinalPosition(const EDyOrigin& origin) const noexcept
{
  return DyGetPositionWithOrigin(this->mCentralFinalPosition, this->mFrameSize, origin);
}

} /// ::dy namespace