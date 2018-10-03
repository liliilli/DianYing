#ifndef GUARD_DY_HELPER_INTERNAL_TMPCHECKTYPEPARAMS_H
#define GUARD_DY_HELPER_INTERNAL_TMPCHECKTYPEPARAMS_H
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

namespace dy
{

template <typename TGoal, typename TArg>
[[nodiscard]] constexpr bool __DyTryMatchGoalArg() noexcept
{
    using _TGoal = std::remove_reference_t<std::remove_cv_t<TGoal>>;
    using _TArg  = std::remove_reference_t<std::remove_cv_t<TArg>>;

    constexpr bool _typeMatched = std::is_same_v<_TGoal, _TArg> || std::is_constructible_v<_TGoal, _TArg>;
    return _typeMatched;
};

///
/// @struct MetaTest
/// @brief
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
template <typename... T>
struct MetaTest
{
    template <typename... U>
    struct TypeMatched
    {
        enum {
            TGoalTypesCount = sizeof...(T),
            TArgsTypesCount = sizeof...(U)
        };

        static constexpr bool _countMatched = TGoalTypesCount == TArgsTypesCount;
        static_assert(_countMatched, "Test failed, type parameter counts are not matched.");

        template <bool BFlag = (... && __DyTryMatchGoalArg<T, U>())>
        struct Result : public std::integral_constant<bool, BFlag> {};
    };
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_INTERNAL_TMPCHECKTYPEPARAMS_H