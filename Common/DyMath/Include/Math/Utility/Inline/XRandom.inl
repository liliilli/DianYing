#pragma once
///
/// MIT License
/// Copyright (c) 2018-2019 Jongmin Yun
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include <random>
#include <Math/Utility/XMath.h>
#include <Math/Common/XGlobalMacroes.h>
#include "..\XRandom.h"

namespace dy::math
{

namespace details
{

inline std::mt19937& GetRandomDevice() noexcept
{
  static std::random_device randomDevice;
  static std::mt19937 rngGenerator(randomDevice());

  return rngGenerator;
}

} /// ::dy::math::details namespace

template<typename TType, typename>
TI32 RandomUniformInteger(TType start, TType inclusiveEnd) noexcept
{
  std::uniform_int_distribution<TType> dist{start, inclusiveEnd};
  return dist(details::GetRandomDevice());
}

template <typename TType, typename>
TF32 RandomUniformReal(TType start, TType end) noexcept
{
  std::uniform_real_distribution<TType> dist{start, end};
  return dist(details::GetRandomDevice());
}

template <typename TType, typename>
void RandomUniformIntegers(
  std::size_t iCount, std::vector<TType> oContainer, 
  TType iStart, TType iInclusiveEnd) noexcept
{
  std::uniform_int_distribution<TType> dist{iStart, iInclusiveEnd};
  auto& gen = details::GetRandomDevice();
  for (TIndex i = 0; i < iCount; ++i)
  {
    oContainer.emplace_back(dist(gen));
  }
}

template <typename TType, typename>
void RandomUniformReals(
  std::size_t iCount, std::vector<TType> oContainer, 
  TType iStart, TType iEnd) noexcept
{
  std::uniform_real_distribution<TType> dist{iStart, iEnd};
  auto& gen = details::GetRandomDevice();
  for (TIndex i = 0; i < iCount; ++i)
  {
    oContainer.emplace_back(dist(gen));
  }
}

template<typename TType, typename>
DVector2<TType> RandomVector2Length(TReal length) noexcept
{
  const auto theta = RandomUniformReal<TType>(-kPi<TType>, kPi<TType>);
  return {length * std::cos(theta), length * std::sin(theta)};
}

template<typename TType, typename>
DVector3<TType> RandomVector3Length(TReal length) noexcept
{
  const auto theta = RandomUniformReal<TType>(-kPi<TType> / 2, kPi<TType> / 2);
  const auto tho   = RandomUniformReal<TType>(-kPi<TType>, kPi<TType>);

  return {
    length * std::cos(theta) * std::cos(tho), 
    length * std::sin(theta), 
    length * std::cos(theta) * std::sin(tho)};
}

template <typename TType, typename>
DVector2<TType> RandomVector2Range(ERandomPolicy policy, TReal from, TReal priorTo)
{
  switch (policy)
  {
  case ERandomPolicy::Uniform:
    return RandomVector2Length<TType>(1.f) * RandomUniformReal(from, priorTo);
  case ERandomPolicy::Discrete:
    return {};
  default: M_ASSERT_OR_THROW(false, "Unexpected error occurred.");
  }
}

template <typename TType, typename>
DVector3<TType> RandomVector3Range(ERandomPolicy policy, TReal from, TReal priorTo)
{
  switch (policy)
  {
  case ERandomPolicy::Uniform:
    return RandomVector3Length<TType>(1.f) * RandomUniformReal(from, priorTo);
  case ERandomPolicy::Discrete:
    return {};
  default: M_ASSERT_OR_THROW(false, "Unexpected error occurred.");
  }
}


} /// ::dy::math namespace