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

namespace dy::expr
{

template <typename TContainers>
template <std::size_t Id>
void TZipIterator<TContainers>::ForwardItems()
{
  ++std::get<Id>(this->__Value);

  if constexpr (kSize - 1 != Id)
  {
    this->ForwardItems<Id + 1>();
  }
}

template <typename TContainers>
template <std::size_t Id>
bool TZipIterator<TContainers>::CheckIsSame(const TZipIterator<TContainers>& rhs) const
{
  if (std::get<Id>(this->__Value) == std::get<Id>(rhs.__Value))
  {
    if constexpr (kSize - 1 != Id)
    {
      return this->CheckIsSame<Id + 1>(rhs);
    }
    else
    {
      return true;
    }
  }

  return false;
}

template <typename TContainers>
void TZipIterator<TContainers>::swap(TZipIterator& other) noexcept
{
  TIndex tmp = other.__Value;
  other.__Value = this->__Value;
  this->__Value = tmp;
}

template <typename TContainers>
TZipIterator<TContainers>& operator++(TZipIterator<TContainers>& lhs)
{
  lhs.template ForwardItems<0>();
  return lhs;
}

template <typename TContainers>
TZipIterator<TContainers> operator++(TZipIterator<TContainers>& lhs, int)
{
  auto result = lhs; result.template ForwardItems<0>();
  return result;
}

/// C++ named requirements: Swappable
/// Type U is swappable with type T if, for any object u of type U and any object t of type T, 
template <typename TContainers>
void swap(TZipIterator<TContainers>& lhs, TZipIterator<TContainers>& rhs) noexcept
{
  lhs.swap(rhs);
}

/// C++ named requirements: EqualityComparable
template <typename TContainers>
bool operator==(const TZipIterator<TContainers>& lhs, const TZipIterator<TContainers>& rhs)
{
  return lhs.CheckIsSame<0>(rhs);
}

/// C++ named requirements: EqualityComparable
template <typename TContainers>
bool operator!=(const TZipIterator<TContainers>& lhs, const TZipIterator<TContainers>& rhs)
{
  return !(lhs == rhs);
}

} /// ::dy::expr namespace