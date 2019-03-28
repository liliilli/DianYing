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
#include <Dy/Helper/Type/DMatrix4x4.h>

#include <glm/gtc/matrix_transform.hpp>
#include <foundation/PxTransform.h>
#include <nlohmann/json.hpp>

#include <Dy/Helper/Type/DMatrix2x2.h>
#include <Dy/Helper/Type/DMatrix3x3.h>
#include <Dy/Helper/Type/DQuaternion.h>

namespace dy
{

DMatrix4x4::DMatrix4x4(const DVec4& column1, const DVec4& column2,
                           const DVec4& column3, const DVec4& column4) :
    mMatrixValue{ column1, column2, column3, column4 } { }

DMatrix4x4::DMatrix4x4(const glm::mat4& glmMatrix) noexcept
{
  mMatrixValue[0] = FVec4::CreateVec4(glmMatrix[0]);
  mMatrixValue[1] = FVec4::CreateVec4(glmMatrix[1]);
  mMatrixValue[2] = FVec4::CreateVec4(glmMatrix[2]);
  mMatrixValue[3] = FVec4::CreateVec4(glmMatrix[3]);
}

DMatrix4x4::DMatrix4x4(const aiMatrix4x4& aiMatrix) noexcept
{
  this->mMatrixValue[0][0] = aiMatrix.a1;
  this->mMatrixValue[1][0] = aiMatrix.a2;
  this->mMatrixValue[2][0] = aiMatrix.a3;
  this->mMatrixValue[3][0] = aiMatrix.a4;

  this->mMatrixValue[0][1] = aiMatrix.b1;
  this->mMatrixValue[1][1] = aiMatrix.b2;
  this->mMatrixValue[2][1] = aiMatrix.b3;
  this->mMatrixValue[3][1] = aiMatrix.b4;

  this->mMatrixValue[0][2] = aiMatrix.c1;
  this->mMatrixValue[1][2] = aiMatrix.c2;
  this->mMatrixValue[2][2] = aiMatrix.c3;
  this->mMatrixValue[3][2] = aiMatrix.c4;

  this->mMatrixValue[0][3] = aiMatrix.d1;
  this->mMatrixValue[1][3] = aiMatrix.d2;
  this->mMatrixValue[2][3] = aiMatrix.d3;
  this->mMatrixValue[3][3] = aiMatrix.d4;
}

DMatrix4x4::DMatrix4x4(_MIN_ const physx::PxTransform& physxTransform) noexcept
{
  const auto quatMatrix = DQuaternion{physxTransform.q}.GetRotationMatrix4x4();
  this->mMatrixValue[0] = quatMatrix[0];
  this->mMatrixValue[1] = quatMatrix[1];
  this->mMatrixValue[2] = quatMatrix[2];
  this->mMatrixValue[3] = quatMatrix[3];
  this->mMatrixValue[3][0] += physxTransform.p.x;
  this->mMatrixValue[3][1] += physxTransform.p.y;
  this->mMatrixValue[3][2] += physxTransform.p.z;
}

DMatrix4x4& DMatrix4x4::operator=(const glm::mat4& value) noexcept
{
  this->mMatrixValue[0] = FVec4::CreateVec4(value[0]);
  this->mMatrixValue[1] = FVec4::CreateVec4(value[1]);
  this->mMatrixValue[2] = FVec4::CreateVec4(value[2]);
  this->mMatrixValue[3] = FVec4::CreateVec4(value[3]);
  return *this;
}

DMatrix4x4& DMatrix4x4::operator=(const aiMatrix4x4& value) noexcept
{
  this->mMatrixValue[0][0] = value.a1;
  this->mMatrixValue[1][0] = value.a2;
  this->mMatrixValue[2][0] = value.a3;
  this->mMatrixValue[3][0] = value.a4;

  this->mMatrixValue[0][1] = value.b1;
  this->mMatrixValue[1][1] = value.b2;
  this->mMatrixValue[2][1] = value.b3;
  this->mMatrixValue[3][1] = value.b4;

  this->mMatrixValue[0][2] = value.c1;
  this->mMatrixValue[1][2] = value.c2;
  this->mMatrixValue[2][2] = value.c3;
  this->mMatrixValue[3][2] = value.c4;

  this->mMatrixValue[0][3] = value.d1;
  this->mMatrixValue[1][3] = value.d2;
  this->mMatrixValue[2][3] = value.d3;
  this->mMatrixValue[3][3] = value.d4;

  return *this;
}

DMatrix4x4::operator glm::mat2() const noexcept
{
  return glm::mat2{
      mMatrixValue[0][0], mMatrixValue[0][1],
      mMatrixValue[1][0], mMatrixValue[1][1]
  };
}

DMatrix4x4::operator glm::mat3() const noexcept
{
  return glm::mat3{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2],
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2],
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2]
  };
}

