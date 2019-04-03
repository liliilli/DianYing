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

#include <Expr/TCrc32.h>

namespace dy::expr::string
{

/// @brief Get runtime string to crc-32 hash value for using switch-case statements.
constexpr THashVal32 Input(const char* input);

/// @brief Get runtime string to crc-32 hash value for using switch-case statements.
THashVal32 Input(const std::string& iInput);

/// @brief Do compile time conversion of literal string to hash value.
/// Case only support and should be executed on compile time.
constexpr THashVal32 Case(const char* iLiteral);

} /// ::dy::expr::string namespace
#include <Expr/Inline/XStringSwitch.inl>