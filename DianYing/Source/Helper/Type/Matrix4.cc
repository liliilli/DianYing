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
#include <Dy/Helper/Type/Matrix4.h>

#include <Dy/Helper/Type/Matrix2.h>
#include <Dy/Helper/Type/Matrix3.h>

namespace dy
{

DDyMatrix4x4::DDyMatrix4x4(const DVector4& column1, const DVector4& column2,
                           const DVector4& column3, const DVector4& column4) :
    mMatrixValue{ column1, column2, column3, column4 } { }

DDyMatrix4x4::DDyMatrix4x4(const glm::mat2& glmMatrix) noexcept
{
  mMatrixValue[0] = glmMatrix[0];
  mMatrixValue[1] = glmMatrix[1];
}

DDyMatrix4x4::DDyMatrix4x4(const glm::mat3& glmMatrix) noexcept
{
  mMatrixValue[0] = glmMatrix[0];
  mMatrixValue[1] = glmMatrix[1];
  mMatrixValue[2] = glmMatrix[2];
}

DDyMatrix4x4::DDyMatrix4x4(const glm::mat4& glmMatrix) noexcept
{
  mMatrixValue[0] = glmMatrix[0];
  mMatrixValue[1] = glmMatrix[1];
  mMatrixValue[2] = glmMatrix[2];
  mMatrixValue[3] = glmMatrix[3];
}

DDyMatrix4x4& DDyMatrix4x4::operator=(const glm::mat2& value) noexcept
{
  this->mMatrixValue[0] = value[0];
  this->mMatrixValue[1] = value[1];
  this->mMatrixValue[2] = DVector4{};
  this->mMatrixValue[3] = DVector4{};
  return *this;
}

DDyMatrix4x4& DDyMatrix4x4::operator=(const glm::mat3& value) noexcept
{
  this->mMatrixValue[0] = value[0];
  this->mMatrixValue[1] = value[1];
  this->mMatrixValue[2] = value[2];
  this->mMatrixValue[3] = DVector4{};
  return *this;
}

DDyMatrix4x4& DDyMatrix4x4::operator=(const glm::mat4& value) noexcept
{
  this->mMatrixValue[0] = value[0];
  this->mMatrixValue[1] = value[1];
  this->mMatrixValue[2] = value[2];
  this->mMatrixValue[3] = value[3];
  return *this;
}

DDyMatrix4x4::operator glm::mat2() const noexcept
{
  return glm::mat2{
      mMatrixValue[0][0], mMatrixValue[0][1],
      mMatrixValue[1][0], mMatrixValue[1][1]
  };
}

DDyMatrix4x4::operator glm::mat3() const noexcept
{
  return glm::mat3{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2],
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2],
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2]
  };
}

DDyMatrix4x4::operator glm::mat4() const noexcept
{
  return glm::mat4{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2], mMatrixValue[0][3],
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2], mMatrixValue[1][3],
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2], mMatrixValue[2][3],
      mMatrixValue[3][0], mMatrixValue[3][1], mMatrixValue[3][2], mMatrixValue[3][3]
  };
}

DDyMatrix4x4::operator DDyMatrix2x2() const noexcept
{
  return DDyMatrix2x2{
      mMatrixValue[0][0], mMatrixValue[1][0],
      mMatrixValue[0][1], mMatrixValue[1][1]
  };
}

DDyMatrix4x4::operator DDyMatrix3x3() const noexcept
{
  return DDyMatrix3x3{
      mMatrixValue[0][0], mMatrixValue[1][0], mMatrixValue[2][0],
      mMatrixValue[0][1], mMatrixValue[1][1], mMatrixValue[2][1],
      mMatrixValue[0][2], mMatrixValue[1][2], mMatrixValue[2][2],
  };
}

DDyMatrix4x4 DDyMatrix4x4::operator*(const DDyMatrix4x4& rhs) const noexcept
{
  return DDyMatrix4x4{
      (*this)[0][0] * rhs[0][0], (*this)[1][0] * rhs[1][0], (*this)[2][0] * rhs[2][0], (*this)[3][0] * rhs[3][0],
      (*this)[0][1] * rhs[0][1], (*this)[1][1] * rhs[1][1], (*this)[2][1] * rhs[2][1], (*this)[3][1] * rhs[3][1],
      (*this)[0][2] * rhs[0][2], (*this)[1][2] * rhs[1][2], (*this)[2][2] * rhs[2][2], (*this)[3][2] * rhs[3][2],
      (*this)[0][3] * rhs[0][3], (*this)[1][3] * rhs[1][3], (*this)[2][3] * rhs[2][3], (*this)[3][3] * rhs[3][3],
  };
}

