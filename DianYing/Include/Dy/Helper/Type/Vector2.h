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

#include <Dy/Helper/macroes.h>
#include <Dy/Helper/Math/Math.h>

#if defined(_WIN32)
#include <DirectXMath.h>
#endif

namespace dy {
  class DDyMatrix2x2;

  ///
/// @struct DVector2
/// @brief Float type 2-element vector struct.
///
struct DVector2 final {
  float X = 0.f;
  float Y = 0.f;

  DVector2() = default;

  DVector2(const float x, const float y) noexcept : X{x}, Y{y} {};
  explicit DVector2(const float value) noexcept : X{value}, Y{value} {}

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DVector2(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DVector2(const glm::vec2& value) noexcept : X{value.x}, Y{value.y} {}

  DVector2& operator=(const aiVector2D& value) noexcept
  {
    X = value.x;
    Y = value.y;
    return *this;
  }

  DVector2& operator=(const glm::vec2& value) noexcept
  {
    X = value.x;
    Y = value.y;
    return *this;
  }

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return X;
    case 1: return Y;
    default: throw std::out_of_range("DVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index) {
    case 0: return X;
    case 1: return Y;
    default: throw std::out_of_range("DVector2 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  operator aiVector2D() const noexcept
  {
    return aiVector2D{X, Y};
  }

  operator glm::vec2() const noexcept
  {
    return glm::vec2{X, Y};
  }

#if defined(_WIN32)
  DVector2(const DirectX::XMFLOAT2& value) noexcept : X{value.x}, Y{value.y} {}
  DVector2(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMFLOAT2 xmVector = {};
    DirectX::XMStoreFloat2(&xmVector, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
  }

  DVector2& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    X = value.x;
    Y = value.y;
    return *this;
  }

  operator DirectX::XMFLOAT2() const noexcept
  {
    return DirectX::XMFLOAT2{X, Y};
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
  /// @brief Return one-dimensional data chunk of DVector2.
  ///
  FORCEINLINE std::array<float, 2> Data() const noexcept {
    return {X, Y};
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DVector2.
  ///
  FORCEINLINE float GetSquareLength() const noexcept {
    return X * X + Y * Y;
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DVector2.
  ///
  FORCEINLINE float GetLength() const noexcept {
    return std::sqrtf(this->GetSquareLength());
  }

  ///
  /// @brief Return new DVector2 instance of normalized input vector.
  /// @return Normalized DVector2 vector.
  ///
  FORCEINLINE DVector2 Normalize() const noexcept {
    const auto length = this->GetLength();
    return {X / length, Y / length};
  }

  //!
  //! Operators
  //!

  friend DVector2 operator+(DVector2 lhs, const DVector2& rhs) noexcept {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    return lhs;
  }

  friend DVector2 operator-(DVector2 lhs, const DVector2& rhs) noexcept {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    return lhs;
  }

  ///
  /// DVector2 $$ v = (x, y) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVector2 operator*(DVector2 lhs, const float rhs) noexcept {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVector2, element multiplication happens.
  ///
  friend DVector2 operator*(DVector2 lhs, const DVector2& rhs) noexcept {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVector2 operator/(DVector2 lhs, const float rhs) noexcept {
    if (rhs == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT("DVector2 could not be divided by {0}.", rhs);
#endif
    }
    else {
      lhs.X /= rhs;
      lhs.Y /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVector2 operator/(DVector2 lhs, const DVector2& rhs) noexcept {
    if (rhs.X == 0.0f || rhs.Y == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT(
          "DVector2 could not be devided by 0 included DVector2, ({0}, {1})",
          rhs.X, rhs.Y);
#endif
    }
    else {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
    }

    return lhs;
  }

  DVector2& operator+=(const DVector2& value) noexcept {
    this->X += value.X;
    this->Y += value.Y;
    return *this;
  }

  DVector2& operator-=(const DVector2& value) noexcept {
    this->X -= value.X;
    this->Y -= value.Y;
    return *this;
  }

  DVector2& operator*=(const float value) noexcept {
    this->X *= value;
    this->Y *= value;
    return *this;
  }

  DVector2& operator*=(const DVector2& value) noexcept {
    this->X *= value.X;
    this->Y *= value.Y;
    return *this;
  }

  ///
  /// If lhs and rhs are DVector2, element multiplication happens.
  ///
  DVector2& operator/=(const float value) noexcept {
    if (value == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT("DVector2 could not be divided by {0}.", value);
#endif
    }
    else {
      this->X /= value;
      this->Y /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVector2& operator/=(const DVector2& value) noexcept {
    if (value.X == 0.0f || value.Y == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT(
          "DVector2 could not be devided by 0 included DVector2, ({0}, {1})",
          value.X, value.Y);
#endif
    }
    else {
      this->X /= value.X;
      this->Y /= value.Y;
    }

    return *this;
  }

  ///
  /// @brief
  ///
  DVector2 MultiplyMatrix(const dy::DDyMatrix2x2& matrix) const noexcept;

  ///
  /// @brief Compare length of two vectors and return if they are same length.
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return Equal flag.
  ///
  friend bool operator==(const DVector2& lhs, const DVector2& rhs) noexcept {
    return math::IsNearlyEqual(lhs.GetSquareLength(), rhs.GetSquareLength(), 0.001f);
  }

private:
  friend bool operator<(const DVector2& lhs, const DVector2& rhs) noexcept {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVector2& lhs, const DVector2& rhs) noexcept {
    return rhs < lhs;
  }

  friend bool operator<=(const DVector2& lhs, const DVector2& rhs) noexcept {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVector2& lhs, const DVector2& rhs) noexcept {
    return !(lhs < rhs);
  }

public:

  ///
  /// @brief Check if this DVector2 is all zero or nearly equal to zero.
  ///
  bool IsAllZero() const noexcept;

  //!
  //! Static functions
  //!
public:

  ///
  /// @brief Do dot product of (x, y) R^2 vector.
  /// @return Dot product float value.
  ///
  static float Dot(const DVector2& lhs, const DVector2& rhs) noexcept {
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] value
  /// @return
  ///
  static DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float value) noexcept {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVector2 CompMaxLength(const DVector2& lhs, const DVector2& rhs) noexcept {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  static DVector2 CompMinLength(const DVector2& lhs, const DVector2& rhs) noexcept {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] normalized
  /// @return
  ///
  static float AngleRad(const DVector2& lhs, const DVector2& rhs,
                        bool normalized = true) noexcept {
    auto nlhs = lhs;
    auto nrhs = rhs;
    if (!normalized) {
      nlhs = lhs.Normalize();
      nrhs = rhs.Normalize();
    }

    const auto cos = nrhs.X * nlhs.X + nrhs.Y * nlhs.Y;
    const auto sin = nrhs.Y * nlhs.X - nrhs.X * nlhs.Y;
    const auto degree = std::acosf(cos);

    return (sin < 0.0f) ? -degree : degree;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] normalized
  /// @return
  ///
  static float AngleDeg(const DVector2& lhs, const DVector2& rhs,
                        bool normalized = true) noexcept {
    return AngleRad(lhs, rhs, normalized) * 180.f / 3.1415926535f;
  }

  ///
  /// @brief Check if vector DVector2 is all zero or nearly equal to zero.
  ///
  static bool IsAllZero(const DVector2& vector) noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR2_H