DMatrix4x4::operator glm::mat4() const noexcept
{
  return glm::mat4{
      mMatrixValue[0][0], mMatrixValue[0][1], mMatrixValue[0][2], mMatrixValue[0][3],
      mMatrixValue[1][0], mMatrixValue[1][1], mMatrixValue[1][2], mMatrixValue[1][3],
      mMatrixValue[2][0], mMatrixValue[2][1], mMatrixValue[2][2], mMatrixValue[2][3],
      mMatrixValue[3][0], mMatrixValue[3][1], mMatrixValue[3][2], mMatrixValue[3][3]
  };
}

DMatrix4x4::operator DMat2() const noexcept
{
  return DMat2{
      mMatrixValue[0][0], mMatrixValue[1][0],
      mMatrixValue[0][1], mMatrixValue[1][1]
  };
}

DMatrix4x4::operator DMatrix3x3() const noexcept
{
  return DMatrix3x3{
      mMatrixValue[0][0], mMatrixValue[1][0], mMatrixValue[2][0],
      mMatrixValue[0][1], mMatrixValue[1][1], mMatrixValue[2][1],
      mMatrixValue[0][2], mMatrixValue[1][2], mMatrixValue[2][2],
  };
}

DMatrix4x4 DMatrix4x4::operator*(const DMatrix4x4& rhs) const noexcept
{
  return DMatrix4x4{
      (*this)[0][0] * rhs[0][0], (*this)[1][0] * rhs[1][0], (*this)[2][0] * rhs[2][0], (*this)[3][0] * rhs[3][0],
      (*this)[0][1] * rhs[0][1], (*this)[1][1] * rhs[1][1], (*this)[2][1] * rhs[2][1], (*this)[3][1] * rhs[3][1],
      (*this)[0][2] * rhs[0][2], (*this)[1][2] * rhs[1][2], (*this)[2][2] * rhs[2][2], (*this)[3][2] * rhs[3][2],
      (*this)[0][3] * rhs[0][3], (*this)[1][3] * rhs[1][3], (*this)[2][3] * rhs[2][3], (*this)[3][3] * rhs[3][3],
  };
}

DMatrix4x4& DMatrix4x4::operator*=(const DMatrix4x4& rhs) noexcept
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

DMatrix4x4 DMatrix4x4::operator/(const DMatrix4x4& rhs) const
{
  for (int32_t i = 0; i < 4; ++i)
  {
    for (int32_t j = 0; j < 4; ++j)
    {
      if (rhs[i][j] == 0.f) throw std::runtime_error("Divide by zero.");
    }
  }

  return DMatrix4x4{
      (*this)[0][0] / rhs[0][0], (*this)[1][0] / rhs[1][0], (*this)[2][0] / rhs[2][0], (*this)[3][0] / rhs[3][0],
      (*this)[0][1] / rhs[0][1], (*this)[1][1] / rhs[1][1], (*this)[2][1] / rhs[2][1], (*this)[3][1] / rhs[3][1],
      (*this)[0][2] / rhs[0][2], (*this)[1][2] / rhs[1][2], (*this)[2][2] / rhs[2][2], (*this)[3][2] / rhs[3][2],
      (*this)[0][3] / rhs[0][3], (*this)[1][3] / rhs[1][3], (*this)[2][3] / rhs[2][3], (*this)[3][3] / rhs[3][3],
  };
}

DMatrix4x4& DMatrix4x4::operator/=(const DMatrix4x4& rhs)
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

