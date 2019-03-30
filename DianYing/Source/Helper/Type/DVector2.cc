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
#include <Dy/Helper/Type/DVector2.h>


#include <Dy/Helper/Type/DMatrix2x2.h>
#include <nlohmann/json.hpp>

namespace dy::math 
{

void to_json(nlohmann::json& j, const DVec2& p)
{
  j = nlohmann::json { {"X", p.X}, {"Y", p.Y} };
}

void from_json(const nlohmann::json& j, DVec2& p)
{
  using namespace dy::math;
  p.X = j.at("X").get<TF32>();
  p.Y = j.at("Y").get<TF32>();
}

void to_json(nlohmann::json& j, const DIVec2& p)
{
  j = nlohmann::json { {"X", p.X}, {"Y", p.Y} };
}

void from_json(const nlohmann::json& j, DIVec2& p)
{
  p.X = j.at("X").get<TI32>();
  p.Y = j.at("Y").get<TI32>();
}

} /// ::dy::math namespace