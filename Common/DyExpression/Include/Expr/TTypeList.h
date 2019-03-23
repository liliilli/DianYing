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

#include <optional>
#include <Expr/TNull.h>

namespace dy::expr
{

/// @struct TTypeList
/// @brief Typelist type that can stores vary number of types. 
/// @tparam TType Stored actual type.
/// @tparam TList Another typelist attached to this type. 
template <typename TType, typename TList>
struct TTypeList final
{
  /// @brief Stored actual type.
  using Type = TType;
  /// @brief Another typelist attached to this type.
  using List = TList;
};

/// @struct TTypeListTail
/// @brief Tail typelist that can store just one type.
/// @tparam TType Stored actual type.
template <typename TType>
struct TTypeListTail final
{
  /// @brief Stored actual type.
  using Type = TType;
};

/// @struct STypeList
/// @brief Typelist helper API container.
struct STypeList final
{
private:
  template <typename TArg, typename... TArgs> struct TFactory;
  template <typename TArg> struct TFactory<TArg>;
  template <typename TType> struct TLengthOf;
  template <std::size_t I, typename TType> struct TAt;
  template <typename TTargetType, typename TList> struct THasType;
  template <typename TTargetType, typename TList, std::size_t I = 0> struct TGetIndexOf;
  template <typename TList, typename... TArgs> struct TAppend; 
  template <typename TList, template <typename> class TUnaryExecutable> struct TGetTypeIf;

public:
  /// @brief Make type list.
  /// @tparam TArgs Type list to input into the type list.
  template <typename... TArgs>
  using MakeList = typename TFactory<TArgs...>::TTypeList;

  /// @def EXPR_MAKE_TYPELIST
  /// @brief Helper macro function for creating typelist.
  #define EXPR_MAKE_TYPELIST(...) ::dy::expr::STypeList::MakeList<__VA_ARGS__>

  /// @brief Get length of given Typelist type.
  /// @tparam TList Typelist.
  template <typename TList> 
  static constexpr std::size_t LengthOf = TLengthOf<TList>::kSize; 

  /// @brief Get type of given index on given Typelist.
  /// @tparam I Index to find type.
  /// @tparam TList Typelist.
  template <std::size_t I, typename TList>
  using At = typename TAt<I, TList>::Type;

  /// @brief Check given type is in Typelist.
  /// @tparam TTargetType Type to find.
  /// @tparam TList Typelist.
  template <typename TTargetType, typename TList>
  static constexpr bool HasType = THasType<TTargetType, TList>::kResult;

  /// @brief Get index of given type in Typelist.
  /// @tparam TTargetType Type to find.
  /// @tparam TList Typelist.
  template <typename TTargetType, typename TList>
  static constexpr std::optional<std::size_t> GetIndexOf = TGetIndexOf<TTargetType, TList>::kResult;

  /// @brief Append types to the last of list. 
  /// @tparam TList Given original type list.
  /// @tparam TArgs Types to append.
  template <typename TList, typename... TArgs> 
  using Append = typename TAppend<TList, TArgs...>::List;

  /// @brief Get type from typelist if TUnaryExecutable static functor specialization of type is true.
  /// If any of types are not satisfied, just return expr::TNull as failure type.
  /// @tparam TList Typelist.
  /// @tparam TUnaryExecutable Static functor type that can have one type parameter.
  template <typename TList, template <typename> class TUnaryExecutable>
  using GetTypeIf = typename TGetTypeIf<TList, TUnaryExecutable>::Type;
};

} /// ::dy::expr namespace
#include <Expr/Inline/TTypeList.inl>