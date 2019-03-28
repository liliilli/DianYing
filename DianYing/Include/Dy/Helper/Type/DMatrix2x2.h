#ifndef GUARD_DY_HELPER_TYPE_MATRIX2_H
#define GUARD_DY_HELPER_TYPE_MATRIX2_H
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
#include <glm/glm.hpp>
#include <Dy/Helper/Type/DVector2.h>

namespace dy
{

///
/// @class DMatrix2x2
/// @brief
///
class DMatrix2x2 final
{
public:
  DMatrix2x2()  = default;
  ~DMatrix2x2() = default;

  DMatrix2x2(const DMatrix2x2& value) = default;
  DMatrix2x2(DMatrix2x2&& value)      = default;
  DMatrix2x2& operator=(const DMatrix2x2& value)  = default;
  DMatrix2x2& operator=(DMatrix2x2&& value)       = default;
  DMatrix2x2(std::initializer_list<float>&) = delete;

  DMatrix2x2(const float _00, const float _01, const float _10, const float _11) 
    : mMatrixValue{dy::DVec2{_00, _10},
                   dy::DVec2{_01, _11}} {};

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

  DVec2& operator[](std::size_t index) noexcept
  {
    return mMatrixValue[index];
  }

  const DVec2& operator[](std::size_t index) const noexcept
  {
    return mMatrixValue[index];
  }

  DMatrix2x2 operator*(const DMatrix2x2& rhs) const noexcept
  {
    return DMatrix2x2{
        (*this)[0][0] * rhs[0][0],
        (*this)[1][0] * rhs[1][0],
        (*this)[0][1] * rhs[0][1],
        (*this)[1][1] * rhs[1][1]
    };
  }

  DMatrix2x2& operator*=(const DMatrix2x2& rhs) noexcept
  {
    this->mMatrixValue[0][0] = (*this)[0][0] * rhs[0][0];
    this->mMatrixValue[1][0] = (*this)[1][0] * rhs[1][0];
    this->mMatrixValue[0][1] = (*this)[0][1] * rhs[0][1];
    this->mMatrixValue[1][1] = (*this)[1][1] * rhs[1][1];
    return *this;
  }

  DMatrix2x2 operator/(const DMatrix2x2& rhs) const
  {
    bool isDivideByZero = false;
    if (rhs[0][0] == 0.f)       isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    else if (rhs[0][0] == 0.f)  isDivideByZero = true;
    if (isDivideByZero) throw std::runtime_error("Divide by zero.");

    return DMatrix2x2{
        (*this)[0][0] / rhs[0][0],
        (*this)[1][0] / rhs[1][0],
        (*this)[0][1] / rhs[0][1],
        (*this)[1][1] / rhs[1][1]
    };
  }

  DMatrix2x2& operator/=(const DMatrix2x2& rhs)
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

  DMatrix2x2 operator+(const DMatrix2x2& rhs) const noexcept
  {
    return DMatrix2x2{
        (*this)[0][0] + rhs[0][0], (*this)[1][0] + rhs[1][0],
        (*this)[0][1] + rhs[0][1], (*this)[1][1] + rhs[1][1]
    };
  }

  DMatrix2x2& operator+=(const DMatrix2x2& rhs) noexcept
  {
    (*this)[0][0] = (*this)[0][0] + rhs[0][0];
    (*this)[1][0] = (*this)[1][0] + rhs[1][0];
    (*this)[0][1] = (*this)[0][1] + rhs[0][1];
    (*this)[1][1] = (*this)[1][1] + rhs[1][1];

    return *this;
  }

  DMatrix2x2 operator-(const DMatrix2x2& rhs) const noexcept
  {
    return DMatrix2x2{
        (*this)[0][0] - rhs[0][0], (*this)[1][0] - rhs[1][0],
        (*this)[0][1] - rhs[0][1], (*this)[1][1] - rhs[1][1]
    };
  }

  DMatrix2x2& operator-=(const DMatrix2x2& rhs) noexcept
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
  DMatrix2x2 Transpose() const noexcept
  {
    return DMatrix2x2{
      this->mMatrixValue[0][0], this->mMatrixValue[0][1],
      this->mMatrixValue[1][0], this->mMatrixValue[1][1]};
  }

  ///
  /// @brief
  ///
  DMatrix2x2 Multiply(const DMatrix2x2& rhs) const noexcept
  {
    return DMatrix2x2{
        (*this)[0][0] * rhs[0][0] + (*this)[1][0] * rhs[0][1],
        (*this)[0][0] * rhs[1][0] + (*this)[1][0] * rhs[1][1],
        (*this)[0][1] * rhs[0][0] + (*this)[1][1] * rhs[0][1],
        (*this)[0][1] * rhs[1][0] + (*this)[1][1] * rhs[1][1]
    };
  }

  ///
  /// @brief
  ///
  DVec2 MultiplyVector(const DVec2& rhs) const noexcept
  {
    return DVec2{
        (*this)[0][0] * rhs.X + (*this)[1][0] * rhs.Y,
        (*this)[0][1] * rhs.X + (*this)[1][1] * rhs.Y
    };
  }

  ///
  /// @brief
  ///
  DMatrix2x2 Inverse() const
  {
    const float def = (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
    if (def == 0.f) throw std::runtime_error("Inversion derivative must not be zero.");

    DMatrix2x2 result;
    result[0][0] =  (*this)[1][1] / def;
    result[1][1] =  (*this)[0][0] / def;
    result[1][0] = -(*this)[1][0] / def;
    result[0][1] = -(*this)[0][1] / def;
    return result;
  }

  ///
  /// @brief
  ///
  static DMatrix2x2 IdentityMatrix() noexcept
  {
    static DMatrix2x2 identityMatrix2x2{true};
    return identityMatrix2x2;
  }

private:
  /// Identity matrix constructor
  DMatrix2x2(bool);

  /// Column major
  std::array<dy::DVec2, 2> mMatrixValue;
};

} /// ::dy namespace

#endif /// GUARD_DY_HELPER_TYPE_MATRIX2_H