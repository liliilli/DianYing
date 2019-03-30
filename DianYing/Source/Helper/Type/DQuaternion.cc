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
#include <Dy/Helper/Type/DQuaternion.h>
#include <Dy/Helper/Math/Math.h>
#include <Math/Utility/XMath.h>

namespace dy
{

DQuaternion::DQuaternion(const DVec3& eulerAngleXYZ, bool isDegree) 
  : mQuaternion{ glm::vec3{ 
      math::ToNormalizedRadian(eulerAngleXYZ.X * (isDegree ? math::kToRadian<TReal> : 1)), 
      math::ToNormalizedRadian(eulerAngleXYZ.Y * (isDegree ? math::kToRadian<TReal> : 1)), 
      math::ToNormalizedRadian(eulerAngleXYZ.Z * (isDegree ? math::kToRadian<TReal> : 1))
    } 
  }
{}

DQuaternion::DQuaternion(const aiQuaternion& aiQuaternion) :
  mQuaternion{aiQuaternion.w, aiQuaternion.x, aiQuaternion.y, aiQuaternion.z}
{ }

DQuaternion& DQuaternion::operator=(const aiQuaternion& aiQuatenrion)
{
  mQuaternion = glm::quat{aiQuatenrion.w, aiQuatenrion.x, aiQuatenrion.y, aiQuatenrion.z};
  return *this;
}

DQuaternion::DQuaternion(const glm::quat& glmQuat) :
  mQuaternion(glmQuat)
{ }

DQuaternion& DQuaternion::operator=(const glm::quat& glmQuat)
{
  mQuaternion = glmQuat;
  return *this;
}

DQuaternion::DQuaternion(const physx::PxQuat& pxQuat) :
  mQuaternion{pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z}
{ }

DQuaternion& DQuaternion::operator=(const physx::PxQuat& pxQuat)
{
  this->mQuaternion = glm::quat{pxQuat.w, pxQuat.x, pxQuat.y, pxQuat.z};
  return *this;
}

DQuaternion::DQuaternion(TF32 x, TF32 y, TF32 z, TF32 w) :
  mQuaternion{w, x, y, z}
{ }

DMat4 DQuaternion::GetRotationMatrix4x4() const noexcept 
{
  return FMat4::CreateMat4(glm::mat4_cast(this->mQuaternion));
}

DMat3 DQuaternion::GetRotationMatrix3x3() const noexcept 
{
  return FMat3::CreateMat3(glm::mat3_cast(this->mQuaternion));
}

DVec3 DQuaternion::GetEulerRotationDegreeAngleVector() const noexcept 
{
  const auto radians = glm::eulerAngles(this->mQuaternion);
  return {
    math::ToNormalizedDegree(radians.x * math::kToDegree<TReal>), 
    math::ToNormalizedDegree(radians.y * math::kToDegree<TReal>), 
    math::ToNormalizedDegree(radians.z * math::kToDegree<TReal>)
  };
}

DVec3 DQuaternion::GetEulerRotationRadiansAngleVector() const noexcept 
{
  const auto angle = glm::eulerAngles(this->mQuaternion);
  return DVec3{angle.x, angle.y, angle.z};
}

void DQuaternion::AddRotationAngle(const DVec3& eulerAngle) 
{
  // @todo IS IT REALLY PRECISE? AND LILIABLE?
  const auto degree = GetEulerRotationDegreeAngleVector() + eulerAngle;

  this->mQuaternion = glm::quat{
    glm::vec3(glm::radians(degree.X), glm::radians(degree.Y), glm::radians(degree.Z))
  };
}

void DQuaternion::AddRotationAngle(const DQuaternion& quaternion) 
{
  this->mQuaternion = quaternion.pGetQuaternion() * this->mQuaternion;
}

void DQuaternion::AddRotationAngle(EAxis axis, float degreeEulerAngle) 
{
  const auto radians = glm::radians(degreeEulerAngle);
  switch (axis) {
  case EAxis::X: {
      const auto temp = glm::angleAxis(radians, FVec3::ToGlmVec3(DVec3::UnitX()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  case EAxis::Y: {
      const auto temp = glm::angleAxis(radians, FVec3::ToGlmVec3(DVec3::UnitY()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  case EAxis::Z: {
      const auto temp = glm::angleAxis(radians, FVec3::ToGlmVec3(DVec3::UnitZ()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  default:
    MDY_UNEXPECTED_BRANCH();
    break;
  }
}

void DQuaternion::SetRotationAngle(const DVec3& degreeEulerAngle) 
{
  this->mQuaternion = glm::quat
  {
    glm::vec3 
    {
      math::ToNormalizedRadian(degreeEulerAngle.X * math::kToRadian<TReal>),
      math::ToNormalizedRadian(degreeEulerAngle.Y * math::kToRadian<TReal>),
      math::ToNormalizedRadian(degreeEulerAngle.Z * math::kToRadian<TReal>)
    }
  };
}

const glm::quat& DQuaternion::pGetQuaternion() const noexcept 
{
  return this->mQuaternion;
}

} /// ::dy namespace