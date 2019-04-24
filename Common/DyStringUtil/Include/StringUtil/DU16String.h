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

#include <string>
#include <sstream>
#include <vector>

namespace dy
{
  
/// @class DU16String
/// @brief UCS-2 (UTF-16 but no 4bytes characters) string type.
/// This string type is immutable but newly assignable.
class DU16String final
{
public:
  using TCharType = char16_t;
  using TSize = size_t;
  using TIndex = size_t;

  DU16String() = default;
  DU16String(const DU16String&) = default;
  DU16String& operator=(const DU16String&) = default;
  ~DU16String();

  /// @brief Make UCS-2 String type from const char* c-char literals at runtime.
  /// @param cString c-style string start pointer.
  DU16String(const char* cString) noexcept;

  /// @brief Make UCS-2 String type from std::string c++ library string type at runtime.
  /// @param string cpp-style string instance.
  DU16String(const std::string& string) noexcept;

  /// @brief  Set text newly. Surrogated characters which exceeded 2bytes will be discarded.
  /// @param  string utf-8 or applied string.
  void SetText(const char* string) noexcept;
  /// @brief `std::string` overloaded version of `DU16String::SetText`
  void SetText(const std::string& string) noexcept;
  /// @brief `std::string_view` overloaded version of `DU16String::SetText`
  void SetText(const std::string_view& string) noexcept;

  /// @brief  Return UCS-2 (UTF-16 without 4bytes) string lenght.
  /// @return UCS-2 String length.
  TSize GetLength() const noexcept;

  TCharType operator[](TIndex id) const;

private:
  /// @brief Internal routine for updating UCS-2 string.
  void pUpdateString(const char* string) noexcept;

  std::vector<TCharType> mString = {};

public:
  decltype(DU16String::mString)::const_iterator begin() const noexcept;
  decltype(DU16String::mString)::const_iterator cbegin() const noexcept;
  decltype(DU16String::mString)::const_iterator end() const noexcept;
  decltype(DU16String::mString)::const_iterator cend() const noexcept;
};

} /// ::dy namespace