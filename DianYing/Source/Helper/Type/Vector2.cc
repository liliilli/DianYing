#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @brief Introduce opgs16 dependent vector series classes.
///
/// @author Jongmin Yun
///
/// @log
/// 2018-07-31 Create file.
///

/// Header file
#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Math/Math.h>

namespace dy {

bool DVector2::IsAllZero(const DVector2& vector) noexcept {
  return math::IsAllZero(vector);
}

bool DVector2::IsAllZero() const noexcept {
  return math::IsAllZero(*this);
}

} /// ::dy namespace