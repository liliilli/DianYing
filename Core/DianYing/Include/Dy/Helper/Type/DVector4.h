#ifndef GUARD_DY_HELPER_TYPE_VECTOR4_H
#define GUARD_DY_HELPER_TYPE_VECTOR4_H
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

#include <glm/glm.hpp>

// To enable / disable SIMD optimization of DVector4, add // or remove // below macro.
// This use SIMD ~SSE4.1.
//#define MATH_ENABLE_SIMD
#include <Math/Type/Math/DVector4.h>
#include <nlohmann/json_fwd.hpp>

namespace dy
{
  
using DVec4   = math::DVector4<math::TReal>;
using DIVec4  = math::DVector4<math::TI32>;

struct FVec4 final
{
  static DVec4 CreateVec4(const glm::vec4& vec) noexcept;
  static DVec4 CreateVec4(const glm::ivec4& vec) noexcept;
  static DIVec4 CreateIVec4(const glm::ivec4& vec) noexcept;
};

} /// ::dy namespace

namespace dy::math
{
  
void to_json(nlohmann::json& oJson, const DVec4& iVector);
void from_json(const nlohmann::json& iJson, DVec4& oVector);

void to_json(nlohmann::json& oJson, const DIVec4& iVector);
void from_json(const nlohmann::json& iJson, DIVec4& oVector);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR4_H
