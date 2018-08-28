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

namespace dy {

bool DVector3::IsAllZero(const DVector3& vector) noexcept {
  return math::IsAllZero(vector);
}

bool DVector3::IsAllZero() const noexcept {
  return math::IsAllZero(*this);
}

} /// ::dy namespace