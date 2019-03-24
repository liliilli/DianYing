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

#include <utility>

namespace dy::expr
{

template <typename TContainers>
template <typename TArg, typename ... TArgs>
void TZip<TContainers>::AppendRefValue(TArg& value, TArgs&&... values)
{
  this->mRefValues.emplace_back(std::ref(value));
  if constexpr (sizeof...(values) > 0)
  {
    this->AppendRefValue(std::forward<TArgs>(values)...);
  }
}

template <typename TContainers>
template <std::size_t ReverseIndex>
void TZip<TContainers>::InsertIterator(TIndex& value, std::size_t index)
{
  constexpr auto dist = size - ReverseIndex - 1;

  auto& ptr = std::get<dist>(value);
  auto& ref = std::any_cast<WrapperType<dist>>(this->mRefValues.at(dist)).get();
  //using Type = typename TItemIterator<typename WrapperType<id>::type>::type;
  if constexpr (std::is_array_v<std::remove_reference_t<STypeList::At<dist, ContainerType>>>)
  {
    ptr = details::TItemIterator<
      std::remove_reference_t<STypeList::At<dist, ContainerType>>
    >(&ref) + index;
  }
  else
  {
    ptr = ref.begin() + index;
  }

  if constexpr (size - 1 != ReverseIndex)
  {
    this->InsertIterator<ReverseIndex + 1>(value, index);
  }
}

template <typename TContainers>
template <std::size_t ReverseIndex>
void TZip<TContainers>::InsertIterator(TConstIndex& value, std::size_t index) const
{
  constexpr auto dist = size - ReverseIndex - 1;

  auto& cptr = std::get<dist>(value);
  auto& ptr = const_cast<std::remove_const_t<decltype(cptr)>>(cptr);
  const auto& ref = std::any_cast<WrapperType<dist>>(this->mRefValues.at(dist)).get();
  //using Type = typename TItemIterator<typename WrapperType<id>::type>::type;
  if constexpr (std::is_array_v<std::remove_reference_t<STypeList::At<dist, ContainerType>>>)
  {
    ptr = details::TItemConstIterator<
      std::remove_reference_t<STypeList::At<dist, ContainerType>>
    >(&ref) + index;
  }
  else
  {
    ptr = ref.cbegin() + index;
  }

  if constexpr (size - 1 != ReverseIndex)
  {
    this->InsertIterator<ReverseIndex + 1>(value, index);
  }
}

template <typename TContainers>
template <typename ... TArgs>
TZip<TContainers>::TZip(TArgs&&... value)
{
  this->AppendRefValue(std::forward<TArgs>(value)...);
}

template <typename TContainers>
template <std::size_t I>
STypeList::At<I, typename TZip<TContainers>::ContainerType> 
TZip<TContainers>::GetItem()
{
  return std::any_cast<WrapperType<I>>(this->mRefValues.at(I)).get();
}

template <typename TContainers>
typename TZip<TContainers>::TIndex TZip<TContainers>::operator[](std::size_t index)
{
  TIndex value;
  this->InsertIterator<0>(value, index);
  return value;
}

template <typename TContainers>
typename TZip<TContainers>::TConstIndex TZip<TContainers>::operator[](std::size_t index) const
{
  TConstIndex value;
  this->InsertIterator<0>(value, index);
  return value;
}

template <typename TContainers>
TZipIterator<typename TZip<TContainers>::ContainerType> TZip<TContainers>::begin() noexcept
{
  TIndex value;
  this->InsertIterator<0>(value, 0);
  return TZipIterator<ContainerType>{value};
}

template <typename TContainers>
TZipIterator<typename TZip<TContainers>::ContainerType> TZip<TContainers>::end() noexcept
{
  TIndex value;
  this->InsertIterator<0>(value, size - 1); 
  return TZipIterator<ContainerType>{value};
}

template <typename TContainers>
typename TZip<TContainers>::TConstInterator TZip<TContainers>::begin() const noexcept
{
  TConstIndex value;
  this->InsertIterator<0>(value, 0);
  return TZipConstIterator<ContainerType>{value};
}

template <typename TContainers>
typename TZip<TContainers>::TConstInterator TZip<TContainers>::end() const noexcept
{
  TConstIndex value;
  this->InsertIterator<0>(value, 0);
  return TZipConstIterator<ContainerType>{value};
}

template <typename TContainers>
typename TZip<TContainers>::TConstInterator TZip<TContainers>::cbegin() const noexcept
{
  TConstIndex value;
  this->InsertIterator<0>(value, 0);
  return TZipConstIterator<ContainerType>{value};
}

template <typename TContainers>
typename TZip<TContainers>::TConstInterator TZip<TContainers>::cend() const noexcept
{
  TConstIndex value;
  this->InsertIterator<0>(value, 0);
  return TZipConstIterator<ContainerType>{value};
}

template <typename... TContainers>
TZip<STypeList::MakeListExt<ETypeRef::LValue, TContainers...>> Zip(TContainers&&... value)
{
  using ListType = STypeList::MakeListExt<ETypeRef::LValue, TContainers...>;
  return TZip<ListType>{ std::forward<TContainers>(value)... };
}

} /// ::dy::expr namespace