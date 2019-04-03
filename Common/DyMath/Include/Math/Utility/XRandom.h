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

#include <vector>
#include <Math/Common/TGlobalTypes.h>
#include <Math/Type/Math/DVector2.h>
#include <Math/Type/Math/DVector3.h>
#include <Math/Type/Micellanous/ERandomPolicy.h>

namespace dy::math
{

/// @brief Get uniform random Integer value.
template <typename TType, typename = std::enable_if_t<kIsIntegerType<TType>>>
TI32 RandomUniformInteger(TType start = kMinValueOf<TType>, TType inclusiveEnd = kMaxValueOf<TType>) noexcept;

/// @brief Get uniform random real value.
template <typename TType, typename = std::enable_if_t<kIsRealType<TType>>>
TF32 RandomUniformReal(TType start = kMinValueOf<TType>, TType end = kMaxValueOf<TType>) noexcept;

template <typename TType, typename = std::enable_if_t<kIsIntegerType<TType>>>
void RandomUniformIntegers(
  std::size_t iCount, std::vector<TType> oContainer, 
  TType iStart = kMinValueOf<TType>, TType iInclusiveEnd = kMaxValueOf<TType>) noexcept;

template <typename TType, typename = std::enable_if_t<kIsRealType<TType>>>
void RandomUniformReals(
  std::size_t iCount, std::vector<TType> oContainer, 
  TType iStart = kMinValueOf<TType>, TType iEnd = kMaxValueOf<TType>) noexcept;

/// @brief Get DVec2 (x, y) instance have length 'length'.
/// @reference http://nbeloglazov.com/2017/04/09/random-vector-generation.html
template <typename TType, typename = std::enable_if_t<kIsRealType<TType>>>
DVector2<TType> RandomVector2Length(TReal length) noexcept;

/// @brief Get DVec3 (x, y, z) instance have length 'length'.
template <typename TType, typename = std::enable_if_t<kIsRealType<TType>>>
DVector3<TType> RandomVector3Length(TReal length) noexcept;

/// @brief Get DVec2 (x, y) which have length that in range from 'from' to 'priorTo'
/// @reference http://mathworld.wolfram.com/SpherePointPicking.html
/// @reference https://en.wikipedia.org/wiki/Multivariate_normal_distribution
template <typename TType, typename = std::enable_if_t<kIsRealType<TType>>>
DVector2<TType> RandomVector2Range(ERandomPolicy policy, TReal from, TReal priorTo);

/// @brief Get DVec3 (x, y, z) which have length that in range from 'from' to 'priorTo'
template <typename TType, typename = std::enable_if_t<kIsRealType<TType>>>
DVector3<TType> RandomVector3Range(ERandomPolicy policy, TReal from, TReal priorTo);

} /// ::dy::math namespace
#include <Math/Utility/Inline/XRandom.inl>