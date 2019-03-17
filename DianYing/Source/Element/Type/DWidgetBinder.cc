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
#include <Dy/Element/Type/DWidgetBinder.h>
#include <Dy/Element/Widget/FWidget.h>

namespace dy
{

DWidgetBinder::DWidgetBinder(_MIN_ AWidgetObject& bindObject)
{
  this->mPtrUiObject = &bindObject;
  this->pAttachBinderToUiObject();
}

DWidgetBinder::~DWidgetBinder() noexcept
{
  if (this->IsUiObjectValid() == false) { return; }
  // Detach it from UI Object.
  pDetachBinderFromUiObject();
}

DWidgetBinder::DWidgetBinder(const DWidgetBinder& mRefBinder) noexcept
{
  auto* fromPtr = mRefBinder.Get();
  if (fromPtr != nullptr) 
  {
    this->mPtrUiObject = const_cast<AWidgetObject*>(fromPtr);
    this->pAttachBinderToUiObject();
  }
}

DWidgetBinder& DWidgetBinder::operator=(const DWidgetBinder& mRefBinder) noexcept
{
  if (this == &mRefBinder) { return *this; }
  if (this->IsUiObjectValid() == true) { this->pDetachBinderFromUiObject(); }

  auto* fromPtr = mRefBinder.Get();
  if (fromPtr == nullptr) { this->mPtrUiObject = nullptr; }
  else
  {
    this->mPtrUiObject = const_cast<AWidgetObject*>(fromPtr);
    this->pAttachBinderToUiObject();
  }

  return *this;
}

void DWidgetBinder::MDY_PRIVATE(DetachUiObjectFromUiObject)()
{
  this->mPtrUiObject = nullptr;
}

void DWidgetBinder::pAttachBinderToUiObject()
{
  MDY_ASSERT_MSG_FORCE(this->mPtrUiObject != nullptr, "Ptr Ui Object must be valid when attaching.");
  auto& rootWidget = static_cast<FWidget&>(*this->mPtrUiObject);

  rootWidget.MDY_PRIVATE(TryAttachBinderFromBinder)(*this);
}

void DWidgetBinder::pDetachBinderFromUiObject()
{
  MDY_ASSERT_MSG_FORCE(this->mPtrUiObject != nullptr, "Ptr Ui Object must be valid when detaching.");
  auto& rootWidget = static_cast<FWidget&>(*this->mPtrUiObject);

  rootWidget.MDY_PRIVATE(TryDetachBinderFromBinder)(*this);
  this->mPtrUiObject = nullptr;
}

} /// ::dy namespace