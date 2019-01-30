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
#include <Dy/Helper/Type/Quaternion.h>


namespace dy
{

DDyQuaternion::DDyQuaternion(const DDyVector3& eulerAngleXYZ) :
    mQuaternion{ glm::vec3{math::ConvertDegreeToRadian(eulerAngleXYZ.X), math::ConvertDegreeToRadian(eulerAngleXYZ.Y), math::ConvertDegreeToRadian(eulerAngleXYZ.Z)} }
{}

DDyQuaternion::DDyQuaternion(const aiQuaternion& aiQuaternion) :
  mQuaternion{aiQuaternion.w, aiQuaternion.x, aiQuaternion.y, aiQuaternion.z}
{ }

DDyQuaternion& DDyQuaternion::operator=(const aiQuaternion& aiQuatenrion)
{
  mQuaternion = glm::quat{aiQuatenrion.w, aiQuatenrion.x, aiQuatenrion.y, aiQuatenrion.z};
  return *this;
}

DDyQuaternion::DDyQuaternion(const glm::quat& glmQuat) :
  mQuaternion(glmQuat)
{ }

DDyQuaternion& DDyQuaternion::operator=(const glm::quat& glmQuat)
{
  mQuaternion = glmQuat;
  return *this;
}

DDyQuaternion::DDyQuaternion(_MIN_ TF32 x, _MIN_ TF32 y, _MIN_ TF32 z, _MIN_ TF32 w) :
  mQuaternion{w, x, y, z}
{ }

DDyMatrix4x4 DDyQuaternion::GetRotationMatrix4x4() const noexcept 
{
  return glm::mat4_cast(this->mQuaternion);
}

DDyMatrix3x3 DDyQuaternion::GetRotationMatrix3x3() const noexcept 
{
  return glm::mat3_cast(this->mQuaternion);
}

DDyVector3 DDyQuaternion::GetEulerRotationDegreeAngleVector() const noexcept {
  const auto radians = glm::eulerAngles(this->mQuaternion);
  return {math::ConvertRadianToDegree(radians.x), math::ConvertRadianToDegree(radians.y), math::ConvertRadianToDegree(radians.z)};
}

DDyVector3 DDyQuaternion::GetEulerRotationRadiansAngleVector() const noexcept {
  return glm::eulerAngles(this->mQuaternion);
}

void DDyQuaternion::AddRotationAngle(const DDyVector3& eulerAngle) {
  // @todo IS IT REALLY PRECISE? AND LILIABLE?
  const auto degree = GetEulerRotationDegreeAngleVector() + eulerAngle;

  this->mQuaternion = glm::quat{
    glm::vec3(glm::radians(degree.X), glm::radians(degree.Y), glm::radians(degree.Z))
  };
}

void DDyQuaternion::AddRotationAngle(const DDyQuaternion& quaternion) {
  this->mQuaternion = quaternion.pGetQuaternion() * this->mQuaternion;
}

void DDyQuaternion::AddRotationAngle(EDyAxis3D axis, float degreeEulerAngle) {
  const auto radians = glm::radians(degreeEulerAngle);
  switch (axis) {
  case EDyAxis3D::X: {
      const auto temp = glm::angleAxis(radians, static_cast<glm::vec3>(DDyVector3::RightX()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  case EDyAxis3D::Y: {
      const auto temp = glm::angleAxis(radians, static_cast<glm::vec3>(DDyVector3::UpY()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  case EDyAxis3D::Z: {
      const auto temp = glm::angleAxis(radians, static_cast<glm::vec3>(DDyVector3::FrontZ()));
      this->mQuaternion = temp * this->mQuaternion;
    } break;
  default:
    MDY_UNEXPECTED_BRANCH();
    break;
  }
}

void DDyQuaternion::SetRotationAngle(const DDyVector3& degreeEulerAngle) {
  this->mQuaternion = glm::quat{
      glm::vec3{math::ConvertDegreeToRadian(degreeEulerAngle.X),
                math::ConvertDegreeToRadian(degreeEulerAngle.Y),
                math::ConvertDegreeToRadian(degreeEulerAngle.Z)}
  };
}

const glm::quat& DDyQuaternion::pGetQuaternion() const noexcept {
  return this->mQuaternion;
}

} /// ::dy namespace