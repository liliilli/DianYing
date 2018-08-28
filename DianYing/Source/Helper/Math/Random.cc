#include <precompiled.h>
///
/// @license BSD 2-Clause License
///
/// Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
/// If you want to read full statements, read LICENSE file.
///
/// @file Helper/Math/Random.cc
/// @author Jongmin Yun
///
/// @log
/// 2018-08-28 Create file.
///

/// Header file
#include <Dy/Helper/Math/Random.h>

#include <cmath>

#include <chrono>
#include <limits>
#include <random>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/Vector3.h>

//!
//! Data
//!

namespace {

std::random_device sRandomDevice;
std::mt19937 sRngGenerator(sRandomDevice());

std::uniform_int_distribution<int>    sIntDistribution;
std::uniform_real_distribution<float> sRealDistribution{-1'000'000'000.f, 1'000'000'000.f};

}

//!
//! Implemenation
//!

namespace dy::random {

int32_t RandomInteger() {
  return sIntDistribution(sRngGenerator);
}

float RandomFloat() {
  return sRealDistribution(sRngGenerator);
}

int32_t RandomPositiveInteger() {
  const int32_t value = sIntDistribution(sRngGenerator);

  if (value == std::numeric_limits<int32_t>::lowest()) return 0;
  if (value < 0) return -value;
  return value;
}

int32_t RandomNegativeInteger() {
  const int32_t value = sIntDistribution(sRngGenerator);

  if (value > 0) return -value;
  return value;
}

float RandomPositiveFloat() {
  const float value = sRealDistribution(sRngGenerator);

  if (value < 0.f) return -value;
  return value;
}

float RandomNegativeFloat() {
  const float value = sRealDistribution(sRngGenerator);

  if (value > 0.f) return -value;
  return value;
}

int32_t RandomIntegerRange(const int32_t from, const int32_t inclusive_to) {
  //PHITOS_ASSERT(from <= inclusive_to, "");

  auto result = sIntDistribution(sRngGenerator);
  result %= (inclusive_to - from + 1);
  result += from;

  return result;
}

float RandomFloatRange(const float from, const float prior_to) {
  //PHITOS_ASSERT(from <= prior_to, "");

  const std::uniform_real_distribution<float> rngGenerator{from, prior_to};
  return rngGenerator(sRngGenerator);
}

DVector2 RandomVector2Length(float length) {
  DVector2 result;
  float squared_length;

  do {
    result.X = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.Y = RandomFloatRange(-1.0f * length, 1.0f * length);
    squared_length = result.GetSquareLength();
  }
  while (squared_length > std::pow(length * 1.75f, 2) ||
         squared_length < std::pow(length * 0.25f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DVector3 RandomVector3Length(float length) {
  DVector3 result;
  float squared_length;

  do {
    result.X = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.Y = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.Z = RandomFloatRange(-1.0f * length, 1.0f * length);
    squared_length = result.GetSquareLength();
  }
  while (squared_length > std::pow(length * 1.75f, 2) ||
         squared_length < std::pow(length * 0.25f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DVector2 RandomVector2Range(ERandomPolicy policy, float from, float prior_to) {
  switch (policy) {
  case ERandomPolicy::Uniform:
    return RandomVector2Length(1.f) * RandomFloatRange(from, prior_to);
  case ERandomPolicy::Discrete:
    return {};
  default: return {};
  }
}

DVector3 RandomVector3Range(ERandomPolicy policy, float from, float prior_to) {
  switch (policy) {
  case ERandomPolicy::Uniform:
    return RandomVector3Length(1.f) * RandomFloatRange(from, prior_to);
  case ERandomPolicy::Discrete:
    return {};
  default: return {};
  }
}

} /// ::dy::random namespace