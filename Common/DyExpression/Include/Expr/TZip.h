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

#include <functional>
#include <any>
#include <Expr/TTypeList.h>
#include <Expr/Inline/TZipTypesT.inl>
#include <Expr/Inline/TZipIteratorType.inl>
#include <Expr/TZipIterator.h>
#include <Expr/TZipConstIterator.h>

namespace dy::expr
{

/// @class TZip
/// @brief Internal structure for managing zipped values and types.
/// @tparam TContainers Compile time type list type.
template <typename TContainers>
class TZip final 
{
public:
  using ContainerType = TContainers;
  template <std::size_t I>
  using WrapperType     = std::reference_wrapper<std::remove_reference_t<STypeList::At<I, ContainerType>>>;
  using TIndex          = typename TTupleIterator<TContainers>::Type;
  using TConstIndex     = typename TTupleConstIterator<TContainers>::Type;
  using TIterator       = TZipIterator<ContainerType>;
  using TConstInterator = TZipConstIterator<ContainerType>;

  /// @brief Compile time length of container types.
  static constexpr auto size = STypeList::LengthOf<TContainers>;

private:
  /// @brief Constructor helper function for add reference value of each type.
  template <typename TArg, typename... TArgs>
  void AppendRefValue(TArg& value, TArgs&&... values);

  /// @brief Insert reference values into zip iterator.
  template <std::size_t ReverseIndex>
  void InsertIterator(TIndex& value, std::size_t index);

  /// @brief Insert reference values into zip const_iterator.
  template <std::size_t ReverseIndex>
  void InsertIterator(TConstIndex& value, std::size_t index) const;

  /// @brief Constructor.
  template <typename... TArgs> TZip(TArgs&&... value);

public:
  /// @brief Get lvalue reference item of given index.
  template <std::size_t I> STypeList::At<I, ContainerType> GetItem();

  /// @brief Get sequenced items iterable row of given index.
  TIndex operator[](std::size_t index);
  /// @brief Get sequenced items iterable row of given index.
  TConstIndex operator[](std::size_t index) const;

  //!
  //! Below functions are used to support ranged for-loop. (C++11)
  //!

  TZipIterator<ContainerType> begin() noexcept;
  TZipIterator<ContainerType> end() noexcept;

  TConstInterator begin() const noexcept;
  TConstInterator end() const noexcept;

  TConstInterator cbegin() const noexcept;
  TConstInterator cend() const noexcept;

private:
  std::vector<std::any> mRefValues; 

  template <typename... TValue>
  friend TZip<STypeList::MakeListExt<ETypeRef::LValue, TValue...>> Zip(TValue&&... value);
};

/// @brief Helper function for creating TZip (zipping) with vary type parameters.
/// @tparam TValue Type parameters that have lvalue reference or may be so.
template <typename... TValue>
TZip<STypeList::MakeListExt<ETypeRef::LValue, TValue...>> 
Zip(TValue&&... value);

} /// ::dy::expr namespace
#include <Expr/Inline/TZip.inl>
