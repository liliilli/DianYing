#ifndef GUARD_DY_MANAGEMENT_TYPE_PHYSICS_DDyCollisionIssueItem_H
#define GUARD_DY_MANAGEMENT_TYPE_PHYSICS_DDyCollisionIssueItem_H
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

#include <Dy/Component/Type/Physics/ECollisionCallbackType.h>
#include <Dy/Management/Internal/Physics/CollisionResults.h>

//!
//! Forward declaration
//!

namespace dy
{
class CBasePhysicsCollider;
class FDyActor;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @struct DDyCollisionIssueItem
/// @brief Collision issue item for queueing collision callback function.
struct DDyCollisionIssueItem final
{
  ECollisionCallbackType  mType = ECollisionCallbackType::OnHit;
  CBasePhysicsCollider* mPtrSelfCollider  = nullptr;
  FDyActor*           mPtrSelfActor     = nullptr;
  CBasePhysicsCollider* mPtrOtherCollider = nullptr;
  FDyActor*           mPtrOtherActor    = nullptr;
  FDyHitResult        mHitResult{};
};

} /// ::dy namespace

#endif /// GUARD_DY_MANAGEMENT_TYPE_PHYSICS_DDyCollisionIssueItem_H