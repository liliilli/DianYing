#include <precompiled.h>
///
/// MIT License
/// Copyright (c) 2018 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

/// Header file
#include <Dy/Helper/Math/Random.h>

#include <cmath>

#include <limits>
#include <random>

#include <Dy/Helper/Type/Vector2.h>
#include <Dy/Helper/Type/Vector3.h>

#include "Dy/Management/LoggingManager.h"

//!
//! Data
//!

namespace {

std::random_device sRandomDevice;
std::mt19937 sRngGenerator(sRandomDevice());

std::uniform_int_distribution<int>    sIntDistribution;
std::uniform_real_distribution<float> sRealDistribution{-1'000'000'000.f, 1'000'000'000.f};

} /// unnamed namespace

//!
//! Implemenation
//!

namespace dy::random {

int32_t RandomInteger() noexcept
{
  return sIntDistribution(sRngGenerator);
}

float RandomFloat() noexcept
{
  return sRealDistribution(sRngGenerator);
}

int32_t RandomPositiveInteger() noexcept
{
  const int32_t value = sIntDistribution(sRngGenerator);

  if (value == std::numeric_limits<int32_t>::lowest()) return 0;
  if (value < 0) return -value;
  return value;
}

int32_t RandomNegativeInteger() noexcept
{
  const int32_t value = sIntDistribution(sRngGenerator);

  if (value > 0) return -value;
  return value;
}

float RandomPositiveFloat()
{
  const float value = sRealDistribution(sRngGenerator);

  if (value < 0.f) return -value;
  return value;
}

float RandomNegativeFloat()
{
  const float value = sRealDistribution(sRngGenerator);

  if (value > 0.f) return -value;
  return value;
}

int32_t RandomIntegerRange(const int32_t from, const int32_t inclusive_to) noexcept
{
  if (from > inclusive_to)
  {
    MDY_LOG_WARNING("'from' must be equal or less than 'inclusive_to'. From : {}, Inclusive_to : {}", from, inclusive_to);
    return 0;
  }

  auto result = sIntDistribution(sRngGenerator);
  result %= (inclusive_to - from + 1);
  result += from;

  return result;
}

float RandomFloatRange(const float from, const float prior_to) noexcept
{
  if (from >= prior_to)
  {
    MDY_LOG_WARNING("'from' must be less than 'prior_to'. From : {}, Prior to : {}", from, prior_to);
    return from;
  }

  const std::uniform_real_distribution<float> rngGenerator{from, prior_to};
  return rngGenerator(sRngGenerator);
}

DDyVector2 RandomVector2Length(float length) noexcept
{
  if (length < 0)
  {
    MDY_LOG_WARNING("{} | Length must be bigger than or equal to 0. Input length : {}", "RandomVector2Length", length);
    return {};
  }

  DDyVector2 result;
  float squared_length;

  do
  {
    result.X = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.Y = RandomFloatRange(-1.0f * length, 1.0f * length);
    squared_length = result.GetSquareLength();
  }
  while (squared_length > std::pow(length * 1.75f, 2) || squared_length < std::pow(length * 0.25f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DDyVector3 RandomVector3Length(float length) noexcept
{
  if (length < 0)
  {
    MDY_LOG_WARNING("{} | Length must be bigger than or equal to 0. Input length : {}", "RandomVector3Length", length);
    return {};
  }

  DDyVector3 result;
  float squared_length;

  do
  {
    result.X = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.Y = RandomFloatRange(-1.0f * length, 1.0f * length);
    result.Z = RandomFloatRange(-1.0f * length, 1.0f * length);
    squared_length = result.GetSquareLength();
  }
  while (squared_length > std::pow(length * 1.75f, 2) || squared_length < std::pow(length * 0.25f, 2));

  return result * (length / std::sqrtf(squared_length));
}

DDyVector2 RandomVector2Range(EDyRandomPolicy policy, float from, float prior_to)
{
  if (from >= prior_to)
  {
    MDY_LOG_WARNING("{} | 'from' must be less than 'prior_to'. From : {}, Prior to : {}", "RandomVector2Range", from, prior_to);
    return {};
  }

  switch (policy)
  {
  case EDyRandomPolicy::Uniform:
    return RandomVector2Length(1.f) * RandomFloatRange(from, prior_to);
  case EDyRandomPolicy::Discrete:
    // @todo Implement this
    MDY_LOG_WARNING("{} | Discrete distribution is not implemented now.", "RandomVector2Range");
    return {};
  default: return {};
  }
}

DDyVector3 RandomVector3Range(EDyRandomPolicy policy, float from, float prior_to)
{
  if (from >= prior_to)
  {
    MDY_LOG_WARNING("{} | 'from' must be less than 'prior_to'. From : {}, Prior to : {}", "RandomVector3Range", from, prior_to);
    return {};
  }

  switch (policy)
  {
  case EDyRandomPolicy::Uniform:
    return RandomVector3Length(1.f) * RandomFloatRange(from, prior_to);
  case EDyRandomPolicy::Discrete:
    MDY_LOG_WARNING("{} | Discrete distribution is not implemented now.", "RandomVector3Range");
    return {};
  default: return {};
  }
}

} /// ::dy::random namespace