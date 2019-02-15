#ifndef GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H
#define GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H
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

#include "CallbackSpecifier.h"
#include <Dy/Component/Interface/Physics/IDyonColliderBaseDelegator.h>

//!
//! Forward declaration
//!

namespace dy
{
class ADyActorCppScript;
class FDyonHitDelegatorContainer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FDyonHitDelegatorContainer
/// @brief `onHit` collisino function delegator container.
class FDyonHitDelegatorContainer final : public IDyonColliderBaseDelegator<TSignatureCollisionHit>
{
public:
  virtual ~FDyonHitDelegatorContainer() = default;
};

/// @class FDyonOverlapDelegatorContainer
/// @brief `onOverlapBegin` and `onOverlapEnd` collision function delegator container.
class FDyonOverlapDelegatorContainer final : public IDyonColliderBaseDelegator<TSignatureCollisionOverlap>
{
public:
  virtual ~FDyonOverlapDelegatorContainer() = default;
};

/// @class FDyCollisionSignatureContainer
/// @brief Overall collision signature managing container.
class FDyCollisionSignatureContainer final
{
public:
  FDyonHitDelegatorContainer      onHit;
  FDyonOverlapDelegatorContainer  onOverlapBegin;
  FDyonOverlapDelegatorContainer  onOverlapEnd;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H