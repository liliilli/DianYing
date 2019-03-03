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
  DDyVector4 result{};

  for (size_t i = 0; i < 4; ++i)
  {
    float value[4];
    _mm_store_ps(value, _mm_mul_ps(this->__Simd, matrix[i].__Simd));
    result[i] += value[0] + value[1] + value[2] + value[3];
  }

  return result;
}

bool operator==(_MIN_ const DDyVector4& lhs, _MIN_ const DDyVector4& rhs) noexcept
{
  return _mm_movemask_ps(_mm_cmpeq_ps(lhs.__Simd, rhs.__Simd)) == 0xF;
}

bool operator!=(_MIN_ const DDyVector4& lhs, _MIN_ const DDyVector4& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy namespace