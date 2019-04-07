#ifndef GUARD_DY_HELPER_INTERNAL_FHANDLERTMP_INL
#define GUARD_DY_HELPER_INTERNAL_FHANDLERTMP_INL
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

#include <Dy/Helper/Internal/FHandler.h>
#include <Dy/Helper/Internal/FHandle.h>

namespace dy
{

template <typename TType>
FHandler<TType>::~FHandler()
{
  for (auto& ptrHandle : this->mBindHandleList)
  {
    ptrHandle->mHandlerAddress = 0;
  }
  this->mBindHandleList.clear();
}

template <typename TType>
FHandler<TType>::FHandler(FHandler&& iHandler) noexcept
{
  // If given handler value is not null (something is allocated)
  // and bind handle is exist, move handler list and let handle relocate handler location.
  this->pMoveHandler(std::move(iHandler));
}

template <typename TType>
FHandler<TType>& FHandler<TType>::operator=(FHandler&& iHandler) noexcept
{
  if (this == &iHandler) { return *this; }

  // If overwritten, destroy value and handle binding status.
  for (auto& ptrHandle : this->mBindHandleList)
  {
    ptrHandle->mHandlerAddress = 0;
  }
  this->mBindHandleList.clear();

  this->pMoveHandler(std::move(iHandler));
  return *this;
}

template <typename TType>
void FHandler<TType>::AttachHandle(FHandle<TType>& iHandle)
{
  if (this->mBindHandleList.count(&iHandle) == 0)
  {
    iHandle.mHandlerAddress = this;
    this->mBindHandleList.emplace(&iHandle);
  }
}

template <typename TType>
EDySuccess FHandler<TType>::DetachHandle(FHandle<TType>& iHandle)
{
  if (this->mBindHandleList.count(&iHandle) == 1)
  {
    iHandle.mHandlerAddress = nullptr;
    this->mBindHandleList.erase(&iHandle);
    return EDySuccess::DY_SUCCESS;
  }
  return EDySuccess::DY_FAILURE;
}

template <typename TType>
bool FHandler<TType>::IsBeingBinded() const noexcept
{
  return this->mBindHandleList.empty() == false;
}

template <typename TType>
void FHandler<TType>::pMoveHandler(FHandler&& iAnotherHandler) noexcept
{
  if (iAnotherHandler.mBindHandleList.empty() == false)
  {
    // First, move binder handle pointer list.
    this->mBindHandleList = iAnotherHandler.mBindHandleList;
    iAnotherHandler.mBindHandleList.clear();

    // Second, let handles relocate specifier of handler.
    for (auto& ptrHandle : this->mBindHandleList)
    {
      ptrHandle->mHandlerAddress = this;
    }
  }
}

} /// ::dy namespace 

#endif /// GUARD_DY_HELPER_INTERNAL_FHANDLERTMP_INL
