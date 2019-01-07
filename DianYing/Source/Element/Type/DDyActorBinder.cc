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
#include <Dy/Element/Type/DDyActorBinder.h>
#include <Dy/Element/Type/PDyActorCreationDescriptor.h>
#include <Dy/Element/Actor.h>

namespace dy
{

DDyActorBinder::~DDyActorBinder() noexcept
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrActor)) 
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE_SPECIFIER(DetachBinderFromList)(*this)); }

  if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc)) 
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE_SPECIFIER(DetachBinderFromList)(*this)); }
}

DDyActorBinder::DDyActorBinder(_MIN_ const DDyActorBinder& mRefBinder) noexcept :
  mPtrActor {mRefBinder.mPtrActor},
  mPtrCreationDesc {mRefBinder.mPtrCreationDesc}
{
  if (MDY_CHECK_ISNOTNULL(this->mPtrActor))
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE_SPECIFIER(AttachBinderFromList)(*this)); }

  if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc))
  { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE_SPECIFIER(AttachBinderFromList)(*this)); }
}

DDyActorBinder& DDyActorBinder::operator=(_MIN_ const DDyActorBinder& mRefBinder) noexcept
{
  if (this == &mRefBinder) { return *this; }
  if (this->mPtrActor != mRefBinder.mPtrActor)
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrActor)) 
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE_SPECIFIER(DetachBinderFromList)(*this)); }
    this->mPtrActor = mRefBinder.mPtrActor;
    if (MDY_CHECK_ISNOTNULL(this->mPtrActor))
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE_SPECIFIER(AttachBinderFromList)(*this)); }
  }

  if (this->mPtrCreationDesc != mRefBinder.mPtrCreationDesc)
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc)) 
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE_SPECIFIER(DetachBinderFromList)(*this)); }
    this->mPtrCreationDesc = mRefBinder.mPtrCreationDesc;
    if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc))
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE_SPECIFIER(AttachBinderFromList)(*this)); }
  }

  return *this;
}

bool DDyActorBinder::IsActorCreated() const noexcept
{
  return MDY_CHECK_ISNOTNULL(this->mPtrActor);
}

void DDyActorBinder::MDY_PRIVATE_SPECIFIER(BindDescriptor)(_MIN_ PDyActorCreationDescriptor* mPtrDesc) noexcept
{
  this->mPtrCreationDesc = mPtrDesc;
  MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE_SPECIFIER(AttachBinderFromList)(*this));
}

void DDyActorBinder::MDY_PRIVATE_SPECIFIER(DetachDescriptor)(PDyActorCreationDescriptor*) noexcept
{
  this->mPtrCreationDesc = nullptr;
}

void DDyActorBinder::MDY_PRIVATE_SPECIFIER(BindDescriptor)(_MIN_ FDyActor* mPtrDesc) noexcept
{
  this->mPtrActor = mPtrDesc;
  MDY_CALL_ASSERT_SUCCESS(this->mPtrActor->MDY_PRIVATE_SPECIFIER(AttachBinderFromList)(*this));

  if (this->mPtrCreationDesc != nullptr)
  {
    if (MDY_CHECK_ISNOTNULL(this->mPtrCreationDesc)) 
    { MDY_CALL_ASSERT_SUCCESS(this->mPtrCreationDesc->MDY_PRIVATE_SPECIFIER(DetachBinderFromList)(*this)); }
    this->mPtrCreationDesc = nullptr;
  }
}

void DDyActorBinder::MDY_PRIVATE_SPECIFIER(DetachDescriptor)(FDyActor*) noexcept
{
  this->mPtrActor = nullptr;
}


} /// ::dy namespace