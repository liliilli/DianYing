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

#include <array>
#include <string_view>

/// https://stackoverflow.com/questions/11317474/macro-to-count-number-of-arguments
/// https://stackoverflow.com/questions/48045470/portably-detect-va-opt-support
/// https://stackoverflow.com/questions/12447557/can-we-have-recursive-macros
namespace dy::expr
{

/// @def EXPAND
#define EXPAND(x) x
#define EXPR_COMMA() ,

#define __EXPR_NUMBER_ARGS(                     \
  _1, _2, _3, _4, _5, _6, _7, _8,               \
  _9, _10, _11, _12, _13, _14, _15,             \
  _16, _17, _18, _19, _20, _21, _22, _23, _24,  \
  _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N

#define __EXPR_REVERSE_SEQN           \
  32, 31, 30, 29, 28, 27, 26, 25,     \
  24, 23, 22, 21, 20, 19, 18, 17,     \
  16, 15, 14, 13, 12, 11, 10, 9,      \
  8, 7, 6, 5, 4, 3, 2, 1,             \
  0

#define __EXPR_COMMA_SEQN \
  1, 1, 1, 1, 1, 1, 1, 1, \
  1, 1, 1, 1, 1, 1, 1, 1, \
  1, 1, 1, 1, 1, 1, 1, 1, \
  1, 1, 1, 1, 1, 1, 1, 0, 0

#define PP_NARG_01(N) 0
#define PP_NARG_00(N) 1
#define PP_NARG_11(N) N
#define PP_NARG_(...)     EXPAND(__EXPR_NUMBER_ARGS(__VA_ARGS__))
#define PP_HAS_COMMA(...) EXPAND(PP_NARG_(__VA_ARGS__, __EXPR_COMMA_SEQN))
#define PP_NARG_BRANCH(a, b, N) PP_NARG_##a##b(N)
#define PP_NARG_EXPAND_MACRO_ARGUMENTS(a, b, N) PP_NARG_BRANCH(a, b, N)
  
#define EXPR_GET_COUNT_ARGS(...)                      \
PP_NARG_EXPAND_MACRO_ARGUMENTS(                       \
  EXPAND(PP_HAS_COMMA(__VA_ARGS__)),                  \
  EXPAND(PP_HAS_COMMA(EXPR_COMMA __VA_ARGS__ ())),    \
  EXPAND(PP_NARG_(__VA_ARGS__, __EXPR_REVERSE_SEQN()))\
)

#define __EXPR_BRANCH_ENUM_ENTRY(a, b, ...) EXPAND(__EXPR_BRANCH_ENUM_##a##b(__VA_ARGS__))
#define __EXPR_EXPAND_ENUM_ARGUMENTS(a, b, ...) EXPAND(__EXPR_BRANCH_ENUM_ENTRY(a, b, __VA_ARGS__))

#define __EXPR_ENUM_ERROR()         __Error = INT_MAX
#define __EXPR_ENUM_1(Value)        Value, __EXPR_ENUM_ERROR()
#define __EXPR_ENUM_2(Value, ...)   Value, EXPAND(__EXPR_ENUM_1(__VA_ARGS__)) 
#define __EXPR_ENUM_3(Value, ...)   Value, EXPAND(__EXPR_ENUM_2(__VA_ARGS__)) 
#define __EXPR_ENUM_4(Value, ...)   Value, EXPAND(__EXPR_ENUM_3(__VA_ARGS__)) 
#define __EXPR_ENUM_5(Value, ...)   Value, EXPAND(__EXPR_ENUM_4(__VA_ARGS__)) 
#define __EXPR_ENUM_6(Value, ...)   Value, EXPAND(__EXPR_ENUM_5(__VA_ARGS__)) 
#define __EXPR_ENUM_7(Value, ...)   Value, EXPAND(__EXPR_ENUM_6(__VA_ARGS__)) 
#define __EXPR_ENUM_8(Value, ...)   Value, EXPAND(__EXPR_ENUM_7(__VA_ARGS__)) 
#define __EXPR_ENUM_9(Value, ...)   Value, EXPAND(__EXPR_ENUM_8(__VA_ARGS__)) 
#define __EXPR_ENUM_10(Value, ...)  Value, EXPAND(__EXPR_ENUM_9(__VA_ARGS__)) 
#define __EXPR_ENUM_11(Value, ...)  Value, EXPAND(__EXPR_ENUM_10(__VA_ARGS__)) 
#define __EXPR_ENUM_12(Value, ...)  Value, EXPAND(__EXPR_ENUM_11(__VA_ARGS__)) 
#define __EXPR_ENUM_13(Value, ...)  Value, EXPAND(__EXPR_ENUM_12(__VA_ARGS__)) 
#define __EXPR_ENUM_14(Value, ...)  Value, EXPAND(__EXPR_ENUM_13(__VA_ARGS__)) 
#define __EXPR_ENUM_15(Value, ...)  Value, EXPAND(__EXPR_ENUM_14(__VA_ARGS__)) 
#define __EXPR_ENUM_16(Value, ...)  Value, EXPAND(__EXPR_ENUM_15(__VA_ARGS__)) 
#define __EXPR_ENUM_17(Value, ...)  Value, EXPAND(__EXPR_ENUM_16(__VA_ARGS__)) 
#define __EXPR_ENUM_18(Value, ...)  Value, EXPAND(__EXPR_ENUM_17(__VA_ARGS__)) 
#define __EXPR_ENUM_19(Value, ...)  Value, EXPAND(__EXPR_ENUM_18(__VA_ARGS__)) 
#define __EXPR_ENUM_20(Value, ...)  Value, EXPAND(__EXPR_ENUM_19(__VA_ARGS__)) 
#define __EXPR_ENUM_21(Value, ...)  Value, EXPAND(__EXPR_ENUM_20(__VA_ARGS__)) 
#define __EXPR_ENUM_22(Value, ...)  Value, EXPAND(__EXPR_ENUM_21(__VA_ARGS__)) 
#define __EXPR_ENUM_23(Value, ...)  Value, EXPAND(__EXPR_ENUM_22(__VA_ARGS__)) 
#define __EXPR_ENUM_24(Value, ...)  Value, EXPAND(__EXPR_ENUM_23(__VA_ARGS__)) 
#define __EXPR_ENUM_25(Value, ...)  Value, EXPAND(__EXPR_ENUM_24(__VA_ARGS__)) 
#define __EXPR_ENUM_26(Value, ...)  Value, EXPAND(__EXPR_ENUM_25(__VA_ARGS__)) 
#define __EXPR_ENUM_27(Value, ...)  Value, EXPAND(__EXPR_ENUM_26(__VA_ARGS__)) 
#define __EXPR_ENUM_28(Value, ...)  Value, EXPAND(__EXPR_ENUM_27(__VA_ARGS__)) 
#define __EXPR_ENUM_29(Value, ...)  Value, EXPAND(__EXPR_ENUM_28(__VA_ARGS__)) 
#define __EXPR_ENUM_30(Value, ...)  Value, EXPAND(__EXPR_ENUM_29(__VA_ARGS__)) 
#define __EXPR_ENUM_31(Value, ...)  Value, EXPAND(__EXPR_ENUM_30(__VA_ARGS__)) 
#define __EXPR_ENUM_32(Value, ...)  Value, EXPAND(__EXPR_ENUM_31(__VA_ARGS__)) 

#define __EXPR_ENUM_STR_1(Value)        #Value 
#define __EXPR_ENUM_STR_2(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_1(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_3(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_2(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_4(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_3(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_5(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_4(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_6(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_5(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_7(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_6(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_8(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_7(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_9(Value, ...)   #Value, EXPAND(__EXPR_ENUM_STR_8(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_10(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_9(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_11(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_10(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_12(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_11(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_13(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_12(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_14(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_13(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_15(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_14(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_16(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_15(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_17(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_16(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_18(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_17(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_19(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_18(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_20(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_19(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_21(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_20(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_22(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_21(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_23(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_22(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_24(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_23(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_25(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_24(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_26(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_25(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_27(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_26(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_28(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_27(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_29(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_28(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_30(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_29(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_31(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_30(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_32(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_31(__VA_ARGS__)) 

#define __EXPR_ENUM_ENTRY(Prefix, N) __EXPR_ENUM ## Prefix ## N
#define __EXPR_INITIALIZE_ENUM(Prefix, N, ...) EXPAND(__EXPR_ENUM_ENTRY(Prefix, N)(__VA_ARGS__))

// https://m.blog.naver.com/PostView.nhn?blogId=hyw793&logNo=220746053203&proxyReferer=https%3A%2F%2Fwww.google.com%2F
#define EXPR_DEFINE_ENUM(__EnumTypeName__, ...) \
struct __EnumTypeName__ final                   \
{                                               \
public:                                         \
  enum EEnum                                    \
  {                                             \
    EXPAND(__EXPR_INITIALIZE_ENUM(              \
      _                                         \
    , EXPAND(EXPR_GET_COUNT_ARGS(__VA_ARGS__))  \
    , __VA_ARGS__))                             \
  };                                            \
                                                \
private:                                        \
  static constexpr                              \
  std::array<                                   \
    std::string_view                            \
  , EXPAND(EXPR_GET_COUNT_ARGS(__VA_ARGS__))> kNames = \
  {                                             \
    EXPAND(__EXPR_INITIALIZE_ENUM(              \
      _STR_                                     \
    , EXPAND(EXPR_GET_COUNT_ARGS(__VA_ARGS__))  \
    , __VA_ARGS__))                             \
  };                                            \
                                                \
public:                                         \
  static constexpr EEnum ToEnum(std::string_view string) noexcept\
  {                                                             \
    for (size_t i = 0, size = kNames.size(); i < size; ++i)     \
    {                                                           \
      if (string == kNames[i]) { return EEnum(i); }             \
    }                                                           \
    return EEnum::__Error;                                      \
  }                                                             \
                                                                \
  static constexpr const char* ToString(EEnum value) noexcept   \
  {                                                             \
    if (value == EEnum::__Error) { return ""; }                 \
                                                                \
    return kNames[value].data();                                \
  }                                                             \
}

struct DTest final
{
  EXPR_DEFINE_ENUM(ETest, A, B, C, D, E, F, G, H, I, J, K, L, M, N);
};

static_assert(DTest::ETest::ToEnum("A") == DTest::ETest::A);
static_assert(DTest::ETest::ToEnum("C") == DTest::ETest::C);
static_assert(DTest::ETest::ToEnum("D") == DTest::ETest::D);
static_assert(DTest::ETest::ToEnum("E") == DTest::ETest::E);
static_assert(DTest::ETest::ToEnum("N") == DTest::ETest::N);

static_assert(EXPR_GET_COUNT_ARGS('A', 'B', 'C', 'D') == 4); 
static_assert(EXPR_GET_COUNT_ARGS('A', 'B') == 2); 
static_assert(EXPR_GET_COUNT_ARGS('A') == 1); 
static_assert(EXPR_GET_COUNT_ARGS() == 0); 

} /// ::dy::expr namespace