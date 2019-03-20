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
#define M_TOKEN_PASTE(__MAX__, __MAY__) __MAX__ ## __MAY__

/// @macro MDY_MAKENAME
/// @brief Make name.
#define M_MAKENAME(__LHS__, __RHS__) M_TOKEN_PASTE(__LHS__, __RHS__)

/// @macro MDY_TO_STRING
/// @brief Convert String to const char* literal.
#define M_TO_STRING(String) #String

/// @macro M_UNUSED_VAR
/// @brief make unused variable name.
#define M_UNUSED_VAR [[maybe_unused]] auto M_MAKENAME(unused, __LINE__)

#ifdef M_NOUSE_THROW
/// @macro M_ASSERT_OR_THROW
/// @brief Assert or throw with message.
#define M_ASSERT_OR_THROW(Expression, ErrorMessage) \
  assert((Expression) && (M_TO_STRING(ErrorMessage))); \
  throw 0
#else
/// @macro M_ASSERT_OR_THROW
/// @brief Assert or throw with message.
#define M_ASSERT_OR_THROW(Expression, ErrorMessage) \
  { \
    if ((Expression) == false) \
    { \
      throw std::runtime_error(M_TO_STRING(ErrorMessage)); \
    } \
  }
#endif

#define M_RETURN_NOTHING throw 0;

/// @macro 
#define M_NODISCARD [[nodiscard]]