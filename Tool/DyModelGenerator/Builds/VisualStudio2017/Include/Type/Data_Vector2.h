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

#include <array>
#include <stdexcept>

#include <assimp/vector2.h>
#include <nlohmann/json_fwd.hpp>

/// @struct DDyVector2
/// @brief Float type 2-element vector struct.
struct DDyVector2 final {
  float X = 0.f;
  float Y = 0.f;

  DDyVector2() = default;

  DDyVector2(const float x, const float y) noexcept : X{x}, Y{y} {};
  explicit DDyVector2(const float value) noexcept : X{value}, Y{value} {}

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVector2(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector2(const DDyVector2& value) noexcept : X{value.X}, Y{value.Y} {}

  DDyVector2& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  DDyVector2& operator=(const DDyVector2& value) noexcept
  {
    if (this == &value) { return *this; }
    this->X = value.X;
    this->Y = value.Y;
    return *this;
  }

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  operator aiVector2D() const noexcept
  {
    return aiVector2D{this->X, this->Y};
  }

  //!
  //! Methods
  //!

  /// @brief Return one-dimensional data chunk of DDyVector2.
  [[nodiscard]] std::array<float, 2> Data() const noexcept
  {
    return {this->X, this->Y};
  }

  /// @brief Return squared length of this vector.
  /// @return Squared length of this DDyVector2.
  [[nodiscard]] float GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y;
  }

  /// @brief Returns the length of this vector.
  /// @return Length of this DDyVector2.
  [[nodiscard]] float GetLength() const noexcept
  {
    return std::sqrtf(this->GetSquareLength());
  }

  /// @brief Return new DDyVector2 instance of normalized input vector.
  /// @return Normalized DDyVector2 vector.
  [[nodiscard]] DDyVector2 Normalize() const noexcept
  {
    const auto length = this->GetLength();
    return {this->X / length, this->Y / length};
  }

  //!
  //! Operators
  //!

  friend DDyVector2 operator+(DDyVector2 lhs, const DDyVector2& rhs) noexcept {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    return lhs;
  }

  friend DDyVector2 operator-(DDyVector2 lhs, const DDyVector2& rhs) noexcept {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    return lhs;
  }

  ///
  /// DDyVector2 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DDyVector2 operator*(DDyVector2 lhs, const float rhs) noexcept {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DDyVector2, element multiplication happens.
  ///
  friend DDyVector2 operator*(DDyVector2 lhs, const DDyVector2& rhs) noexcept {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DDyVector2 operator/(DDyVector2 lhs, const float rhs) noexcept 
  {
    lhs.X /= rhs;
    lhs.Y /= rhs;
    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVector2 operator/(DDyVector2 lhs, const DDyVector2& rhs) noexcept 
  {
    lhs.X /= rhs.X;
    lhs.Y /= rhs.Y;
    return lhs;
  }

  DDyVector2& operator+=(const DDyVector2& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    return *this;
  }

  DDyVector2& operator-=(const DDyVector2& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    return *this;
  }

  DDyVector2& operator*=(const float value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    return *this;
  }

  DDyVector2& operator*=(const DDyVector2& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    return *this;
  }

  ///
  /// If lhs and rhs are DDyVector2, element multiplication happens.
  ///
  DDyVector2& operator/=(const float value) noexcept
  {
    this->X /= value;
    this->Y /= value;
    return *this;
  }

  /// If rhs vector has any 0 value, this function just do nothing.
  DDyVector2& operator/=(const DDyVector2& value) noexcept
  {
    this->X /= value.X;
    this->Y /= value.Y;
    return *this;
  }

  friend bool operator<(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return !(lhs < rhs);
  }

  friend bool operator==(const DDyVector2& lhs, const DDyVector2& rhs) noexcept;
  friend bool operator!=(const DDyVector2& lhs, const DDyVector2& rhs) noexcept;

public:
  /// @brief Do dot product of (x, y) R^2 vector.
  /// @return Dot product float value.
  [[nodiscard]] static float Dot(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
  }

  /// @brief Get linear interpolated DDyVector2 instance.
  /// @param[in] lhs From DDyVector2 vector.
  /// @param[in] rhs To DDyVector2 vector.
  /// @param[in] value float [0, 1] value, it is okay that value is a out of bound.
  /// @return interpolated vec2 value.
  [[nodiscard]] static DDyVector2 Lerp(const DDyVector2& lhs, const DDyVector2& rhs, float value) noexcept
  {
    return lhs * (1.0f - value) + rhs * value;
  }

  /// @brief Compare two DDyVector2 and return max DDyVector2 instance.
  [[nodiscard]] static const DDyVector2& CompareMaxLength(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  /// @brief Compare two DDyVector2 and return max DDyVector2 instance.
  [[nodiscard]] static const DDyVector2& CompareMinLength(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }
};

void to_json(nlohmann::json& j, const DDyVector2& p);
void from_json(const nlohmann::json& j, DDyVector2& p);
