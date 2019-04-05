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
#include <Expr/XMacroes.h>

/// https://stackoverflow.com/questions/11317474/macro-to-count-number-of-arguments
/// https://stackoverflow.com/questions/48045470/portably-detect-va-opt-support
/// https://stackoverflow.com/questions/12447557/can-we-have-recursive-macros
namespace dy::expr
{

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
#define __EXPR_ENUM_33(Value, ...)  Value, EXPAND(__EXPR_ENUM_32(__VA_ARGS__)) 
#define __EXPR_ENUM_34(Value, ...)  Value, EXPAND(__EXPR_ENUM_33(__VA_ARGS__)) 
#define __EXPR_ENUM_35(Value, ...)  Value, EXPAND(__EXPR_ENUM_34(__VA_ARGS__)) 
#define __EXPR_ENUM_36(Value, ...)  Value, EXPAND(__EXPR_ENUM_35(__VA_ARGS__)) 
#define __EXPR_ENUM_37(Value, ...)  Value, EXPAND(__EXPR_ENUM_36(__VA_ARGS__)) 
#define __EXPR_ENUM_38(Value, ...)  Value, EXPAND(__EXPR_ENUM_37(__VA_ARGS__)) 
#define __EXPR_ENUM_39(Value, ...)  Value, EXPAND(__EXPR_ENUM_38(__VA_ARGS__)) 
#define __EXPR_ENUM_40(Value, ...)  Value, EXPAND(__EXPR_ENUM_39(__VA_ARGS__)) 
#define __EXPR_ENUM_41(Value, ...)  Value, EXPAND(__EXPR_ENUM_40(__VA_ARGS__)) 
#define __EXPR_ENUM_42(Value, ...)  Value, EXPAND(__EXPR_ENUM_41(__VA_ARGS__)) 
#define __EXPR_ENUM_43(Value, ...)  Value, EXPAND(__EXPR_ENUM_42(__VA_ARGS__)) 
#define __EXPR_ENUM_44(Value, ...)  Value, EXPAND(__EXPR_ENUM_43(__VA_ARGS__)) 
#define __EXPR_ENUM_45(Value, ...)  Value, EXPAND(__EXPR_ENUM_44(__VA_ARGS__)) 
#define __EXPR_ENUM_46(Value, ...)  Value, EXPAND(__EXPR_ENUM_45(__VA_ARGS__)) 
#define __EXPR_ENUM_47(Value, ...)  Value, EXPAND(__EXPR_ENUM_46(__VA_ARGS__)) 
#define __EXPR_ENUM_48(Value, ...)  Value, EXPAND(__EXPR_ENUM_47(__VA_ARGS__)) 

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
#define __EXPR_ENUM_STR_33(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_32(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_34(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_33(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_35(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_34(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_36(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_35(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_37(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_36(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_38(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_37(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_39(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_38(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_40(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_39(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_41(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_40(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_42(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_41(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_43(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_42(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_44(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_43(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_45(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_44(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_46(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_45(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_47(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_46(__VA_ARGS__)) 
#define __EXPR_ENUM_STR_48(Value, ...)  #Value, EXPAND(__EXPR_ENUM_STR_47(__VA_ARGS__)) 

#define __EXPR_ENUM_ENTRY(Prefix, N) __EXPR_ENUM ## Prefix ## N
#define __EXPR_INITIALIZE_ENUM(Prefix, N, ...) EXPAND(__EXPR_ENUM_ENTRY(Prefix, N)(__VA_ARGS__))

/// @def EXPR_DEFINE_ENUM
/// @brief Define extended enumration.
/// @reference https://m.blog.naver.com/PostView.nhn?blogId=hyw793&logNo=220746053203&proxyReferer=https%3A%2F%2Fwww.google.com%2F
///
/// @brief __EnumTypeName__ The name of enumeration type.
/// @brief ... Enumration value list.
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
  using _ = EEnum;                              \
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
  static constexpr EEnum ToEnum(const char* string) noexcept    \
  {                                                             \
    for (size_t i = 0, size = kNames.size(); i < size; ++i)     \
    {                                                           \
      if (string == kNames[i]) { return EEnum(i); }             \
    }                                                           \
    return EEnum::__Error;                                      \
  }                                                             \
                                                                \
  static const char* ToString(EEnum value) noexcept             \
  {                                                             \
    if (value == EEnum::__Error) { return ""; }                 \
    return kNames[value].data();                                \
  }                                                             \
                                                                \
  static EEnum ToEnum(const std::string& string) noexcept       \
  {                                                             \
    for (size_t i = 0, size = kNames.size(); i < size; ++i)     \
    {                                                           \
      if (string == kNames[i].data()) { return EEnum(i); }      \
    }                                                           \
    return EEnum::__Error;                                      \
  }                                                             \
}

/// @def EXPR_E
/// @brief Helper macor for specifying Enumration type's actual enumration type.
#define EXPR_E(Value) Value::_

#ifdef false
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
#endif

} /// ::dy::expr namespace