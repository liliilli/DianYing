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
#include <glm/glm.hpp>

#include <Dy/Helper/Type/DVector2.h>
#include <Dy/Helper/Type/DVector3.h>

namespace dy
{

class DMatrix4x4;

/// @struct DVector4
/// @brief Float type 4-element vector struct.
struct DVector4 final
{
  union 
  { 
    __m128 __Simd{}; 
    struct { float X; float Y; float Z; float W; };
  };

  DVector4() = default;
  DVector4(const DVector4&) = default;
  DVector4& operator=(const DVector4&) = default;

  explicit DVector4(const float value) noexcept : X{value}, Y{value}, Z{value}, W{value} {};
  explicit DVector4(const DVector2& value) noexcept : X{value.X}, Y{value.Y}, Z{0.0f}, W{1.0f} {};
  explicit DVector4(const DVector3& value) noexcept : X{value.X}, Y{value.Y}, Z{value.Z}, W{1.0f} {};
  DVector4(const float x, const float y, const float z) noexcept : X(x), Y(y), Z(z), W{1.0f} {};
  DVector4(const float x, const float y, const float z, const float w) noexcept : X(x), Y(y), Z(z), W{w} {};
  DVector4(const DVector3& iXyz, const float w) : X{iXyz.X}, Y{iXyz.Y}, Z{iXyz.Z}, W{w} {};

  //!
  //! Constructor and assign operator for dependencies.
  //!

  explicit DVector4(const glm::vec2& value) noexcept : X{value.x}, Y{value.y} {}
  explicit DVector4(const glm::vec3& value) noexcept : X{value.x}, Y{value.y}, Z{value.z} {}
  DVector4(const glm::vec4& value) noexcept : X{value.x}, Y{value.y}, Z{value.z}, W{value.w} {}
  DVector4(const std::array<TF32, 4>& value) noexcept : X{value[0]}, Y{value[1]}, Z{value[2]}, W{value[3]} {}

  DVector4& operator=(const glm::vec3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = 1.0f;
    return *this;
  }

  DVector4& operator=(const glm::vec4& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = value.w;
    return *this;
  }

  TF32& operator[](std::size_t index);

  const TF32& operator[](std::size_t index) const;

#if defined(_WIN32)
  explicit DVector4(const DirectX::XMFLOAT2& value) noexcept :
      X{value.x}, Y{value.y}, Z{0.0f}, W{1.0f} {}
  explicit DVector4(const DirectX::XMFLOAT3& value) noexcept :
      X{value.x}, Y{value.y}, Z{value.z}, W{1.0f} {}
  DVector4(const DirectX::XMFLOAT4& value) noexcept :
      X{value.x}, Y{value.y}, Z{value.z}, W{value.w} {}
  DVector4(DirectX::FXMVECTOR value) noexcept
  {
    DirectX::XMFLOAT4 xmVector = {};
    DirectX::XMStoreFloat4(&xmVector, value);
    this->X = xmVector.x;
    this->Y = xmVector.y;
    this->Z = xmVector.z;
    this->W = xmVector.w;
  }

  DVector4& operator=(const DirectX::XMFLOAT2& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = 0.0f;
    this->W = 1.0f;
    return *this;
  }

  DVector4& operator=(const DirectX::XMFLOAT3& value) noexcept
  {
    this->X = value.x;
    this->Y = value.y;
    this->Z = value.z;
    this->W = 1.0f;
    return *this;
  }

  DVector4& operator=(const DirectX::XMFLOAT4& value) noexcept
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

  /// @brief Return sequence data of this instance.
  MDY_NODISCARD const TF32* Data() const noexcept;

  /// @brief Return sequence data of this instance.
  MDY_NODISCARD TF32* Data() noexcept;

  /// @brief Multiply with matrix and return new vector4.
  /// If origin vector is V, and matrix is M, so new vector4 V' = (V^T)M.
  DVector4 MultiplyMatrix(const DMatrix4x4& matrix) const noexcept;

  //!
  //! Operators
  //!

  DVector4& operator+=(const DVector4& value) noexcept;

  DVector4& operator-=(const DVector4& value) noexcept;

  DVector4& operator*=(const float value) noexcept;

  DVector4& operator*=(const DVector4& value) noexcept;

  /// If lhs and rhs are DVector4, element multiplication happens.
  DVector4& operator/=(const float value) noexcept;

  /// If rhs vector has any 0 value, this function just do nothing.
  DVector4& operator/=(const DVector4& value) noexcept;
};

DVector4 operator+(DVector4 lhs, const DVector4& rhs) noexcept;
DVector4 operator-(DVector4 lhs, const DVector4& rhs) noexcept;

/// DVector4 $$ v = (x, y, z) $$ and value $$ a $$ $$ av $$.
DVector4 operator*(DVector4 lhs, const float rhs) noexcept;
/// DVector4 $$ v = (x, y, z) $$ and value $$ a $$ $$ av $$.
DVector4 operator*(TF32 lhs, const DVector4& rhs) noexcept;
/// If lhs and rhs are DVector4, element multiplication happens.
DVector4 operator*(DVector4 lhs, const DVector4& rhs) noexcept;

/// If rhs has 0 value, this function just do nothing.
DVector4 operator/(DVector4 lhs, const float rhs) noexcept;
/// If rhs vector has any 0 value, this function just do nothing.
DVector4 operator/(DVector4 lhs, const DVector4& rhs) noexcept;

bool operator==(const DVector4& lhs, const DVector4& rhs) noexcept;
bool operator!=(const DVector4& lhs, const DVector4& rhs) noexcept;

void to_json(nlohmann::json& oJson, const DVector4& iVector);
void from_json(const nlohmann::json& iJson, DVector4& oVector);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR4_H