DDyMatrix4x4& DDyMatrix4x4::operator*=(const DDyMatrix4x4& rhs) noexcept
{
  this->mMatrixValue[0][0] = (*this)[0][0] * rhs[0][0];
  this->mMatrixValue[1][0] = (*this)[1][0] * rhs[1][0];
  this->mMatrixValue[2][0] = (*this)[2][0] * rhs[2][0];
  this->mMatrixValue[3][0] = (*this)[3][0] * rhs[3][0];

  this->mMatrixValue[0][1] = (*this)[0][1] * rhs[0][1];
  this->mMatrixValue[1][1] = (*this)[1][1] * rhs[1][1];
  this->mMatrixValue[2][1] = (*this)[2][1] * rhs[2][1];
  this->mMatrixValue[3][1] = (*this)[3][1] * rhs[3][1];

  this->mMatrixValue[0][2] = (*this)[0][2] * rhs[0][2];
  this->mMatrixValue[1][2] = (*this)[1][2] * rhs[1][2];
  this->mMatrixValue[2][2] = (*this)[2][2] * rhs[2][2];
  this->mMatrixValue[3][2] = (*this)[3][2] * rhs[3][2];

  this->mMatrixValue[0][3] = (*this)[0][3] * rhs[0][3];
  this->mMatrixValue[1][3] = (*this)[1][3] * rhs[1][3];
  this->mMatrixValue[2][3] = (*this)[2][3] * rhs[2][3];
  this->mMatrixValue[3][3] = (*this)[3][3] * rhs[3][3];
  return *this;
}

DDyMatrix4x4 DDyMatrix4x4::operator/(const DDyMatrix4x4& rhs) const
{
  for (int32_t i = 0; i < 4; ++i)
  {
    for (int32_t j = 0; j < 4; ++j)
    {
      if (rhs[i][j] == 0.f) throw std::runtime_error("Divide by zero.");
    }
  }

  return DDyMatrix4x4{
      (*this)[0][0] / rhs[0][0], (*this)[1][0] / rhs[1][0], (*this)[2][0] / rhs[2][0], (*this)[3][0] / rhs[3][0],
      (*this)[0][1] / rhs[0][1], (*this)[1][1] / rhs[1][1], (*this)[2][1] / rhs[2][1], (*this)[3][1] / rhs[3][1],
      (*this)[0][2] / rhs[0][2], (*this)[1][2] / rhs[1][2], (*this)[2][2] / rhs[2][2], (*this)[3][2] / rhs[3][2],
      (*this)[0][3] / rhs[0][3], (*this)[1][3] / rhs[1][3], (*this)[2][3] / rhs[2][3], (*this)[3][3] / rhs[3][3],
  };
}

DDyMatrix4x4& DDyMatrix4x4::operator/=(const DDyMatrix4x4& rhs)
{
  for (int32_t i = 0; i < 4; ++i)
  {
    for (int32_t j = 0; j < 4; ++j)
    {
      if (rhs[i][j] == 0.f) throw std::runtime_error("Divide by zero.");
    }
  }

  this->mMatrixValue[0][0] = (*this)[0][0] / rhs[0][0];
  this->mMatrixValue[1][0] = (*this)[1][0] / rhs[1][0];
  this->mMatrixValue[2][0] = (*this)[2][0] / rhs[2][0];
  this->mMatrixValue[3][0] = (*this)[3][0] / rhs[3][0];

  this->mMatrixValue[0][1] = (*this)[0][1] / rhs[0][1];
  this->mMatrixValue[1][1] = (*this)[1][1] / rhs[1][1];
  this->mMatrixValue[2][1] = (*this)[2][1] / rhs[2][1];
  this->mMatrixValue[3][1] = (*this)[3][1] / rhs[3][1];

  this->mMatrixValue[0][2] = (*this)[0][2] / rhs[0][2];
  this->mMatrixValue[1][2] = (*this)[1][2] / rhs[1][2];
  this->mMatrixValue[2][2] = (*this)[2][2] / rhs[2][2];
  this->mMatrixValue[3][2] = (*this)[3][2] / rhs[3][2];

  this->mMatrixValue[0][3] = (*this)[0][3] / rhs[0][3];
  this->mMatrixValue[1][3] = (*this)[1][3] / rhs[1][3];
  this->mMatrixValue[2][3] = (*this)[2][3] / rhs[2][3];
  this->mMatrixValue[3][3] = (*this)[3][3] / rhs[3][3];
  return *this;;
}

