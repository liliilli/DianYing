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
#include <Dy/Element/Abstract/ADyTransformable.h>

namespace dy
{

//!
//! Getter
//!

const DVector3& ADyTransformable::GetLocalSpacePosition() const noexcept
{
  return this->mLocalPosition;
}

const DVector3& ADyTransformable::GetWorldSpacePosition() const noexcept
{
  return this->mWorldPosition;
}

const DVector3& ADyTransformable::GetFinalWorldSpacePosition() const noexcept
{
  return this->m_summed_world_position;
}

const DVector3& ADyTransformable::GetParentWorldSpacePosition() const noexcept
{
  return this->m_propagated_world_basis_position;
}

float ADyTransformable::GetLocalEulerAngle(EAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EAxis3D::X: return this->mLocalEulerAngle.X;
  case EAxis3D::Y: return this->mLocalEulerAngle.Y;
  case EAxis3D::Z: return this->mLocalEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float ADyTransformable::GetWorldEulerAngle(EAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EAxis3D::X: return this->mWorldEulerAngle.X;
  case EAxis3D::Y: return this->mWorldEulerAngle.Y;
  case EAxis3D::Z: return this->mWorldEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float ADyTransformable::GetFinalEulerAngle(EAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EAxis3D::X: return this->m_object_final_rotation_euler_angle.X;
  case EAxis3D::Y: return this->m_object_final_rotation_euler_angle.Y;
  case EAxis3D::Z: return this->m_object_final_rotation_euler_angle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

const DVector3& ADyTransformable::GetLocalScale() const noexcept
{
  return this->mLocalScale;
}

const DVector3& ADyTransformable::GetWorldScale() const noexcept
{
  return this->mWorldScale;
}

//!
//! Setter
//!

void ADyTransformable::SetLocalSpacePosition(const DVector3& localPosition) noexcept
{

}

void ADyTransformable::SetWorldSpacePosition(const DVector3& worldPosition) noexcept
{

}

void ADyTransformable::SetWorldPositionFromFinalPosition(const DVector3& finalPosition) noexcept
{

}

void ADyTransformable::AddOffsetLocalSpacePosition(EAxis3D axis, float value) noexcept
{

}

void ADyTransformable::AddOffsetWorldSpacePosition(EAxis3D axis, float value) noexcept
{

}

void ADyTransformable::SetLocalEulerAngle(const DVector3& eulerAngleValue) noexcept
{

}

void ADyTransformable::SetWorldEulerAngle(const DVector3& eulerAngleValue) noexcept
{

}

void ADyTransformable::AddLocalEulerAngle(EAxis3D axis, float eulerAngleValue) noexcept
{

}

void ADyTransformable::AddWorldEulerAngle(EAxis3D axis, float eulerAngleValue) noexcept
{

}

void ADyTransformable::SetLocalScale(const DVector3& xyz_value) noexcept
{

}

void ADyTransformable::SetWorldScale(const DVector3& xyz_value) noexcept
{

}

const DMatrix4x4& ADyTransformable::GetTransform() const noexcept
{
  return this->mFinalTransform;
}

} /// ::dy namespace