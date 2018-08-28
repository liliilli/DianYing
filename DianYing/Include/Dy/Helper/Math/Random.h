#ifndef GUARD_DY_HELPER_MATH_RANDOM_F
#define GUARD_DY_HELPER_MATH_RANDOM_F

///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Dy/Helper/Math/Random.h
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

#include <cstdint>

//!
//! Forward declaration
//!

namespace dy {
struct DVector2;
struct DVector3;
} /// ::opgs16 namespace

//!
//! Declaration
//!

namespace dy::random {

///
/// @enum ERandomPolicy
///
enum class ERandomPolicy {
  Uniform,
  Discrete,
};

///
/// @brief Get random integer (32bit) uniformly.
///
int32_t RandomInteger();

///
/// @brief Get random float (single-precision) uniformly.
///
float RandomFloat();

///
/// @brief Get random integer (32bit) but positive uniformly.
///
int32_t RandomPositiveInteger();

///
/// @brief Get random negative integer (32bit) uniformly.
///
int32_t RandomNegativeInteger();

///
/// @brief Get random positive float (single-presicion) uniformly.
///
float RandomPositiveFloat();

///
/// @brief Get random negative float (single-precision) uniformly.
///
float RandomNegativeFloat();

///
/// @brief Get integer from 'from' to 'inclusive_to' integer uniformly.
///
int32_t RandomIntegerRange(const int32_t from, const int32_t inclusive_to);

///
/// @brief Get float from 'from' to 'prior_to' not included prior_to uniformly.
///
float RandomFloatRange(const float from, const float prior_to);

///
/// @brief Get DVector2 (x, y) instance have length 'length'.
///
DVector2 RandomVector2Length(float length);

///
/// @brief Get DVector3 (x, y, z) instance have length 'length'.
///
DVector3 RandomVector3Length(float length);

///
/// @brief
///
DVector2 RandomVector2Range(ERandomPolicy policy, float from, float prior_to);

///
/// @brief
///
DVector3 RandomVector3Range(ERandomPolicy policy, float from, float prior_to);

} /// ::opgs16::math namespace

#endif /// GUARD_DY_HELPER_MATH_MATH_F