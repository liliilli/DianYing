#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Type/Matrix3.h>

#include <glm/glm.hpp>
#include <Dy/Helper/Type/Matrix2.h>

namespace dy
{

DDyMatrix3x3::operator glm::mat2() const noexcept
{
  return glm::mat2{
      mMatrixValue[0][0], mMatrixValue[0][1],
      mMatrixValue[1][0], mMatrixValue[1][1]
  };
}

DDyMatrix3x3::operator DDyMatrix2x2() const noexcept
{
  return DDyMatrix2x2{
      mMatrixValue[0][0], mMatrixValue[1][0],
      mMatrixValue[0][1], mMatrixValue[1][1]
  };
}

DDyMatrix3x3::operator glm::mat3() const noexcept
{
  return glm::mat3{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2],
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2],
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2] };
}

DDyMatrix3x3::operator glm::mat4() const noexcept
{
  return glm::mat4{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2], 0,
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2], 0,
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2], 0,
      0, 0, 0, 0 };
}

DDyMatrix3x3 DDyMatrix3x3::operator*(const DDyMatrix3x3& rhs) const noexcept
{
  return DDyMatrix3x3{
      (*this)[0][0] * rhs[0][0], (*this)[1][0] * rhs[1][0], (*this)[2][0] * rhs[2][0],
      (*this)[0][1] * rhs[0][1], (*this)[1][1] * rhs[1][1], (*this)[2][1] * rhs[2][1],
      (*this)[0][2] * rhs[0][2], (*this)[1][2] * rhs[1][2], (*this)[2][2] * rhs[2][2]
  };
}

DDyMatrix3x3& DDyMatrix3x3::operator*=(const DDyMatrix3x3& rhs) noexcept
{
  this->mMatrixValue[0][0] = (*this)[0][0] * rhs[0][0];
  this->mMatrixValue[1][0] = (*this)[1][0] * rhs[1][0];
  this->mMatrixValue[2][0] = (*this)[2][0] * rhs[2][0];

  this->mMatrixValue[0][1] = (*this)[0][1] * rhs[0][1];
  this->mMatrixValue[1][1] = (*this)[1][1] * rhs[1][1];
  this->mMatrixValue[2][1] = (*this)[2][1] * rhs[2][1];

  this->mMatrixValue[0][2] = (*this)[0][2] * rhs[0][2];
  this->mMatrixValue[1][2] = (*this)[1][2] * rhs[1][2];
  this->mMatrixValue[2][2] = (*this)[2][2] * rhs[2][2];
  return *this;
}

DDyMatrix3x3 DDyMatrix3x3::operator/(const DDyMatrix3x3& rhs) const
{
  for (int32_t i = 0; i < 3; ++i)
  {
    for (int32_t j = 0; j < 3; ++j)
    {
      if (rhs[i][j] == 0.f) throw std::runtime_error("Divide by zero.");
    }
  }

  return DDyMatrix3x3{
      (*this)[0][0] / rhs[0][0], (*this)[1][0] / rhs[1][0], (*this)[2][0] / rhs[2][0],
      (*this)[0][1] / rhs[0][1], (*this)[1][1] / rhs[1][1], (*this)[2][1] / rhs[2][1],
      (*this)[0][2] / rhs[0][2], (*this)[1][2] / rhs[1][2], (*this)[2][2] / rhs[2][2]
  };
}

DDyMatrix3x3& DDyMatrix3x3::operator/=(const DDyMatrix3x3& rhs)
{
  for (int32_t i = 0; i < 3; ++i)
  {
    for (int32_t j = 0; j < 3; ++j)
    {
      if (rhs[i][j] == 0.f) throw std::runtime_error("Divide by zero.");
    }
  }

  this->mMatrixValue[0][0] = (*this)[0][0] / rhs[0][0];
  this->mMatrixValue[1][0] = (*this)[1][0] / rhs[1][0];
  this->mMatrixValue[2][0] = (*this)[2][0] / rhs[2][0];

  this->mMatrixValue[0][1] = (*this)[0][1] / rhs[0][1];
  this->mMatrixValue[1][1] = (*this)[1][1] / rhs[1][1];
  this->mMatrixValue[2][1] = (*this)[2][1] / rhs[2][1];

  this->mMatrixValue[0][2] = (*this)[0][2] / rhs[0][2];
  this->mMatrixValue[1][2] = (*this)[1][2] / rhs[1][2];
  this->mMatrixValue[2][2] = (*this)[2][2] / rhs[2][2];
  return *this;
}

DDyMatrix3x3 DDyMatrix3x3::operator+(const DDyMatrix3x3& rhs) const noexcept
{
  return DDyMatrix3x3{
      (*this)[0][0] + rhs[0][0], (*this)[1][0] + rhs[1][0], (*this)[2][0] + rhs[2][0],
      (*this)[0][1] + rhs[0][1], (*this)[1][1] + rhs[1][1], (*this)[2][1] + rhs[2][1],
      (*this)[0][2] + rhs[0][2], (*this)[1][2] + rhs[1][2], (*this)[2][2] + rhs[2][2]
  };
}

