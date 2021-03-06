#ifndef GUARD_DY_COMPONENT_INTERNAL_PHYSICS_CallbackSpecifier_H
#define GUARD_DY_COMPONENT_INTERNAL_PHYSICS_CallbackSpecifier_H
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

#include <Dy/Helper/System/TypeTraits.h>
#include <Dy/Helper/Type/DVector3.h>

//!
//! Forward declaration
//!

namespace dy
{
class CBasePhysicsCollider;
class FActor;
struct FHitResult;
struct FDyOverlapResult;
class AActorCppScript;
}

//!
//! Implementation
//!

namespace dy
{

/**
 * Delegate for notification of blocking collision against a specific component.  
 * NormalImpulse will be filled in for physics-simulating bodies, but will be zero for swept-component blocking collisions. 
 */

struct ISignatureCollisionDelegate {};

struct TSignatureCollisionHit final : public ISignatureCollisionDelegate
{
  template <typename TType, typename = std::enable_if_t<IsInheritancedFrom<TType, AActorCppScript>, bool>>
  using RawType = void(TType::*)(
      CBasePhysicsCollider* iSelfComponent, FActor& iRefActor, 
      CBasePhysicsCollider* iOtherCollider, FActor& iRefOtherActor, FHitResult& result);
  using WrapType = void(CBasePhysicsCollider*, FActor&, CBasePhysicsCollider*, FActor&, FHitResult&);
};

struct TSignatureCollisionOverlap final : public ISignatureCollisionDelegate
{
  template <typename TType, typename = std::enable_if_t<IsInheritancedFrom<TType, AActorCppScript>, bool>>
  using RawType = void(TType::*)(
      CBasePhysicsCollider* iSelfComponent, FActor& iRefActor, 
      CBasePhysicsCollider* iOtherCollider, FActor& iRefOtherActor, FHitResult& result);
  using WrapType = void(CBasePhysicsCollider*, FActor&, CBasePhysicsCollider*, FActor&, FHitResult&);
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_PHYSICS_CallbackSpecifier_H