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


namespace dy
{

DQuaternion::DQuaternion(const DVector3& eulerAngleXYZ) :
    mQuaternion{ glm::vec3{math::ConvertDegreeToRadian(eulerAngleXYZ.X), math::ConvertDegreeToRadian(eulerAngleXYZ.Y), math::ConvertDegreeToRadian(eulerAngleXYZ.Z)} }
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

DMatrix4x4 DQuaternion::GetRotationMatrix4x4() const noexcept 
{
  return glm::mat4_cast(this->mQuaternion);
}

DMatrix3x3 DQuaternion::GetRotationMatrix3x3() const noexcept 
{
  return glm::mat3_cast(this->mQuaternion);
}

DVector3 DQuaternion::GetEulerRotationDegreeAngleVector() const noexcept {
  const auto radians = glm::eulerAngles(this->mQuaternion);
  return {math::ConvertRadianToDegree(radians.x), math::ConvertRadianToDegree(radians.y), math::ConvertRadianToDegree(radians.z)};
}

DVector3 DQuaternion::GetEulerRotationRadiansAngleVector() const noexcept {
  return glm::eulerAngles(this->mQuaternion);
}

void DQuaternion::AddRotationAngle(const DVector3& eulerAngle) {
  // @todo IS IT REALLY PRECISE? AND LILIABLE?
  const auto degree = GetEulerRotationDegreeAngleVector() + eulerAngle;

  this->mQuaternion = glm::quat{
    glm::vec3(glm::radians(degree.X), glm::radians(degree.Y), glm::radians(degree.Z))
  };
}

void DQuaternion::AddRotationAngle(const DQuaternion& quaternion) {
  this->mQuaternion = quaternion.pGetQuaternion() * this->mQuaternion;
}

void DQuaternion::AddRotationAngle(EAxis3D axis, float degreeEulerAngle) {
  const auto radians = glm::radians(degreeEulerAngle);
  switch (axis) {
  case EAxis3D::X: {
      const auto temp = glm::angleAxis(radians, static_cast<glm::vec3>(DVector3::RightX()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  case EAxis3D::Y: {
      const auto temp = glm::angleAxis(radians, static_cast<glm::vec3>(DVector3::UpY()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  case EAxis3D::Z: {
      const auto temp = glm::angleAxis(radians, static_cast<glm::vec3>(DVector3::FrontZ()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  default:
    MDY_UNEXPECTED_BRANCH();
    break;
  }
}

void DQuaternion::SetRotationAngle(const DVector3& degreeEulerAngle) {
  this->mQuaternion = glm::quat{
      glm::vec3{math::ConvertDegreeToRadian(degreeEulerAngle.X),
                math::ConvertDegreeToRadian(degreeEulerAngle.Y),
                math::ConvertDegreeToRadian(degreeEulerAngle.Z)}
  };
}

const glm::quat& DQuaternion::pGetQuaternion() const noexcept {
  return this->mQuaternion;
}

} /// ::dy namespace