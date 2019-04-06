#ifndef GUARD_DY_HELPER_INTERNAL_FPROXYHANDLERTMP_INL
#define GUARD_DY_HELPER_INTERNAL_FPROXYHANDLERTMP_INL
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

#include <Dy/Helper/Internal/FProxyHandler.h>
#include <Dy/Helper/Internal/FProxyHandle.h>

namespace dy
{

template <typename TType>
template <typename ... TArgs>
FProxyHandler<TType>::FProxyHandler(TArgs&&... iArgs)
  : mValue(new TType(std::forward<TArgs>(iArgs)...)) 
{ }

template <typename TType>
FProxyHandler<TType>::FProxyHandler(TType* iRawInstance)
  : mValue{iRawInstance} 
{ }

template <typename TType>
FProxyHandler<TType>::~FProxyHandler()
{
  if (this->mValue != nullptr)
  {
    delete this->mValue;
    this->mValue = nullptr;
  }

  for (auto& ptrHandle : this->mBindHandleList)
  {
    ptrHandle->mHandlerAddress = 0;
  }
  this->mBindHandleList.clear();
}

template <typename TType>
FProxyHandler<TType>::FProxyHandler(FProxyHandler&& iHandler) noexcept
{
  // If given handler value is not null (something is allocated)
  // and bind handle is exist, move handler list and let handle relocate handler location.
  this->pMoveHandler(std::move(iHandler));
}

template <typename TType>
FProxyHandler<TType>& FProxyHandler<TType>::operator=(FProxyHandler&& iHandler) noexcept
{
  if (this == &iHandler) { return *this; }

  // If overwritten, destroy value and handle binding status.
  if (this->mValue != nullptr)
  {
    for (auto& ptrHandle : this->mBindHandleList)
    {
      ptrHandle->mHandlerAddress = 0;
    }
    this->mBindHandleList.clear();

    delete this->mValue;
    this->mValue = nullptr;
  }

  this->pMoveHandler(std::move(iHandler));
  return *this;
}

template <typename TType>
void FProxyHandler<TType>::AttachHandle(FProxyHandle<TType>& iHandle)
{
  if (this->mBindHandleList.count(&iHandle) == 0)
  {
    iHandle.mHandlerAddress = this;
    this->mBindHandleList.emplace(&iHandle);
  }
}

template <typename TType>
EDySuccess FProxyHandler<TType>::DetachHandle(FProxyHandle<TType>& iHandle)
{
  if (this->mBindHandleList.count(&iHandle) == 1)
  {
    iHandle.mHandlerAddress = nullptr;
    this->mBindHandleList.erase(&iHandle);
    return DY_SUCCESS;
  }
  return DY_FAILURE;
}

template <typename TType>
bool FProxyHandler<TType>::IsBeingBinded() const noexcept
{
  return this->mBindHandleList.empty() == false;
}

template <typename TType>
void FProxyHandler<TType>::pMoveHandler(FProxyHandler&& iAnotherHandler) noexcept
{
  if (iAnotherHandler.mValue != nullptr
    &&  iAnotherHandler.mBindHandleList.empty() == false)
  {
    // First, move binder handle pointer list.
    this->mBindHandleList = iAnotherHandler.mBindHandleList;
    iAnotherHandler.mBindHandleList.clear();

    // Second, let handles relocate specifier of handler.
    for (auto& ptrHandle : this->mBindHandleList)
    {
      ptrHandle->mHandlerAddress = this;
    }

    // At last, set iHandler's value to nullptr.
    this->mValue = iAnotherHandler.mValue;
    iAnotherHandler.mValue = nullptr;
  }
}

} /// ::dy namespace 

#endif /// GUARD_DY_HELPER_INTERNAL_FPROXYHANDLERTMP_INL
