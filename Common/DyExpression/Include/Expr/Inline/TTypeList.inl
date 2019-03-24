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

#include <type_traits>

namespace dy::expr
{

template <ETypeRef EValue, typename TArg, typename... TArgs>
struct STypeList::TFactory final
{
  using TTypeList = TTypeList<
    typename TResultType<EValue, TArg>::Type, 
    typename TFactory<EValue, TArgs...>::TTypeList>;
};

template <ETypeRef EValue, typename TArg> 
struct STypeList::TFactory<EValue, TArg> final
{
  using TTypeList = TTypeListTail<
    typename TResultType<EValue, TArg>::Type>;
};

template <typename TArg> 
struct STypeList::TResultType<ETypeRef::None, TArg> final
{
  using Type = TArg;
};

template <typename TArg> 
struct STypeList::TResultType<ETypeRef::LValue, TArg> final
{
  using Type = std::add_lvalue_reference_t<TArg>;
};

template <typename TArg> 
struct STypeList::TResultType<ETypeRef::RValue, TArg> final
{
  using Type = std::add_rvalue_reference_t<TArg>;
};

template <typename TType>
struct STypeList::TLengthOf<TTypeListTail<TType>> final
{
  static constexpr std::size_t kSize = 1;
};

template <typename TType, typename TListType>
struct STypeList::TLengthOf<TTypeList<TType, TListType>> final
{
  static constexpr std::size_t kSize = 1 + TLengthOf<TListType>::kSize;
};

template <typename TType, typename TListType>
struct STypeList::TAt<0, TTypeList<TType, TListType>> final
{
  using Type = typename TTypeList<TType, TListType>::Type;
};

template <typename TType>
struct STypeList::TAt<0, TTypeListTail<TType>> final
{
  using Type = typename TTypeListTail<TType>::Type;
};

template <std::size_t I, typename TType, typename TListType>
struct STypeList::TAt<I, TTypeList<TType, TListType>> final
{
  using Type = typename TAt<I - 1, TListType>::Type;
};

template <std::size_t I, typename TType>
struct STypeList::TAt<I, TTypeListTail<TType>> final
{
  static_assert(
    I == 0, 
    "Failed to get a type from Typelist. Given index is out of range.");
};

template <typename TTargetType, typename TType>
struct STypeList::THasType<TTargetType, TTypeListTail<TType>> final
{
  using Type = typename TTypeListTail<TType>::Type;
  static constexpr bool kResult = std::is_same_v<TTargetType, Type>;
};

template <typename TTargetType, typename TType, typename TList>
struct STypeList::THasType<TTargetType, TTypeList<TType, TList>> final
{
  using Type = typename TTypeList<TType, TList>::Type;
  using List = typename TTypeList<TType, TList>::List;

  static constexpr bool kResult =
    std::is_same_v<TTargetType, Type> 
  ? true 
  : THasType<TTargetType, List>::kResult;
};

template <typename TTargetType, typename TType, std::size_t I> 
struct STypeList::TGetIndexOf<TTargetType, TTypeListTail<TType>, I> final 
{
  using Type = typename TTypeListTail<TType>::Type;

  static constexpr std::optional<std::size_t> kResult = 
    std::is_same_v<TTargetType, Type> 
  ? I 
  : std::optional<std::size_t>(std::nullopt); // NEED TO BE EXPLICIT.
};

template <typename TTargetType, typename TType, typename TList, std::size_t I> 
struct STypeList::TGetIndexOf<TTargetType, TTypeList<TType, TList>, I> final 
{
  using Type = typename TTypeList<TType, TList>::Type;
  using List = typename TTypeList<TType, TList>::List;
  
  static constexpr std::optional<std::size_t> kResult = 
    std::is_same_v<TTargetType, Type> 
  ? I 
  : TGetIndexOf<TTargetType, List, I + 1>::kResult;
};

template <typename TType, typename... TArgs> 
struct STypeList::TAppend<TTypeListTail<TType>, TArgs...> final
{
  using List = MakeList<TType, TArgs...>;
};

template <typename TType, typename TList, typename... TArgs>
struct STypeList::TAppend<TTypeList<TType, TList>, TArgs...> final
{
  using List = TTypeList<TType, typename TAppend<TList, TArgs...>::List>;
};

template <typename TType, template <typename> class TUnaryExecutable>
struct STypeList::TGetTypeIf<TTypeListTail<TType>, TUnaryExecutable> final
{
  using TItemType = typename TTypeListTail<TType>::Type;
  using Type = std::conditional_t<
    TUnaryExecutable<TItemType>::kResult == true,
    TItemType,
    TNull>;
};

template <typename TType, typename TList, template <typename> class TUnaryExecutable>
struct STypeList::TGetTypeIf<TTypeList<TType, TList>, TUnaryExecutable> final
{
  using TItemType = typename TTypeList<TType, TList>::Type;
  using Type = std::conditional_t<
    TUnaryExecutable<TItemType>::kResult == true,
    TItemType,
    typename TGetTypeIf<TList, TUnaryExecutable>::Type>;
};


} /// ::dy::expr namespace