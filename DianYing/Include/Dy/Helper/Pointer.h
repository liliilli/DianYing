#ifndef GUARD_DY_HELPER_POINTER_H
#define GUARD_DY_HELPER_POINTER_H
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
/// This file is inspired by GSL pointer.h file.
/// @reference https://github.com/Microsoft/GSL/blob/master/include/gsl/pointers
///

#include <algorithm>    // for forward
#include <iosfwd>       // for ptrdiff_t, nullptr_t, ostream, size_t
#include <memory>       // for shared_ptr, unique_ptr
#include <system_error> // for hash
#include <type_traits>  // for enable_if_t, is_convertible, is_assignable

#include <Dy/Helper/Internal/Contracts.h>

#if defined(_MSC_VER) && _MSC_VER < 1910
#pragma push_macro("constexpr")
#define constexpr /*constexpr*/
#endif /// defined(_MSC_VER) && _MSC_VER < 1910

namespace dy
{

//!
//! GSL.Owner: ownership pointers
//!

///
/// Owner
///
/// Owner<TType> is designed as a bridge for code that must deal directly with owning pointers for some reason
///
/// TType must be a pointer type
/// - disallow construction from any type other than pointer type
///
template <class TType, class = std::enable_if_t<std::is_pointer_v<TType>>>
using Owner = TType;

///
/// @class NotNull
///
/// Restricts a pointer or smart pointer to only hold non-null values.
/// Has zero size overhead over TType.
///
/// If TType is a pointer (i.e. TType == TConvType*) then
/// - allow construction from TConvType*
/// - disallow construction from nullptr_t
/// - disallow default construction
/// - ensure construction from null TConvType* fails
/// - allow implicit conversion to TConvType*
///
template <class TType>
class NotNull final
{
public:
  static_assert(std::is_assignable<TType&, std::nullptr_t>::value, "TType cannot be assigned nullptr.");

  template <typename TConvType, typename = std::enable_if_t<std::is_convertible_v<TConvType, TType>>>
  constexpr explicit NotNull(TConvType&& u) : mPtr(std::forward<TConvType>(u))
  {
    Expects(mPtr != nullptr);
  }

  template <typename TConvType, typename = std::enable_if_t<std::is_convertible_v<TConvType, TType>>>
  constexpr NotNull(const NotNull<TConvType>& other) : NotNull(other.Get())
  { }

  template <typename = std::enable_if_t<!std::is_same_v<std::nullptr_t, TType>>>
  constexpr explicit NotNull(TType u) : mPtr(u)
  {
    Expects(mPtr != nullptr);
  }

  NotNull(NotNull&& other)                  = default;
  NotNull& operator=(NotNull&& other)       = default;
  NotNull(const NotNull& other)             = default;
  NotNull& operator=(const NotNull& other)  = default;
  ~NotNull()                                = default;

  constexpr TType Get() const
  {
    Ensures(mPtr != nullptr);
    return mPtr;
  }

  constexpr operator TType() const            { return Get(); }
  constexpr TType operator->() const          { return Get(); }
  constexpr decltype(auto) operator*() const  { return *Get(); }

  // prevents compilation when someone attempts to assign a null pointer constant
  NotNull(std::nullptr_t)               = delete;
  NotNull& operator=(std::nullptr_t)    = delete;

  // unwanted operators...pointers only point to single objects!
  NotNull& operator++()                 = delete;
  NotNull& operator--()                 = delete;
  NotNull operator++(int)               = delete;
  NotNull operator--(int)               = delete;
  NotNull& operator+=(std::ptrdiff_t)   = delete;
  NotNull& operator-=(std::ptrdiff_t)   = delete;
  void operator[](std::ptrdiff_t) const = delete;

private:
  TType mPtr;
};

template <class TType>
auto DyMakeNotNull(TType&& t)
{
  return NotNull<std::remove_cv_t<std::remove_reference_t<TType>>>{std::forward<TType>(t)};
}

template <class TType>
std::ostream& operator<<(std::ostream& os, const NotNull<TType>& val)
{
  os << val.get();
  return os;
}

template <class T, class U>
auto operator==(const NotNull<T>& lhs, const NotNull<U>& rhs) -> decltype(lhs.get() == rhs.get())
{
  return lhs.get() == rhs.get();
}

template <class T, class U>
auto operator!=(const NotNull<T>& lhs, const NotNull<U>& rhs) -> decltype(lhs.get() != rhs.get())
{
  return lhs.get() != rhs.get();
}

template <class T, class U>
auto operator<(const NotNull<T>& lhs, const NotNull<U>& rhs) -> decltype(lhs.get() < rhs.get())
{
  return lhs.get() < rhs.get();
}

template <class T, class U>
auto operator<=(const NotNull<T>& lhs, const NotNull<U>& rhs) -> decltype(lhs.get() <= rhs.get())
{
  return lhs.get() <= rhs.get();
}

template <class T, class U>
auto operator>(const NotNull<T>& lhs, const NotNull<U>& rhs) -> decltype(lhs.get() > rhs.get())
{
  return lhs.get() > rhs.get();
}

template <class T, class U>
auto operator>=(const NotNull<T>& lhs, const NotNull<U>& rhs) -> decltype(lhs.get() >= rhs.get())
{
  return lhs.get() >= rhs.get();
}

// more unwanted operators
template <class TType, class TConvType>
std::ptrdiff_t operator-(const NotNull<TType>&, const NotNull<TConvType>&)  = delete;
template <class TType>
NotNull<TType> operator-(const NotNull<TType>&, std::ptrdiff_t)             = delete;
template <class TType>
NotNull<TType> operator+(const NotNull<TType>&, std::ptrdiff_t)             = delete;
template <class TType>
NotNull<TType> operator+(std::ptrdiff_t, const NotNull<TType>&)             = delete;

} /// namespace dy

namespace std
{

template <class TType>
struct hash<dy::NotNull<TType>>
{
  std::size_t operator()(const dy::NotNull<TType>& value) const { return hash<TType>{}(value); }
};

} /// namespace std

#if defined(_MSC_VER) && _MSC_VER < 1910
#undef constexpr
#pragma pop_macro("constexpr")

#endif // defined(_MSC_VER) && _MSC_VER < 1910

#endif /// GUARD_DY_HELPER_POINTER_H