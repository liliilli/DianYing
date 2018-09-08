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
#include <Dy/Helper/Type/Quaternion.h>
#include <Phitos/Dbg/assert.h>

namespace dy
{

DDyQuaternion::DDyQuaternion(const DVector3& eulerAngleXYZ) :
    mQuaternion{ glm::vec3{math::DegToRad(eulerAngleXYZ.X), math::DegToRad(eulerAngleXYZ.Y), math::DegToRad(eulerAngleXYZ.Z)} }
{}

DDyMatrix4x4 DDyQuaternion::GetRotationMatrix4x4() const noexcept {
  return glm::mat4_cast(this->mQuaternion);
}

DDyMatrix3x3 DDyQuaternion::GetRotationMatrix3x3() const noexcept {
  return glm::mat3_cast(this->mQuaternion);
}

DVector3 DDyQuaternion::GetEulerRotationDegreeAngleVector() const noexcept {
  const auto radians = glm::eulerAngles(this->mQuaternion);
  return {math::RadToDeg(radians.x), math::RadToDeg(radians.y), math::RadToDeg(radians.z)};
}

DVector3 DDyQuaternion::GetEulerRotationRadiansAngleVector() const noexcept {
  return glm::eulerAngles(this->mQuaternion);
}

void DDyQuaternion::AddRotationAngle(const DVector3& eulerAngle) {
  // @todo IS IT REALLY PRECISE? AND LILIABLE?
  const auto degree = GetEulerRotationDegreeAngleVector() + eulerAngle;

  this->mQuaternion = glm::quat{
    glm::vec3(glm::radians(degree.X), glm::radians(degree.Y), glm::radians(degree.Z))
  };
}

void DDyQuaternion::AddRotationAngle(const DDyQuaternion& quaternion) {
  this->mQuaternion = quaternion.pGetQuaternion() * this->mQuaternion;
}

void DDyQuaternion::AddRotationAngle(EAxis3D axis, float degreeEulerAngle) {
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
    PHITOS_UNEXPECTED_BRANCH();
    break;
  }
}

void DDyQuaternion::SetRotationAngle(const DVector3& degreeEulerAngle) {
  this->mQuaternion = glm::quat{
      glm::vec3{math::DegToRad(degreeEulerAngle.X),
                math::DegToRad(degreeEulerAngle.Y),
                math::DegToRad(degreeEulerAngle.Z)}
  };
}

const glm::quat& DDyQuaternion::pGetQuaternion() const noexcept {
  return this->mQuaternion;
}

} /// ::dy namespace