#ifndef GUARD_DY_HELPER_TYPE_MATRIX4_H
#define GUARD_DY_HELPER_TYPE_MATRIX4_H
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

#include <nlohmann/json_fwd.hpp>
#include <foundation/PxTransform.h>
#include <glm/mat4x4.hpp>

#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DVector4.h>
#include <Dy/Helper/Type/DQuaternion.h>
#include <Math/Type/Math/DMatrix4.h>

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace dy
{

using DMat4 = math::DMatrix4<TReal>;

struct FMat4 final
{
  /// @brief Create Matrix4 with scale.
  static DMat4 CreateWithScale(TReal x, TReal y, TReal z);
  /// @brief Create Matrix4 with scale.
  static DMat4 CreateWithScale(const DVec3& vec);

  /// @brief Create Matrix4 with translation.
  static DMat4 CreateWithTranslation(TReal x, TReal y, TReal z);
  /// @brief Create Matrix4 with translation.
  static DMat4 CreateWithTranslation(const DVec3& vec);

  /// @brief Get new matrix with rotating vector (pitch, yaw, roll)
  static DMat4 Rotate(const DMat4& matrix, const DVec3& angle, bool isDegree = true);
  /// @brief Get new matrix with rotating quaternion (w, x, y, z)
  static DMat4 Rotate(const DMat4& matrix, const DQuat& quat);
  /// @brief Get new matrix with scale of vector (x, y, z)
  static DMat4 Scale(const DMat4& matrix, const DVec3& scale);
  /// @brief Get new matrix with position of vector (x, y, z)
  static DMat4 Locate(const DMat4& matrix, const DVec3& position);
  /// @brief Get new matrix that translated with given vector (x, y, z) position.
  static DMat4 Translate(const DMat4& matrix, const DVec3& offsetPos);

  static DMat4 CreateMat4(const glm::mat4& mat) noexcept;
  static DMat4 CreateMat4(const physx::PxTransform& transform) noexcept;
};

} /// ::dy namespace

namespace dy::math
{

void to_json(nlohmann::json& oJson, const DMat4& iMat);
void from_json(const nlohmann::json& iJson, DMat4& oMat);

} /// ::dy::math namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX4_H