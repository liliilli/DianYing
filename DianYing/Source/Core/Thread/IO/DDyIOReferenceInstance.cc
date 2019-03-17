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
#include <Dy/Core/Thread/IO/DDyIOReferenceInstance.h>
#include <Dy/Helper/System/Idioms.h>
#include <Dy/Core/Resource/Type/IBinderBase.h>

namespace dy
{

void DDyIOReferenceInstance::AttachBinder(IBinderBase* iPtrBase) noexcept
{
  MDY_SYNC_LOCK_GUARD(mContainerMutex);
  this->mPtrBoundBinderList.emplace(iPtrBase);
}

void DDyIOReferenceInstance::DetachBinder(IBinderBase* iPtrBase) noexcept
{
  MDY_SYNC_LOCK_GUARD(mContainerMutex);
  this->mPtrBoundBinderList.erase(iPtrBase);
}

void DDyIOReferenceInstance::SetValid(void*& iPtrInstance) noexcept
{
  this->mIsResourceValid  = true;
  this->mPtrInstance      = iPtrInstance;
}

void DDyIOReferenceInstance::SetNotValid() noexcept
{
  this->mIsResourceValid  = false;
  this->mPtrInstance      = nullptr;
}

bool DDyIOReferenceInstance::HaveToBeGCed() const noexcept
{
  if (this->mIsResourceValid == true
  &&  this->mScope == EResourceScope::Temporal 
  &&  this->mPtrBoundBinderList.empty() == true) { return true; }
  else { return false; }
}

} /// ::dy namespace