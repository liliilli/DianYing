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
#include <Dy/Component/CDyTransform.h>

namespace dy
{


//!
//! Getter
//!

CDyTransform::CDyTransform(FDyActor& actorReference) : ADyBaseTransform(actorReference)
{ }

const DDyVector3& CDyTransform::GetLocalSpacePosition() const noexcept
{
  return this->mLocalPosition;
}

const DDyVector3& CDyTransform::GetWorldSpacePosition() const noexcept
{
  return this->mWorldPosition;
}

const DDyVector3& CDyTransform::GetFinalWorldSpacePosition() const noexcept
{
  return this->mSummedFinalRenderingPosition;
}

const DDyVector3& CDyTransform::GetParentWorldSpacePosition() const noexcept
{
#ifdef false
  return this->m_propagated_world_basis_position;
#endif
  return this->mPositionBasisFromParent;
}

float CDyTransform::GetLocalEulerAngle(EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mLocalEulerAngle.X;
  case EDyAxis3D::Y: return this->mLocalEulerAngle.Y;
  case EDyAxis3D::Z: return this->mLocalEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float CDyTransform::GetWorldEulerAngle(EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mWorldEulerAngle.X;
  case EDyAxis3D::Y: return this->mWorldEulerAngle.Y;
  case EDyAxis3D::Z: return this->mWorldEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

float CDyTransform::GetFinalEulerAngle(EDyAxis3D direction) const noexcept
{
  switch (direction)
  {
  case EDyAxis3D::X: return this->mSummedFinalRenderingEulerAngle.X;
  case EDyAxis3D::Y: return this->mSummedFinalRenderingEulerAngle.Y;
  case EDyAxis3D::Z: return this->mSummedFinalRenderingEulerAngle.Z;
  default: return MDY_INITIALIZE_DEFINT;
  }
}

const DDyVector3& CDyTransform::GetLocalScale() const noexcept
{
  return this->mLocalScale;
}

const DDyVector3& CDyTransform::GetWorldScale() const noexcept
{
  return this->mWorldScale;
}

//!
//! Setter
//!

void CDyTransform::SetLocalSpacePosition(const DDyVector3& localPosition) noexcept
{

}

void CDyTransform::SetWorldSpacePosition(const DDyVector3& worldPosition) noexcept
{

}

void CDyTransform::SetWorldPositionFromFinalPosition(const DDyVector3& finalPosition) noexcept
{

}

void CDyTransform::AddOffsetLocalSpacePosition(EDyAxis3D axis, float value) noexcept
{

}

void CDyTransform::AddOffsetWorldSpacePosition(EDyAxis3D axis, float value) noexcept
{

}

void CDyTransform::SetLocalEulerAngle(const DDyVector3& eulerAngleValue) noexcept
{

}

void CDyTransform::SetWorldEulerAngle(const DDyVector3& eulerAngleValue) noexcept
{

}

void CDyTransform::AddLocalEulerAngle(EDyAxis3D axis, float eulerAngleValue) noexcept
{

}

void CDyTransform::AddWorldEulerAngle(EDyAxis3D axis, float eulerAngleValue) noexcept
{

}

void CDyTransform::SetLocalScale(const DDyVector3& xyz_value) noexcept
{

}

void CDyTransform::SetWorldScale(const DDyVector3& xyz_value) noexcept
{

}

const DDyMatrix4x4& CDyTransform::GetTransform() const noexcept
{
  return this->mFinalRenderingTransform;
}

} /// ::dy namespace