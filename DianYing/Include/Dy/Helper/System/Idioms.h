#ifndef GUARD_DY_HELPER_SYSTEM_IDIOMS_H
#define GUARD_DY_HELPER_SYSTEM_IDIOMS_H
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
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

namespace dy
{

/// @brief Vector fast erase.
template <typename TType, typename TAllocator>
void DyFastErase(_MINOUT_ std::vector<TType, TAllocator>& iVector, _MIN_ TU32 iIndex)
{
  MDY_ASSERT(iIndex < iVector.size(), "Unexpected error occurred.");

  std::swap(iVector[iIndex], iVector.back());
  iVector.pop_back();
}

///
/// @brief 
/// @param 
/// @tparam TCtorArgs
///
template <typename TType, typename TAllocator, typename... TCtorArgs>
void DySafeUniquePtrEmplaceBack(
    _MINOUT_ std::vector<std::unique_ptr<TType>, TAllocator>& list,
    _MINOUT_ TCtorArgs&&... args)
{
  list.emplace_back(nullptr);
  MDY_ASSERT(list.back() == nullptr, "Unexpected error occurred.");

  auto ptrsmtInstance = std::make_unique<TType>(std::forward<TCtorArgs>(args)...);
  list.back() = std::move(ptrsmtInstance);
  MDY_ASSERT(list.back() != nullptr, "Unexpected error occurred.");
}

}

#endif /// GUARD_DY_HELPER_SYSTEM_IDIOMS_H