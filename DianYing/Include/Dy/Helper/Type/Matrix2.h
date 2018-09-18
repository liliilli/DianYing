#ifndef GUARD_DY_HELPER_TYPE_MATRIX2_H
#define GUARD_DY_HELPER_TYPE_MATRIX2_H
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
#include <glm/glm.hpp>
#include <Dy/Helper/Type/Vector2.h>

namespace dy
{

///
/// @class DDyMatrix2x2
/// @brief
///
class DDyMatrix2x2 final
{
public:
  DDyMatrix2x2()  = default;
  ~DDyMatrix2x2() = default;

  DDyMatrix2x2(const DDyMatrix2x2& value) = default;
  DDyMatrix2x2(DDyMatrix2x2&& value)      = default;
  DDyMatrix2x2& operator=(const DDyMatrix2x2& value)  = default;
  DDyMatrix2x2& operator=(DDyMatrix2x2&& value)       = default;
  DDyMatrix2x2(std::initializer_list<float>&) = delete;

  DDyMatrix2x2(const float _00, const float _01, const float _10, const float _11) :
      mMatrixValue{dy::DDyVector2{_00, _10},
                   dy::DDyVector2{_01, _11}} {};

  DDyMatrix2x2(const glm::mat2& glmMatrix) noexcept
  {
    mMatrixValue[0] = glmMatrix[0];
    mMatrixValue[1] = glmMatrix[1];
  }

  //!
  //! Conversion operators
  //!

  operator glm::mat2() const noexcept
  {
    return glm::mat2{mMatrixValue[0], mMatrixValue[1]};
  }

  DDyMatrix2x2& operator=(const glm::mat2& value) noexcept
  {
    this->mMatrixValue[0] = value[0];
    this->mMatrixValue[1] = value[1];
    return *this;
  }

  operator glm::mat3() const noexcept
  {
    return glm::mat3{
        mMatrixValue[0][0], mMatrixValue[1][0], 0,
        mMatrixValue[0][1], mMatrixValue[1][1], 0,
        0, 0, 0};
  }

  operator glm::mat4() const noexcept
  {
    return glm::mat4{
        mMatrixValue[0][0], mMatrixValue[1][0], 0, 0,
        mMatrixValue[0][1], mMatrixValue[1][1], 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};
  }

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

    DirectX::XMFLOAT2 rowFloat = {mMatrixValue[0][0], mMatrixValue[1][0]};
    returnMatrix.r[0] = DirectX::XMLoadFloat2(&rowFloat);
    rowFloat = {mMatrixValue[0][1], mMatrixValue[1][1]};
    returnMatrix.r[1] = DirectX::XMLoadFloat2(&rowFloat);

    return returnMatrix;
  }
#endif

  DDyVector2& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DDyVector2& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DDyMatrix2x2 operator*(const DDyMatrix2x2& rhs) const noexcept
  {
    return DDyMatrix2x2{
        (*this)[0][0] * rhs[0][0],
        (*this)[1][0] * rhs[1][0],
        (*this)[0][1] * rhs[0][1],
        (*this)[1][1] * rhs[1][1]
    };
  }

  DDyMatrix2x2& operator*=(const DDyMatrix2x2& rhs) noexcept
  {
    this->mMatrixValue[0][0] = (*this)[0][0] * rhs[0][0];
    this->mMatrixValue[1][0] = (*this)[1][0] * rhs[1][0];
    this->mMatrixValue[0][1] = (*this)[0][1] * rhs[0][1];
    this->mMatrixValue[1][1] = (*this)[1][1] * rhs[1][1];
    return *this;
  }

  DDyMatrix2x2 operator/(const DDyMatrix2x2& rhs) const
  {
    bool isDivideByZero = false;
    if (rhs[0][0] == 0.f)       isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    if (isDivideByZero) throw std::runtime_error("Divide by zero.");

    return DDyMatrix2x2{
        (*this)[0][0] / rhs[0][0],
        (*this)[1][0] / rhs[1][0],
        (*this)[0][1] / rhs[0][1],
        (*this)[1][1] / rhs[1][1]
    };
  }