DDyMatrix4x4 DDyMatrix4x4::operator+(const DDyMatrix4x4& rhs) const noexcept
{
  return DDyMatrix4x4{
      (*this)[0][0] + rhs[0][0], (*this)[1][0] + rhs[1][0], (*this)[2][0] + rhs[2][0], (*this)[3][0] + rhs[3][0],
      (*this)[0][1] + rhs[0][1], (*this)[1][1] + rhs[1][1], (*this)[2][1] + rhs[2][1], (*this)[3][1] + rhs[3][1],
      (*this)[0][2] + rhs[0][2], (*this)[1][2] + rhs[1][2], (*this)[2][2] + rhs[2][2], (*this)[3][2] + rhs[3][2],
      (*this)[0][3] + rhs[0][3], (*this)[1][3] + rhs[1][3], (*this)[2][3] + rhs[2][3], (*this)[3][3] + rhs[3][3],
  };
}

DDyMatrix4x4& DDyMatrix4x4::operator+=(const DDyMatrix4x4& rhs) noexcept
{
  this->mMatrixValue[0][0] += rhs[0][0];
  this->mMatrixValue[1][0] += rhs[1][0];
  this->mMatrixValue[2][0] += rhs[2][0];
  this->mMatrixValue[3][0] += rhs[3][0];

  this->mMatrixValue[0][1] += rhs[0][1];
  this->mMatrixValue[1][1] += rhs[1][1];
  this->mMatrixValue[2][1] += rhs[2][1];
  this->mMatrixValue[3][1] += rhs[3][1];

  this->mMatrixValue[0][2] += rhs[0][2];
  this->mMatrixValue[1][2] += rhs[1][2];
  this->mMatrixValue[2][2] += rhs[2][2];
  this->mMatrixValue[3][2] += rhs[3][2];

  this->mMatrixValue[0][3] += rhs[0][3];
  this->mMatrixValue[1][3] += rhs[1][3];
  this->mMatrixValue[2][3] += rhs[2][3];
  this->mMatrixValue[3][3] += rhs[3][3];;

  return *this;
}

DDyMatrix4x4 DDyMatrix4x4::operator-(const DDyMatrix4x4& rhs) const noexcept
{
  return DDyMatrix4x4{
      (*this)[0][0] - rhs[0][0], (*this)[1][0] - rhs[1][0], (*this)[2][0] - rhs[2][0], (*this)[3][0] - rhs[3][0],
      (*this)[0][1] - rhs[0][1], (*this)[1][1] - rhs[1][1], (*this)[2][1] - rhs[2][1], (*this)[3][1] - rhs[3][1],
      (*this)[0][2] - rhs[0][2], (*this)[1][2] - rhs[1][2], (*this)[2][2] - rhs[2][2], (*this)[3][2] - rhs[3][2],
      (*this)[0][3] - rhs[0][3], (*this)[1][3] - rhs[1][3], (*this)[2][3] - rhs[2][3], (*this)[3][3] - rhs[3][3],
  };
}

DDyMatrix4x4& DDyMatrix4x4::operator-=(const DDyMatrix4x4& rhs) noexcept
{
  this->mMatrixValue[0][0] -= rhs[0][0];
  this->mMatrixValue[1][0] -= rhs[1][0];
  this->mMatrixValue[2][0] -= rhs[2][0];
  this->mMatrixValue[3][0] -= rhs[3][0];

  this->mMatrixValue[0][1] -= rhs[0][1];
  this->mMatrixValue[1][1] -= rhs[1][1];
  this->mMatrixValue[2][1] -= rhs[2][1];
  this->mMatrixValue[3][1] -= rhs[3][1];

  this->mMatrixValue[0][2] -= rhs[0][2];
  this->mMatrixValue[1][2] -= rhs[1][2];
  this->mMatrixValue[2][2] -= rhs[2][2];
  this->mMatrixValue[3][2] -= rhs[3][2];

  this->mMatrixValue[0][3] -= rhs[0][3];
  this->mMatrixValue[1][3] -= rhs[1][3];
  this->mMatrixValue[2][3] -= rhs[2][3];
  this->mMatrixValue[3][3] -= rhs[3][3];;

  return *this;
}

DDyMatrix4x4 DDyMatrix4x4::Transpose() const noexcept
{
  return DDyMatrix4x4{
      this->mMatrixValue[0][0], this->mMatrixValue[0][1], this->mMatrixValue[0][2], this->mMatrixValue[0][3],
      this->mMatrixValue[1][0], this->mMatrixValue[1][1], this->mMatrixValue[1][2], this->mMatrixValue[1][3],
      this->mMatrixValue[2][0], this->mMatrixValue[2][1], this->mMatrixValue[2][2], this->mMatrixValue[2][3],
      this->mMatrixValue[3][0], this->mMatrixValue[3][1], this->mMatrixValue[3][2], this->mMatrixValue[3][3]
  };
}