DMatrix4x4 DMatrix4x4::operator+(const DMatrix4x4& rhs) const noexcept
{
  return DMatrix4x4{
      (*this)[0][0] + rhs[0][0], (*this)[1][0] + rhs[1][0], (*this)[2][0] + rhs[2][0], (*this)[3][0] + rhs[3][0],
      (*this)[0][1] + rhs[0][1], (*this)[1][1] + rhs[1][1], (*this)[2][1] + rhs[2][1], (*this)[3][1] + rhs[3][1],
      (*this)[0][2] + rhs[0][2], (*this)[1][2] + rhs[1][2], (*this)[2][2] + rhs[2][2], (*this)[3][2] + rhs[3][2],
      (*this)[0][3] + rhs[0][3], (*this)[1][3] + rhs[1][3], (*this)[2][3] + rhs[2][3], (*this)[3][3] + rhs[3][3],
  };
}

DMatrix4x4& DMatrix4x4::operator+=(const DMatrix4x4& rhs) noexcept
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

DMatrix4x4 DMatrix4x4::operator-(const DMatrix4x4& rhs) const noexcept
{
  return DMatrix4x4{
      (*this)[0][0] - rhs[0][0], (*this)[1][0] - rhs[1][0], (*this)[2][0] - rhs[2][0], (*this)[3][0] - rhs[3][0],
      (*this)[0][1] - rhs[0][1], (*this)[1][1] - rhs[1][1], (*this)[2][1] - rhs[2][1], (*this)[3][1] - rhs[3][1],
      (*this)[0][2] - rhs[0][2], (*this)[1][2] - rhs[1][2], (*this)[2][2] - rhs[2][2], (*this)[3][2] - rhs[3][2],
      (*this)[0][3] - rhs[0][3], (*this)[1][3] - rhs[1][3], (*this)[2][3] - rhs[2][3], (*this)[3][3] - rhs[3][3],
  };
}

DMatrix4x4& DMatrix4x4::operator-=(const DMatrix4x4& rhs) noexcept
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

DMatrix4x4 DMatrix4x4::Transpose() const noexcept
{
  return DMatrix4x4{
      this->mMatrixValue[0][0], this->mMatrixValue[0][1], this->mMatrixValue[0][2], this->mMatrixValue[0][3],
      this->mMatrixValue[1][0], this->mMatrixValue[1][1], this->mMatrixValue[1][2], this->mMatrixValue[1][3],
      this->mMatrixValue[2][0], this->mMatrixValue[2][1], this->mMatrixValue[2][2], this->mMatrixValue[2][3],
      this->mMatrixValue[3][0], this->mMatrixValue[3][1], this->mMatrixValue[3][2], this->mMatrixValue[3][3]
  };
}

