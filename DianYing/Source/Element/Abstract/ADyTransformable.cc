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
#include <Dy/Element/Abstract/ADyTransformable.h>

namespace dy
{

//!
//! Getter
//!

const DDyVector3& ADyTransformable::GetLocalSpacePosition() const noexcept
{
  return this->mLocalPosition;
}

const DDyVector3& ADyTransformable::GetWorldSpacePosition() const noexcept
{
  return this->mWorldPosition;
}

const DDyVector3& ADyTransformable::GetFinalWorldSpacePosition() const noexcept
{
  return this->m_summed_world_position;
}

const DDyVector3& ADyTransformable::GetParentWorldSpacePosition() const noexcept
{
  return this->m_propagated_world_basis_position;
}

float ADyTransformable::GetLocalEulerAngle(EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mLocalEulerAngle.X;
  case EDyAxis3D::Y: return this->mLocalEulerAngle.Y;
  case EDyAxis3D::Z: return this->mLocalEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float ADyTransformable::GetWorldEulerAngle(EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mWorldEulerAngle.X;
  case EDyAxis3D::Y: return this->mWorldEulerAngle.Y;
  case EDyAxis3D::Z: return this->mWorldEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float ADyTransformable::GetFinalEulerAngle(EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->m_object_final_rotation_euler_angle.X;
  case EDyAxis3D::Y: return this->m_object_final_rotation_euler_angle.Y;
  case EDyAxis3D::Z: return this->m_object_final_rotation_euler_angle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

const DDyVector3& ADyTransformable::GetLocalScale() const noexcept
{
  return this->mLocalScale;
}

const DDyVector3& ADyTransformable::GetWorldScale() const noexcept
{
  return this->mWorldScale;
}

//!
//! Setter
//!

void ADyTransformable::SetLocalSpacePosition(const DDyVector3& localPosition) noexcept
{

}

void ADyTransformable::SetWorldSpacePosition(const DDyVector3& worldPosition) noexcept
{

}

void ADyTransformable::SetWorldPositionFromFinalPosition(const DDyVector3& finalPosition) noexcept
{

}

void ADyTransformable::AddOffsetLocalSpacePosition(EDyAxis3D axis, float value) noexcept
{

}

void ADyTransformable::AddOffsetWorldSpacePosition(EDyAxis3D axis, float value) noexcept
{

}

void ADyTransformable::SetLocalEulerAngle(const DDyVector3& eulerAngleValue) noexcept
{

}

void ADyTransformable::SetWorldEulerAngle(const DDyVector3& eulerAngleValue) noexcept
{

}

void ADyTransformable::AddLocalEulerAngle(EDyAxis3D axis, float eulerAngleValue) noexcept
{

}

void ADyTransformable::AddWorldEulerAngle(EDyAxis3D axis, float eulerAngleValue) noexcept
{

}

void ADyTransformable::SetLocalScale(const DDyVector3& xyz_value) noexcept
{

}

void ADyTransformable::SetWorldScale(const DDyVector3& xyz_value) noexcept
{

}

const DDyMatrix4x4& ADyTransformable::GetTransform() const noexcept
{
  return this->mFinalTransform;
}

} /// ::dy namespace