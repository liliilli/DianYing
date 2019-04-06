#ifndef GUARD_DY_HELPER_INTERNAL_FPROXYHANDLETMP_INL
#define GUARD_DY_HELPER_INTERNAL_FPROXYHANDLETMP_INL
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

#include <Dy/Helper/Internal/FProxyHandle.h>
#include <Dy/Helper/Internal/FProxyHandler.h>

namespace dy
{

template <typename TType>
FProxyHandle<TType>::~FProxyHandle()
{
  this->Reset();
}

template <typename TType>
FProxyHandle<TType>::FProxyHandle(const FProxyHandle& iHandle)
  : mHandlerAddress{iHandle.mHandlerAddress}
{
  // Attach to handler.
  if (this->IsBinding() == true) { this->mHandlerAddress->AttachHandle(*this); }
}

template <typename TType>
FProxyHandle<TType>& FProxyHandle<TType>::operator=(const FProxyHandle& iHandle)
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
FProxyHandle<TType>::FProxyHandle(FProxyHandle&& iHandle) noexcept 
{
  if (iHandle.IsBinding() == true)
  {
    this->mHandlerAddress = iHandle.mHandlerAddress;
    iHandle.Reset();
    
    if (this->IsBinding() == true) { this->mHandlerAddress->AttachHandle(*this); }
  }
}

template <typename TType>
FProxyHandle<TType>& FProxyHandle<TType>::operator=(FProxyHandle&& iHandle) noexcept
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
TType* FProxyHandle<TType>::operator->()
{
  return this->mHandlerAddress->mValue;
}

template <typename TType>
bool FProxyHandle<TType>::IsBinding() const noexcept
{
  return this->mHandlerAddress != nullptr;
}

template <typename TType>
void FProxyHandle<TType>::Reset()
{
  // Detach from handler.
  if (this->IsBinding() == true)
  {
    this->mHandlerAddress->DetachHandle(*this);
    this->mHandlerAddress = nullptr;
  }
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_FPROXYHANDLETMP_INL