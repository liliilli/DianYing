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
