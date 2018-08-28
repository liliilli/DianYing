#ifndef GUARD_DY_HELPER_TYPE_VECTOR4_H
#define GUARD_DY_HELPER_TYPE_VECTOR4_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Type/Vector4.h
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

#include <assimp/vector3.h>
#include <glm/glm.hpp>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/Vector3.h>

namespace dy {
///
/// @struct DVector4
/// @brief Float type 4-element vector struct.
///
struct DVector4 final {
  float X = 0.f;
  float Y = 0.f;
  float Z = 0.f;
  float W = 1.f;

  DVector4() = default;

  explicit DVector4(const float value) noexcept : X{value}, Y{value}, Z{value}, W{value} {};
  explicit DVector4(const DVector2& value) noexcept : X{value.X}, Y{value.Y}, Z{0.0f}, W{1.0f} {};
  explicit DVector4(const DVector3& value) noexcept : X{value.X}, Y{value.Y}, Z{value.Z}, W{1.0f} {};
  DVector4(const float x, const float y) noexcept : X(x), Y(y), Z(0.0f), W{1.0f} {};
  DVector4(const float x, const float y, const float z) noexcept : X(x), Y(y), Z(z), W{1.0f} {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DVector4(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DVector4(const aiVector3D& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DVector4(const glm::vec2& value) noexcept : X{value.x}, Y{value.y} {}
  DVector4(const glm::vec3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DVector4(const glm::vec4& value) noexcept : X{value.x}, Y{value.y}, Z{value.z}, W{value.w} {}

  DVector4& operator=(const aiVector2D& value) noexcept {
    X = value.x;
    Y = value.y;
    Z = 0.0f;
    W = 1.0f;
    return *this;
  }

  DVector4& operator=(const aiVector3D& value) noexcept {
    X = value.x;
    Y = value.y;
    Z = value.z;
    W = 1.0f;
    return *this;
  }

  DVector4& operator=(const glm::vec2& value) noexcept {
    X = value.x;
    Y = value.y;
    Z = 0.0f;
    W = 1.0f;
    return *this;
  }

  DVector4& operator=(const glm::vec3& value) noexcept {
    X = value.x;
    Y = value.y;
    Z = value.z;
    W = 1.0f;
    return *this;
  }

  DVector4& operator=(const glm::vec4& value) noexcept {
    X = value.x;
    Y = value.y;
    Z = value.z;
    W = value.w;
    return *this;
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator aiVector2D() const noexcept
  {
    return aiVector2D{X, Y};
  }

  explicit operator aiVector3D() const noexcept
  {
    return aiVector3D{X, Y, Z};
  }

  explicit operator glm::vec2() const noexcept
  {
    return glm::vec2{X, Y};
  }

  explicit operator glm::vec3() const noexcept
  {
    return glm::vec3{X, Y, Z};
  }

  operator glm::vec4() const noexcept
  {
    return glm::vec4{X, Y, Z, W};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief
  /// Return sequence data of this instance.
  ///
  std::array<float, 4> Data() const noexcept {
    return {X, Y, Z, W};
  }

  //!
  //! Operators
  //!

  friend DVector4 operator+(DVector4 lhs, const DVector4& rhs) noexcept {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    lhs.Z += rhs.Z;
    lhs.W += rhs.W;
    return lhs;
  }

  friend DVector4 operator-(DVector4 lhs, const DVector4& rhs) noexcept {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    lhs.Z -= rhs.Z;
    lhs.W -= rhs.W;
    return lhs;
  }

  ///
  /// DVector4 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DVector4 operator*(DVector4 lhs, const float rhs) noexcept {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    lhs.Z *= rhs;
    lhs.W *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DVector4, element multiplication happens.
  ///
  friend DVector4 operator*(DVector4 lhs, const DVector4& rhs) noexcept {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    lhs.Z *= rhs.Z;
    lhs.W *= rhs.W;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DVector4 operator/(DVector4 lhs, const float rhs) noexcept {
    if (rhs == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT("DVector4 could not be divided by {0}.", rhs);
#endif
    }
    else {
      lhs.X /= rhs;
      lhs.Y /= rhs;
      lhs.Z /= rhs;
      lhs.W /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DVector4 operator/(DVector4 lhs, const DVector4& rhs) noexcept {
    if (rhs.X == 0.0f || rhs.Y == 0.0f || rhs.Z == 0.0f || rhs.W == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT(
          "DVector4 could not be devided by 0 included DVector4, ({0}, {1}, {2})",
          rhs.X, rhs.Y, rhs.Z);
#endif
    }
    else {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
      lhs.Z /= rhs.Z;
      lhs.W /= rhs.W;
    }

    return lhs;
  }

  DVector4& operator+=(const DVector4& value) noexcept {
    this->X += value.X;
    this->Y += value.Y;
    this->Z += value.Z;
    this->W += value.W;
    return *this;
  }

  DVector4& operator-=(const DVector4& value) noexcept {
    this->X -= value.X;
    this->Y -= value.Y;
    this->Z -= value.Z;
    this->W -= value.W;
    return *this;
  }

  DVector4& operator*=(const float value) noexcept {
    this->X *= value;
    this->Y *= value;
    this->Z *= value;
    this->W *= value;
    return *this;
  }

  DVector4& operator*=(const DVector4& value) noexcept {
    this->X *= value.X;
    this->Y *= value.Y;
    this->Z *= value.Z;
    this->W *= value.W;
    return *this;
  }

  ///
  /// If lhs and rhs are DVector4, element multiplication happens.
  ///
  DVector4& operator/=(const float value) noexcept {
    if (value == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT("DVector4 could not be divided by {0}.", value);
#endif
    }
    else {
      this->X /= value;
      this->Y /= value;
      this->Z /= value;
      this->W /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DVector4& operator/=(const DVector4& value) noexcept {
    if (value.X == 0.0f || value.Y == 0.0f || value.Z == 0.0f || value.W == 0.0f) {
#ifdef false
      PUSH_LOG_CRITICAL_EXT(
          "DVector4 could not be devided by 0 included DVector4, ({0}, {1}, {2})",
          value.X, value.Y, value.Z);
#endif
    }
    else {
      this->X /= value.X;
      this->Y /= value.Y;
      this->Z /= value.Z;
      this->W /= value.W;
    }

    return *this;
  }

  ///
  /// @brief Check if this DVector4 is all zero or nearly equal to zero.
  ///
  bool IsAllZero() const noexcept;

  ///
  /// @brief Check if vector DVector4 is all zero or nearly equal to zero.
  ///
  static bool IsAllZero(const DVector4& vector) noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR4_H
