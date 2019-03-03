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

namespace dy
{

class DDyMatrix4x4;

///
/// @struct DDyVector4
/// @brief Float type 4-element vector struct.
///
struct DDyVector4 final
{
  union 
  { 
    __m128 __Simd{}; 
    struct { float X; float Y; float Z; float W; };
  };

  DDyVector4() = default;
  DDyVector4(const DDyVector4&) = default;
  DDyVector4& operator=(const DDyVector4&) = default;

  explicit DDyVector4(const float value) noexcept : X{value}, Y{value}, Z{value}, W{value} {};
  explicit DDyVector4(const DDyVector2& value) noexcept : X{value.X}, Y{value.Y}, Z{0.0f}, W{1.0f} {};
  explicit DDyVector4(const DDyVector3& value) noexcept : X{value.X}, Y{value.Y}, Z{value.Z}, W{1.0f} {};
  DDyVector4(const float x, const float y, const float z) noexcept : X(x), Y(y), Z(z), W{1.0f} {};
  DDyVector4(const float x, const float y, const float z, const float w) noexcept : X(x), Y(y), Z(z), W{w} {};
  DDyVector4(const DDyVector3& iXyz, const float w) : X{iXyz.X}, Y{iXyz.Y}, Z{iXyz.Z}, W{w} {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  DDyVector4(const aiVector2D& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector4(const aiVector3D& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DDyVector4(const glm::vec2& value) noexcept : X{value.x}, Y{value.y} {}
  DDyVector4(const glm::vec3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DDyVector4(const glm::vec4& value) noexcept : X{value.x}, Y{value.y}, Z{value.z}, W{value.w} {}
  DDyVector4(const std::array<TF32, 4>& value) noexcept : X{value[0]}, Y{value[1]}, Z{value[2]}, W{value[3]} {}

  DDyVector4& operator=(const aiVector2D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const aiVector3D& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const glm::vec2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const glm::vec3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const glm::vec4& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = value.w;
    return *this;
  }

  auto& operator[](std::size_t index)
  {
    switch (index)
    {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

  const auto& operator[](std::size_t index) const
  {
    switch (index) {
    case 0: return this->X;
    case 1: return this->Y;
    case 2: return this->Z;
    case 3: return this->W;
    default: throw std::out_of_range("DDyVector2 range is out of bound.");
    }
  }

#if defined(_WIN32)
  explicit DDyVector4(const DirectX::XMFLOAT2& value) noexcept :
      X{value.x}, Y{value.y}, Z{0.0f}, W{1.0f} {}
  explicit DDyVector4(const DirectX::XMFLOAT3& value) noexcept :
      X{value.x}, Y{value.y}, Z{value.z}, W{1.0f} {}
  DDyVector4(const DirectX::XMFLOAT4& value) noexcept :
      X{value.x}, Y{value.y}, Z{value.z}, W{value.w} {}
  DDyVector4(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMFLOAT4 xmVector = {};
    DirectX::XMStoreFloat4(&xmVector, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
    this->Z = xmVector.z;
    this->W = xmVector.w;
  }

  DDyVector4& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const DirectX::XMFLOAT3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = 1.0f;
    return *this;
  }

  DDyVector4& operator=(const DirectX::XMFLOAT4& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = value.w;
    return *this;
  }

  explicit operator DirectX::XMFLOAT2() const noexcept
  {
    return DirectX::XMFLOAT2{this->X, this->Y};
  }

  explicit operator DirectX::XMFLOAT3() const noexcept
  {
    return DirectX::XMFLOAT3{this->X, this->Y, this->Z};
  }

  operator DirectX::XMFLOAT4() const noexcept
  {
    return DirectX::XMFLOAT4{this->X, this->Y, this->Z, this->W};
  }

  operator DirectX::XMVECTOR() const noexcept
  {
    auto xmVector4 = static_cast<DirectX::XMFLOAT4>(*this);
    return XMLoadFloat4(&xmVector4);
  }
#endif /// End defined(_WIN32)

  //!
  //! Conversion operators for dependencies.
  //!

  explicit operator aiVector2D() const noexcept
  {
    return aiVector2D{this->X, this->Y};
  }

  explicit operator aiVector3D() const noexcept
  {
    return aiVector3D{this->X, this->Y, this->Z};
  }

  explicit operator glm::vec2() const noexcept
  {
    return glm::vec2{this->X, this->Y};
  }

  explicit operator glm::vec3() const noexcept
  {
    return glm::vec3{this->X, this->Y, this->Z};
  }

  operator glm::vec4() const noexcept
  {
    return glm::vec4{this->X, this->Y, this->Z, this->W};
  }

  //!
  //! Methods
  //!

  ///
  /// @brief
  /// Return sequence data of this instance.
  ///
  [[nodiscard]] FORCEINLINE std::array<float, 4> Data() const noexcept
  {
    return {this->X, this->Y, this->Z, this->W};
  }

  ///
  /// @brief
  ///
  DDyVector4 MultiplyMatrix(const dy::DDyMatrix4x4& matrix) const noexcept;

  //!
  //! Operators
  //!

  friend DDyVector4 operator+(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.__Simd = _mm_add_ps(lhs.__Simd, rhs.__Simd);
    return lhs;
  }

  friend DDyVector4 operator-(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.__Simd = _mm_sub_ps(lhs.__Simd, rhs.__Simd);
    return lhs;
  }

  /// DDyVector4 $$ v = (x, y, z) $$ and value $$ a $$
  /// $$ av $$.
  friend DDyVector4 operator*(DDyVector4 lhs, const float rhs) noexcept
  {
    lhs.__Simd = _mm_mul_ps(lhs.__Simd, _mm_set_ps(rhs, rhs, rhs, rhs));
    return lhs;
  }

  /// If lhs and rhs are DDyVector4, element multiplication happens.
  friend DDyVector4 operator*(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    lhs.__Simd = _mm_mul_ps(lhs.__Simd, rhs.__Simd);
    return lhs;
  }

  /// If rhs has 0 value, this function just do nothing.
  friend DDyVector4 operator/(DDyVector4 lhs, const float rhs) noexcept
  {
    if (rhs == 0.0f) { MDY_LOG_CRITICAL_D("DDyVector4 could not be divided by {0}.", rhs); }
    else
    {
      lhs.__Simd = _mm_div_ps(lhs.__Simd, _mm_set_ps(rhs, rhs, rhs, rhs));
    }

    return lhs;
  }

  /// If rhs vector has any 0 value, this function just do nothing.
  friend DDyVector4 operator/(DDyVector4 lhs, const DDyVector4& rhs) noexcept
  {
    if (rhs.X == 0.0f || rhs.Y == 0.0f || rhs.Z == 0.0f || rhs.W == 0.0f)
    {
      MDY_LOG_CRITICAL_D("DDyVector4 could not be devided by 0 included DDyVector4, ({0}, {1}, {2})", rhs.X, rhs.Y, rhs.Z);
    }
    else
    {
      lhs.__Simd = _mm_div_ps(lhs.__Simd, rhs.__Simd);
    }

    return lhs;
  }

  DDyVector4& operator+=(const DDyVector4& value) noexcept
  {
    this->__Simd = _mm_add_ps(this->__Simd, value.__Simd);
    return *this;
  }

  DDyVector4& operator-=(const DDyVector4& value) noexcept
  {
    this->__Simd = _mm_sub_ps(this->__Simd, value.__Simd);
    return *this;
  }

  DDyVector4& operator*=(const float value) noexcept
  {
    this->__Simd = _mm_mul_ps(this->__Simd, _mm_set_ps(value, value, value, value));
    return *this;
  }

  DDyVector4& operator*=(const DDyVector4& value) noexcept
  {
    this->__Simd = _mm_sub_ps(this->__Simd, value.__Simd);
    return *this;
  }

  /// If lhs and rhs are DDyVector4, element multiplication happens.
  DDyVector4& operator/=(const float value) noexcept
  {
    if (value == 0.0f)
    {
      MDY_LOG_CRITICAL_D("DDyVector4 could not be divided by {0}.", value);
    }
    else
    {
      this->__Simd = _mm_div_ps(this->__Simd, _mm_set_ps(value, value, value, value));
    }

    return *this;
  }

  /// If rhs vector has any 0 value, this function just do nothing.
  DDyVector4& operator/=(const DDyVector4& value) noexcept
  {
    if (value.X == 0.0f || value.Y == 0.0f || value.Z == 0.0f || value.W == 0.0f)
    {
      MDY_LOG_CRITICAL_D("DDyVector4 could not be devided by 0 included DDyVector4, ({0}, {1}, {2})", value.X, value.Y, value.Z);
    }
    else
    {
      this->__Simd = _mm_div_ps(this->__Simd, value.__Simd);
    }

    return *this;
  }

  friend bool operator==(_MIN_ const DDyVector4& lhs, _MIN_ const DDyVector4& rhs) noexcept;
  friend bool operator!=(_MIN_ const DDyVector4& lhs, _MIN_ const DDyVector4& rhs) noexcept;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR4_H
