#ifndef GUARD_DY_HELPER_TYPE_DYSTRING_H
#define GUARD_DY_HELPER_TYPE_DYSTRING_H
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

namespace dy
{

///
/// @class DDyString
/// @brief UCS-2 (UTF-16 but no 4bytes characters) string type. This string type is immutable.
///
class DDyString final
{
public:
  //!
  //! Constructor
  //!

  ///
  /// @brief Make UCS-2 String type from const char* c-char literals at runtime.
  /// @param cString
  ///
  DDyString(_MIN_ const char* cString) noexcept;

  ///
  /// @brief Make UCS-2 String type from std::string c++ library string type at runtime.
  /// @param string
  ///
  explicit DDyString(_MIN_ const std::string& string) noexcept;

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
  // cbegin

  ///
  /// @brief
  /// @return
  ///
  // cend

  ///
  /// @brief
  /// @return
  ///
  MDY_NODISCARD TC16 operator[](_MIN_ const TU32 id)
  { // Integrity check
    if (id > this->mString.size())
    {
      throw std::out_of_range(fmt::format("Id {} is out of range from DDyString. Length is {}",
                              id, this->mString.size()));
    }

    // If all okay, just return.
    return this->mString[id];
  }

private:
  std::vector<TC16> mString = {};
};

///
/// @macro  MIMMSTR
/// @brief  Just
/// @mparam __MACString__
///
#define MIMMSTR(__MACString__) ::dy::DDyString{__MACString__}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_DYSTRING_H