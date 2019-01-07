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

#include <Dy/Helper/Type/Matrix3.h>
#include <Dy/Helper/Type/Vector4.h>
#include <assimp/matrix4x4.h>

namespace dy
{

///
/// @class DDyMatrix4x4
/// @brief
///
class DDyMatrix4x4 final
{
public:
  DDyMatrix4x4()  = default;
  ~DDyMatrix4x4() = default;

  DDyMatrix4x4(const DDyMatrix4x4& value) = default;
  DDyMatrix4x4(DDyMatrix4x4&& value) = default;
  DDyMatrix4x4& operator=(const DDyMatrix4x4& value) = default;
  DDyMatrix4x4& operator=(DDyMatrix4x4&& value) = default;
  DDyMatrix4x4(std::initializer_list<float>&) = delete;

  DDyMatrix4x4(const float _00, const float _01, const float _02, const float _03,
               const float _10, const float _11, const float _12, const float _13,
               const float _20, const float _21, const float _22, const float _23,
               const float _30, const float _31, const float _32, const float _33) :
      mMatrixValue{dy::DDyVector4{_00, _10, _20, _30},
                   dy::DDyVector4{_01, _11, _21, _31},
                   dy::DDyVector4{_02, _12, _22, _32},
                   dy::DDyVector4{_03, _13, _23, _33}} {}

  DDyMatrix4x4(const DDyVector4& column1, const DDyVector4& column2,
               const DDyVector4& column3, const DDyVector4& column4);

  DDyMatrix4x4(const glm::mat2& glmMatrix) noexcept;

  DDyMatrix4x4(const glm::mat3& glmMatrix) noexcept;

  DDyMatrix4x4(const glm::mat4& glmMatrix) noexcept;

  DDyMatrix4x4(const aiMatrix4x4& aiMatrix) noexcept;

  DDyMatrix4x4& operator=(const glm::mat2& value) noexcept;

  DDyMatrix4x4& operator=(const glm::mat3& value) noexcept;

  DDyMatrix4x4& operator=(const glm::mat4& value) noexcept;

  DDyMatrix4x4& operator=(const aiMatrix4x4& value) noexcept;

  //!
  //! Conversion operators
  //!

  explicit operator glm::mat2() const noexcept;
  explicit operator glm::mat3() const noexcept;
  explicit operator DDyMatrix2x2() const noexcept;
  explicit operator DDyMatrix3x3() const noexcept;

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

  DDyVector4& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DDyVector4& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DDyMatrix4x4 operator*(const DDyMatrix4x4& rhs) const noexcept;

  DDyMatrix4x4& operator*=(const DDyMatrix4x4& rhs) noexcept;

  DDyMatrix4x4 operator/(const DDyMatrix4x4& rhs) const;

  DDyMatrix4x4& operator/=(const DDyMatrix4x4& rhs);

  DDyMatrix4x4 operator+(const DDyMatrix4x4& rhs) const noexcept;

  DDyMatrix4x4& operator+=(const DDyMatrix4x4& rhs) noexcept;

  DDyMatrix4x4 operator-(const DDyMatrix4x4& rhs) const noexcept;

  DDyMatrix4x4& operator-=(const DDyMatrix4x4& rhs) noexcept;

  friend bool operator==(_MIN_ const DDyMatrix4x4& lhs, _MIN_ const DDyMatrix4x4& rhs) noexcept;
  friend bool operator!=(_MIN_ const DDyMatrix4x4& lhs, _MIN_ const DDyMatrix4x4& rhs) noexcept;

  ///
  /// @brief
  ///
  DDyMatrix4x4 Transpose() const noexcept;

  ///
  /// @brief
  ///
  DDyMatrix4x4 Multiply(const DDyMatrix4x4& rhs) const noexcept;

  ///
  /// @brief
  ///
  DDyVector4 MultiplyVector(const DDyVector4& rhs) const noexcept;

  ///
  /// @brief
  ///
  DDyMatrix4x4 Inverse() const;

  ///
  /// @brief
  ///
  static DDyMatrix4x4 IdentityMatrix() noexcept;

  ///
  /// @brief
  ///
  static DDyMatrix4x4 CreateWithScale(const DDyVector3& scaleVector);

  ///
  /// @brief
  ///
  static DDyMatrix4x4 CreateWithTranslation(const DDyVector3& translationPoint);

private:
  /// Identity matrix constructor
  explicit DDyMatrix4x4(bool);

  /// Column major
  std::array<dy::DDyVector4, 4> mMatrixValue;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX4_H