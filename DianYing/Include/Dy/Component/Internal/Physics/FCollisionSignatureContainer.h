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

#include <Dy/Component/Internal/Physics/XCallbackSpecifiers.h>
#include <Dy/Component/Interface/Physics/IOnColliderBaseDelegator.h>

//!
//! Forward declaration
//!

namespace dy
{
class AActorCppScript;
class FOnHitDelegatorContainer;
} /// ::dy namespace

//!
//! Implementation
//!

namespace dy
{

/// @class FOnHitDelegatorContainer
/// @brief `onHit` collisino function delegator container.
class FOnHitDelegatorContainer final : public IOnColliderBaseDelegator<TSignatureCollisionHit>
{
public:
  virtual ~FOnHitDelegatorContainer() = default;
};

/// @class FOnOverlapDelegatorContainer
/// @brief `onOverlapBegin` and `onOverlapEnd` collision function delegator container.
class FOnOverlapDelegatorContainer final : public IOnColliderBaseDelegator<TSignatureCollisionOverlap>
{
public:
  virtual ~FOnOverlapDelegatorContainer() = default;
};

/// @class FCollisionSignatureContainer
/// @brief Overall collision signature managing container.
class FCollisionSignatureContainer final
{
public:
  FOnHitDelegatorContainer      onHit;
  FOnOverlapDelegatorContainer  onOverlapBegin;
  FOnOverlapDelegatorContainer  onOverlapEnd;
};

} /// ::dy namespace

#endif /// GUARD_DY_COMPONENT_INTERNAL_PHYSICS_FDyCollisionSignatureContainer_H