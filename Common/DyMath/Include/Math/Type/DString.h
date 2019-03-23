#ifndef GUARD_DY_HELPER_TYPE_DYSTRING_H
#define GUARD_DY_HELPER_TYPE_DYSTRING_H
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

namespace dy
{

/// @class DString
/// @brief UCS-2 (UTF-16 but no 4bytes characters) string type.
/// This string type is immutable but newly assignable.
class DString final
{
public:
  DString() = default;
  DString(const DString&) = default;
  DString& operator=(const DString&) = default;
  ~DString();

  ///
  /// @brief Make UCS-2 String type from const char* c-char literals at runtime.
  /// @param cString c-style string start pointer.
  ///
  DString(_MIN_ const char* cString) noexcept;

  ///
  /// @brief Make UCS-2 String type from std::string c++ library string type at runtime.
  /// @param string cpp-style string instance.
  ///
  DString(_MIN_ const std::string& string) noexcept;

  ///
  /// @brief  Set text newly. Surrogated characters which exceeded 2bytes will be discarded.
  /// @param  string utf-8 or applied string.
  ///
  void SetText(_MIN_ const char* string) noexcept;
  /// @brief `std::string` overloaded version of `DString::SetText`
  void SetText(_MIN_ const std::string& string) noexcept;
  /// @brief `std::string_view` overloaded version of `DString::SetText`
  void SetText(_MIN_ const std::string_view& string) noexcept;

  ///
  /// @brief  Return UCS-2 (UTF-16 without 4bytes) string lenght.
  /// @return UCS-2 String length.
  ///
  FORCEINLINE MDY_NODISCARD TI32 GetLength() const noexcept
  {
    return static_cast<TI32>(this->mString.size());
  }

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD TC16 operator[](_MIN_ const TU32 id) const
  { // Integrity check
    if (id > this->mString.size())
    {
      throw std::out_of_range(fmt::format("Id {} is out of range from DString. Length is {}",
                              id, this->mString.size()));
    }

    // If all okay, just return.
    return this->mString[id];
  }

private:
  std::vector<TC16> mString = {};

public:
  /// @brief Internal routine for updating UCS-2 string.
  void pUpdateString(_MIN_ const char* string) noexcept;

  ///
  /// @brief
  /// @return
  ///
  decltype(DString::mString)::const_iterator begin() const noexcept
  {
    return mString.begin();
  }

  ///
  /// @brief
  /// @return
  ///
  decltype(DString::mString)::const_iterator cbegin() const noexcept
  {
    return mString.cbegin();
  }

  ///
  /// @brief
  /// @return
  ///
  decltype(DString::mString)::const_iterator end() const noexcept
  {
    return mString.end();
  }

  ///
  /// @brief
  /// @return
  ///
  decltype(DString::mString)::const_iterator cend() const noexcept
  {
    return mString.cend();
  }};

///
/// @macro  MIMMSTR
/// @brief  Just
/// @mparam __MACString__
///
#define MIMMSTR(__MACString__) ::dy::DString{__MACString__}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_DYSTRING_H