#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/Vector4.cc
///
/// @brief Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

/// Header file
#include <Dy/Helper/Type/Vector4.h>

#include <Dy/Helper/Math/Math.h>
#include <Dy/Helper/Type/Matrix4.h>

namespace dy {

TF32& DDyVector4::operator[](std::size_t index)
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

const TF32& DDyVector4::operator[](std::size_t index) const
{
  switch (index) {
  case 0: return this->X;
  case 1: return this->Y;
  case 2: return this->Z;
  case 3: return this->W;
  default: throw std::out_of_range("DDyVector2 range is out of bound.");
  }
}

const TF32* DDyVector4::Data() const noexcept
{
  return &this->X;
}

TF32* DDyVector4::Data() noexcept
{
  return &this->X;
}

DDyVector4 DDyVector4::MultiplyMatrix(const dy::DDyMatrix4x4& matrix) const noexcept
{
  DDyVector4 result{};

  for (size_t i = 0; i < 4; ++i)
  {
    float value[4];
    _mm_store_ps(value, _mm_mul_ps(this->__Simd, matrix[i].__Simd));
    result[i] += value[0] + value[1] + value[2] + value[3];
  }

  return result;
}

DDyVector4& DDyVector4::operator+=(const DDyVector4& value) noexcept
{
  this->__Simd = _mm_add_ps(this->__Simd, value.__Simd);
  return *this;
}

DDyVector4& DDyVector4::operator-=(const DDyVector4& value) noexcept
{
  this->__Simd = _mm_add_ps(this->__Simd, value.__Simd);
  return *this;
}

DDyVector4& DDyVector4::operator*=(const float value) noexcept
{
  this->__Simd = _mm_mul_ps(this->__Simd, _mm_set_ps(value, value, value, value));
  return *this;
}

DDyVector4& DDyVector4::operator*=(const DDyVector4& value) noexcept
{
  this->__Simd = _mm_sub_ps(this->__Simd, value.__Simd);
  return *this;
}

DDyVector4& DDyVector4::operator/=(const float value) noexcept
{
  if (value == 0.0f)
  {
    DyPushLogDebugCritical("DDyVector4 could not be divided by {0}.", value);
  }
  else
  {
    this->__Simd = _mm_div_ps(this->__Simd, _mm_set_ps(value, value, value, value));
  }

  return *this;
}

DDyVector4& DDyVector4::operator/=(const DDyVector4& value) noexcept
{
  if (value.X == 0.0f || value.Y == 0.0f || value.Z == 0.0f || value.W == 0.0f)
  {
    DyPushLogDebugCritical(
      "DDyVector4 could not be devided by 0 included DDyVector4, ({0}, {1}, {2}, {3})", 
      value.X, value.Y, value.Z, value.W);
  }
  else
  {
    this->__Simd = _mm_div_ps(this->__Simd, value.__Simd);
  }

  return *this;
}

void to_json(nlohmann::json& oJson, const DDyVector4& iVector)
{
  oJson = nlohmann::json
  { 
    {"X", iVector.X}, {"Y", iVector.Y}, {"Z", iVector.Z}, {"W", iVector.W}
  };
}

void from_json(const nlohmann::json& iJson, DDyVector4& oVector)
{
  DyJsonGetValueFromTo(iJson, "X", oVector.X);
  DyJsonGetValueFromTo(iJson, "Y", oVector.Y);
  DyJsonGetValueFromTo(iJson, "Z", oVector.Z);
  DyJsonGetValueFromTo(iJson, "W", oVector.W);
}

DDyVector4 operator+(DDyVector4 lhs, const DDyVector4& rhs) noexcept
{
  lhs.__Simd = _mm_add_ps(lhs.__Simd, rhs.__Simd);
  return lhs;
}

DDyVector4 operator-(DDyVector4 lhs, const DDyVector4& rhs) noexcept
{
  lhs.__Simd = _mm_sub_ps(lhs.__Simd, rhs.__Simd);
  return lhs;
}

DDyVector4 operator*(DDyVector4 lhs, const float rhs) noexcept
{
  lhs.__Simd = _mm_mul_ps(lhs.__Simd, _mm_set_ps(rhs, rhs, rhs, rhs));
  return lhs;
}

DDyVector4 operator*(TF32 lhs, const DDyVector4& rhs) noexcept
{
  DDyVector4 result = rhs;
  result.__Simd = _mm_mul_ps(result.__Simd, _mm_set_ps(lhs, lhs, lhs, lhs));

  return result;
}

DDyVector4 operator*(DDyVector4 lhs, const DDyVector4& rhs) noexcept
{
  lhs.__Simd = _mm_mul_ps(lhs.__Simd, rhs.__Simd);
  return lhs;
}

DDyVector4 operator/(DDyVector4 lhs, const float rhs) noexcept
{
  if (rhs == 0.0f) { DyPushLogDebugCritical("DDyVector4 could not be divided by {0}.", rhs); }
  else
  {
    lhs.__Simd = _mm_div_ps(lhs.__Simd, _mm_set_ps(rhs, rhs, rhs, rhs));
  }

  return lhs;
}

DDyVector4 operator/(DDyVector4 lhs, const DDyVector4& rhs) noexcept
{
  if (rhs.X == 0.0f || rhs.Y == 0.0f || rhs.Z == 0.0f || rhs.W == 0.0f)
  {
    DyPushLogDebugCritical(
      "DDyVector4 could not be devided by 0 included DDyVector4, ({0}, {1}, {2}, {3})", 
      rhs.X, rhs.Y, rhs.Z, rhs.W);
  }
  else
  {
    lhs.__Simd = _mm_div_ps(lhs.__Simd, rhs.__Simd);
  }

  return lhs;
}

bool operator==(const DDyVector4& lhs, const DDyVector4& rhs) noexcept
{
  return _mm_movemask_ps(_mm_cmpeq_ps(lhs.__Simd, rhs.__Simd)) == 0xF;
}

bool operator!=(const DDyVector4& lhs, const DDyVector4& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy namespace