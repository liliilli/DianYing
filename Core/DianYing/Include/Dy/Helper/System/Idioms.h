#ifndef GUARD_DY_HELPER_SYSTEM_IDIOMS_H
#define GUARD_DY_HELPER_SYSTEM_IDIOMS_H
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

#include <vector>
#include <utility>
#include <Dy/Helper/System/Assertion.h>

namespace dy
{

/// @brief Vector fast erase.
template <typename TType, typename TAllocator>
void FaseErase(std::vector<TType, TAllocator>& ioVector, TU32 iIndex)
{
  MDY_ASSERT_MSG(iIndex < ioVector.size(), "Unexpected error occurred.");

  std::swap(ioVector[iIndex], ioVector.back());
  ioVector.pop_back();
}

/// @brief Vector fast erase.
template <typename TType, typename TAllocator>
void FaseErase(
    std::vector<TType, TAllocator>& ioVector, 
    const typename std::vector<TType, TAllocator>::iterator& iIndex)
{
  if (iIndex == ioVector.end()) { return; }
  FaseErase(ioVector, static_cast<TU32>(std::distance(ioVector.begin(), iIndex)));
}

/// @brief Vector fast erase.
template <typename TType, typename TAllocator>
void EraseRemove(
    std::vector<TType, TAllocator>& ioVector, 
    const typename std::vector<TType, TAllocator>::value_type& iValue)
{
  ioVector.erase( std::remove( ioVector.begin(), ioVector.end(), iValue ), ioVector.end() );
}

/// @brief Vector fast erase.
template <typename TType, typename TAllocator, typename TFunctor>
void EraseRemoveIf(std::vector<TType, TAllocator>& p, _MIN_ TFunctor&& iPredcate)
{
  p.erase( std::remove_if( p.begin(), p.end(), iPredcate ), p.end() );
}

/// @brief Insert heap (unique_ptr) instance into list safely.
/// Heap instance will be created internally using args.
template <typename TType, typename TAllocator, typename... TCtorArgs>
void SafeUniquePtrEmplaceBack(
    std::vector<std::unique_ptr<TType>, TAllocator>& ioList,
    TCtorArgs&&... args)
{
  ioList.emplace_back(nullptr);
  MDY_ASSERT_MSG(ioList.back() == nullptr, "Unexpected error occurred.");

  auto ptrsmtInstance = std::make_unique<TType>(std::forward<TCtorArgs>(args)...);
  ioList.back() = std::move(ptrsmtInstance);
  MDY_ASSERT_MSG(ioList.back() != nullptr, "Unexpected error occurred.");
}

}

#endif /// GUARD_DY_HELPER_SYSTEM_IDIOMS_H