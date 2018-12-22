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
#include <Dy/Helper/Type/Matrix2.h>
#include <nlohmann/json.hpp>

namespace dy {

bool DDyVector2::IsAllZero(const DDyVector2& vector) noexcept {
  return math::IsAllZero(vector);
}

DDyVector2 DDyVector2::MultiplyMatrix(const dy::DDyMatrix2x2& matrix) const noexcept
{
  return DDyVector2{
      this->X * matrix[0][0] + this->Y * matrix[0][1],
      this->X * matrix[1][0] + this->Y * matrix[1][1]
  };
}

bool DDyVector2::IsAllZero() const noexcept {
  return math::IsAllZero(*this);
}

void to_json(nlohmann::json& j, const DDyVector2& p)
{
  j = nlohmann::json
  {
      {"X", p.X},
      {"Y", p.Y}
  };
}

void from_json(const nlohmann::json& j, DDyVector2& p)
{
  p.X = j.at("X").get<TF32>();
  p.Y = j.at("Y").get<TF32>();
}

} /// ::dy namespace