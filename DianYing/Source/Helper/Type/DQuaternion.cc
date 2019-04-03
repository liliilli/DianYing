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
#include <Math/Utility/XMath.h>

namespace dy
{

physx::PxQuat FQuat::ToPxQuat(const math::DQuaternion<TReal>& quat)
{
  return physx::PxQuat{ quat.X(), quat.Y(), quat.Z(), quat.W() };
}

DQuat FQuat::CreateQuat(const physx::PxQuat& quat)
{
  return DQuat{quat.x, quat.y, quat.z, quat.w};
}

} /// ::dy namespace