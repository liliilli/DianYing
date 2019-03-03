#ifndef GUARD_DY_HELPER_TYPE_VECTOR3_H
#define GUARD_DY_HELPER_TYPE_VECTOR3_H

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Type/Vector3.h
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
#include <nlohmann/json_fwd.hpp>
#include <foundation/PxVec3.h>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Math/Math.h>

namespace dy {
  class DDyMatrix3x3;

///
/// @struct DDyVector3
/// @brief Float type 3-element vector struct.
///
struct DDyVector3 final {
  float X = 0.f;
  float Y = 0.f;
  float Z = 0.f;

  DDyVector3() = default;

  explicit DDyVector3(const float value)        noexcept : X{value}, Y{value}, Z{value} {};
  explicit DDyVector3(const DDyVector2& value)  noexcept : X{value.X}, Y{value.Y}, Z{0.0f} {};
  DDyVector3(const float x, const float y, const float z) noexcept : X(x), Y(y), Z(z) {};
  DDyVector3(const DDyVector3& value)                     noexcept = default;

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVector3(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector3(const aiVector3D& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DDyVector3(const glm::vec2& value)  noexcept : X{value.x}, Y{value.y} {};
  DDyVector3(const glm::vec3& value)  noexcept : X{value.x}, Y{value.y}, Z{value.z} {};
  DDyVector3(const physx::PxVec3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {};

  DDyVector3& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    return *this;
  }

  DDyVector3& operator=(const aiVector3D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  DDyVector3& operator=(const glm::vec2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    return *this;
  }

  DDyVector3& operator=(const glm::vec3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  DDyVector3& operator=(const physx::PxVec3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  DDyVector3& operator=(const DDyVector3& value) noexcept = default;

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator glm::vec2() const noexcept
  {
    return glm::vec2{this->X, this->Y};
  }

  explicit operator aiVector2D() const noexcept
  {
    return aiVector2D{this->X, this->Y};
  }

  operator glm::vec3() const noexcept
  {
    return glm::vec3{this->X, this->Y, this->Z};
  }

  operator aiVector3D() const noexcept
  {
    return aiVector3D{this->X, this->Y, this->Z};
  }

  operator physx::PxVec3() const noexcept
  {
    return physx::PxVec3{this->X, this->Y, this->Z};
  }

#if defined(_WIN32)
  explicit DDyVector3(const DirectX::XMFLOAT2& value) noexcept : X{value.x}, Y{value.y}, Z{0.0f} {}
  DDyVector3(const DirectX::XMFLOAT3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DDyVector3(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMFLOAT3 xmVector = {};
    DirectX::XMStoreFloat3(&xmVector, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
    this->Z = xmVector.z;
  }

  DDyVector3& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    return *this;
  }

  DDyVector3& operator=(const DirectX::XMFLOAT3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    return *this;
  }

  explicit operator DirectX::XMFLOAT2() const noexcept
  {
    return DirectX::XMFLOAT2{this->X, this->Y};
  }

  operator DirectX::XMFLOAT3() const noexcept
  {
    return DirectX::XMFLOAT3{this->X, this->Y, this->Z};
  }

  operator DirectX::XMVECTOR() const noexcept
  {
    auto xmVector = static_cast<DirectX::XMFLOAT3>(*this);
    return XMLoadFloat3(&xmVector);
  }
#endif /// End defined(_WIN32)

  //!
  //! Methods
  //!

  ///
  /// @brief Return sequence data of this instance.
  ///
  [[nodiscard]] std::array<float, 3> Data() const noexcept
  {
    return {this->X, this->Y, this->Z};
  }

  ///
  /// @brief Returns the length of this vector.
  /// @return Length of this DDyVector3.
  ///
  [[nodiscard]] FORCEINLINE float GetLength() const noexcept
  {
    return std::sqrtf(GetSquareLength());
  }

  ///
  /// @brief Return squared length of this vector.
  /// @return Squared length of this DDyVector3.
  ///
  [[nodiscard]] FORCEINLINE float GetSquareLength() const noexcept
  {
    return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
  }

  ///
  /// @brief Return new DDyVector3 instance of normalized input vector.
  /// @return Normalized DDyVector3 vector.
  ///
  [[nodiscard]] FORCEINLINE DDyVector3 Normalize() const noexcept
  {
    const auto length = this->GetLength();
    return {this->X / length, this->Y / length, this->Z / length};
  }

  //!
  //! Operators
  //!

  friend DDyVector3 operator+(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X += rhs.X;
    lhs.Y += rhs.Y;
    lhs.Z += rhs.Z;
    return lhs;
  }

  friend DDyVector3 operator-(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X -= rhs.X;
    lhs.Y -= rhs.Y;
    lhs.Z -= rhs.Z;
    return lhs;
  }

  ///
  /// DDyVector3 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  ///
  friend DDyVector3 operator*(DDyVector3 lhs, const float rhs) noexcept
  {
    lhs.X *= rhs;
    lhs.Y *= rhs;
    lhs.Z *= rhs;
    return lhs;
  }

  ///
  /// If lhs and rhs are DDyVector3, element multiplication happens.
  ///
  friend DDyVector3 operator*(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    lhs.X *= rhs.X;
    lhs.Y *= rhs.Y;
    lhs.Z *= rhs.Z;
    return lhs;
  }

  ///
  /// If rhs has 0 value, this function just do nothing.
  ///
  friend DDyVector3 operator/(DDyVector3 lhs, const float rhs) noexcept
  {
    if (rhs == 0.0f)
    {
      DyPushLogDebugCritical("DDyVector3 could not be divided by {0}.", rhs);
    }
    else
    {
      lhs.X /= rhs;
      lhs.Y /= rhs;
      lhs.Z /= rhs;
    }

    return lhs;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  friend DDyVector3 operator/(DDyVector3 lhs, const DDyVector3& rhs) noexcept
  {
    if (rhs.X == 0.0f || rhs.Y == 0.0f || rhs.Z == 0.0f)
    {
      DyPushLogDebugCritical("DDyVector3 could not be devided by 0 included DDyVector3, ({0}, {1}, {2})", rhs.X, rhs.Y, rhs.Z);
    }
    else
    {
      lhs.X /= rhs.X;
      lhs.Y /= rhs.Y;
      lhs.Z /= rhs.Z;
    }

    return lhs;
  }

  DDyVector3& operator+=(const DDyVector3& value) noexcept
  {
    this->X += value.X;
    this->Y += value.Y;
    this->Z += value.Z;
    return *this;
  }

  DDyVector3& operator-=(const DDyVector3& value) noexcept
  {
    this->X -= value.X;
    this->Y -= value.Y;
    this->Z -= value.Z;
    return *this;
  }

  DDyVector3& operator*=(const float value) noexcept
  {
    this->X *= value;
    this->Y *= value;
    this->Z *= value;
    return *this;
  }

  DDyVector3& operator*=(const DDyVector3& value) noexcept
  {
    this->X *= value.X;
    this->Y *= value.Y;
    this->Z *= value.Z;
    return *this;
  }

  ///
  /// If lhs and rhs are DDyVector3, element multiplication happens.
  ///
  DDyVector3& operator/=(const float value) noexcept
  {
    if (value == 0.0f)
    {
      DyPushLogDebugCritical("DDyVector3 could not be divided by {0}.", value);
    }
    else
    {
      this->X /= value;
      this->Y /= value;
      this->Z /= value;
    }

    return *this;
  }

  ///
  /// If rhs vector has any 0 value, this function just do nothing.
  ///
  DDyVector3& operator/=(const DDyVector3& value) noexcept
  {
    if (value.X == 0.0f || value.Y == 0.0f || value.Z == 0.0f)
    {
      DyPushLogDebugCritical("DDyVector3 could not be devided by 0 included DDyVector3, ({0}, {1}, {2})", value.X, value.Y, value.Z);
    }
    else
    {
      this->X /= value.X;
      this->Y /= value.Y;
      this->Z /= value.Z;
    }

    return *this;
  }

  ///
  /// @brief Multiply with DDyMatrix3x3 as (v^T * M)
  ///
  DDyVector3 MultiplyMatrix(const dy::DDyMatrix3x3& matrix) const noexcept;

  friend bool operator<(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return lhs.GetSquareLength() < rhs.GetSquareLength();
  }

  friend bool operator>(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return rhs < lhs;
  }

  friend bool operator<=(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return !(rhs < lhs);
  }

  friend bool operator>=(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return !(lhs < rhs);
  }

  friend bool operator==(_MIN_ const DDyVector3& lhs, _MIN_ const DDyVector3& rhs) noexcept;
  friend bool operator!=(_MIN_ const DDyVector3& lhs, _MIN_ const DDyVector3& rhs) noexcept;

  //!
  //! Static functions
  //!

  ///
  /// @brief Do dot product of (x, y, z) R^3 vector.
  /// @return Dot product float value.
  ///
  [[nodiscard]] static FORCEINLINE float Dot(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
  }

  ///
  /// @brief Cross product of (x, y, z) R^3 vector.
  /// @return Cross product float value.
  ///
  [[nodiscard]] static FORCEINLINE DDyVector3 Cross(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return
    {
        lhs.Y * rhs.Z - rhs.Y * lhs.Z,
        lhs.Z * rhs.X - rhs.Z * lhs.X,
        lhs.X * rhs.Y - rhs.X * lhs.Y
    };
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @param[in] value
  /// @return
  ///
  [[nodiscard]] static FORCEINLINE DDyVector3 Lerp(const DDyVector3& lhs, const DDyVector3& rhs, float value) noexcept
  {
    return lhs * (1.0f - value) + rhs * value;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  [[nodiscard]] static FORCEINLINE DDyVector3 CompareMaxLength(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return (lhs >= rhs) ? lhs : rhs;
  }

  ///
  /// @brief
  /// @param[in] lhs
  /// @param[in] rhs
  /// @return
  ///
  [[nodiscard]] static FORCEINLINE DDyVector3 CompareMinLength(const DDyVector3& lhs, const DDyVector3& rhs) noexcept
  {
    return (lhs < rhs) ? lhs : rhs;
  }

  ///
  /// @brief Check if vector DDyVector3 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] static bool IsAllZero(const DDyVector3& vector) noexcept;

  ///
  /// @brief Return {0, 0, 1} front DDyVector3 vector.
  ///
  static DDyVector3 FrontZ() noexcept
  {
    static DDyVector3 ret{0, 0, 1};
    return ret;
  }

  ///
  /// @brief Return {1, 0, 0} right DDyVector3 vector.
  ///
  static DDyVector3 RightX() noexcept
  {
    static DDyVector3 ret{1, 0, 0};
    return ret;
  }

  ///
  /// @brief Return {0, 1, 0} up DDyVector3 vector.
  ///
  static DDyVector3 UpY() noexcept
  {
    static DDyVector3 ret{0, 1, 0};
    return ret;
  }

  ///
  /// @brief Check if this DDyVector3 is all zero or nearly equal to zero.
  ///
  [[nodiscard]] bool IsAllZero() const noexcept;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyVector3& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyVector3& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR3_H