DDyMatrix3x3& DDyMatrix3x3::operator+=(const DDyMatrix3x3& rhs) noexcept
{
  (*this)[0][0] = (*this)[0][0] + rhs[0][0];
  (*this)[1][0] = (*this)[1][0] + rhs[1][0];
  (*this)[2][0] = (*this)[2][0] + rhs[2][0];

  (*this)[0][1] = (*this)[0][1] + rhs[0][1];
  (*this)[1][1] = (*this)[1][1] + rhs[1][1];
  (*this)[2][1] = (*this)[2][1] + rhs[2][1];

  (*this)[0][2] = (*this)[0][2] + rhs[0][2];
  (*this)[1][2] = (*this)[1][2] + rhs[1][2];
  (*this)[2][2] = (*this)[2][2] + rhs[2][2];
  return *this;
}

DDyMatrix3x3 DDyMatrix3x3::operator-(const DDyMatrix3x3& rhs) const noexcept
{
  return DDyMatrix3x3{
      (*this)[0][0] - rhs[0][0], (*this)[1][0] - rhs[1][0], (*this)[2][0] - rhs[2][0],
      (*this)[0][1] - rhs[0][1], (*this)[1][1] - rhs[1][1], (*this)[2][1] - rhs[2][1],
      (*this)[0][2] - rhs[0][2], (*this)[1][2] - rhs[1][2], (*this)[2][2] - rhs[2][2],
  };
}

DDyMatrix3x3& DDyMatrix3x3::operator-=(const DDyMatrix3x3& rhs) noexcept
{
  (*this)[0][0] = (*this)[0][0] - rhs[0][0];
  (*this)[1][0] = (*this)[1][0] - rhs[1][0];
  (*this)[2][0] = (*this)[2][0] - rhs[2][0];

  (*this)[0][1] = (*this)[0][1] - rhs[0][1];
  (*this)[1][1] = (*this)[1][1] - rhs[1][1];
  (*this)[2][1] = (*this)[2][1] - rhs[2][1];

  (*this)[0][2] = (*this)[0][2] - rhs[0][2];
  (*this)[1][2] = (*this)[1][2] - rhs[1][2];
  (*this)[2][2] = (*this)[2][2] - rhs[2][2];
  return *this;
}

DDyMatrix3x3 DDyMatrix3x3::Transpose() const noexcept
{
  return DDyMatrix3x3{
      this->mMatrixValue[0][0], this->mMatrixValue[0][1], this->mMatrixValue[0][2],
      this->mMatrixValue[1][0], this->mMatrixValue[1][1], this->mMatrixValue[1][2],
      this->mMatrixValue[2][0], this->mMatrixValue[2][1], this->mMatrixValue[2][2]
  };
}

DDyMatrix3x3 DDyMatrix3x3::Multiply(const DDyMatrix3x3& rhs) const noexcept
{
  return DDyMatrix3x3{
      (*this)[0][0] * rhs[0][0] + (*this)[1][0] * rhs[0][1] + (*this)[2][0] * rhs[0][2],
      (*this)[0][0] * rhs[1][0] + (*this)[1][0] * rhs[1][1] + (*this)[2][0] * rhs[1][2],
      (*this)[0][0] * rhs[2][0] + (*this)[1][0] * rhs[2][1] + (*this)[2][0] * rhs[2][2],

      (*this)[0][1] * rhs[0][0] + (*this)[1][1] * rhs[0][1] + (*this)[2][1] * rhs[0][2],
      (*this)[0][1] * rhs[1][0] + (*this)[1][1] * rhs[1][1] + (*this)[2][1] * rhs[1][2],
      (*this)[0][1] * rhs[2][0] + (*this)[1][1] * rhs[2][1] + (*this)[2][1] * rhs[2][2],

      (*this)[0][2] * rhs[0][0] + (*this)[1][2] * rhs[0][1] + (*this)[2][2] * rhs[0][2],
      (*this)[0][2] * rhs[1][0] + (*this)[1][2] * rhs[1][1] + (*this)[2][2] * rhs[1][2],
      (*this)[0][2] * rhs[2][0] + (*this)[1][2] * rhs[2][1] + (*this)[2][2] * rhs[2][2],
  };
}

DVector3 DDyMatrix3x3::MultiplyVector(const DVector3& rhs) const noexcept
{
  return DVector3{
      (*this)[0][0] * rhs.X + (*this)[1][0] * rhs.Y + (*this)[2][0] * rhs.Z,
      (*this)[0][1] * rhs.X + (*this)[1][1] * rhs.Y + (*this)[2][1] * rhs.Z,
      (*this)[0][2] * rhs.X + (*this)[1][2] * rhs.Y + (*this)[2][2] * rhs.Z
  };
}

DDyMatrix3x3 DDyMatrix3x3::Inverse() const
{
  // temporary
  const glm::mat3 mat = *this;
  return glm::inverse(mat);
}

DDyMatrix3x3 DDyMatrix3x3::IdentityMatrix() noexcept
{
  static DDyMatrix3x3 identityMatrix3x3{ true };
  return identityMatrix3x3;
}

DDyMatrix3x3::DDyMatrix3x3(bool)
{
  mMatrixValue[0][0] = 1;
  mMatrixValue[1][1] = 1;
  mMatrixValue[2][2] = 1;
}

} /// ::dy namespace