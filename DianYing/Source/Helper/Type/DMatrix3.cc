#include <precompiled.h>
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

/// Header file
#include <Dy/Helper/Type/DMatrix3x3.h>

#include <glm/glm.hpp>
#include <Dy/Helper/Type/DMatrix2x2.h>

namespace dy
{

DMatrix3x3::operator glm::mat2() const noexcept
{
  return glm::mat2{
      mMatrixValue[0][0], mMatrixValue[0][1],
      mMatrixValue[1][0], mMatrixValue[1][1]
  };
}

#ifdef false
DMatrix3x3::operator DMat2() const noexcept
{
  return DMat2{
      mMatrixValue[0][0], mMatrixValue[1][0],
      mMatrixValue[0][1], mMatrixValue[1][1]
  };
}
#endif

DMatrix3x3::operator glm::mat3() const noexcept
{
  return glm::mat3{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2],
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2],
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2] };
}

DMatrix3x3::operator glm::mat4() const noexcept
{
  return glm::mat4{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2], 0,
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2], 0,
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2], 0,
      0, 0, 0, 0 };
}

DMatrix3x3 DMatrix3x3::operator*(const DMatrix3x3& rhs) const noexcept
{
  return DMatrix3x3{
      (*this)[0][0] * rhs[0][0], (*this)[1][0] * rhs[1][0], (*this)[2][0] * rhs[2][0],
      (*this)[0][1] * rhs[0][1], (*this)[1][1] * rhs[1][1], (*this)[2][1] * rhs[2][1],
      (*this)[0][2] * rhs[0][2], (*this)[1][2] * rhs[1][2], (*this)[2][2] * rhs[2][2]
  };
}

DMatrix3x3& DMatrix3x3::operator*=(const DMatrix3x3& rhs) noexcept
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

DMatrix3x3 DMatrix3x3::operator/(const DMatrix3x3& rhs) const
{
  for (int32_t i = 0; i < 3; ++i)
  {
    for (int32_t j = 0; j < 3; ++j)
    {
      if (rhs[i][j] == 0.f) throw std::runtime_error("Divide by zero.");
    }
  }

  return DMatrix3x3{
      (*this)[0][0] / rhs[0][0], (*this)[1][0] / rhs[1][0], (*this)[2][0] / rhs[2][0],
      (*this)[0][1] / rhs[0][1], (*this)[1][1] / rhs[1][1], (*this)[2][1] / rhs[2][1],
      (*this)[0][2] / rhs[0][2], (*this)[1][2] / rhs[1][2], (*this)[2][2] / rhs[2][2]
  };
}

DMatrix3x3& DMatrix3x3::operator/=(const DMatrix3x3& rhs)
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

DMatrix3x3 DMatrix3x3::operator+(const DMatrix3x3& rhs) const noexcept
{
  return DMatrix3x3{
      (*this)[0][0] + rhs[0][0], (*this)[1][0] + rhs[1][0], (*this)[2][0] + rhs[2][0],
      (*this)[0][1] + rhs[0][1], (*this)[1][1] + rhs[1][1], (*this)[2][1] + rhs[2][1],
      (*this)[0][2] + rhs[0][2], (*this)[1][2] + rhs[1][2], (*this)[2][2] + rhs[2][2]
  };
}

DMatrix3x3& DMatrix3x3::operator+=(const DMatrix3x3& rhs) noexcept
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

DMatrix3x3 DMatrix3x3::operator-(const DMatrix3x3& rhs) const noexcept
{
  return DMatrix3x3{
      (*this)[0][0] - rhs[0][0], (*this)[1][0] - rhs[1][0], (*this)[2][0] - rhs[2][0],
      (*this)[0][1] - rhs[0][1], (*this)[1][1] - rhs[1][1], (*this)[2][1] - rhs[2][1],
      (*this)[0][2] - rhs[0][2], (*this)[1][2] - rhs[1][2], (*this)[2][2] - rhs[2][2],
  };
}

DMatrix3x3& DMatrix3x3::operator-=(const DMatrix3x3& rhs) noexcept
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

DMatrix3x3 DMatrix3x3::Transpose() const noexcept
{
  return DMatrix3x3{
      this->mMatrixValue[0][0], this->mMatrixValue[0][1], this->mMatrixValue[0][2],
      this->mMatrixValue[1][0], this->mMatrixValue[1][1], this->mMatrixValue[1][2],
      this->mMatrixValue[2][0], this->mMatrixValue[2][1], this->mMatrixValue[2][2]
  };
}

DMatrix3x3 DMatrix3x3::Multiply(const DMatrix3x3& rhs) const noexcept
{
  return DMatrix3x3{
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

DVec3 DMatrix3x3::MultiplyVector(const DVec3& rhs) const noexcept
{
  return DVec3{
      (*this)[0][0] * rhs.X + (*this)[1][0] * rhs.Y + (*this)[2][0] * rhs.Z,
      (*this)[0][1] * rhs.X + (*this)[1][1] * rhs.Y + (*this)[2][1] * rhs.Z,
      (*this)[0][2] * rhs.X + (*this)[1][2] * rhs.Y + (*this)[2][2] * rhs.Z
  };
}

DMatrix3x3 DMatrix3x3::Inverse() const
{
  // temporary
  const glm::mat3 mat = *this;
  return glm::inverse(mat);
}

DMatrix3x3 DMatrix3x3::IdentityMatrix() noexcept
{
  static DMatrix3x3 identityMatrix3x3{ true };
  return identityMatrix3x3;
}

DMatrix3x3::DMatrix3x3(bool)
{
  mMatrixValue[0][0] = 1;
  mMatrixValue[1][1] = 1;
  mMatrixValue[2][2] = 1;
}

bool operator==(_MIN_ const DMatrix3x3& lhs, _MIN_ const DMatrix3x3& rhs) noexcept
{
  for (TI32 i = 0; i < 3; ++i)
  { 
    if (lhs.mMatrixValue[i] != rhs.mMatrixValue[i]) { return false; }
  }
  return true;
}

bool operator!=(_MIN_ const DMatrix3x3& lhs, _MIN_ const DMatrix3x3& rhs) noexcept
{
  return !(lhs == rhs);
}

} /// ::dy namespace