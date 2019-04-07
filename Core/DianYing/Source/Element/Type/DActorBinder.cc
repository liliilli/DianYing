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
#include <Dy/Element/Type/DActorBinder.h>
#include <Dy/Element/Type/PActorCreationDescriptor.h>
#include <Dy/Element/FActor.h>

namespace dy
{

DActorBinder::~DActorBinder() noexcept
{
  if (this->mPtrActor != nullptr) 
  { 
    this->mPtrActor->MDY_PRIVATE(DetachBinderFromList)(*this); 
    this->mPtrActor = nullptr;
  }

  if (this->mPtrCreationDesc != nullptr) 
  { 
    this->mPtrCreationDesc->MDY_PRIVATE(DetachBinderFromList)(*this); 
    this->mPtrCreationDesc = nullptr;
  }
}

DActorBinder::DActorBinder(const DActorBinder& mRefBinder) noexcept 
  : mPtrActor {mRefBinder.mPtrActor},
    mPtrCreationDesc {mRefBinder.mPtrCreationDesc}
{
  if (this->mPtrActor != nullptr)
  { 
    this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this); 
  }

  if (this->mPtrCreationDesc != nullptr)
  { 
    this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this); 
  }
}

DActorBinder& DActorBinder::operator=(const DActorBinder& mRefBinder) noexcept
{
  if (this == &mRefBinder) { return *this; }
  if (this->mPtrActor != mRefBinder.mPtrActor)
  {
    if (this->mPtrActor != nullptr) 
    { 
      this->mPtrActor->MDY_PRIVATE(DetachBinderFromList)(*this); 
      this->mPtrActor = nullptr;
    }

    this->mPtrActor = mRefBinder.mPtrActor;
    if (this->mPtrActor != nullptr)
    { 
      this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this); 
    }
  }

  if (this->mPtrCreationDesc != mRefBinder.mPtrCreationDesc)
  {
    if (this->mPtrCreationDesc != nullptr) 
    { 
      this->mPtrCreationDesc->MDY_PRIVATE(DetachBinderFromList)(*this); 
      this->mPtrCreationDesc = nullptr;
    }

    this->mPtrCreationDesc = mRefBinder.mPtrCreationDesc;
    if (this->mPtrCreationDesc != nullptr)
    { 
      this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this); 
    }
  }

  return *this;
}

bool DActorBinder::IsActorCreated() const noexcept
{
  return this->mPtrActor != nullptr;
}

void DActorBinder::MDY_PRIVATE(BindDescriptor)(PActorCreationDescriptor* mPtrDesc) noexcept
{
  if (this->mPtrCreationDesc == mPtrDesc) { return; }

  if (this->mPtrCreationDesc != nullptr)
  {
    this->mPtrCreationDesc->MDY_PRIVATE(DetachBinderFromList)(*this);
    this->mPtrCreationDesc = nullptr;
  }

  this->mPtrCreationDesc = mPtrDesc;
  if (this->mPtrCreationDesc != nullptr)
  {
    this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this);
  }
}

void DActorBinder::MDY_PRIVATE(DetachDescriptor)(PActorCreationDescriptor*) noexcept
{
  if (this->mPtrCreationDesc != nullptr)
  {
    this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this);
    this->mPtrCreationDesc = nullptr;
  }
}

void DActorBinder::MDY_PRIVATE(BindDescriptor)(FActor* mPtrDesc) noexcept
{
  if (this->mPtrActor == mPtrDesc) { return; }

  if (this->mPtrActor != nullptr)
  {
    this->mPtrActor->MDY_PRIVATE(DetachBinderFromList)(*this);
    this->mPtrActor = nullptr;
  }

  this->mPtrActor = mPtrDesc;
  if (this->mPtrActor != nullptr)
  {
    this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this);
  }
}

void DActorBinder::MDY_PRIVATE(DetachDescriptor)(FActor*) noexcept
{
  if (this->mPtrActor != nullptr)
  {
    this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this);
    this->mPtrActor = nullptr;
  }
}


} /// ::dy namespace