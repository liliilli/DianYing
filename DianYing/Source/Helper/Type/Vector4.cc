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

DDyVector4 DDyVector4::MultiplyMatrix(const dy::DDyMatrix4x4& matrix) const noexcept
{
  return DDyVector4{
      this->X * matrix[0][0] + this->Y * matrix[0][1] + this->Z * matrix[0][2] + this->W * matrix[0][3],
      this->X * matrix[1][0] + this->Y * matrix[1][1] + this->Z * matrix[1][2] + this->W * matrix[1][3],
      this->X * matrix[2][0] + this->Y * matrix[2][1] + this->Z * matrix[2][2] + this->W * matrix[2][3],
      this->X * matrix[3][0] + this->Y * matrix[3][1] + this->Z * matrix[3][2] + this->W * matrix[3][3]
  };
}

bool DDyVector4::IsAllZero() const noexcept
{
  return math::IsAllZero(*this);
}

bool DDyVector4::IsAllZero(const DDyVector4& vector) noexcept
{
  return math::IsAllZero(vector);
}

bool operator==(_MIN_ const DDyVector4& lhs, _MIN_ const DDyVector4& rhs) noexcept
{
  return lhs.X == rhs.X 
      && lhs.Y == rhs.Y
      && lhs.Z == rhs.Z
      && lhs.W == rhs.W;
}

bool operator!=(_MIN_ const DDyVector4& lhs, _MIN_ const DDyVector4& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy namespace