#ifndef GUARD_DY_HELPER_TYPE_VECTOR2_H
#define GUARD_DY_HELPER_TYPE_VECTOR2_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Type/vector2.h
///
/// @brief
/// Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

#include <array>
#include <stdexcept>

#include <assimp/vector2.h>
#include <glm/glm.hpp>

#include <Dy/Helper/Macroes.h>
#include <Dy/Helper/Math/Math.h>
#include <Dy/Management/LoggingManager.h>

#if defined(_WIN32)
#include <DirectXMath.h>
#endif

namespace dy {
  class DDyMatrix2x2;

  ///
/// @struct DDyVector2
/// @brief Float type 2-element vector struct.
///
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
  DDyVector2(const glm::vec2& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector2(const DDyVector2& value) noexcept = default;

  DDyVector2& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  DDyVector2& operator=(const glm::vec2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  DDyVector2& operator=(const DDyVector2& value) noexcept = default;

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

  operator glm::vec2() const noexcept
  {
    return glm::vec2{this->X, this->Y};
  }

#if defined(_WIN32)
  DDyVector2(const DirectX::XMFLOAT2& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector2(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMFLOAT2 xmVector = {};
    DirectX::XMStoreFloat2(&xmVector, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
  }

  DDyVector2& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  operator DirectX::XMFLOAT2() const noexcept
  {
    return DirectX::XMFLOAT2{this->X, this->Y};
  }

  operator DirectX::XMVECTOR() const noexcept
  {
    auto xmVector = static_cast<DirectX::XMFLOAT2>(*this);
    return XMLoadFloat2(&xmVector);
  }
#endif /// End defined(_WIN32)

  //!
  //! Methods
  //!

  ///
  /// @brief Return one-dimensional data chunk of DDyVector2.
  ///
  [[nodiscard]] FORCEINLINE std::array<float, 2> Data() const noexcept
  {
    return {this->X, this->Y};
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DDyVector2.
  ///
  [[nodiscard]] FORCEINLINE float GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y;
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DDyVector2.
  ///
  [[nodiscard]] FORCEINLINE float GetLength() const noexcept
  {
    return std::sqrtf(this->GetSquareLength());
  }

  ///
  /// @brief Return new DDyVector2 instance of normalized input vector.
  /// @return Normalized DDyVector2 vector.
  ///
  [[nodiscard]] FORCEINLINE DDyVector2 Normalize() const noexcept
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
    if (rhs == 0.0f)
    {
      MDY_LOG_CRITICAL_D("DDyVector2 could not be divided by {0}.", rhs);
    }
    else
    {
      lhs.X /= rhs;
      lhs.Y /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVector2 operator/(DDyVector2 lhs, const DDyVector2& rhs) noexcept {
    if (rhs.X == 0.0f || rhs.Y == 0.0f)
    {
      MDY_LOG_CRITICAL_D( "DDyVector2 could not be devided by 0 included DDyVector2, ({0}, {1})", rhs.X, rhs.Y);
    }
    else
    {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
    }

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
    if (value == 0.0f)
    {
      MDY_LOG_CRITICAL_D("DDyVector2 could not be divided by {0}.", value);
    }
    else
    {
      this->X /= value;
      this->Y /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DDyVector2& operator/=(const DDyVector2& value) noexcept
  {
    if (value.X == 0.0f || value.Y == 0.0f)
    {
      MDY_LOG_CRITICAL_D("DDyVector2 could not be devided by 0 included DDyVector2, ({0}, {1})", value.X, value.Y);
    }
    else
    {
      this->X /= value.X;
      this->Y /= value.Y;
    }

    return *this;
  }

  ///
  /// @brief Multiply with DDyMatrix2x2 as (v^T * M)
  ///
  DDyVector2 MultiplyMatrix(const dy::DDyMatrix2x2& matrix) const noexcept;

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return Equal flag.
  ///
  friend bool operator==(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return math::IsNearlyEqual(lhs.GetSquareLength(), rhs.GetSquareLength(), 0.001f);
  }

private:
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

public:

  ///
  /// @brief Check if this DDyVector2 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] bool IsAllZero() const noexcept;

  //!
  //! Static functions
  //!
public:
  ///
  /// @brief Do dot product of (x, y) R^2 vector.
  /// @return Dot product float value.
  ///
  [[nodiscard]] static FORCEINLINE float Dot(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
  }

  ///
  /// @brief Get linear interpolated DDyVector2 instance.
  /// @param[in] lhs From DDyVector2 vector.
  /// @param[in] rhs To DDyVector2 vector.
  /// @param[in] value float [0, 1] value, it is okay that value is a out of bound.
  /// @return interpolated vec2 value.
  ///
  [[nodiscard]] static FORCEINLINE DDyVector2 Lerp(const DDyVector2& lhs, const DDyVector2& rhs, float value) noexcept
  {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief Compare two DDyVector2 and return max DDyVector2 instance.
  ///
  [[nodiscard]] static FORCEINLINE const DDyVector2& CompareMaxLength(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief Compare two DDyVector2 and return max DDyVector2 instance.
  ///
  [[nodiscard]] static FORCEINLINE const DDyVector2& CompareMinLength(const DDyVector2& lhs, const DDyVector2& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief Check if vector DDyVector2 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] static bool IsAllZero(const DDyVector2& vector) noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR2_H