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
#include <Dy/Core/Thread/IO/DIOReferenceInstance.h>
#include <Dy/Core/Resource/Type/IBinderBase.h>

namespace dy
{

void DIOReferenceInstance::AttachBinder(IBinderBase& iRefBase) noexcept
{
  MDY_SYNC_LOCK_GUARD(mContainerMutex);
  this->mInternalHandler.AttachHandle(iRefBase.mHandle);
}

void DIOReferenceInstance::DetachBinder(IBinderBase& iRefBase) noexcept
{
  MDY_SYNC_LOCK_GUARD(mContainerMutex);
  this->mInternalHandler.DetachHandle(iRefBase.mHandle);
}

void DIOReferenceInstance::SetValid(void*& iPtrInstance) noexcept
{
  if (this->mIsResourceValid == true) { return; }

  this->mIsResourceValid  = true;
  this->mPtrInstance      = iPtrInstance;

  {
    MDY_SYNC_LOCK_GUARD(this->mContainerMutex);
    for (const auto& ptrHandle : this->mInternalHandler)
    {
      if (ptrHandle == nullptr) { continue; }

      auto* ptrBinder = static_cast<IBinderBase*>(ptrHandle->GetUserData());
      ptrBinder->TryUpdateResourcePtr(this->mPtrInstance);
    }
  }
}

void DIOReferenceInstance::SetNotValid() noexcept
{
  if (this->mIsResourceValid == false) { return; }

  this->mIsResourceValid  = false;
  this->mPtrInstance      = nullptr;

  {
    MDY_SYNC_LOCK_GUARD(this->mContainerMutex);
    for (const auto& ptrHandle : this->mInternalHandler)
    {
      if (ptrHandle == nullptr) { continue; }

      auto* ptrBinder = static_cast<IBinderBase*>(ptrHandle->GetUserData());
      ptrBinder->TryDetachResourcePtr();
    }
  }
}

bool DIOReferenceInstance::HaveToBeGCed() const noexcept
{
  if (this->mIsResourceValid == true
  &&  this->mScope == EResourceScope::Temporal 
  &&  this->mInternalHandler.IsBeingBinded() == false) 
  { 
    return true; 
  }

  return false;
}

bool DIOReferenceInstance::IsBeingBound() const noexcept
{
  return this->mInternalHandler.IsBeingBinded();
}

} /// ::dy namespace