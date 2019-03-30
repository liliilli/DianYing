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
#include <Dy/Helper/Math/Math.h>

#include <glm/gtc/quaternion.hpp>

#include <Dy/Helper/Type/DVector3.h>
#include <Dy/Helper/Type/DQuaternion.h>
#include <Math/Utility/XMath.h>

namespace dy::math 
{

DQuaternion Slerp(const DQuaternion& lhs, const DQuaternion& rhs, TF32 offset) noexcept
{
  return glm::slerp(lhs.pGetQuaternion(), rhs.pGetQuaternion(), offset);
}

DVec3 ConvertQuaternionToRadianEuler(const DQuaternion& iQuat)
{
  return ConvertQuaternionToRadianEuler(iQuat.W(), iQuat.X(), iQuat.Y(), iQuat.Z());
}

DVec3 ConvertQuaternionToRadianEuler(TF32 w, TF32 x, TF32 y, TF32 z)
{
  DVec3 radianAngle;

  // roll (x-axis rotation)
  const TF64 sinrCosp = +2.0 * (w * x + y * z);
  const TF64 cosrCosp = +1.0 - 2.0 * (x * x + y * y);
	radianAngle.X = atan2(sinrCosp, cosrCosp);

	// pitch (y-axis rotation)
	const TF64 sinp = +2.0 * (w * y - z * x);
	if (fabs(sinp) >= 1)
  { // use 90 degrees if out of range
    radianAngle.Y = copysign(math::kPi<TF32> / 2, sinp); 
  }
	else { radianAngle.Y = asin(sinp); }

	// yaw (z-axis rotation)
	const TF64 sinyCosp = +2.0 * (w * z + x * y);
	const TF64 cosyCosp = +1.0 - 2.0 * (y * y + z * z);  
	radianAngle.Z = atan2(sinyCosp, cosyCosp);

  return radianAngle;
}

} /// ::dy::math namespace