DDyMatrix4x4 DDyMatrix4x4::Multiply(const DDyMatrix4x4& rhs) const noexcept
{
  // Naive! O(N^3)
  return DDyMatrix4x4{
      (*this)[0][0] * rhs[0][0] + (*this)[1][0] * rhs[0][1] + (*this)[2][0] * rhs[0][2] + (*this)[3][0] * rhs[0][3],
      (*this)[0][0] * rhs[1][0] + (*this)[1][0] * rhs[1][1] + (*this)[2][0] * rhs[1][2] + (*this)[3][0] * rhs[1][3],
      (*this)[0][0] * rhs[2][0] + (*this)[1][0] * rhs[2][1] + (*this)[2][0] * rhs[2][2] + (*this)[3][0] * rhs[2][3],
      (*this)[0][0] * rhs[3][0] + (*this)[1][0] * rhs[3][1] + (*this)[2][0] * rhs[3][2] + (*this)[3][0] * rhs[3][3],

      (*this)[0][1] * rhs[0][0] + (*this)[1][1] * rhs[0][1] + (*this)[2][1] * rhs[0][2] + (*this)[3][1] * rhs[0][3],
      (*this)[0][1] * rhs[1][0] + (*this)[1][1] * rhs[1][1] + (*this)[2][1] * rhs[1][2] + (*this)[3][1] * rhs[1][3],
      (*this)[0][1] * rhs[2][0] + (*this)[1][1] * rhs[2][1] + (*this)[2][1] * rhs[2][2] + (*this)[3][1] * rhs[2][3],
      (*this)[0][1] * rhs[3][0] + (*this)[1][1] * rhs[3][1] + (*this)[2][1] * rhs[3][2] + (*this)[3][1] * rhs[3][3],

      (*this)[0][2] * rhs[0][0] + (*this)[1][2] * rhs[0][1] + (*this)[2][2] * rhs[0][2] + (*this)[3][2] * rhs[0][3],
      (*this)[0][2] * rhs[1][0] + (*this)[1][2] * rhs[1][1] + (*this)[2][2] * rhs[1][2] + (*this)[3][2] * rhs[1][3],
      (*this)[0][2] * rhs[2][0] + (*this)[1][2] * rhs[2][1] + (*this)[2][2] * rhs[2][2] + (*this)[3][2] * rhs[2][3],
      (*this)[0][2] * rhs[3][0] + (*this)[1][2] * rhs[3][1] + (*this)[2][2] * rhs[3][2] + (*this)[3][2] * rhs[3][3],

      (*this)[0][3] * rhs[0][0] + (*this)[1][3] * rhs[0][1] + (*this)[2][3] * rhs[0][2] + (*this)[3][3] * rhs[0][3],
      (*this)[0][3] * rhs[1][0] + (*this)[1][3] * rhs[1][1] + (*this)[2][3] * rhs[1][2] + (*this)[3][3] * rhs[1][3],
      (*this)[0][3] * rhs[2][0] + (*this)[1][3] * rhs[2][1] + (*this)[2][3] * rhs[2][2] + (*this)[3][3] * rhs[2][3],
      (*this)[0][3] * rhs[3][0] + (*this)[1][3] * rhs[3][1] + (*this)[2][3] * rhs[3][2] + (*this)[3][3] * rhs[3][3]
  };
}

DVector4 DDyMatrix4x4::MultiplyVector(const DVector4& rhs) const noexcept
{
  return DVector4{
      (*this)[0][0] * rhs.X + (*this)[1][0] * rhs.Y + (*this)[2][0] * rhs.Z + (*this)[3][0] * rhs.W,
      (*this)[0][1] * rhs.X + (*this)[1][1] * rhs.Y + (*this)[2][1] * rhs.Z + (*this)[3][1] * rhs.W,
      (*this)[0][2] * rhs.X + (*this)[1][2] * rhs.Y + (*this)[2][2] * rhs.Z + (*this)[3][2] * rhs.W,
      (*this)[0][3] * rhs.X + (*this)[1][3] * rhs.Y + (*this)[2][3] * rhs.Z + (*this)[3][3] * rhs.W
  };
}

DDyMatrix4x4 DDyMatrix4x4::Inverse() const
{
  // temporary
  const glm::mat4 mat = *this;
  return glm::inverse(mat);
}

DDyMatrix4x4 DDyMatrix4x4::IdentityMatrix() noexcept
{
  static DDyMatrix4x4 staticInstance { true };
  return staticInstance;
}

DDyMatrix4x4::DDyMatrix4x4(bool)
{
  this->mMatrixValue[0][0] = 1;
  this->mMatrixValue[1][1] = 1;
  this->mMatrixValue[2][2] = 1;
  this->mMatrixValue[3][3] = 1;
}

} /// ::dy namespace