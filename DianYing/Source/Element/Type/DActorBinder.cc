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
  if (MDY_CHECK_ISNOTNULL(this->mPtrActor)) 
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE(DetachBinderFromList)(*this)); }

  if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc)) 
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE(DetachBinderFromList)(*this)); }
}

DActorBinder::DActorBinder(_MIN_ const DActorBinder& mRefBinder) noexcept :
  mPtrActor {mRefBinder.mPtrActor},
  mPtrCreationDesc {mRefBinder.mPtrCreationDesc}
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrActor))
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this)); }

  if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc))
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this)); }
}

DActorBinder& DActorBinder::operator=(_MIN_ const DActorBinder& mRefBinder) noexcept
{
  if (this == &mRefBinder) { return *this; }
  if (this->mPtrActor != mRefBinder.mPtrActor)
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrActor)) 
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE(DetachBinderFromList)(*this)); }
    this->mPtrActor = mRefBinder.mPtrActor;
    if (MDY_CHECK_ISNOTNULL(this->mPtrActor))
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this)); }
  }

  if (this->mPtrCreationDesc != mRefBinder.mPtrCreationDesc)
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc)) 
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE(DetachBinderFromList)(*this)); }
    this->mPtrCreationDesc = mRefBinder.mPtrCreationDesc;
    if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc))
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this)); }
  }

  return *this;
}

bool DActorBinder::IsActorCreated() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrActor);
}

void DActorBinder::MDY_PRIVATE(BindDescriptor)(_MIN_ PActorCreationDescriptor* mPtrDesc) noexcept
{
  this->mPtrCreationDesc = mPtrDesc;
  MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE(AttachBinderFromList)(*this));
}

void DActorBinder::MDY_PRIVATE(DetachDescriptor)(PActorCreationDescriptor*) noexcept
{
  this->mPtrCreationDesc = nullptr;
}

void DActorBinder::MDY_PRIVATE(BindDescriptor)(_MIN_ FActor* mPtrDesc) noexcept
{
  this->mPtrActor = mPtrDesc;
  MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE(AttachBinderFromList)(*this));

  if (this->mPtrCreationDesc != nullptr)
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc)) 
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE(DetachBinderFromList)(*this)); }
    this->mPtrCreationDesc = nullptr;
  }
}

void DActorBinder::MDY_PRIVATE(DetachDescriptor)(FActor*) noexcept
{
  this->mPtrActor = nullptr;
}


} /// ::dy namespace