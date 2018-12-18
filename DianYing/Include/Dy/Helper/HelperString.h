#ifndef GUARD_DY_HELPER_HELPERSTRING_H
#define GUARD_DY_HELPER_HELPERSTRING_H
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

#include <algorithm>
#include <string>
#include <random>

#include <Dy/Helper/System/Macroes.h>
#include <Dy/Helper/GlobalType.h>

namespace dy
{

/// @brief Get random string wich length count.
MDY_NODISCARD std::string DyGetRandomString(_MIN_ TU32 length)
{
  static std::default_random_engine Rng(std::random_device{}());
  auto randchar = []() -> char
  {
    constexpr char charset[]  = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr auto maxIndex   = (sizeof(charset) - 1);
    return charset[Rng() % maxIndex];
  };

  std::string str(length, 0);
  std::generate_n(str.begin(), length, randchar);
  return str;
}

} /// ::dy namesapce

#endif /// GUARD_DY_HELPER_HELPERSTRING_H