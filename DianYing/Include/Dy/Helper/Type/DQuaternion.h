#ifndef GUARD_DY_TYPE_QUATERNION_H
#define GUARD_DY_TYPE_QUATERNION_H
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

#include <foundation/PxQuat.h>
#include <Math/Type/Math/DQuat.h>

//!
//! Implementation
//!

namespace dy 
{

using DQuat = math::DQuaternion<TReal>;

struct FQuat final
{
  static physx::PxQuat ToPxQuat(const DQuat& quat);

  static DQuat CreateQuat(const physx::PxQuat& quat);
};

} /// ::dy namespace

#endif /// GUARD_DY_TYPE_QUATERNION_H
