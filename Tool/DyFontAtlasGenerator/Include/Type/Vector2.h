#ifndef GUARD_DY_HELPER_TYPE_VECTOR2_H
#define GUARD_DY_HELPER_TYPE_VECTOR2_H

#include <nlohmann/json.hpp>
#include <Math/Type/Math/DVector2.h>

namespace dy::math
{

using DDyVector2 = DVector2<TReal>;

template <typename TType>
void to_json(nlohmann::json& j, const DVector2<TType>& p)
{
  j = nlohmann::json { {"X", p.X}, {"Y", p.Y} };
}

template <typename TType>
void from_json(const nlohmann::json& j, DVector2<TType>& p)
{
  p.X = j.at("X").get<TType>();
  p.Y = j.at("Y").get<TType>();
}

} /// ::dy::math namespace

using dy::math::DDyVector2;

#endif /// GUARD_DY_HELPER_TYPE_VECTOR2_H