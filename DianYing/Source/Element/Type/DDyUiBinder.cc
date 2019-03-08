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
#include <Dy/Element/Type/DDyUiBinder.h>
#include <Dy/Element/Canvas/Widget.h>

namespace dy
{

DDyUiBinder::DDyUiBinder(_MIN_ FDyUiObject& bindObject)
{
  this->mPtrUiObject = &bindObject;
  this->pAttachBinderToUiObject();
}

DDyUiBinder::~DDyUiBinder() noexcept
{
  if (this->IsUiObjectValid() == false) { return; }
  // Detach it from UI Object.
  pDetachBinderFromUiObject();
}

DDyUiBinder::DDyUiBinder(const DDyUiBinder& mRefBinder) noexcept
{
  auto* fromPtr = mRefBinder.Get();
  if (fromPtr != nullptr) 
  {
    this->mPtrUiObject = const_cast<FDyUiObject*>(fromPtr);
    this->pAttachBinderToUiObject();
  }
}

DDyUiBinder& DDyUiBinder::operator=(const DDyUiBinder& mRefBinder) noexcept
{
  if (this == &mRefBinder) { return *this; }
  if (this->IsUiObjectValid() == true) { this->pDetachBinderFromUiObject(); }

  auto* fromPtr = mRefBinder.Get();
  if (fromPtr == nullptr) { this->mPtrUiObject = nullptr; }
  else
  {
    this->mPtrUiObject = const_cast<FDyUiObject*>(fromPtr);
    this->pAttachBinderToUiObject();
  }

  return *this;
}

void DDyUiBinder::MDY_PRIVATE(DetachUiObjectFromUiObject)()
{
  this->mPtrUiObject = nullptr;
}

void DDyUiBinder::pAttachBinderToUiObject()
{
  MDY_ASSERT_MSG_FORCE(this->mPtrUiObject != nullptr, "Ptr Ui Object must be valid when attaching.");
  auto& rootWidget = static_cast<FDyUiWidget&>(*this->mPtrUiObject);

  rootWidget.MDY_PRIVATE(TryAttachBinderFromBinder)(*this);
}

void DDyUiBinder::pDetachBinderFromUiObject()
{
  MDY_ASSERT_MSG_FORCE(this->mPtrUiObject != nullptr, "Ptr Ui Object must be valid when detaching.");
  auto& rootWidget = static_cast<FDyUiWidget&>(*this->mPtrUiObject);

  rootWidget.MDY_PRIVATE(TryDetachBinderFromBinder)(*this);
  this->mPtrUiObject = nullptr;
}

} /// ::dy namespace