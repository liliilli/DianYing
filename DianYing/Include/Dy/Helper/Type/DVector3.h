#ifndef GUARD_DY_HELPER_TYPE_VECTOR3_H
#define GUARD_DY_HELPER_TYPE_VECTOR3_H
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

#include <Math/Type/Math/DVector3.h>
#include <nlohmann/json_fwd.hpp>
#include <foundation/PxVec3.h>
#include <glm/detail/type_vec3.hpp>

//!
//! Forward declaration
//!

namespace dy
{

class DMatrix3x3;

} /// ::dy namespace

//!
//! Implementation
//! 

namespace dy 
{

/// @brief Float type 2-element vector struct.
using DVec3 = math::DVector3<math::TReal>;
using DIVec3 = math::DVector3<math::TI32>;

inline DVec3 ToVec3(const physx::PxVec3& vec)
{
  return DVec3{vec.x, vec.y, vec.z};
}

std::string ToString(const DVec3& vec) noexcept;
std::string ToString(const DIVec3& vec) noexcept;

physx::PxVec3 ToPxVec3(const DVec3& vec) noexcept;
physx::PxVec3 ToPxVec3(const DIVec3& vec) noexcept;

glm::vec3 ToGlmVec3(const DVec3& vec) noexcept;
glm::vec3 ToGlmVec3(const DIVec3& vec) noexcept;

DVec3 CreateVec3(const physx::PxVec3& vec) noexcept;
DVec3 CreateVec3(const glm::vec3& vec) noexcept;

} /// ::dy namespace

namespace dy::math
{

void to_json(nlohmann::json& j, const DVec3& p);
void from_json(const nlohmann::json& j, DVec3& p);

void to_json  (nlohmann::json& j, const DIVec3& p);
void from_json(const nlohmann::json& j, DIVec3& p);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_VECTOR3_H