DMatrix4x4 DMatrix4x4::Multiply(const DMatrix4x4& rhs) const noexcept
{
  // Naive! O(N^3)
  return DMatrix4x4{
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

DVec4 DMatrix4x4::MultiplyVector(const DVec4& rhs) const noexcept
{
  return DVec4{
      (*this)[0][0] * rhs.X + (*this)[1][0] * rhs.Y + (*this)[2][0] * rhs.Z + (*this)[3][0] * rhs.W,
      (*this)[0][1] * rhs.X + (*this)[1][1] * rhs.Y + (*this)[2][1] * rhs.Z + (*this)[3][1] * rhs.W,
      (*this)[0][2] * rhs.X + (*this)[1][2] * rhs.Y + (*this)[2][2] * rhs.Z + (*this)[3][2] * rhs.W,
      (*this)[0][3] * rhs.X + (*this)[1][3] * rhs.Y + (*this)[2][3] * rhs.Z + (*this)[3][3] * rhs.W
  };
}

DMatrix4x4 DMatrix4x4::Inverse() const
{
  // temporary
  const glm::mat4 mat = *this;
  return glm::inverse(mat);
}

DMatrix4x4 DMatrix4x4::IdentityMatrix() noexcept
{
  static DMatrix4x4 staticInstance { true };
  return staticInstance;
}

DMatrix4x4 DMatrix4x4::CreateWithScale(const DVec3& scaleVector)
{
  return DMatrix4x4{
    scaleVector.X, 0, 0, 0,
    0, scaleVector.Y, 0, 0,
    0, 0, scaleVector.Z, 0,
    0, 0, 0,             1};
}

DMatrix4x4 DMatrix4x4::CreateWithTranslation(const DVec3& translationPoint)
{
  return DMatrix4x4{
    1, 0, 0, translationPoint.X,
    0, 1, 0, translationPoint.Y,
    0, 0, 1, translationPoint.Z,
    0, 0, 0, 1};
}

DMatrix4x4 DMatrix4x4::OrthoProjection(TF32 left, TF32 right, TF32 bottom, TF32 top, TF32 near, TF32 far)
{
  return glm::ortho(left, right, bottom, top, near, far);
}

DMatrix4x4& DMatrix4x4::Scale(_MIN_ const DVec3& iScaleFactor)
{
  const auto mat = glm::scale(static_cast<glm::mat4>(*this), FVec3::ToGlmVec3(iScaleFactor));
  (*this)[0] = FVec4::CreateVec4(mat[0]);
  (*this)[1] = FVec4::CreateVec4(mat[1]);
  (*this)[2] = FVec4::CreateVec4(mat[2]);
  (*this)[3] = FVec4::CreateVec4(mat[3]);
  return *this;
}

DMatrix4x4& DMatrix4x4::Rotate(_MIN_ const DVec3& iRotationDegreeAngle)
{
  const auto mat = this->Multiply(DQuaternion(iRotationDegreeAngle).GetRotationMatrix4x4());
  (*this)[0] = mat[0];
  (*this)[1] = mat[1];
  (*this)[2] = mat[2];
  (*this)[3] = mat[3];
  return *this;
}

DMatrix4x4& DMatrix4x4::Rotate(_MIN_ const DQuaternion& iRotationQuaternion)
{
  const auto mat = this->Multiply(iRotationQuaternion.GetRotationMatrix4x4());
  (*this)[0] = mat[0];
  (*this)[1] = mat[1];
  (*this)[2] = mat[2];
  (*this)[3] = mat[3];
  return *this;
}

DMatrix4x4& DMatrix4x4::Translate(_MIN_ const DVec3& iPosition)
{
  (*this)[3][0] += iPosition.X;
  (*this)[3][1] += iPosition.Y;
  (*this)[3][2] += iPosition.Z;
  return *this;
}

DMatrix4x4::DMatrix4x4(bool)
{
  this->mMatrixValue[0][0] = 1;
  this->mMatrixValue[0][3] = 0;
  this->mMatrixValue[1][1] = 1;
  this->mMatrixValue[1][3] = 0;
  this->mMatrixValue[2][2] = 1;
  this->mMatrixValue[2][3] = 0;
  this->mMatrixValue[3][3] = 1;
}

bool operator==(_MIN_ const DMatrix4x4& lhs, _MIN_ const DMatrix4x4& rhs) noexcept
{
  for (TI32 i = 0; i < 4; ++i)
  { 
    if (lhs.mMatrixValue[i] != rhs.mMatrixValue[i]) { return false; }
  }
  return true;
}

bool operator!=(_MIN_ const DMatrix4x4& lhs, _MIN_ const DMatrix4x4& rhs) noexcept
{
  return !(lhs == rhs);
}

void to_json(_MINOUT_ nlohmann::json& j, _MIN_ const DMatrix4x4& p)
{
  j = nlohmann::json
  {
    {"00", p[0][0]}, {"01", p[0][1]}, {"02", p[0][2]}, {"03", p[0][3]},
    {"10", p[1][0]}, {"11", p[1][1]}, {"12", p[1][2]}, {"13", p[1][3]},
    {"20", p[1][0]}, {"21", p[2][1]}, {"22", p[2][2]}, {"23", p[2][3]},
    {"30", p[1][0]}, {"31", p[3][1]}, {"32", p[3][2]}, {"33", p[3][3]},
  };
}

void from_json(_MIN_ const nlohmann::json& j, _MOUT_ DMatrix4x4& p)
{
  p[0][0] = j["00"].get<float>(); p[0][1] = j["01"].get<float>(); p[0][2] = j["02"].get<float>(); p[0][3] = j["03"].get<float>();
  p[1][0] = j["10"].get<float>(); p[1][1] = j["11"].get<float>(); p[1][2] = j["12"].get<float>(); p[1][3] = j["13"].get<float>();
  p[2][0] = j["20"].get<float>(); p[2][1] = j["21"].get<float>(); p[2][2] = j["22"].get<float>(); p[2][3] = j["23"].get<float>();
  p[3][0] = j["30"].get<float>(); p[3][1] = j["31"].get<float>(); p[3][2] = j["32"].get<float>(); p[3][3] = j["33"].get<float>();
}

} /// ::dy namespace