  DDyMatrix2x2& operator/=(const DDyMatrix2x2& rhs)
  {
    bool isDivideByZero = false;
    if (rhs[0][0] == 0.f)       isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    if (isDivideByZero) throw std::runtime_error("Divide by zero.");

    this->mMatrixValue[0][0] = (*this)[0][0] / rhs[0][0];
    this->mMatrixValue[1][0] = (*this)[1][0] / rhs[1][0];
    this->mMatrixValue[0][1] = (*this)[0][1] / rhs[0][1];
    this->mMatrixValue[1][1] = (*this)[1][1] / rhs[1][1];
    return *this;
  }

  DDyMatrix2x2 operator+(const DDyMatrix2x2& rhs) const noexcept
  {
    return DDyMatrix2x2{
        (*this)[0][0] + rhs[0][0], (*this)[1][0] + rhs[1][0],
        (*this)[0][1] + rhs[0][1], (*this)[1][1] + rhs[1][1]
    };
  }

  DDyMatrix2x2& operator+=(const DDyMatrix2x2& rhs) noexcept
  {
    (*this)[0][0] = (*this)[0][0] + rhs[0][0];
    (*this)[1][0] = (*this)[1][0] + rhs[1][0];
    (*this)[0][1] = (*this)[0][1] + rhs[0][1];
    (*this)[1][1] = (*this)[1][1] + rhs[1][1];

    return *this;
  }

  DDyMatrix2x2 operator-(const DDyMatrix2x2& rhs) const noexcept
  {
    return DDyMatrix2x2{
        (*this)[0][0] - rhs[0][0], (*this)[1][0] - rhs[1][0],
        (*this)[0][1] - rhs[0][1], (*this)[1][1] - rhs[1][1]
    };
  }

  DDyMatrix2x2& operator-=(const DDyMatrix2x2& rhs) noexcept
  {
    (*this)[0][0] = (*this)[0][0] - rhs[0][0];
    (*this)[1][0] = (*this)[1][0] - rhs[1][0];
    (*this)[0][1] = (*this)[0][1] - rhs[0][1];
    (*this)[1][1] = (*this)[1][1] - rhs[1][1];

    return *this;
  }

  ///
  /// @brief
  ///
  DDyMatrix2x2 Transpose() const noexcept
  {
    return DDyMatrix2x2{
      this->mMatrixValue[0][0], this->mMatrixValue[0][1],
      this->mMatrixValue[1][0], this->mMatrixValue[1][1]};
  }

  ///
  /// @brief
  ///
  DDyMatrix2x2 Multiply(const DDyMatrix2x2& rhs) const noexcept
  {
    return DDyMatrix2x2{
        (*this)[0][0] * rhs[0][0] + (*this)[1][0] * rhs[0][1],
        (*this)[0][0] * rhs[1][0] + (*this)[1][0] * rhs[1][1],
        (*this)[0][1] * rhs[0][0] + (*this)[1][1] * rhs[0][1],
        (*this)[0][1] * rhs[1][0] + (*this)[1][1] * rhs[1][1]
    };
  }

  ///
  /// @brief
  ///
  DDyVector2 MultiplyVector(const DDyVector2& rhs) const noexcept
  {
    return DDyVector2{
        (*this)[0][0] * rhs.X + (*this)[1][0] * rhs.Y,
        (*this)[0][1] * rhs.X + (*this)[1][1] * rhs.Y
    };
  }

  ///
  /// @brief
  ///
  DDyMatrix2x2 Inverse() const
  {
    const float def = (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
    if (def == 0.f) throw std::runtime_error("Inversion derivative must not be zero.");

    DDyMatrix2x2 result;
    result[0][0] =  (*this)[1][1] / def;
    result[1][1] =  (*this)[0][0] / def;
    result[1][0] = -(*this)[1][0] / def;
    result[0][1] = -(*this)[0][1] / def;
    return result;
  }

  ///
  /// @brief
  ///
  static DDyMatrix2x2 IdentityMatrix() noexcept
  {
    static DDyMatrix2x2 identityMatrix2x2{true};
    return identityMatrix2x2;
  }

private:
  /// Identity matrix constructor
  DDyMatrix2x2(bool);

  /// Column major
  std::array<dy::DDyVector2, 2> mMatrixValue;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX2_H