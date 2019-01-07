#ifndef GUARD_DY_HELPER_INTERNAL_TMPCHECKTYPEPARAMS_H
#define GUARD_DY_HELPER_INTERNAL_TMPCHECKTYPEPARAMS_H
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

namespace dy
{

namespace tmp
{

///
/// @brief  Test TArgs type is able to match TGoal in compile time.
/// @tparam TGoal
/// @tparam TArg
/// @return
///
template <typename TGoal, typename TArg>
MDY_NODISCARD constexpr bool __DyTryMatchGoalArg() noexcept
{
    using _TGoal = std::remove_reference_t<std::remove_cv_t<TGoal>>;
    using _TArg  = std::remove_reference_t<std::remove_cv_t<TArg>>;

    return std::is_same_v<_TGoal, _TArg> || std::is_constructible_v<_TGoal, _TArg>;
};

// Specify function type parameters.
template <typename...> struct __Params {};

// Use this type to extracts parameter types from function. (stores to _Types<...>!)
template <typename TSignature> struct __FunctionArgs;
template <typename TReturn, typename... TArgs>
struct __FunctionArgs<TReturn(TArgs...)>
{
  using ParamType = __Params<TArgs...>;
};

// Use this type to execute function param type matching test.
template <typename...> struct MetaTestFunc;
template <template <typename... TGoal> class TContainer, typename... TInput, typename... TGoal>
struct MetaTestFunc<TContainer<TGoal...>, TInput...>
{
    // Integrity test
    static_assert(std::is_same_v<__Params<TGoal...>, TContainer<TGoal...>>);

    // Parameter count checking test
    static constexpr bool _countMatched = sizeof...(TGoal) == sizeof...(TInput);
    static_assert(_countMatched, "Test failed, type parameter counts are not matched.");

    // Type matching test
    template <bool BFlag = (... && __DyTryMatchGoalArg<TGoal, TInput>())>
    struct Result : public std::integral_constant<bool, BFlag> {};
};

} /// internal namespace

///
/// @struct MetaTest
/// @brief Test matching goal types and arbitary arguments from variadic type parameters.
/// ~~~~ Example ~~~~
///
/// template <typename... TGoalTypes, typename... TArgs>
/// void TestFunction(TArgs&&...)
/// {
///    static_assert(Test<TGoalTypes...>::template TypeMatched<TArgs...>::template Result<>::value, "Test failed");
/// }
///
/// std::string i = "Hello world";
/// std::vector<int> vec = {1, 2, 3, 4, 5};
/// int a = 362'555;
/// int* hA = new int(450);
/// TestFunction<const std::vector<int>&, const std::string&, const int, const int*>(vec, i, a, hA);
/// delete hA;
/// -----------------
///
template <typename... T> struct MetaTest
{
  template <typename... U> struct TypeMatched
  {
    static constexpr bool _countMatched = sizeof...(T) == sizeof...(U);
    static_assert(_countMatched, "Test failed, type parameter counts are not matched.");

    template <bool BFlag = (... && ::dy::tmp::__DyTryMatchGoalArg<T, U>())>
    struct Result : public std::integral_constant<bool, BFlag> {};
  };
};

///
/// @brief  Test inputted argument types are being able to match parameter types of arbitary function in compile time.
/// ~~~~ Example ~~~~
/// std::string i = "Hello world";
/// std::vector<int> vec = {1, 2, 3, 4, 5};
/// int a = 362'555;
/// int* hA = new int(450);
///
/// void Function3(const std::string&, const std::vector<int>&, long long, const int*);
///
/// DyTestMatchFunctionParam<decltype(Function3)>(i, vec, a, hA);
/// ...or just using lambda and using namespace ::dy::__; somewhere...
/// -----------------
///
/// @tparam TGoalFunction Function type to extract parameter types from.
/// @tparam TArgs         Argument types to match.
///
template <typename TGoalFunction, typename... TArgs>
constexpr bool DyTestMatchFunctionParam() noexcept
{
  using namespace ::dy::tmp;

  using _TGoalType = typename __FunctionArgs<TGoalFunction>::ParamType;
  return MetaTestFunc<_TGoalType, TArgs...>::template Result<>::value;
}

///
/// @macro MDY_TEST_MATCH_FUNCTIONPARAM
/// @brief Test function parameter matching test as macro version.
///
#define MDY_TEST_MATCH_FUNCTIONPARAM(__MAErrorString__, __MAFunction__, ...)                      \
  []()                                                                                            \
  {                                                                                               \
    using namespace ::dy::tmp;                                                                    \
    static_assert(MetaTestFunc<__FunctionArgs<decltype(__MAFunction__)>::ParamType, __VA_ARGS__>  \
        ::Result<>::value, __MAErrorString__);                                                    \
  }

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_TMPCHECKTYPEPARAMS_H