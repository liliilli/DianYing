#ifndef GUARD_DY_HELPER_INTERNAL_FHANDLETMP_INL
#define GUARD_DY_HELPER_INTERNAL_FHANDLETMP_INL
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

#include <Dy/Helper/Internal/FHandle.h>
#include <Dy/Helper/Internal/FHandler.h>

namespace dy
{

template <typename TType>
FHandle<TType>::~FHandle()
{
  this->Reset();
}

template <typename TType>
FHandle<TType>::FHandle(const FHandle& iHandle)
  : mHandlerAddress{iHandle.mHandlerAddress}
{
  // Attach to handler.
  if (this->IsBinding() == true) { this->mHandlerAddress->AttachHandle(*this); }
}

template <typename TType>
FHandle<TType>& FHandle<TType>::operator=(const FHandle& iHandle)
{
  if (this == &iHandle) { return *this; }
  if (this->mHandlerAddress == iHandle.mHandlerAddress) { return *this; }

  // If this instance binds something, detach from this, at first.
  this->Reset();

  // Attach.
  this->mHandlerAddress = iHandle.mHandlerAddress;
  if (this->IsBinding() == true) { this->mHandlerAddress->AttachHandle(*this); }

  return *this;
}

template <typename TType>
FHandle<TType>::FHandle(FHandle&& iHandle) noexcept 
{
  if (iHandle.IsBinding() == true)
  {
    this->mHandlerAddress = iHandle.mHandlerAddress;
    iHandle.Reset();
    
    if (this->IsBinding() == true) { this->mHandlerAddress->AttachHandle(*this); }
  }
}

template <typename TType>
FHandle<TType>& FHandle<TType>::operator=(FHandle&& iHandle) noexcept
{
  if (this == &iHandle) { return *this; }
  if (this->mHandlerAddress == iHandle.mHandlerAddress) 
  { 
    iHandle.Reset();
  }
  else
  {
    this->Reset();

    this->mHandlerAddress = iHandle.mHandlerAddress;
    iHandle.Reset();

    if (this->IsBinding() == true) { this->mHandlerAddress->AttachHandle(*this); }
  }

  return *this;
}

template <typename TType>
bool FHandle<TType>::IsBinding() const noexcept
{
  return this->mHandlerAddress != nullptr;
}

template <typename TType>
void FHandle<TType>::Reset()
{
  // Detach from handler.
  if (this->IsBinding() == true)
  {
    this->mHandlerAddress->DetachHandle(*this);
    this->mHandlerAddress = nullptr;
  }
}

template <typename TType>
void FHandle<TType>::SetUserData(void* iPtr)
{
  this->mData = iPtr;
}

template <typename TType>
void* FHandle<TType>::GetUserData() const noexcept
{
  return this->mData;
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FHANDLETMP_INL