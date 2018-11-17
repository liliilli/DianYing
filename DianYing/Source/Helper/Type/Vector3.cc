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

/// Header file
#include <Dy/Helper/Type/Vector3.h>

#include <nlohmann/json.hpp>
#include <Dy/Helper/Math/Math.h>
#include <Dy/Helper/Type/Matrix3.h>
#include <Dy/Helper/Library/HelperJson.h>

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

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DDyVector3& p)
{
  j = nlohmann::json
  {
    {"X", p.X},
    {"Y", p.Y},
    {"Z", p.Z}
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DDyVector3& p)
{
  p.X = DyJsonGetValueFrom<TF32>(j, "X");
  p.Y = DyJsonGetValueFrom<TF32>(j, "Y");
  p.Z = DyJsonGetValueFrom<TF32>(j, "Z");
}

} /// ::dy namespace