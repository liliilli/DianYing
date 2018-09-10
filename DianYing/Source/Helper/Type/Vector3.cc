#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Type/Vector3.cc
///
/// @brief Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

/// Header file
#include <Dy/Helper/Type/Vector3.h>

#include <Dy/Helper/Math/Math.h>
#include <Dy/Helper/Type/Matrix3.h>

namespace dy {

DDyVector3 DDyVector3::MultiplyMatrix(const dy::DDyMatrix3x3& matrix) const noexcept
{
  return DDyVector3{
      this->X * matrix[0][0] + this->Y * matrix[0][1] + this->Z * matrix[0][2],
      this->X * matrix[1][0] + this->Y * matrix[1][1] + this->Z * matrix[1][2],
      this->X * matrix[2][0] + this->Y * matrix[2][1] + this->Z * matrix[2][2]
  };
}

bool DDyVector3::IsAllZero(const DDyVector3& vector) noexcept {
  return math::IsAllZero(vector);
}

bool DDyVector3::IsAllZero() const noexcept {
  return math::IsAllZero(*this);
}

} /// ::dy namespace