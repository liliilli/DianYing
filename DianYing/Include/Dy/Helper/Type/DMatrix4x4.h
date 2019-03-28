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

#include <array>

#include <assimp/matrix4x4.h>
#include <nlohmann/json_fwd.hpp>

#include <Dy/Helper/Type/DMatrix3x3.h>
#include <Dy/Helper/Type/DVector4.h>
#include "DMatrix2x2.h"

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace dy
{
  class DQuaternion;

  ///
/// @class DMatrix4x4
/// @brief
///
class DMatrix4x4 final
{
public:
  DMatrix4x4()  = default;
  ~DMatrix4x4() = default;

  DMatrix4x4(const DMatrix4x4& value) = default;
  DMatrix4x4(DMatrix4x4&& value) = default;
  DMatrix4x4& operator=(const DMatrix4x4& value) = default;
  DMatrix4x4& operator=(DMatrix4x4&& value) = default;
  DMatrix4x4(std::initializer_list<float>&) = delete;

  DMatrix4x4(const float _00, const float _01, const float _02, const float _03,
               const float _10, const float _11, const float _12, const float _13,
               const float _20, const float _21, const float _22, const float _23,
               const float _30, const float _31, const float _32, const float _33) :
      mMatrixValue{dy::DVec4{_00, _10, _20, _30},
                   dy::DVec4{_01, _11, _21, _31},
                   dy::DVec4{_02, _12, _22, _32},
                   dy::DVec4{_03, _13, _23, _33}} {}

  DMatrix4x4(const DVec4& column1, const DVec4& column2,
               const DVec4& column3, const DVec4& column4);

  DMatrix4x4(const glm::mat4& glmMatrix) noexcept;

  DMatrix4x4(const aiMatrix4x4& aiMatrix) noexcept;

  DMatrix4x4(const physx::PxTransform& physxTransform) noexcept;

  DMatrix4x4& operator=(const glm::mat4& value) noexcept;

  DMatrix4x4& operator=(const aiMatrix4x4& value) noexcept;

  //!
  //! Conversion operators
  //!

  explicit operator glm::mat2() const noexcept;
  explicit operator glm::mat3() const noexcept;
  explicit operator DMatrix2x2() const noexcept;
  explicit operator DMatrix3x3() const noexcept;

  operator glm::mat4() const noexcept;

#ifdef false
  explicit operator aiMatrix3x3t<float>() const noexcept
  {
    return aiMatrix3x3t<float>{
        mMatrixValue[0][0], mMatrixValue[0][1], 0,
        mMatrixValue[1][0], mMatrixValue[1][1], 0,
        0, 0, 0};
  }

  operator aiMatrix4x4t<float>() const noexcept
  {
    return aiMatrix4x4t<float>{
        mMatrixValue[0][0], mMatrixValue[0][1], 0, 0,
        mMatrixValue[1][0], mMatrixValue[1][1], 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};
  }
#endif

#if defined(_WIN32)
  operator DirectX::XMMATRIX() const noexcept
  {
    DirectX::XMMATRIX returnMatrix = {};

    DirectX::XMFLOAT4 rowFloat = {};
    rowFloat = {mMatrixValue[0][0], mMatrixValue[1][0], mMatrixValue[2][0], mMatrixValue[3][0]};
    returnMatrix.r[0] = DirectX::XMLoadFloat4(&rowFloat);
    rowFloat = {mMatrixValue[0][1], mMatrixValue[1][1], mMatrixValue[2][1], mMatrixValue[3][1]};
    returnMatrix.r[1] = DirectX::XMLoadFloat4(&rowFloat);
    rowFloat = {mMatrixValue[0][2], mMatrixValue[1][2], mMatrixValue[2][2], mMatrixValue[3][2]};
    returnMatrix.r[2] = DirectX::XMLoadFloat4(&rowFloat);
    rowFloat = {mMatrixValue[0][3], mMatrixValue[1][3], mMatrixValue[2][3], mMatrixValue[3][3]};
    returnMatrix.r[3] = DirectX::XMLoadFloat4(&rowFloat);
    return returnMatrix;
  }
#endif

  DVec4& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DVec4& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DMatrix4x4 operator*(const DMatrix4x4& rhs) const noexcept;

  DMatrix4x4& operator*=(const DMatrix4x4& rhs) noexcept;

  DMatrix4x4 operator/(const DMatrix4x4& rhs) const;

  DMatrix4x4& operator/=(const DMatrix4x4& rhs);

  DMatrix4x4 operator+(const DMatrix4x4& rhs) const noexcept;

  DMatrix4x4& operator+=(const DMatrix4x4& rhs) noexcept;

  DMatrix4x4 operator-(const DMatrix4x4& rhs) const noexcept;

  DMatrix4x4& operator-=(const DMatrix4x4& rhs) noexcept;

  friend bool operator==(_MIN_ const DMatrix4x4& lhs, _MIN_ const DMatrix4x4& rhs) noexcept;
  friend bool operator!=(_MIN_ const DMatrix4x4& lhs, _MIN_ const DMatrix4x4& rhs) noexcept;

  /// @brief
  DMatrix4x4 Transpose() const noexcept;

  /// @brief
  DMatrix4x4 Multiply(const DMatrix4x4& rhs) const noexcept;

  /// @brief
  DVec4 MultiplyVector(const DVec4& rhs) const noexcept;

  /// @brief
  DMatrix4x4 Inverse() const;

  /// @brief
  static DMatrix4x4 IdentityMatrix() noexcept;

  /// @brief
  static DMatrix4x4 CreateWithScale(const DVec3& scaleVector);

  /// @brief
  static DMatrix4x4 CreateWithTranslation(const DVec3& translationPoint);

  /// @brief Get orthographic projection of OpenGL. \n
  /// [ 2/(r-l)    0       0    -(r+l)/(r-l) ] \n
  /// [   0     2/(t-b)    0    -(t+b)/(t-b) ] \n
  /// [   0        0   -2/(f-n) -(f+n)/(f-n) ] \n
  /// [   0        0       0          1      ]
  static DMatrix4x4 OrthoProjection(_MIN_ TF32 left, _MIN_ TF32 right, _MIN_ TF32 bottom, _MIN_ TF32 top, _MIN_ TF32 near, _MIN_ TF32 far);

  /// @brief Scale matrix.
  DMatrix4x4& Scale(_MIN_ const DVec3& iScaleFactor);
  /// @brief Rotate matrix.
  DMatrix4x4& Rotate(_MIN_ const DVec3& iRotationDegreeAngle);
  /// @brief Rotate matrix.
  DMatrix4x4& Rotate(_MIN_ const DQuaternion& iRotationQuaternion);
  /// @brief Translate matrix.
  DMatrix4x4& Translate(_MIN_ const DVec3& iPosition);

private:
  /// Identity matrix constructor
  explicit DMatrix4x4(bool);

  /// Column major
  std::array<dy::DVec4, 4> mMatrixValue;
};

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DMatrix4x4& p);
void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DMatrix4x4& p);

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX4_H