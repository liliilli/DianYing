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

#include <cstdint>
#include <string_view>

#include <Expr/Inline/TCrc32Pre.inl>
namespace dy::expr
{
  
/// @brief Crc 32-bit hash value type.
using THashVal32 = uint32_t; 

/// @brief Do encryption and return crc32 hash value for arbitary string in compile time.
constexpr THashVal32 ToHashCrc32(const char* s) noexcept;

/// @brief Do encryption and return crc32 hash value for arbitary string in compile time.
constexpr THashVal32 ToHashCrc32(std::string_view sv) noexcept;

} /// ::dy::expr namespace
#include <Expr/Inline/TCrc32Post.inl>