#ifndef GUARD_DY_HELPER_TYPE_VECTOR2_H
#define GUARD_DY_HELPER_TYPE_VECTOR2_H

#include <array>
#include <nlohmann/json.hpp>

namespace dy
{
///
/// @struct DDyVector2
/// @brief Float type 2-element vector struct.
///
struct DDyVector2 final
{
  float X = 0.f;
  float Y = 0.f;

  DDyVector2() = default;

  DDyVector2(const float x, const float y) noexcept : X{x}, Y{y} {};
  explicit DDyVector2(const float value) noexcept : X{value}, Y{value} {}

  //!
  //! Constructor and assign operator for dependencies.
  //!

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
  //! Methods
  //!

  ///
  /// @brief Return one-dimensional data chunk of DDyVector2.
  ///
  std::array<float, 2> Data() const noexcept { return {this->X, this->Y}; }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DDyVector2.
  ///
  float GetSquareLength() const noexcept { return this->X * this->X + this->Y * this->Y; }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DDyVector2.
  ///
  float GetLength() const noexcept { return std::sqrtf(this->GetSquareLength()); }

  ///
  /// @brief Return new DDyVector2 instance of normalized input vector.
  /// @return Normalized DDyVector2 vector.
  ///
  DDyVector2 Normalize() const noexcept
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
  friend DDyVector2 operator/(DDyVector2 lhs, const float rhs) noexcept {
    Q_ASSERT(rhs != 0.0f);

    lhs.X /= rhs;
    lhs.Y /= rhs;
    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVector2 operator/(DDyVector2 lhs, const DDyVector2& rhs) noexcept {
    Q_ASSERT(rhs.X == 0.0f || rhs.Y == 0.0f);

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
};

inline void to_json(nlohmann::json& j, const DDyVector2& p)
{
  j = nlohmann::json
  {
      {"X", p.X},
      {"Y", p.Y}
  };
}

inline void from_json(const nlohmann::json& j, DDyVector2& p)
{
  p.X = j.at("X").get<float>();
  p.Y = j.at("Y").get<float>();
}

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR2_H