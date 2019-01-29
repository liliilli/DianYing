#ifndef GUARD_DY_HELPER_TYPE_VECTORINT4_H
#define GUARD_DY_HELPER_TYPE_VECTORINT4_H
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

#include <array>
#include <nlohmann/json_fwd.hpp>

/// @struct DDyVectorInt4
/// @brief TI32 type 4-element vector struct.
struct DDyVectorInt4 final {
  signed int X = 0;
  signed int Y = 0;
  signed int Z = 0;
  signed int W = 0;

  DDyVectorInt4() = default;
  ~DDyVectorInt4() = default;

  DDyVectorInt4( const signed int x,  const signed int y,  const signed int z,  const signed int w) noexcept :
      X{x}, Y{y}, Z{z}, W{w} {}
  explicit DDyVectorInt4( const signed int value) noexcept :
      X{value}, Y{value}, Z{value}, W{value} {}

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVectorInt4(const DDyVectorInt4& value) noexcept = default;
  DDyVectorInt4& operator=(const DDyVectorInt4& value) noexcept = default;

  auto& operator[]( const std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DDyVectorInt4 range is out of bound.");
    }
  }

  const auto& operator[]( const std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DDyVectorInt4 range is out of bound.");
    }
  }

  //!
  //! Methods
  //!

  /// @brief Return start pointer of instance. 
  const signed int* Data() const noexcept { return &this->X; }

  //!
  //! Operators
  //!

  friend DDyVectorInt4 operator+( DDyVectorInt4& lhs,  const DDyVectorInt4& rhs) noexcept {
    lhs.X += rhs.X; lhs.Y += rhs.Y; lhs.Z += rhs.Z; lhs.W += rhs.W;
    return lhs;
  }

  friend DDyVectorInt4 operator-( DDyVectorInt4& lhs,  const DDyVectorInt4& rhs) noexcept {
    lhs.X -= rhs.X; lhs.Y -= rhs.Y; lhs.Z -= rhs.Z; lhs.W -= rhs.W;
    return lhs;
  }

  ///
  /// DDyVectorInt4 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DDyVectorInt4 operator*( DDyVectorInt4& lhs,  const signed int rhs) noexcept {
    lhs.X *= rhs; lhs.Y *= rhs; lhs.Z *= rhs; lhs.W *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DDyVectorInt4, element multiplication happens.
  ///
  friend DDyVectorInt4 operator*( DDyVectorInt4& lhs,  const DDyVectorInt4& rhs) noexcept {
    lhs.X *= rhs.X; lhs.Y *= rhs.Y; lhs.Z *= rhs.Z; lhs.W *= rhs.W;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DDyVectorInt4 operator/(DDyVectorInt4& lhs, const signed int rhs) noexcept {
    { lhs.X /= rhs; lhs.Y /= rhs; lhs.Z /= rhs; lhs.W /= rhs; }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVectorInt4 operator/(DDyVectorInt4 lhs, const DDyVectorInt4& rhs) noexcept {
    {
      lhs.X /= rhs.X; lhs.Y /= rhs.Y; lhs.Z /= rhs.Z; lhs.W /= rhs.W;
    }

    return lhs;
  }

  DDyVectorInt4& operator+=(const DDyVectorInt4& value) noexcept
  {
    this->X += value.X; this->Y += value.Y; this->Z += value.Z; this->W += value.W;
    return *this;
  }

  DDyVectorInt4& operator-=(const DDyVectorInt4& value) noexcept
  {
    this->X -= value.X; this->Y -= value.Y; this->Z -= value.Z; this->W -= value.W;
    return *this;
  }

  DDyVectorInt4& operator*=(const signed int value) noexcept
  {
    this->X *= value; this->Y *= value; this->Z *= value; this->W *= value;
    return *this;
  }

  DDyVectorInt4& operator*=(const DDyVectorInt4& value) noexcept
  {
    this->X *= value.X; this->Y *= value.Y; this->Z *= value.Z; this->W *= value.W;
    return *this;
  }

  ///
  /// If lhs and rhs are DDyVectorInt4, element multiplication happens.
  ///
  DDyVectorInt4& operator/=(const signed int value) noexcept
  {
    { this->X /= value; this->Y /= value; this->W /= value; this->Z /= value; }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DDyVectorInt4& operator/=(const DDyVectorInt4& value) noexcept
  {
    { this->X /= value.X; this->Y /= value.Y; this->Z /= value.Z; this->W /= value.W; }

    return *this;
  }

  /// @brief Compare each element of two vectors and return if there are all matched. 
  friend bool operator==(const DDyVectorInt4& lhs, const DDyVectorInt4& rhs) noexcept
  {
    return lhs.X == rhs.X 
        && lhs.Y == rhs.Y
        && lhs.Z == rhs.Z
        && lhs.W == rhs.W;
  }

public:
  /// @brief Check if this DDyVectorInt4 is all zero or nearly equal to zero.
  bool IsAllZero() const noexcept
  {
    return this->X == 0 && this->Y == 0 && this->Z == 0 && this->W == 0;
  }
};

inline void to_json(nlohmann::json& j, const DDyVectorInt4& p) 
{
  (void)j;
  (void)p;
}
inline void from_json(const nlohmann::json& j, DDyVectorInt4& p) 
{
  (void)j;
  (void)p;
}

#endif /// GUARD_DY_HELPER_TYPE_VECTORINT4_H