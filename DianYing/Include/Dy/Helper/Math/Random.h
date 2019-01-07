#ifndef GUARD_DY_HELPER_MATH_RANDOM_F
#define GUARD_DY_HELPER_MATH_RANDOM_F
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

#include <cstdint>

//!
//! Forward declaration
//!

namespace dy {
struct DDyVector2;
struct DDyVector3;
} /// ::opgs16 namespace

//!
//! Declaration
//!

namespace dy::random {

///
/// @enum EDyRandomPolicy
/// @brief Could be used in decinding random distribution policy.
///
enum class EDyRandomPolicy {
  Uniform,
  Discrete,
};

///
/// @brief Get random integer (32bit) uniformly.
///
[[nodiscard]] int32_t RandomInteger() noexcept;

///
/// @brief Get random float (single-precision) uniformly.
///
[[nodiscard]] float RandomFloat() noexcept;

///
/// @brief Get random integer (32bit) but positive uniformly.
///
[[nodiscard]] int32_t RandomPositiveInteger() noexcept;

///
/// @brief Get random negative integer (32bit) uniformly.
///
[[nodiscard]] int32_t RandomNegativeInteger() noexcept;

///
/// @brief Get random positive float (single-presicion) uniformly.
///
[[nodiscard]] float RandomPositiveFloat();

///
/// @brief Get random negative float (single-precision) uniformly.
///
[[nodiscard]] float RandomNegativeFloat();

///
/// @brief Get integer from 'from' to 'inclusive_to' integer uniformly.
///
[[nodiscard]] int32_t RandomIntegerRange(const int32_t from, const int32_t inclusive_to) noexcept;

///
/// @brief Get float from 'from' to 'prior_to' not included prior_to uniformly.
///
[[nodiscard]] float RandomFloatRange(const float from, const float prior_to) noexcept;

///
/// @brief Get DDyVector2 (x, y) instance have length 'length'.
///
[[nodiscard]] DDyVector2 RandomVector2Length(float length) noexcept;

///
/// @brief Get DDyVector3 (x, y, z) instance have length 'length'.
///
[[nodiscard]] DDyVector3 RandomVector3Length(float length) noexcept;

///
/// @brief Get DDyVector2 (x, y) which have length that in range from 'from' to 'priorTo'
///
[[nodiscard]] DDyVector2 RandomVector2Range(EDyRandomPolicy policy, float from, float priorTo);

///
/// @brief Get DDyVector3 (x, y, z) which have length that in range from 'from' to 'priorTo'
///
[[nodiscard]] DDyVector3 RandomVector3Range(EDyRandomPolicy policy, float from, float priorTo);

} /// ::opgs16::math namespace

#endif /// GUARD_DY_HELPER_MATH_MATH_F