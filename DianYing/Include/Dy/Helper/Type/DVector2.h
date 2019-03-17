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

#include <Dy/Helper/System/Macroes.h>
#include <Dy/Helper/Math/Math.h>
#include <Dy/Management/MLog.h>
#include <nlohmann/json_fwd.hpp>

#if defined(_WIN32)
#include <DirectXMath.h>
#endif

//!
//! Forward declaration
//!

namespace dy
{
class DMatrix2x2;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{
///
/// @struct DVector2
/// @brief Float type 2-element vector struct.
///
struct DVector2 final 
{
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
  DVector2(const DVector2& value) noexcept : X{value.X}, Y{value.Y} {}

  DVector2& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  DVector2& operator=(const glm::vec2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    return *this;
  }

  DVector2& operator=(const DVector2& value) noexcept
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
    default: throw std::out_of_range("DVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    default: throw std::out_of_range("DVector2 range is out of bound.");
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

  /// @brief Return one-dimensional data chunk of DVector2.
  MDY_NODISCARD TF32* Data() noexcept;
  /// @brief Return one-dimensional data chunk of DVector2.
  MDY_NODISCARD const TF32* Data() const noexcept;

  /// @brief Return squared length of this vector.
  /// @return Squared length of this DVector2.
  MDY_NODISCARD TF32 GetSquareLength() const noexcept;

  /// @brief Returns the length of this vector.
  /// @return Length of this DVector2.
  MDY_NODISCARD TF32 GetLength() const noexcept
  {
    return std::sqrtf(this->GetSquareLength());
  }

  ///
  /// @brief Return new DVector2 instance of normalized input vector.
  /// @return Normalized DVector2 vector.
  ///
  [[nodiscard]] FORCEINLINE DVector2 Normalize() const noexcept
  {
    const auto length = this->GetLength();
    return {this->X / length, this->Y / length};
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
    if (rhs == 0.0f)
    {
      DyPushLogDebugCritical("DVector2 could not be divided by {0}.", rhs);
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
  friend DVector2 operator/(DVector2 lhs, const DVector2& rhs) noexcept {
    if (rhs.X == 0.0f || rhs.Y == 0.0f)
    {
      DyPushLogDebugCritical( "DVector2 could not be devided by 0 included DVector2, ({0}, {1})", rhs.X, rhs.Y);
    }
    else
    {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
    }

    return lhs;
  }

  DVector2& operator+=(const DVector2& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    return *this;
  }

  DVector2& operator-=(const DVector2& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    return *this;
  }

  DVector2& operator*=(const float value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    return *this;
  }

  DVector2& operator*=(const DVector2& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    return *this;
  }

  ///
  /// If lhs and rhs are DVector2, element multiplication happens.
  ///
  DVector2& operator/=(const float value) noexcept
  {
    if (value == 0.0f)
    {
      DyPushLogDebugCritical("DVector2 could not be divided by {0}.", value);
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
  DVector2& operator/=(const DVector2& value) noexcept
  {
    if (value.X == 0.0f || value.Y == 0.0f)
    {
      DyPushLogDebugCritical("DVector2 could not be devided by 0 included DVector2, ({0}, {1})", value.X, value.Y);
    }
    else
    {
      this->X /= value.X;
      this->Y /= value.Y;
    }

    return *this;
  }

  ///
  /// @brief Multiply with DMatrix2x2 as (v^T * M)
  ///
  DVector2 MultiplyMatrix(const dy::DMatrix2x2& matrix) const noexcept;

  friend bool operator<(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return !(lhs < rhs);
  }

  friend bool operator==(_MIN_ const DVector2& lhs, _MIN_ const DVector2& rhs) noexcept;
  friend bool operator!=(_MIN_ const DVector2& lhs, _MIN_ const DVector2& rhs) noexcept;

public:

  ///
  /// @brief Check if this DVector2 is all zero or nearly equal to zero.
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
  [[nodiscard]] static FORCEINLINE float Dot(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return lhs.X * rhs.X + lhs.Y * rhs.Y;
  }

  ///
  /// @brief Get linear interpolated DVector2 instance.
  /// @param[in] lhs From DVector2 vector.
  /// @param[in] rhs To DVector2 vector.
  /// @param[in] value float [0, 1] value, it is okay that value is a out of bound.
  /// @return interpolated vec2 value.
  ///
  [[nodiscard]] static FORCEINLINE DVector2 Lerp(const DVector2& lhs, const DVector2& rhs, float value) noexcept
  {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief Compare two DVector2 and return max DVector2 instance.
  ///
  [[nodiscard]] static FORCEINLINE const DVector2& CompareMaxLength(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief Compare two DVector2 and return max DVector2 instance.
  ///
  [[nodiscard]] static FORCEINLINE const DVector2& CompareMinLength(const DVector2& lhs, const DVector2& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief Check if vector DVector2 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] static bool IsAllZero(const DVector2& vector) noexcept;
};

void to_json(nlohmann::json& j, const DVector2& p);
void from_json(const nlohmann::json& j, DVector2& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR2_H