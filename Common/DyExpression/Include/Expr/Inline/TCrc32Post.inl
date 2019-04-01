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

namespace dy::expr
{

/// @brief Do encryption and return crc32 hash value for arbitary string in compile time.
constexpr THashVal32 ToHashCrc32(const char* s) noexcept 
{ 
  return details::Crc32Rec(0xFFFFFFFF, s); 
}

constexpr THashVal32 ToHashCrc32(std::string_view sv) noexcept
{
  return details::Crc32Rec(0xFFFFFFFF, sv.data());
}

static_assert(ToHashCrc32("Hello world!") == 0x1B851995);

} /// ::dy::expr namespace