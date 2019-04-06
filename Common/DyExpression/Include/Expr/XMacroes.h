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

/// DONT USE THIS DIRECTLY
#define EXPR_TOKEN_PASTE(__MAX__, __MAY__) __MAX__ ## __MAY__

/// @macro EXPR_MAKENAME
/// @brief Make name.
#define EXPR_MAKENAME(__LHS__, __RHS__) EXPR_TOKEN_PASTE(__LHS__, __RHS__)

/// @macro EXPR_TO_STRING
/// @brief Convert String to const char* literal.
#define EXPR_TO_STRING(String) #String

/// @macro MATH_UNUSED_VAR
/// @brief make unused variable name.
#define EXPR_UNUSED EXPR_MAKENAME(unused, __LINE__)

/// @macro M_UNUSED_VAR
/// @brief make unused variable name.
#define EXPR_UNUSED_VAR [[maybe_unused]] auto&& EXPR_MAKENAME(unused, __LINE__)

/// @macro EXPR_DEPRECATED
/// @brief Depereacted speicifer.
#define EXPR_DEPRECATED(__MAVersion__, __MAFunction__, __MAAlternative__) \
  [[deprecated(EXPR_TO_STRING(__MAFunction__) " is deprecated from " EXPR_TO_STRING(__MAVersion__) ". Use " EXPR_TO_STRING(__MAAlternative__) " instead.")]]

/// @macro EXPR_BIND_BEGIN_END
/// @brief Help forward iteratable type to bind .begin() and .end() to function.
#define EXPR_BIND_BEGIN_END(__MAIteratorableType__) \
  __MAIteratorableType__.begin(), __MAIteratorableType__.end()

/// @macro EXPR_BIND_CBEGIN_CEND
/// @brief Help forward iteratable type to bind .begin() and .end() to function.
#define EXPR_BIND_CBEGIN_CEND(__MAIteratorableType__) \
  __MAIteratorableType__.cbegin(), __MAIteratorableType__.cend()

/// @def EXPAND
/// @brief EXPAND macro for helping covering of __VA_ARGS__
#define EXPAND(x) x

/// @def EXPR_COMMA
/// @brief Just comma macro for handling of __VA_ARGS__
#define EXPR_COMMA() ,

#define __EXPR_NUMBER_ARGS(                     \
  _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15,             \
  _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, \
  _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, N, ...) N

#define __EXPR_REVERSE_SEQN           \
  48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
  32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
  16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,          \
  0

#define __EXPR_COMMA_SEQN \
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0

#define PP_NARG_01(N) 0 // Branch 01
#define PP_NARG_00(N) 1 // Branch 00
#define PP_NARG_11(N) N // Branch 11
#define PP_NARG_(...)     EXPAND(__EXPR_NUMBER_ARGS(__VA_ARGS__))
#define PP_HAS_COMMA(...) EXPAND(PP_NARG_(__VA_ARGS__, __EXPR_COMMA_SEQN))
#define PP_NARG_BRANCH(a, b, N)                 PP_NARG_##a##b(N)
#define PP_NARG_EXPAND_MACRO_ARGUMENTS(a, b, N) PP_NARG_BRANCH(a, b, N)

/// @def EXPR_GET_COUNT_ARGS
/// @brief Get the number of given variadic arguments of macro.
#define EXPR_GET_COUNT_ARGS(...)                      \
PP_NARG_EXPAND_MACRO_ARGUMENTS(                       \
  EXPAND(PP_HAS_COMMA(__VA_ARGS__)),                  \
  EXPAND(PP_HAS_COMMA(EXPR_COMMA __VA_ARGS__ ())),    \
  EXPAND(PP_NARG_(__VA_ARGS__, __EXPR_REVERSE_SEQN()))\
)
