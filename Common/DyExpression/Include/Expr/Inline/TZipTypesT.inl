#pragma once
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

#include <Expr/TExtendedTraits.h>

namespace dy::expr::details
{

template <typename TType, typename = void> struct TItemIterator_T;

template <typename TType>
struct TItemIterator_T<TType, std::enable_if_t<std::is_array_v<TType>>> final
{
  using Type = std::decay_t<TType>;
  using ConstType = const std::remove_pointer_t<Type>*;
};

template <typename TType>
struct TItemIterator_T<TType, std::enable_if_t<std::is_array_v<TType> == false>> final
{
  using Type      = typename TType::iterator;
  using ConstType = typename TType::const_iterator;
};

/// @brief Get iteration type of given item type.
/// If array, get T*, if given type supports begin(), end(), return with ::iterator.
template <typename TType> using TItemIterator = typename TItemIterator_T<TType>::Type;
template <typename TType> using TItemConstIterator = typename TItemIterator_T<TType>::ConstType;

} /// ::dy::expr::details