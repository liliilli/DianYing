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
struct TZipIterator final
{
  static constexpr auto kSize = STypeList::LengthOf<TContainers>;
  using TIndex = typename TTupleIterator<TContainers>::Type;

  /// std::iterator_traits
#if defined(_WIN64) == true
  using difference_type   = int64_t;
#else
  using difference_type   = int32_t;
#endif
  using value_type        = TIndex;
  using pointer           = TIndex*;
  using reference         = TIndex&;
  using iterator_category = std::forward_iterator_tag;

  template <std::size_t Id> void ForwardItems();
  
  template <std::size_t Id> bool CheckIsSame(const TZipIterator<TContainers>& rhs) const;

  /// C++ named requirements: CopyConstructible
  TZipIterator(TIndex value) : __Value{std::move(value)} {};

  /// C++ named requirements: Swappable
  /// Type U is swappable with type T if, for any object u of type U and any object t of type T, 
  void swap(TZipIterator& other) noexcept;
  
  TIndex& operator*() noexcept  { return this->__Value; }
  TIndex* operator->() noexcept { return this->__Value; }
  TIndex __Value;
};

} /// ::dy::expr namespace
#include <Expr/Inline/TZipIterator.inl>