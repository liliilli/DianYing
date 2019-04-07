#ifndef GUARD_DY_HELPER_CONTAINERHELPER_H
#define GUARD_DY_HELPER_CONTAINERHELPER_H
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

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

namespace dy
{

template <typename TKey, typename TInputKey>
constexpr bool bIsInputKeyChangeableToKey = false
    || std::is_same_v<TKey, TInputKey>
    || std::is_constructible_v<TInputKey, TKey>
    || std::is_convertible_v<TInputKey, TKey>;

template <
  template <
    typename TKey,
    typename TValue,
    typename = std::hash<TKey>,
    typename = std::equal_to<TKey>,
    typename = std::allocator<std::pair<const TKey, TValue>>
  > class TContainer = std::unordered_map,
  typename TKey,
  typename TValue,
  typename TInputKey,
  typename = std::enable_if_t<
    std::is_same_v<
        TContainer<TKey, TValue>,
        std::unordered_map<TKey, TValue>
    >
  >
>
MDY_NODISCARD bool Contains(const TContainer<TKey, TValue>& hashMap, const TInputKey& specifier) noexcept
{
    static_assert(bIsInputKeyChangeableToKey<TKey, TInputKey>,
        "TInputKey type cannot be used as a specifier key type of given specialized "
        "std::unordered_map.");

    return hashMap.find(specifier) != hashMap.end();
}

template <
  template <
    typename TKey,
    typename TValue,
    typename = std::less<TKey>,
    typename = std::allocator<std::pair<const TKey, TValue>>
  > class TContainer = std::map,
  typename TKey,
  typename TValue,
  typename TInputKey,
  typename = std::enable_if_t<
    std::is_same_v<
      TContainer<TKey, TValue>,
      std::map<TKey, TValue>
    >
  >
>
MDY_NODISCARD bool Contains(const TContainer<TKey, TValue>& map, const TInputKey& specifier)
{
  static_assert(false
    || std::is_same_v<TKey, TInputKey>
    || std::is_constructible_v<TInputKey, TKey>
    || std::is_convertible_v<TInputKey, TKey>,
    "TInputKey type cannot be used as a specifier key type of given specialized "
    "std::map.");
  return map.find(specifier) != map.end();
}

template <
  template <
    typename TKey,
    typename = std::less<TKey>,
    typename = std::allocator<TKey>
  > class TContainer = std::set,
  typename TKey,
  typename TInputKey,
  typename = std::enable_if_t<
    std::is_same_v<
      TContainer<TKey>,
      std::set<TKey>
    >
  >
>
MDY_NODISCARD bool Contains(const TContainer<TKey>& set, const TInputKey& specifier)
{
  static_assert(false
    || std::is_same_v<TKey, TInputKey>
    || std::is_constructible_v<TInputKey, TKey>
    || std::is_convertible_v<TInputKey, TKey>,
    "TInputKey type cannot be used as a specifier key type of given specialized std::set.");
  return set.find(specifier) != set.end();
}

template <
  template <
    typename TKey,
    typename = std::hash<TKey>,
    typename = std::equal_to<TKey>,
    typename = std::allocator<TKey>
  > class TContainer = std::unordered_set,
  typename TKey,
  typename TInputKey,
  typename = std::enable_if_t<
    std::is_same_v<
      TContainer<TKey>,
      std::unordered_set<TKey>
    >
  >
>
MDY_NODISCARD bool Contains(const TContainer<TKey>& set, const TInputKey& specifier)
{
  static_assert(false
    || std::is_same_v<TKey, TInputKey>
    || std::is_constructible_v<TInputKey, TKey>
    || std::is_convertible_v<TInputKey, TKey>,
    "TInputKey type cannot be used as a specifier key type of given specialized std::unordered_set.");
  return set.find(specifier) != set.end();
}

/// @brief Vector contains.
template <
  typename TType, 
  typename TAllocator, 
  typename TFunctor
>
MDY_NODISCARD bool ContainsIf(const std::vector<TType, TAllocator>& p, TFunctor&& iPredcate)
{
  return std::any_of(p.begin(), p.end(), iPredcate);
}

/// @brief Vector contains.
template <typename TType, typename TAllocator>
MDY_NODISCARD bool Contains(const std::vector<TType, TAllocator>& p, const TType& target)
{
  for (const auto& item : p) 
  { 
    if (item == target) { return true; } 
  }
  return false;
}

/// @brief Array contains.
template <typename TType, std::size_t TAmount, typename TFunctor>
MDY_NODISCARD bool ContainsIf(const std::array<TType, TAmount>& p, TFunctor&& iPredicate)
{
  return std::any_of(MDY_BIND_CBEGIN_CEND(p), iPredicate);
}

/// @brief Array contains.
template <typename TType, std::size_t TAmount>
MDY_NODISCARD bool Contains(_MIN_ const std::array<TType, TAmount>& p, _MIN_ const TType& target)
{
  for (const auto& item : p) { if (item == target) { return true; } }
  return false;
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_CONTAINERHELPER_H