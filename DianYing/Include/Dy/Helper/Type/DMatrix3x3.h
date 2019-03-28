#ifndef GUARD_DY_HELPER_TYPE_MATRIX3_H
#define GUARD_DY_HELPER_TYPE_MATRIX3_H
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
#include <Dy/Helper/Type/DVector3.h>
#include <glm/glm.hpp>

namespace dy
{

///
/// @class DMatrix3x3
/// @brief
///
class DMatrix3x3 final
{
public:
  DMatrix3x3()  = default;
  ~DMatrix3x3() = default;

  DMatrix3x3(const DMatrix3x3& value) = default;
  DMatrix3x3(DMatrix3x3&& value) = default;
  DMatrix3x3& operator=(const DMatrix3x3& value) = default;
  DMatrix3x3& operator=(DMatrix3x3&& value) = default;
  DMatrix3x3(std::initializer_list<float>&) = delete;

  DMatrix3x3(const float _00, const float _01, const float _02,
               const float _10, const float _11, const float _12,
               const float _20, const float _21, const float _22) :
      mMatrixValue{dy::DVec3{_00, _10, _20},
                   dy::DVec3{_01, _11, _21},
                   dy::DVec3{_02, _12, _22}} {}

  DMatrix3x3(const DVec3& column1, const DVec3& column2, const DVec3& column3) :
      mMatrixValue{ column1, column2, column3 } {}

#ifdef false
  DMatrix3x3(const glm::mat2& glmMatrix) noexcept
  {
    mMatrixValue[0] = glmMatrix[0];
    mMatrixValue[1] = glmMatrix[1];
  }
#endif

  DMatrix3x3(const glm::mat3& glmMatrix) noexcept
  {
    mMatrixValue[0] = CreateVec3(glmMatrix[0]);
    mMatrixValue[1] = CreateVec3(glmMatrix[1]);
    mMatrixValue[2] = CreateVec3(glmMatrix[2]);
  }

#ifdef false
  DMatrix3x3& operator=(const glm::mat2& value) noexcept
  {
    this->mMatrixValue[0] = value[0];
    this->mMatrixValue[1] = value[1];
    this->mMatrixValue[2] = DVec3{};
    return *this;
  }

  DMatrix3x3& operator=(const glm::mat3& value) noexcept
  {
    this->mMatrixValue[0] = value[0];
    this->mMatrixValue[1] = value[1];
    this->mMatrixValue[2] = value[2];
    return *this;
  }
#endif

  //!
  //! Conversion operators
  //!

  explicit operator glm::mat2() const noexcept;
  operator glm::mat3() const noexcept;
  operator glm::mat4() const noexcept;

#ifdef false
  operator aiMatrix3x3t<float>() const noexcept
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

    DirectX::XMFLOAT3 rowFloat = {};
    rowFloat = {mMatrixValue[0][0], mMatrixValue[1][0], mMatrixValue[2][0]};
    returnMatrix.r[0] = DirectX::XMLoadFloat3(&rowFloat);
    rowFloat = {mMatrixValue[0][1], mMatrixValue[1][1], mMatrixValue[2][1]};
    returnMatrix.r[1] = DirectX::XMLoadFloat3(&rowFloat);
    rowFloat = {mMatrixValue[0][2], mMatrixValue[1][2], mMatrixValue[2][2]};
    returnMatrix.r[2] = DirectX::XMLoadFloat3(&rowFloat);

    return returnMatrix;
  }
#endif

  DVec3& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DVec3& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DMatrix3x3 operator*(const DMatrix3x3& rhs) const noexcept;

  DMatrix3x3& operator*=(const DMatrix3x3& rhs) noexcept;

  DMatrix3x3 operator/(const DMatrix3x3& rhs) const;

  DMatrix3x3& operator/=(const DMatrix3x3& rhs);

  DMatrix3x3 operator+(const DMatrix3x3& rhs) const noexcept;

  DMatrix3x3& operator+=(const DMatrix3x3& rhs) noexcept;

  DMatrix3x3 operator-(const DMatrix3x3& rhs) const noexcept;

  DMatrix3x3& operator-=(const DMatrix3x3& rhs) noexcept;

  friend bool operator==(_MIN_ const DMatrix3x3& lhs, _MIN_ const DMatrix3x3& rhs) noexcept;
  friend bool operator!=(_MIN_ const DMatrix3x3& lhs, _MIN_ const DMatrix3x3& rhs) noexcept;

  ///
  /// @brief
  ///
  DMatrix3x3 Transpose() const noexcept;

  ///
  /// @brief
  ///
  DMatrix3x3 Multiply(const DMatrix3x3& rhs) const noexcept;

  ///
  /// @brief
  ///
  DVec3 MultiplyVector(const DVec3& rhs) const noexcept;

  ///
  /// @brief
  ///
  DMatrix3x3 Inverse() const;

  ///
  /// @brief
  ///
  static DMatrix3x3 IdentityMatrix() noexcept;

private:
  /// Identity matrix constructor
  explicit DMatrix3x3(bool);

  /// Column major
  std::array<dy::DVec3, 3> mMatrixValue;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX3_H