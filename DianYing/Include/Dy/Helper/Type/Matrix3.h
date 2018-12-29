#ifndef GUARD_DY_HELPER_TYPE_MATRIX3_H
#define GUARD_DY_HELPER_TYPE_MATRIX3_H
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

#include <array>
#include <Dy/Helper/Type/Vector3.h>

namespace dy
{

///
/// @class DDyMatrix3x3
/// @brief
///
class DDyMatrix3x3 final
{
public:
  DDyMatrix3x3()  = default;
  ~DDyMatrix3x3() = default;

  DDyMatrix3x3(const DDyMatrix3x3& value) = default;
  DDyMatrix3x3(DDyMatrix3x3&& value) = default;
  DDyMatrix3x3& operator=(const DDyMatrix3x3& value) = default;
  DDyMatrix3x3& operator=(DDyMatrix3x3&& value) = default;
  DDyMatrix3x3(std::initializer_list<float>&) = delete;

  DDyMatrix3x3(const float _00, const float _01, const float _02,
               const float _10, const float _11, const float _12,
               const float _20, const float _21, const float _22) :
      mMatrixValue{dy::DDyVector3{_00, _10, _20},
                   dy::DDyVector3{_01, _11, _21},
                   dy::DDyVector3{_02, _12, _22}} {}

  DDyMatrix3x3(const DDyVector3& column1, const DDyVector3& column2, const DDyVector3& column3) :
      mMatrixValue{ column1, column2, column3 } {}

  DDyMatrix3x3(const glm::mat2& glmMatrix) noexcept
  {
    mMatrixValue[0] = glmMatrix[0];
    mMatrixValue[1] = glmMatrix[1];
  }

  DDyMatrix3x3(const glm::mat3& glmMatrix) noexcept
  {
    mMatrixValue[0] = glmMatrix[0];
    mMatrixValue[1] = glmMatrix[1];
    mMatrixValue[2] = glmMatrix[2];
  }

  DDyMatrix3x3& operator=(const glm::mat2& value) noexcept
  {
    this->mMatrixValue[0] = value[0];
    this->mMatrixValue[1] = value[1];
    this->mMatrixValue[2] = DDyVector3{};
    return *this;
  }

  DDyMatrix3x3& operator=(const glm::mat3& value) noexcept
  {
    this->mMatrixValue[0] = value[0];
    this->mMatrixValue[1] = value[1];
    this->mMatrixValue[2] = value[2];
    return *this;
  }

  //!
  //! Conversion operators
  //!

  explicit operator glm::mat2() const noexcept;
  explicit operator DDyMatrix2x2() const noexcept;
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

  DDyVector3& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DDyVector3& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DDyMatrix3x3 operator*(const DDyMatrix3x3& rhs) const noexcept;

  DDyMatrix3x3& operator*=(const DDyMatrix3x3& rhs) noexcept;

  DDyMatrix3x3 operator/(const DDyMatrix3x3& rhs) const;

  DDyMatrix3x3& operator/=(const DDyMatrix3x3& rhs);

  DDyMatrix3x3 operator+(const DDyMatrix3x3& rhs) const noexcept;

  DDyMatrix3x3& operator+=(const DDyMatrix3x3& rhs) noexcept;

  DDyMatrix3x3 operator-(const DDyMatrix3x3& rhs) const noexcept;

  DDyMatrix3x3& operator-=(const DDyMatrix3x3& rhs) noexcept;

  friend bool operator==(_MIN_ const DDyMatrix3x3& lhs, _MIN_ const DDyMatrix3x3& rhs) noexcept;
  friend bool operator!=(_MIN_ const DDyMatrix3x3& lhs, _MIN_ const DDyMatrix3x3& rhs) noexcept;

  ///
  /// @brief
  ///
  DDyMatrix3x3 Transpose() const noexcept;

  ///
  /// @brief
  ///
  DDyMatrix3x3 Multiply(const DDyMatrix3x3& rhs) const noexcept;

  ///
  /// @brief
  ///
  DDyVector3 MultiplyVector(const DDyVector3& rhs) const noexcept;

  ///
  /// @brief
  ///
  DDyMatrix3x3 Inverse() const;

  ///
  /// @brief
  ///
  static DDyMatrix3x3 IdentityMatrix() noexcept;

private:
  /// Identity matrix constructor
  explicit DDyMatrix3x3(bool);

  /// Column major
  std::array<dy::DDyVector3, 3> mMatrixValue;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX3_H